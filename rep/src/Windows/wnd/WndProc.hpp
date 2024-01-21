#pragma once
#include"WndWidg.hpp"

// message handler
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{


	switch (message)
	{
		case WM_CREATE:
		{
			CreateWidgets(hWnd, message, wParam, lParam);
			return static_cast<LRESULT>(0);
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return static_cast<LRESULT>(0);
		}

		case WM_MOUSEMOVE:
		{
			xPos = LOWORD(lParam);
			yPos = HIWORD(lParam);
			InvalidateRect(hWnd, NULL, true);
		}

		case WM_PAINT: // graphics
		{
			return WmPaint(hWnd, message, wParam, lParam);
		}

		case WM_COMMAND: // events
		{
			return WmProc(hWnd, message, wParam, lParam);
		}

		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}

	return static_cast<LRESULT>(0);
}