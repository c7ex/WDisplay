#pragma once
#include"WmList.hpp"

// event handler
LRESULT WmProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		case WM_COMMAND_BUTTON:
		{
			SetWindowTextA(hLable, "Lable");
			return static_cast<LRESULT>(0);
		}

		default:
		{
			return static_cast<LRESULT>(0);
		}
	}
}