#include "WindowManager.h"
#include <iostream>
#include "widgets.h"

void UpdateStatusBar(HWND statusBar, GraphState* GraphState) {
    Point2D WindowSize = GraphState->GetWindowSize();
    Point2D MousePosition = GraphState->GetMousePosition();
    Point2D AreaSpan = GraphState->GetAreaSpan();
    Point2D Coord = GraphState->GetCoord();
    Point2D Ref = GraphState->GetReference();
    Point2D Data[] = { AreaSpan, Coord, MousePosition, WindowSize, Ref };
    widgets::status_bar::UpdateData(statusBar, 5, Data);
}

void RepaintGraphArea(HWND hwnd, HWND statusBar) {
    RECT graphRect;
    GetClientRect(hwnd, &graphRect);
    RECT statusRect;
    GetWindowRect(statusBar, &statusRect);
    MapWindowPoints(NULL, hwnd, (LPPOINT)&statusRect, 2);
    int statusBarHeight = statusRect.bottom - statusRect.top;
    graphRect.bottom -= statusBarHeight;
    InvalidateRect(hwnd, &graphRect, FALSE);
}

WindowManager::WindowManager(const std::wstring& className)
    : hwnd(NULL), hInstance(GetModuleHandle(NULL)), className(className) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowManager::WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className.c_str();
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClass(&wc)) {
        MessageBox(NULL, L"Ошибка регистрации класса окна!", L"Ошибка", MB_ICONERROR);
    }
}

WindowManager::~WindowManager() {
    if (hStatusBar) {
        DestroyWindow(hStatusBar);
        hStatusBar = NULL;
    }
    if (hwnd) {
        DestroyWindow(hwnd);
        hwnd = NULL;
    }
    
    UnregisterClass(className.c_str(), hInstance);
}

bool WindowManager::Create(const wchar_t* title, int width, int height) {
    hwnd = CreateWindowEx(
        0, className.c_str(), title, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL, NULL, hInstance, this
    );

    if (hwnd == NULL) {
        MessageBox(NULL, L"Ошибка создания окна!", L"Ошибка", MB_ICONERROR);
        return false;
    }

    hStatusBar = CreateWindowEx(
        0, STATUSCLASSNAME, NULL,
        WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
        0, 0, 0, 0, hwnd, NULL, hInstance, NULL
    );

    if (!hStatusBar) {
        MessageBox(NULL, L"Ошибка создания статус-бара!", L"Ошибка", MB_ICONERROR);
    }

    Point2D size(width, height);
    graphState.UpdateWindowSize(width, height);
    graphState.UpdateAreaSpan(1, 1);

    UpdateStatusBar(hStatusBar, &graphState);

    widgets::status_bar::UpdateSections(hStatusBar, 5, graphState.GetWindowSize().x);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    return true;
}

void WindowManager::Run() {
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK WindowManager::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    WindowManager* pThis = nullptr;
    GraphState* pGraphState = nullptr;

    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<WindowManager*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    else {
        pThis = reinterpret_cast<WindowManager*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        pGraphState = reinterpret_cast<GraphState*>(&(pThis->graphState));
    }

    if (pThis == nullptr && uMsg != WM_NCCREATE) {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    if (pGraphState == nullptr){
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    switch (uMsg)
    {
        case WM_DESTROY:
            if (pThis->hStatusBar) {
                DestroyWindow(pThis->hStatusBar);
                pThis->hStatusBar = NULL;
            }
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Получаем размеры всей клиентской области
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);

            // Создаем буфер в памяти
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
            HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

            // Очищаем фон в буфере
            HBRUSH hBrush = CreateSolidBrush(RGB(0, 56, 89));  // Темно-серый
            FillRect(memDC, &clientRect, hBrush);
            DeleteObject(hBrush);  // Удаляем кисть после использования

            // Рисуем график в буфере
            pGraphState->DrawGraph(memDC);

            // Переносим результат на экран
            BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, memDC, 0, 0, SRCCOPY);

            // Очистка
            SelectObject(memDC, oldBitmap);
            DeleteObject(memBitmap);
            DeleteDC(memDC);

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_ERASEBKGND:
            return 1; // Предотвращаем стандартную очистку, чтобы избежать мерцания


        case WM_SIZE:
        {
            pGraphState->UpdateWindowSize(lParam);
            pGraphState->UpdateMousePosition(lParam);
            pGraphState->UpdateCoord();

            UpdateStatusBar(pThis->hStatusBar, pGraphState);
            widgets::status_bar::UpdateSections(pThis->hStatusBar, 5, pGraphState->GetWindowSize().x);
            RepaintGraphArea(hwnd, pThis->hStatusBar);
            return 0;
        }

        case WM_MOUSEMOVE:
        {
            pGraphState->UpdateMousePosition(lParam);
            pThis->graphState.UpdateTugboat(lParam);
            pGraphState->UpdateCoord();
            UpdateStatusBar(pThis->hStatusBar, pGraphState);
            widgets::status_bar::UpdateSections(pThis->hStatusBar, 5, pGraphState->GetWindowSize().x);
            RepaintGraphArea(hwnd, pThis->hStatusBar);
            return 0;
        }

        case WM_LBUTTONDOWN:
        {
            pThis->graphState.BeginTugboat(lParam);
            return 0;
        }

        case WM_LBUTTONUP:
        {
            pThis->graphState.StopTugboat(lParam);
            return 0;
        }

        case WM_MOUSEWHEEL:
        {
            // start hardcode
            // lParam(window) -> lParam(client)
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            POINT pt = { x, y };
            ScreenToClient(hwnd, &pt);
            pThis->graphState.UpdateMousePosition(pt.x, pt.y);
            // end hardcode

            pThis->graphState.UpdateScale(
                GET_WHEEL_DELTA_WPARAM(wParam) < 0 ? 
                ScaleDirection::Decrease: 
                ScaleDirection::Increase);

            UpdateStatusBar(pThis->hStatusBar, pGraphState);
            widgets::status_bar::UpdateSections(pThis->hStatusBar, 5, pGraphState->GetWindowSize().x);
            RepaintGraphArea(hwnd, pThis->hStatusBar);
            return 0;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}