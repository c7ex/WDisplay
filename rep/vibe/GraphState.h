#ifndef GRAPHSTATE_H
#define GRAPHSTATE_H

#include "Point2D.h"
#include <windows.h>
#include <vector>

enum class AxesSelection { xAxes, yAxes };

class AxesState {
private:
    std::vector<double> discrets = { 1,2,5 };
    Point2D minimal_count_lines = Point2D{ 12., 12. };

    double SearchOptimalDiscret(AxesSelection axesSelection, Point2D span);

public:
    Point2D GetOptimalDiscret(Point2D span);
};

struct TugboatState {
    Point2D last_reference_position;
    Point2D reference_position{ 0, 0 };
    Point2D hold_position;
    Point2D current_position;
    bool hold = false;
};

enum class ScaleDirection { Increase, Decrease };

struct ZoomState {
    static constexpr double growth_default_scale = 1.07;
};

struct WindowState {
    RECT margin = { 50, 20, 20, 100 }; // left, top, right, bottom
    RECT points_plot;
    Point2D reference_window_offset;
    Point2D reference_abstruct_value;
    Point2D size_plot_default;
    Point2D size_plot_current;
    Point2D size_default;
    Point2D size_current;
    Point2D size_compress;
    Point2D mouse_position;
};

class GraphState {
private:
    AxesState axesState;
    TugboatState tugboatState;
    ZoomState zoomState;
    WindowState windowState;

    Point2D area_span;
    Point2D coord;

public:
    explicit GraphState();

    Point2D GetReference() const;
    void BeginTugboat(const LPARAM& lParam);
    void UpdateTugboat(const LPARAM& lParam);
    void StopTugboat(const LPARAM& lParam);
    void InstallReference();

    void UpdateScale(ScaleDirection ûcaleDirection);

    Point2D GetWindowSize() const;
    void UpdateWindowSize(const LPARAM& lParam);
    void SetWindowDefaultSize(int width, int height);

    Point2D GetMousePosition() const;
    void UpdateMousePosition(const LPARAM& lParam);
    void UpdateMousePosition(double x, double y);

    Point2D GetAreaSpan() const;
    void UpdateAreaSpan(int x_size, int y_size);

    Point2D GetCoord() const;
    void UpdateCoord();

    Point2D Abstruct2Pixel(double x, double y) const;
    Point2D Pixel2Abstruct(int x, int y) const;

    void DrawGraph(const HDC& hdc);
};

#endif // GRAPHSTATE_H
