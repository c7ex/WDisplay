#ifndef OTHERg_H
#define OTHERg_H

Size2d CalculateScaleLevel(Size2d area) {
    return Size2d{
        std::floor(log10(area.x)),
        std::floor(log10(area.y))
    };
}

double AxesState::FindOptimalStep(AxesSelection axis, Size2d span) {
    double current_span = (axis == AxesSelection::xAxes) ? span.x : span.y;
    double min_lines = (axis == AxesSelection::xAxes) ? reference_grid_lines.x : reference_grid_lines.y;

    double level = ceil(log10(current_span)) - 1;
    double scale = pow(10, level);

    auto steps = discretization_steps;
    for (auto& step : steps) step *= scale;

    size_t index = steps.size() - 1;
    while (true) {
        double lines = current_span / steps[index];
        if (lines >= min_lines) break;

        if (index-- == 0) {
            index = steps.size() - 1;
            scale /= 10;
            for (auto& step : steps) step /= 10;
        }
    }

    if (axis == AxesSelection::xAxes) { indexes.x = index; scales.x = scale; }
    if (axis == AxesSelection::yAxes) { indexes.y = index; scales.y = scale; }

    return steps[index];
}

Size2d AxesState::GetOptimalGridStep(Size2d span) {
    return Size2d(
        FindOptimalStep(AxesSelection::xAxes, span),
        FindOptimalStep(AxesSelection::yAxes, span)
    );
}

Size2d AxesState::GetHighGridStep() {
    auto steps = discretization_steps;
    size_t last_index = steps.size() - 1;

    Size2d ind = indexes;
    Size2d scl = scales;

    ind.x++;
    if (ind.x > last_index) {
        ind.x = 0;
        scl.x *= 10;
    }

    ind.y++;
    if (ind.y > last_index) {
        ind.y = 0;
        scl.y *= 10;
    }

    ind.x = discretization_steps[ind.x];
    ind.y = discretization_steps[ind.y];

    Size2d step = ind * scl;

    return step;
}

Size2d AxesState::GetLowGridStep() {
    auto steps = discretization_steps;
    size_t last_index = steps.size() - 1;

    Size2d ind = indexes;
    Size2d scl = scales;

    ind.x--;
    if (ind.x < 0) {
        ind.x = last_index;
        scl.x /= 10;
    }

    ind.y--;
    if (ind.y < 0) {
        ind.y = last_index;
        scl.y /= 10;
    }

    ind.x = discretization_steps[ind.x];
    ind.y = discretization_steps[ind.y];

    Size2d step = ind * scl;

    return step;
}

void AxesState::DrawGrid(HDC hdc, GraphState& gs, const Size2d &step, 
    GridDrawSelection gridDrawSelection) {

    Position2d ref = gs.GetReferencePosition();
    Size2d area = gs.GetVisibleArea();
    RECT plot = gs.GetPlotArea();

    const double x_min = ref.x;
    const double x_max = ref.x + area.x;
    const double y_min = ref.y;
    const double y_max = ref.y + area.y;

    const int left = plot.left;
    const int top = plot.top;
    const int right = plot.right;
    const int bottom = plot.bottom;

    if (gridDrawSelection == GridDrawSelection::Vertical) {
        for (double x = std::floor(x_min / step.x) * step.x; x <= x_max; x += step.x) {
            const Position2d p1 = gs.ConvertToPixelCoords(x, y_min);
            const Position2d p2 = gs.ConvertToPixelCoords(x, y_max);
            if (p1.x >= left && p1.x <= right) {
                MoveToEx(hdc, p1.x, top, NULL);
                LineTo(hdc, p2.x, bottom);
            }
        }
    }

    if (gridDrawSelection == GridDrawSelection::Horizontal) {
        for (double y = std::floor(y_min / step.y) * step.y; y <= y_max; y += step.y) {
            const Position2d p1 = gs.ConvertToPixelCoords(x_min, y);
            const Position2d p2 = gs.ConvertToPixelCoords(x_max, y);
            if (p1.y >= top && p1.y <= bottom) {
                MoveToEx(hdc, left, p1.y, NULL);
                LineTo(hdc, right, p2.y);
            }
        }
    }
}

Size2d AxesState::Alpha(Size2d span, Size2d step) {
    Size2d alpha(0, 0);
    Size2d lines = span / step;
    
    alpha = Size2d(
        1. - (sqrt(abs(lines.x - reference_grid_lines.x)) / 4.5),
        1. - (sqrt(abs(lines.y - reference_grid_lines.y)) / 4.5));

    if (lines.x <= reference_grid_lines.x) alpha.x = 1.;
    if (lines.y <= reference_grid_lines.y) alpha.y = 1.;

    if (alpha.x <= 0) alpha.x = 0;
    if (alpha.y <= 0) alpha.y = 0;

    return alpha;
}

#endif