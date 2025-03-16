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
        tugboatState.hold_position = mouse_position;
        tugboatState.hold = true;
    }
}

void GraphState::UpdateTugboat(const LPARAM& lParam) {
    if (tugboatState.hold == true) {
        UpdateMousePosition(lParam);
        tugboatState.current_position = mouse_position;
        Point2D pixel_scale = area_span / window_size;
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


// ZoomState realization ->

void GraphState::UpdateScale(ScaleDirection scaleDirection) {
    if (tugboatState.hold == true) return;
#define CURSORZOOM
#ifdef CURSORZOOM
    Point2D save_abstruct_mouse_position = Pixel2Abstruct(mouse_position.x, mouse_position.y);
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
    Point2D new_abstruct_mouse_position = Pixel2Abstruct(mouse_position.x, mouse_position.y);
    Point2D delta_abstruct_mouse_position = save_abstruct_mouse_position - new_abstruct_mouse_position;
    tugboatState.reference_position += delta_abstruct_mouse_position;
#endif

    UpdateCoord();
}


// Other realization ->

Point2D GraphState::GetWindowSize() const {
    return window_size;
}

void GraphState::UpdateWindowSize(const LPARAM& lParam) {
    this->window_size.x = LOWORD(lParam);
    this->window_size.y = HIWORD(lParam);
}

void GraphState::UpdateWindowSize(int width, int height) {
    this->window_size.x = width;
    this->window_size.y = height;
}

Point2D GraphState::GetMousePosition() const {
    return mouse_position;
}

void GraphState::UpdateMousePosition(const LPARAM& lParam) {
    this->mouse_position.x = LOWORD(lParam);
    this->mouse_position.y = HIWORD(lParam);
}

void GraphState::UpdateMousePosition(double x, double y) {
    this->mouse_position.x = x;
    this->mouse_position.y = y;
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
    Point2D pixel_scale = area_span / window_size;
    Point2D relative_coord = pixel_scale * mouse_position;
    double inverse_y = area_span.y - relative_coord.y;

    coord = Point2D(
        tugboatState.reference_position.x + relative_coord.x,
        tugboatState.reference_position.y + inverse_y
    );
}

Point2D GraphState::GetCoord() const {
    return coord;
}

Point2D GraphState::Abstruct2Pixel(double x, double y) const {
    Point2D pixel_scale = window_size / area_span;
    double pixel_x = (x - tugboatState.reference_position.x) * pixel_scale.x;
    double pixel_y = (area_span.y - y + tugboatState.reference_position.y) * pixel_scale.y;
    Point2D pixel_position(pixel_x, pixel_y);
    
    //Point2D current_point(x, (y)); //+
    //Point2D relative_coord = current_point - tugboatState.reference_position; //+
    //double inverse_y = area_span.y - relative_coord.y; //+
    //relative_coord.y = inverse_y; // +
    //Point2D pixel_position = relative_coord * pixel_scale; // +
    
    return pixel_position;
}

Point2D GraphState::Pixel2Abstruct(int x, int y) const {
    Point2D abstruct_scale = area_span / window_size;
    double absruct_x = (double)x * abstruct_scale.x;
    double absruct_y = (double)(window_size.y - y) * abstruct_scale.y;
    Point2D abstruct_coord(absruct_x, absruct_y);

    //Point2D current_point((double)x, (double)(window_size.y - y)); // +
    //Point2D relative_coord = current_point * abstruct_scale; // +
    //Point2D abstruct_coord = tugboatState.reference_position + relative_coord; // +

    return abstruct_coord;
}

Point2D GraphState::GetOptimalDiscret() {
    double level, scale;
    double max_index, index;
    double optimal_count_lines, optimal_discret_x, optimal_discret_y;
    std::vector<double> new_discrets;

    // x - part
    new_discrets = axesState.discrets;
    max_index = new_discrets.size() - 1;
    level = ceil(log10(area_span.x)) - 1;
    scale = pow(10, level);

    for (int i = 0; i < new_discrets.size(); i++) { new_discrets[i] *= scale; }
    
    index = max_index;
    optimal_discret_x = new_discrets[index];

    while (1) {
        optimal_count_lines = area_span.x / optimal_discret_x;
        if ((optimal_count_lines >= axesState.minimal_count_lines.x))
        {
            break;
        }

        index--;
        if (index < 0) {
            index = max_index;
            level--;
            scale = pow(10, level);
            new_discrets = axesState.discrets;
            for (int i = 0; i < new_discrets.size(); i++) { new_discrets[i] *= scale; }
        }

        optimal_discret_x = new_discrets[index];
    }

    // y - part
    new_discrets = axesState.discrets;
    max_index = new_discrets.size() - 1;
    level = ceil(log10(area_span.y)) - 1;
    scale = pow(10, level);

    for (int i = 0; i < new_discrets.size(); i++) { new_discrets[i] *= scale; }

    index = max_index;
    optimal_discret_y = new_discrets[index];

    while (1) {
        optimal_count_lines = area_span.y / optimal_discret_y;
        if ((optimal_count_lines >= axesState.minimal_count_lines.y))
        {
            break;
        }

        index--;
        if (index < 0) {
            index = max_index;
            level--;
            scale = pow(10, level);
            new_discrets = axesState.discrets;
            for (int i = 0; i < new_discrets.size(); i++) { new_discrets[i] *= scale; }
        }

        optimal_discret_y = new_discrets[index];
    }

    return Point2D(optimal_discret_x, optimal_discret_y);
}


// DrawGraph realization ->

void GraphState::DrawGraph(const HDC& hdc)
{
    Point2D optimal_discret = GetOptimalDiscret();

    double x_min = tugboatState.reference_position.x;
    double x_max = tugboatState.reference_position.x + area_span.x;
    double y_min = tugboatState.reference_position.y;
    double y_max = tugboatState.reference_position.y + area_span.y;

    HPEN gridPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100)); // Тёмно-серые линии
    HPEN oldPen = (HPEN)SelectObject(hdc, gridPen);

    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);

    for (double x = std::floor(x_min / optimal_discret.x) * optimal_discret.x; x <= x_max; x += optimal_discret.x)
    {
        Point2D p1 = Abstruct2Pixel(x, y_min);
        Point2D p2 = Abstruct2Pixel(x, y_max);
        MoveToEx(hdc, p1.x, p1.y, NULL);
        LineTo(hdc, p2.x, p2.y);
    }

    for (double y = std::floor(y_min / optimal_discret.y) * optimal_discret.y; y <= y_max; y += optimal_discret.y)
    {
        Point2D p1 = Abstruct2Pixel(x_min, y);
        Point2D p2 = Abstruct2Pixel(x_max, y);
        MoveToEx(hdc, p1.x, p1.y, NULL);
        LineTo(hdc, p2.x, p2.y);
    }

    Point2D p1 = Abstruct2Pixel(20, 20);
    Point2D p2 = Abstruct2Pixel(30, 30);
    Rectangle(hdc, p1.x, p1.y, p2.x, p2.y);

    SelectObject(hdc, oldPen);
    DeleteObject(gridPen);
}