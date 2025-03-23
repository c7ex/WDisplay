#include "GraphState.h"
#include <ctime>
#include <iostream>

// Create ->

GraphState::GraphState() {}


// TugboatState realization ->

Point2D GraphState::GetReference() const {
    return tugboatState.reference_position;
}

void GraphState::BeginTugboat(const LPARAM& lParam) {
    if (tugboatState.hold == false) {
        UpdateMousePosition(lParam);
        tugboatState.last_reference_position = tugboatState.reference_position;
        tugboatState.hold_position = windowState.mouse_position;
        tugboatState.hold = true;
    }
}

void GraphState::UpdateTugboat(const LPARAM& lParam) {
    if (tugboatState.hold == true) {
        UpdateMousePosition(lParam);
        tugboatState.current_position = windowState.mouse_position;
        Point2D pixel_scale = area_span / windowState.size_current; // 0xAA?
        Point2D divergence = tugboatState.hold_position - tugboatState.current_position;
        Point2D abstruct_offset = pixel_scale * divergence;
        abstruct_offset.Y_inversion();
        tugboatState.reference_position = tugboatState.last_reference_position + abstruct_offset;
    }
}

void GraphState::StopTugboat(const LPARAM& lParam) {
    UpdateTugboat(lParam);
    if (tugboatState.hold == true)
        tugboatState.hold = false;
}

void GraphState::InstallReference() {
    //Point2D window_offset = Point2D(windowState.margin.left, windowState.margin.bottom);

}


// ZoomState realization ->

void GraphState::UpdateScale(ScaleDirection scaleDirection) {
    if (tugboatState.hold == true) return;
#define CURSORZOOM
#ifdef CURSORZOOM
    Point2D save_abstruct_mouse_position = 
        Pixel2Abstruct(windowState.mouse_position.x, windowState.mouse_position.y);
#else
    Point2D save_area_span = area_span;
#endif

    if (scaleDirection == ScaleDirection::Increase) area_span /= zoomState.growth_default_scale;
    if (scaleDirection == ScaleDirection::Decrease) area_span *= zoomState.growth_default_scale;

#ifndef CURSORZOOM
    // central zoom
    Point2D reference_offset = save_area_span - area_span;
    reference_offset /= 2.;
    tugboatState.reference_position += reference_offset;
#else
    // cursor zoom
    Point2D new_abstruct_mouse_position = 
        Pixel2Abstruct(windowState.mouse_position.x, windowState.mouse_position.y);
    Point2D delta_abstruct_mouse_position = save_abstruct_mouse_position - new_abstruct_mouse_position;
    tugboatState.reference_position += delta_abstruct_mouse_position;
#endif

    UpdateCoord();
}


// Other realization ->

Point2D GraphState::GetWindowSize() const {
    return windowState.size_current;
}

void GraphState::UpdateWindowSize(const LPARAM& lParam) {
    this->windowState.size_current.x = LOWORD(lParam);
    this->windowState.size_current.y = HIWORD(lParam);
    windowState.size_compress = windowState.size_current / windowState.size_default;
    windowState.size_plot_current = windowState.size_default * windowState.size_compress;

    // set points plot in window coordinates
    windowState.points_plot = RECT
    {
        ((long)((double)windowState.margin.left * windowState.size_compress.x)),
        ((long)(((double)windowState.margin.top) * windowState.size_compress.y)),
        (long)(windowState.size_current.x - ((double)windowState.margin.right * windowState.size_compress.x)),
        (long)(windowState.size_current.y - ((double)windowState.margin.bottom * windowState.size_compress.y))
    };

    windowState.reference_window_offset = Point2D(
        windowState.points_plot.left,
        windowState.points_plot.bottom);
}

void GraphState::SetWindowDefaultSize(int width, int height) {
    windowState.size_default.x = width;
    windowState.size_default.y = height;
    windowState.size_current = windowState.size_default;
    windowState.size_compress = windowState.size_current / windowState.size_default;

    // set points plot in window coordinates
    windowState.points_plot = RECT
    {
        (windowState.margin.left),
        (windowState.margin.top),
        ((long)windowState.size_current.x - windowState.margin.right),
        ((long)windowState.size_current.y - windowState.margin.bottom)
    };

    std::cout << windowState.points_plot.bottom << std::endl;

    windowState.reference_window_offset = Point2D(
        windowState.points_plot.left,
        windowState.points_plot.bottom);

    windowState.size_plot_default = Point2D(
        windowState.size_default.x - (windowState.margin.left + windowState.margin.right),
        windowState.size_default.y - (windowState.margin.bottom + windowState.margin.top));

    windowState.size_plot_current = windowState.size_plot_default;
}

