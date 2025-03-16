#include "WindowManager.h"

#include <iostream>
#include <thread>
#include <vector>
#include <string>

// Функция для создания и запуска окна в отдельном потоке
void CreateAndRunWindow(WindowManager& window, const std::wstring& title, int width, int height) {
    if (window.Create(title.c_str(), width, height)) {
        window.Run(); // Запускаем цикл обработки сообщений
    }
}

void CallWindows()
{
    int windowCount = 2; // Например, создадим 3 окна

       // Вектор для хранения объектов WindowManager
    std::vector<WindowManager> windows;
    windows.reserve(windowCount); // Резервируем память

    // Вектор для хранения потоков
    std::vector<std::thread> threads;
    threads.reserve(windowCount); // Резервируем память

    // Создаем окна и потоки
    for (int i = 0; i < windowCount; ++i) {
        // Уникальное имя класса для каждого окна
        std::wstring className = L"WindowClass" + std::to_wstring(i + 1);

        // Создаем объект WindowManager
        windows.emplace_back(className);

        // Заголовок окна в формате "Window [X]"
        std::wstring title = L"Window [" + std::to_wstring(i + 1) + L"]";

        // Создаем поток для окна
        threads.emplace_back(CreateAndRunWindow, std::ref(windows[i]), title, 1000, 600);
    }

    // Ждем завершения всех потоков
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