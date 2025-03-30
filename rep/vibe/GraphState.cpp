#include "GraphState.h"
#include <ctime>
#include <iostream>
#include "Other.h"

GraphState::GraphState() {}

// Реализация работы с перемещением графика
Position2d GraphState::GetReferencePosition() const {
    return tugboat_state_.reference_position;
}

void GraphState::StartDragging(const LPARAM& lParam) {
    if (!tugboat_state_.is_active) {
        UpdateMousePosition(lParam);
        tugboat_state_.last_reference_position = tugboat_state_.reference_position;
        tugboat_state_.hold_position = window_state_.mouse_position;
        tugboat_state_.is_active = true;
    }
}

void GraphState::UpdateDrag(const LPARAM& lParam) {
    if (tugboat_state_.is_active) {
        UpdateMousePosition(lParam);
        tugboat_state_.current_position = window_state_.mouse_position;
        const Scale2d pixel_scale = visible_area_ / window_state_.current_plot_size;
        Offset2d drag_offset = tugboat_state_.hold_position - tugboat_state_.current_position;
        Offset2d world_offset = pixel_scale * drag_offset;
        world_offset.invert_y();
        tugboat_state_.reference_position = tugboat_state_.last_reference_position + world_offset;
    }
}

void GraphState::StopDragging(const LPARAM& lParam) {
    UpdateDrag(lParam);
    tugboat_state_.is_active = false;
}

// Реализация масштабирования
void GraphState::ApplyZoom(ScaleDirection direction) {
    if (tugboat_state_.is_active) return;

#ifdef CURSORZOOM
    const Position2d saved_world_pos =
        ConvertToWorldCoords(window_state_.mouse_position.x, window_state_.mouse_position.y);
#else
    const Size2d saved_visible_area = visible_area_;
#endif

    if (direction == ScaleDirection::ZoomIn) {
        visible_area_ /= zoom_state_.default_scale_factor;
    }
    else {
        visible_area_ *= zoom_state_.default_scale_factor;
    }

#ifdef CURSORZOOM
    const Position2d new_world_pos =
        ConvertToWorldCoords(window_state_.mouse_position.x, window_state_.mouse_position.y);
    const Offset2d pos_offset = saved_world_pos - new_world_pos;
    tugboat_state_.reference_position += pos_offset;
#else
    const Offset2d center_offset = (saved_visible_area - visible_area_) / 2.0;
    tugboat_state_.reference_position += center_offset;
#endif

    UpdateCoordinates();
}

// Работа с размерами окна
RECT GraphState::GetPlotArea() const {
    return window_state_.plot_area;
}

Size2d GraphState::GetWindowSize() const {
    return window_state_.current_window_size;
}

void GraphState::UpdateWindowSize(const LPARAM& lParam) {
    window_state_.current_window_size.x = LOWORD(lParam);
    window_state_.current_window_size.y = HIWORD(lParam);
    window_state_.size_compression = window_state_.current_window_size / window_state_.default_window_size;

    // Область графика в координатах окна
    window_state_.plot_area = {
        static_cast<long>(window_state_.margins.left * window_state_.size_compression.x),
        static_cast<long>(window_state_.margins.top * window_state_.size_compression.y),
        static_cast<long>(window_state_.current_window_size.x - window_state_.margins.right * window_state_.size_compression.x),
        static_cast<long>(window_state_.current_window_size.y - window_state_.margins.bottom * window_state_.size_compression.y)
    };

    window_state_.plot_reference_offset = Position2d(
        window_state_.plot_area.left,
        window_state_.plot_area.bottom);

    window_state_.current_plot_size = window_state_.default_plot_size * window_state_.size_compression;
}

void GraphState::InitializeWindowSize(int width, int height) {
    window_state_.default_window_size = Size2d(width, height);
    window_state_.current_window_size = window_state_.default_window_size;
    window_state_.size_compression = Size2d(1.0, 1.0);

    window_state_.plot_area = {
        window_state_.margins.left,
        window_state_.margins.top,
        static_cast<long>(window_state_.current_window_size.x - window_state_.margins.right),
        static_cast<long>(window_state_.current_window_size.y - window_state_.margins.bottom)
    };

    window_state_.plot_reference_offset = Position2d(
        window_state_.plot_area.left,
        window_state_.plot_area.bottom);

    window_state_.default_plot_size = Size2d(
        window_state_.default_window_size.x - (window_state_.margins.left + window_state_.margins.right),
        window_state_.default_window_size.y - (window_state_.margins.bottom + window_state_.margins.top));

    window_state_.current_plot_size = window_state_.default_plot_size;
}