Point2D GraphState::GetMousePosition() const {
    return windowState.mouse_position;
}

void GraphState::UpdateMousePosition(const LPARAM& lParam) {
    this->windowState.mouse_position.x = LOWORD(lParam);
    this->windowState.mouse_position.y = HIWORD(lParam);
}

void GraphState::UpdateMousePosition(double x, double y) {
    this->windowState.mouse_position.x = x;
    this->windowState.mouse_position.y = y;
}

Point2D GraphState::GetAreaSpan() const {
    return area_span;
}

void GraphState::UpdateAreaSpan(int x_size, int y_size) {
    this->area_span.x = x_size;
    this->area_span.y = y_size;
}


// Coord realization ->

void GraphState::UpdateCoord() {
    coord = Pixel2Abstruct(windowState.mouse_position.x, windowState.mouse_position.y);
    
    //Point2D pixel_scale = area_span / windowState.size_plot_current; //0xAA
    //Point2D relative_coord = pixel_scale * windowState.mouse_position;
    //double inverse_y = area_span.y - relative_coord.y;

    //coord = Point2D(
    //    tugboatState.reference_position.x + relative_coord.x,
    //    tugboatState.reference_position.y + inverse_y
    //);
}

Point2D GraphState::GetCoord() const {
    return coord;
}

/*  -> Входные данные - абстрактные координаты
    1. Вычесть из текущих абстрактных координат референсные абстрактные коррдинаты
    2. Определить вес пикселя в абстрактных координатах
    3. Определить расстояние в пикселях от референсной абстрактной точки
    4. Перевести в координаты окна: Y - инверсия
*/
Point2D GraphState::Abstruct2Pixel(double x, double y) const {
    // new
    Point2D input_abstruct_position = Point2D(x, y);
    Point2D delta = input_abstruct_position - tugboatState.reference_position;
    Point2D scale_abstruct = windowState.size_plot_current / area_span;
    Point2D relative_window_offset = delta * scale_abstruct;
    relative_window_offset.Y_inversion();
    Point2D pixel_position = windowState.reference_window_offset + relative_window_offset;

    // very old
    //Point2D current_point(x, (y)); //+
    //Point2D relative_coord = current_point - tugboatState.reference_position; //+
    //double inverse_y = area_span.y - relative_coord.y; //+
    //relative_coord.y = inverse_y; // +
    //Point2D pixel_position = relative_coord * pixel_scale; // +
    
    return pixel_position;
}

/*  -> Входные данные - координаты окна
    1. Посчитать расстояние от точки референса до входных данных [в пикселях]
    3. Инвертировать Y, т.к. ось инвертирована
    4. Перевести дельту [в пикселях] в [абстрактные координаты] через (area_span / plot_size)
    5. К абстракстному значению референсной точки прибавить дельту
*/
Point2D GraphState::Pixel2Abstruct(int x, int y) const {
    // new
    Point2D pixel_position = Point2D(x, y);
    Point2D delta = pixel_position - windowState.reference_window_offset;
    Point2D scale_pixel = area_span / windowState.size_plot_current;
    Point2D abstruct_delta = delta * scale_pixel;
    abstruct_delta.Y_inversion();
    Point2D abstruct_coord = tugboatState.reference_position + abstruct_delta;

    // very old
    //Point2D current_point((double)x, (double)(window_size.y - y)); // +
    //Point2D relative_coord = current_point * abstruct_scale; // +
    //Point2D abstruct_coord = tugboatState.reference_position + relative_coord; // +

    return abstruct_coord;
}

