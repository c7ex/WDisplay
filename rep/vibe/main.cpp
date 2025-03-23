#include "WindowManager.h"

#include <iostream>
#include <thread>
#include <vector>
#include <string>

void CreateAndRunWindow(WindowManager& window, const std::wstring& title, int width, int height) {
    if (window.Create(title.c_str(), width, height)) {
        window.Run();
    }
}

void CallWindows()
{
    int windowCount = 2;

    std::vector<WindowManager> windows;
    windows.reserve(windowCount);

    std::vector<std::thread> threads;
    threads.reserve(windowCount);

    for (int i = 0; i < windowCount; ++i) {
        std::wstring className = L"WindowClass" + std::to_wstring(i + 1);

        windows.emplace_back(className);

        std::wstring title = L"Window [" + std::to_wstring(i + 1) + L"]";

        threads.emplace_back(CreateAndRunWindow, std::ref(windows[i]), title, 1000, 600);
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

int main() {

    int a = 1;

    for (;;)
    {
        if (a)
            CallWindows();
        else
            break;

        std::cin >> a;
    }

    return 0;
}