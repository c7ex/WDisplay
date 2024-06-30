#pragma once
#include"wds_events.hpp"
#include"widgets.hpp"

# define WM_CORE 	WM_MOUSEWHEEL: case WM_LBUTTONDOWN: case WM_LBUTTONUP: case WM_MOUSEMOVE: case WM_SIZE: case WM_KEYDOWN: case WM_KEYUP

LRESULT CALLBACK MainMessageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
			lpMMI->ptMinTrackSize.x = 300;
			lpMMI->ptMinTrackSize.y = 300;
		}

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
			return wds_events(hWnd, message, wParam, lParam);
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
			return paint_frame(hWnd, message, wParam, lParam);
		}

		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}

	return 0;
}