double AxesState::SearchOptimalDiscret(AxesSelection axesSelection, Point2D span) {
    double current_span = 0;
    double current_minimal_count_lines = 0;

    if (axesSelection == AxesSelection::xAxes) { 
        current_span = span.x;
        current_minimal_count_lines = minimal_count_lines.x;
    }

    if (axesSelection == AxesSelection::yAxes) {
        current_span = span.y;
        current_minimal_count_lines = minimal_count_lines.y;
    }

    double level, scale;
    double max_index, index;
    double optimal_count_lines, optimal_discret;
    std::vector<double> new_discrets;

    new_discrets = discrets;
    max_index = new_discrets.size() - 1;
    level = ceil(log10(current_span)) - 1;
    scale = pow(10, level);

    for (int i = 0; i < new_discrets.size(); i++) new_discrets[i] *= scale;

    index = max_index;
    optimal_discret = new_discrets[index];

    while (1) {
        optimal_count_lines = current_span / optimal_discret;

        if ((optimal_count_lines >= current_minimal_count_lines)) break;

        index--;
        if (index < 0) {
            index = max_index;
            level--;
            scale = pow(10, level);
            new_discrets = discrets;
            for (int i = 0; i < new_discrets.size(); i++) new_discrets[i] *= scale;
        }

        optimal_discret = new_discrets[index];
    }

    return optimal_discret;
}

Point2D AxesState::GetOptimalDiscret(Point2D span) {
    return Point2D(
        SearchOptimalDiscret(AxesSelection::xAxes, span),
        SearchOptimalDiscret(AxesSelection::yAxes, span));
}


// DrawGraph realization ->

void GraphState::DrawGraph(const HDC& hdc)
{
    Point2D optimal_discret = axesState.GetOptimalDiscret(area_span);

    double x_min = tugboatState.reference_position.x;
    double x_max = tugboatState.reference_position.x + area_span.x;
    double y_min = tugboatState.reference_position.y;
    double y_max = tugboatState.reference_position.y + area_span.y;

    // Ограничиваем область рисования
    int left = windowState.margin.left * windowState.size_compress.x;
    int top = windowState.margin.top * windowState.size_compress.y;
    int right = windowState.size_current.x - (windowState.margin.right * windowState.size_compress.x);
    int bottom = windowState.size_current.y - (windowState.margin.bottom * windowState.size_compress.y);

    RECT clipRect = { left, top, right, bottom };
    HRGN clipRegion = CreateRectRgn(left, top, right, bottom);
    SelectClipRgn(hdc, clipRegion);

    HPEN gridPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100)); // Тёмно-серые линии
    HPEN oldPen = (HPEN)SelectObject(hdc, gridPen);

    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);

    for (double x = std::floor(x_min / optimal_discret.x) * optimal_discret.x; x <= x_max; x += optimal_discret.x)
    {
        Point2D p1 = Abstruct2Pixel(x, y_min);
        Point2D p2 = Abstruct2Pixel(x, y_max);
        if (p1.x >= left && p1.x <= right) {
            MoveToEx(hdc, p1.x, top, NULL);
            LineTo(hdc, p2.x, bottom);
        }
    }

    for (double y = std::floor(y_min / optimal_discret.y) * optimal_discret.y; y <= y_max; y += optimal_discret.y)
    {
        Point2D p1 = Abstruct2Pixel(x_min, y);
        Point2D p2 = Abstruct2Pixel(x_max, y);
        if (p1.y >= top && p1.y <= bottom) {
            MoveToEx(hdc, left, p1.y, NULL);
            LineTo(hdc, right, p2.y);
        }
    }

    SelectObject(hdc, oldPen);
    DeleteObject(gridPen);

    Point2D p1 = Abstruct2Pixel(20, 20);
    Point2D p2 = Abstruct2Pixel(30, 30);

    Rectangle(hdc, p1.x, p1.y, p2.x, p2.y);
    SelectClipRgn(hdc, NULL);
    DeleteObject(clipRegion);


    HPEN refPen = CreatePen(PS_SOLID, 1, RGB(250, 250, 250));
    HPEN newPen = (HPEN)SelectObject(hdc, refPen);

    long xl = windowState.points_plot.left;
    long yt = windowState.points_plot.top;
    long xr = windowState.points_plot.right;
    long yb = windowState.points_plot.bottom;

    MoveToEx(hdc, xl, yb, NULL);
    LineTo(hdc, xr, yb);
    MoveToEx(hdc, xr, yb, NULL);
    LineTo(hdc, xr, yt);
    MoveToEx(hdc, xl, yt, NULL);
    LineTo(hdc, xr, yt);
    MoveToEx(hdc, xl, yb, NULL);
    LineTo(hdc, xl, yt);

    SelectObject(hdc, newPen);
    DeleteObject(refPen);
}