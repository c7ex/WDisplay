#ifndef GRAPHSTATE_H
#define GRAPHSTATE_H

#include "Point2D.h"
#include <windows.h>
#include <vector>

struct AxesState {
    std::vector<double> discrets = { 1,2,5 };
    Point2D minimal_count_lines = Point2D{ 12., 12. };
};

struct TugboatState {
    Point2D last_reference_position;
    Point2D reference_position;
    Point2D hold_position;
    Point2D current_position;
    bool hold = false;
};

struct ZoomState {
    static constexpr double growth_default_scale = 1.07;
};

enum class ScaleDirection { Increase, Decrease };

class GraphState {
private:
    AxesState axesState;
    TugboatState tugboatState;
    ZoomState zoomState;

    Point2D window_size;
    Point2D mouse_position;

    Point2D area_span;
    Point2D coord;

public:
    explicit GraphState();

    Point2D GetReference() const;
    void BeginTugboat(const LPARAM& lParam);
    void UpdateTugboat(const LPARAM& lParam);
    void StopTugboat(const LPARAM& lParam);

    void UpdateScale(ScaleDirection ûcaleDirection);

    Point2D GetWindowSize() const;
    void UpdateWindowSize(const LPARAM& lParam);
    void UpdateWindowSize(int width, int height);

    Point2D GetMousePosition() const;
    void UpdateMousePosition(const LPARAM& lParam);
    void UpdateMousePosition(double x, double y);

    Point2D GetAreaSpan() const;
    void UpdateAreaSpan(int x_size, int y_size);

    Point2D GetCoord() const;
    void UpdateCoord();

    Point2D Abstruct2Pixel(double x, double y) const;
    Point2D Pixel2Abstruct(int x, int y) const;

    Point2D GetOptimalDiscret();

    void DrawGraph(const HDC& hdc);
};

#endif // GRAPHSTATE_H
