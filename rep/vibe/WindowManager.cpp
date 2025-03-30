#include "WindowManager.h"
#include <iostream>
#include "widgets.h"

void UpdateStatusBar(HWND statusBar, GraphState* graphState) {
    const Size2d windowSize = graphState->GetWindowSize();
    const Position2d mousePos = graphState->GetMousePosition();
    const Size2d visibleArea = graphState->GetVisibleArea();
    const Position2d worldCoord = graphState->GetCurrentCoordinates();
    const Position2d referencePos = graphState->GetReferencePosition();

    const Vec2d statusData[] = {
        visibleArea,       // Размер видимой области
        worldCoord,        // Текущие мировые координаты
        mousePos,          // Позиция мыши в пикселях
        windowSize,        // Размер окна
        referencePos       // Референсная позиция
    };

    widgets::status_bar::UpdateData(statusBar, 5, statusData);
}

void RepaintGraphArea(HWND hwnd, HWND statusBar) {
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);

    RECT statusRect;
    GetWindowRect(statusBar, &statusRect);
    MapWindowPoints(nullptr, hwnd, reinterpret_cast<POINT*>(&statusRect), 2);

    const int statusHeight = statusRect.bottom - statusRect.top;
    clientRect.bottom -= statusHeight;

    InvalidateRect(hwnd, &clientRect, FALSE);
}

WindowManager::WindowManager(const std::wstring& className)
    : hwnd_(nullptr),
    hInstance_(GetModuleHandle(nullptr)),
    className_(className),
    statusBar_(nullptr) {

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowManager::WindowProc;
    wc.hInstance = hInstance_;
    wc.lpszClassName = className_.c_str();
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);

    if (!RegisterClass(&wc)) {
        MessageBox(nullptr, L"Window class registration failed!", L"Error", MB_ICONERROR);
    }
}

WindowManager::~WindowManager() {
    if (statusBar_) DestroyWindow(statusBar_);
    if (hwnd_) DestroyWindow(hwnd_);
    UnregisterClass(className_.c_str(), hInstance_);
}

bool WindowManager::Create(const wchar_t* title, int width, int height) {
    hwnd_ = CreateWindowEx(
        0, className_.c_str(), title, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        nullptr, nullptr, hInstance_, this
    );

    if (!hwnd_) {
        MessageBox(nullptr, L"Window creation failed!", L"Error", MB_ICONERROR);
        return false;
    }

    statusBar_ = CreateWindowEx(
        0, STATUSCLASSNAME, nullptr,
        WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
        0, 0, 0, 0, hwnd_, nullptr, hInstance_, nullptr
    );

    if (!statusBar_) {
        MessageBox(nullptr, L"Status bar creation failed!", L"Error", MB_ICONERROR);
    }

    graphState_.InitializeWindowSize(width, height);
    graphState_.SetVisibleArea(60, 200);

    UpdateStatusBar(statusBar_, &graphState_);
    widgets::status_bar::UpdateSections(statusBar_, 5, width);

    ShowWindow(hwnd_, SW_SHOW);
    UpdateWindow(hwnd_);
    return true;
}

void WindowManager::Run() {
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK WindowManager::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    WindowManager* manager = reinterpret_cast<WindowManager*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    if (msg == WM_NCCREATE) {
        CREATESTRUCT* create = reinterpret_cast<CREATESTRUCT*>(lParam);
        manager = reinterpret_cast<WindowManager*>(create->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(manager));
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    if (!manager) {
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT clientRect;
        GetClientRect(hwnd, &clientRect);

        // Двойная буферизация
        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP memBmp = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
        HBITMAP oldBmp = static_cast<HBITMAP>(SelectObject(memDC, memBmp));

        // Очистка фона
        FillRect(memDC, &clientRect, CreateSolidBrush(RGB(0, 56, 89)));

        // Отрисовка графика
        manager->graphState_.RenderGraph(memDC);

        // Копирование на экран
        BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, memDC, 0, 0, SRCCOPY);

        // Очистка
        SelectObject(memDC, oldBmp);
        DeleteObject(memBmp);
        DeleteDC(memDC);

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_ERASEBKGND:
        return 1;

    case WM_SIZE:
        manager->graphState_.UpdateWindowSize(lParam);
        UpdateStatusBar(manager->statusBar_, &manager->graphState_);
        widgets::status_bar::UpdateSections(manager->statusBar_, 5,
            static_cast<int>(manager->graphState_.GetWindowSize().x));
        RepaintGraphArea(hwnd, manager->statusBar_);
        return 0;

    case WM_MOUSEMOVE:
        manager->graphState_.UpdateMousePosition(lParam);
        manager->graphState_.UpdateDrag(lParam);
        manager->graphState_.UpdateCoordinates();
        UpdateStatusBar(manager->statusBar_, &manager->graphState_);
        RepaintGraphArea(hwnd, manager->statusBar_);
        return 0;

    case WM_LBUTTONDOWN:
        manager->graphState_.StartDragging(lParam);
        return 0;

    case WM_LBUTTONUP:
        manager->graphState_.StopDragging(lParam);
        return 0;

    case WM_MOUSEWHEEL: {
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        ScreenToClient(hwnd, &pt);
        manager->graphState_.UpdateMousePosition(pt.x, pt.y);

        const ScaleDirection direction = (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
            ? ScaleDirection::ZoomOut
            : ScaleDirection::ZoomIn;

        manager->graphState_.ApplyZoom(direction);

        UpdateStatusBar(manager->statusBar_, &manager->graphState_);
        RepaintGraphArea(hwnd, manager->statusBar_);
        return 0;
    }
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}