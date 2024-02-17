#pragma once
#include"graphic_core.hpp"
#include"widgets.hpp"

# define WM_CORE 	WM_MOUSEWHEEL: case WM_LBUTTONDOWN: case WM_LBUTTONUP: case WM_MOUSEMOVE: case WM_SIZE: case WM_KEYDOWN: case WM_KEYUP

LRESULT CALLBACK MainMessageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
		{
			CreateWidgets(hWnd, message, wParam, lParam);
			return 0;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_CORE: // update state graphic
		{
			return MainGraphicHandler(hWnd, message, wParam, lParam);
		}

		case WM_COMMAND: // events widgets
		{
			return events(hWnd, message, wParam, lParam);
		}
		
		case WM_TIMER: // repaint from timer event
		{
			InvalidateRect(hWnd, NULL, FALSE);
		}

		case WM_PAINT: // repaint from paint event
		{
			return rendering(hWnd, message, wParam, lParam);
		}

		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}

	return 0;
}