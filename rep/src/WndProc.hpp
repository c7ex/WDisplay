#pragma once
#include"CreateWidgets.hpp"

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

		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}

	return static_cast<LRESULT>(0);
}