// Работа с координатами мыши
Position2d GraphState::GetMousePosition() const {
    return window_state_.mouse_position;
}

void GraphState::UpdateMousePosition(const LPARAM& lParam) {
    window_state_.mouse_position.x = LOWORD(lParam);
    window_state_.mouse_position.y = HIWORD(lParam);
}

void GraphState::UpdateMousePosition(double x, double y) {
    window_state_.mouse_position.x = x;
    window_state_.mouse_position.y = y;
}

// Работа с видимой областью
Size2d GraphState::GetVisibleArea() const {
    return visible_area_;
}

void GraphState::SetVisibleArea(double width, double height) {
    visible_area_.x = width;
    visible_area_.y = height;
}

// Преобразование координат
void GraphState::UpdateCoordinates() {
    current_coord_ = ConvertToWorldCoords(window_state_.mouse_position.x, window_state_.mouse_position.y);
}

Position2d GraphState::GetCurrentCoordinates() const {
    return current_coord_;
}

Position2d GraphState::ConvertToPixelCoords(double world_x, double world_y) const {
    const Position2d world_pos(world_x, world_y);
    const Offset2d world_offset = world_pos - tugboat_state_.reference_position;
    const Scale2d world_to_pixel = window_state_.current_plot_size / visible_area_;
    Offset2d pixel_offset = world_offset * world_to_pixel;
    pixel_offset.invert_y();
    return window_state_.plot_reference_offset + pixel_offset;
}

Position2d GraphState::ConvertToWorldCoords(int pixel_x, int pixel_y) const {
    const Position2d pixel_pos(pixel_x, pixel_y);
    const Offset2d pixel_offset = pixel_pos - window_state_.plot_reference_offset;
    const Scale2d pixel_to_world = visible_area_ / window_state_.current_plot_size;
    Offset2d world_offset = pixel_offset * pixel_to_world;
    world_offset.invert_y();
    return tugboat_state_.reference_position + world_offset;
}

HPEN CreateTransparentPen(COLORREF bgColor, COLORREF fgColor, double alpha) {
    alpha = max(0.0, min(1.0, alpha)); // Ограничиваем alpha в пределах [0,1]

    // Извлекаем компоненты цветов
    int bgR = GetRValue(bgColor), bgG = GetGValue(bgColor), bgB = GetBValue(bgColor);
    int fgR = GetRValue(fgColor), fgG = GetGValue(fgColor), fgB = GetBValue(fgColor);

    // Вычисляем итоговый цвет с учетом прозрачности
    int blendedR = static_cast<int>(bgR * (1 - alpha) + fgR * alpha);
    int blendedG = static_cast<int>(bgG * (1 - alpha) + fgG * alpha);
    int blendedB = static_cast<int>(bgB * (1 - alpha) + fgB * alpha);

    // Создаём перо с получившимся цветом
    return CreatePen(PS_SOLID, 1, RGB(blendedR, blendedG, blendedB));
}

