#pragma once
#include"WmList.hpp"

// WM_COMMAND handler
LRESULT WmProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		case WM_COMMAND_BUTTON:
		{
			MessageBox(hWnd, TEXT(""), TEXT(""), 0);
			return static_cast<LRESULT>(0);
		}

		default:
		{
			return static_cast<LRESULT>(0);
		}
	}
}