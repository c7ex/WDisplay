#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include "GraphState.h"
#include <windowsx.h>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <commctrl.h>
#pragma comment(lib, "comctl32.lib") 

// WindowManager.h (פנאדלוםע)
class WindowManager {
private:
    HWND hwnd;
    HINSTANCE hInstance;
    std::wstring className;
    HWND hStatusBar = NULL;

    GraphState graphState;

public:
    WindowManager(const std::wstring& className);
    ~WindowManager();
    bool Create(const wchar_t* title, int width, int height);
    void Run();
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif