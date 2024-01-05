#pragma once
#include"WmList.hpp"
#include"string"

// WM_COMMAND handler
LRESULT WmProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		case WM_COMMAND_BUTTON:
		{
			std::string r = std::to_string(rand());
			SetWindowTextA(hLable, (r.c_str()));
			return static_cast<LRESULT>(0);
		}

		default:
		{
			return static_cast<LRESULT>(0);
		}
	}
}