// Отрисовка графика
void GraphState::RenderGraph(const HDC& hdc) {
    HPEN grid_pen;
    HPEN current_pen;
    HBRUSH current_brush;

    // Границы области рисования
    const int left = window_state_.plot_area.left;
    const int top = window_state_.plot_area.top;
    const int right = window_state_.plot_area.right;
    const int bottom = window_state_.plot_area.bottom;

    // Установка области отсечения
    HRGN clip_region = CreateRectRgn(left, top, right, bottom);
    SelectClipRgn(hdc, clip_region);

    // current lines
    const Size2d slp = axes_state_.GetOptimalGridStep(visible_area_);

    // lines > 1
    const Size2d slh = axes_state_.GetHighGridStep();

    // lines > 100
    const Size2d sll = axes_state_.GetLowGridStep();

    std::cout << "scale\t" << axes_state_.scales.x << std::endl;
    std::cout << "sl high\t" << slh.x << std::endl;
    std::cout << "sl present\t" << slp.x << std::endl;
    std::cout << "sl low\t" << sll.x << std::endl << std::endl;

    Size2d test_alpha_h = axes_state_.Alpha(visible_area_, slh);
    Size2d test_alpha_p = axes_state_.Alpha(visible_area_, slp);
    Size2d test_alpha_l = axes_state_.Alpha(visible_area_, sll);

    //std::cout << "sl high\t" << test_alpha_h.x << std::endl;
    //std::cout << "sl present\t" << test_alpha_p.x << std::endl;
    //std::cout << "sl low\t" << test_alpha_l.x << std::endl << std::endl;

    COLORREF back_color = RGB(0, 56, 89);
    COLORREF grid_color_h = RGB(250, 0, 0);
    COLORREF grid_color_p = RGB(0, 250, 0);
    COLORREF grid_color_l = RGB(0, 0, 100);
    
    // Рисование сетки
    grid_pen = CreateTransparentPen(back_color, grid_color_l, test_alpha_l.x);
    current_pen = (HPEN)SelectObject(hdc, grid_pen);
    axes_state_.DrawGrid(hdc, *this, sll, GridDrawSelection::Vertical);
    SelectObject(hdc, current_pen);
    DeleteObject(grid_pen);

    grid_pen = CreateTransparentPen(back_color, grid_color_l, test_alpha_l.y);
    current_pen = (HPEN)SelectObject(hdc, grid_pen);
    axes_state_.DrawGrid(hdc, *this, sll, GridDrawSelection::Horizontal);
    SelectObject(hdc, current_pen);
    DeleteObject(grid_pen);

    grid_pen = CreateTransparentPen(back_color, grid_color_p, test_alpha_p.x);
    current_pen = (HPEN)SelectObject(hdc, grid_pen);
    axes_state_.DrawGrid(hdc, *this, slp, GridDrawSelection::Vertical);
    SelectObject(hdc, current_pen);
    DeleteObject(grid_pen);

    grid_pen = CreateTransparentPen(back_color, grid_color_p, test_alpha_p.y);
    current_pen = (HPEN)SelectObject(hdc, grid_pen);
    axes_state_.DrawGrid(hdc, *this, slp, GridDrawSelection::Horizontal);
    SelectObject(hdc, current_pen);
    DeleteObject(grid_pen);

    grid_pen = CreateTransparentPen(back_color, grid_color_h, test_alpha_h.x);
    current_pen = (HPEN)SelectObject(hdc, grid_pen);
    axes_state_.DrawGrid(hdc, *this, slh, GridDrawSelection::Vertical);
    SelectObject(hdc, current_pen);
    DeleteObject(grid_pen);

    grid_pen = CreateTransparentPen(back_color, grid_color_h, test_alpha_h.y);
    current_pen = (HPEN)SelectObject(hdc, grid_pen);
    axes_state_.DrawGrid(hdc, *this, slh, GridDrawSelection::Horizontal);
    SelectObject(hdc, current_pen);
    DeleteObject(grid_pen);

    // Рисование рамки
    HPEN border_pen = CreatePen(PS_SOLID, 1, RGB(250, 250, 250));
    current_pen = (HPEN)SelectObject(hdc, border_pen);

    MoveToEx(hdc, left, bottom, NULL);
    LineTo(hdc, right, bottom);
    LineTo(hdc, right, top);
    LineTo(hdc, left, top);
    LineTo(hdc, left, bottom);

    // Центральная точка (только для центрального масштабирования)
#ifndef CURSORZOOM
    const long center_x = left + (right - left) / 2;
    const long center_y = top + (bottom - top) / 2;
    Ellipse(hdc, center_x - 1, center_y - 1, center_x + 1, center_y + 1);
#endif

    SelectObject(hdc, current_pen);
    DeleteObject(border_pen);

    // Рисование прямоугольника (20,20,30,30)
    HPEN rect_pen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    HBRUSH rect_brush = CreateSolidBrush(RGB(255, 0, 0)); // Закрасить красным

    current_pen = (HPEN)SelectObject(hdc, rect_pen);
    current_brush = (HBRUSH)SelectObject(hdc, rect_brush);

    Position2d test_point1 = ConvertToPixelCoords(20, 20);
    Position2d test_point2 = ConvertToPixelCoords(30, 30);

    Rectangle(hdc, test_point1.x, test_point1.y, test_point2.x, test_point2.y);

    SelectObject(hdc, current_pen);
    SelectObject(hdc, current_brush);
    DeleteObject(rect_pen);
    DeleteObject(rect_brush);

    SelectClipRgn(hdc, NULL);
    DeleteObject(clip_region);
}