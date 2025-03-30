#ifndef GRAPHSTATE_H
#define GRAPHSTATE_H

#include "Vec2d.h"
#include <windows.h>
#include <vector>

// Семантические псевдонимы
using Position2d = Vec2d;  // Для координат положения
using Size2d = Vec2d;      // Для размеров
using Scale2d = Vec2d;     // Для масштабирования
using Offset2d = Vec2d;    // Для смещений

class GraphState;

enum class GridDrawSelection {
    Horizontal,
    Vertical
};

enum class AxesSelection {
    xAxes,
    yAxes
};

class AxesState {
private:
    std::vector<double> discretization_steps = { 1.0, 2.0, 5.0};
    Size2d reference_grid_lines = Size2d{ 5.0, 5.0 };

public:
    Size2d indexes = Size2d(1, 1);
    Size2d scales = Size2d(1, 1);

public:
    double FindOptimalStep(AxesSelection axis, Size2d span);
    Size2d GetOptimalGridStep(Size2d span);
    Size2d GetHighGridStep();
    Size2d GetLowGridStep();
    Size2d Alpha(Size2d span, Size2d step);
    void DrawGrid(HDC hdc, GraphState& gs, const Size2d& step, GridDrawSelection gridDrawSelection);
};

struct TugboatState {
    Position2d last_reference_position;
    Position2d reference_position{ 0.0, 0.0 };  // Явно используем Position2d
    Position2d hold_position;
    Position2d current_position;
    bool is_active = false;  // Более описательное имя
};

enum class ScaleDirection { 
    ZoomIn, 
    ZoomOut 
};

struct ZoomState {
    static constexpr double default_scale_factor = 1.07;  // Уточненное имя
};

struct WindowState {
    RECT margins = { 100, 0, 0, 0 };  // left, top, right, bottom
    RECT plot_area;
    Position2d plot_reference_offset;  // Более точное имя
    Size2d default_plot_size;
    Size2d current_plot_size;
    Size2d default_window_size;
    Size2d current_window_size;
    Scale2d size_compression;  // Используем Scale2d
    Position2d mouse_position;  // Position2d для координат
};

class GraphState {
private:
    AxesState axes_state_;
    TugboatState tugboat_state_;
    ZoomState zoom_state_;
    WindowState window_state_;

    Size2d visible_area_;
    Position2d current_coord_;

public:
    explicit GraphState();

    Position2d GetReferencePosition() const;
    void StartDragging(const LPARAM& lParam);
    void UpdateDrag(const LPARAM& lParam);
    void StopDragging(const LPARAM& lParam);

    void ApplyZoom(ScaleDirection direction);

    RECT GetPlotArea() const;
    Size2d GetWindowSize() const;
    void UpdateWindowSize(const LPARAM& lParam);
    void InitializeWindowSize(int width, int height);

    Position2d GetMousePosition() const;
    void UpdateMousePosition(const LPARAM& lParam);
    void UpdateMousePosition(double x, double y);

    Size2d GetVisibleArea() const;
    void SetVisibleArea(double width, double height);

    Position2d GetCurrentCoordinates() const;
    void UpdateCoordinates();

    Position2d ConvertToPixelCoords(double world_x, double world_y) const;
    Position2d ConvertToWorldCoords(int pixel_x, int pixel_y) const;

    void RenderGraph(const HDC& hdc);
};

#endif // GRAPHSTATE_H