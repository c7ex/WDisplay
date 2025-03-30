#ifndef WIDGETS_1337_H
#define WIDGETS_1337_H

#include <windows.h>
#include <Commctrl.h>
#include <cstdlib>

#include "Vec2d.h"

namespace widgets {
    namespace status_bar {
        const wchar_t captions[][64] =
        {
            L"Area: X: %.3f, Y: %.3f",
            L"Coord: X: %.3f, Y: %.3f",
            L"Mouse: X: %.0f, Y: %.0f",
            L"Window: W: %.0f, H: %.0f",
            L"Ref: X: %.3f, Y: %.3f"
        };

        inline void UpdateSections(HWND hwnd, int countSections, int windowWidth) {
            if ((windowWidth <= 0) || (countSections <= 0)) return;

            int lastElem = countSections - 1;
            int* container = reinterpret_cast<int*>(malloc(sizeof(int) * countSections));
            if (!container) return;  // проверка успешности выделения памяти

            int partSectionsInWindow = windowWidth / countSections;

            for (int i = 0; i < lastElem; ++i) {
                container[i] = partSectionsInWindow * (i + 1);
            }
            container[lastElem] = -1;  // последний элемент = -1

            if (container) {
                SendMessage(hwnd, SB_SETPARTS, countSections, (LPARAM)container);
                SendMessage(hwnd, WM_SIZE, 0, 0);
                free(container);
            }
        }

        inline void UpdateData(HWND hwnd, int countSections,const Vec2d* data) {
            for (int i = 0; i < countSections; ++i) {
                wchar_t status[64];
                swprintf(status, 64, captions[i], data[i].x, data[i].y);
                SendMessage(hwnd, SB_SETTEXT, i, (LPARAM)status);
            }
        }
    }
}

#endif