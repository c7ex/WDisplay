#pragma once

#define WM_COMMAND_BUTTON 0

LRESULT events(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		case WM_COMMAND_BUTTON:
		{
			return 0;
		}

		default:
		{
			return 0;
		}
	}
}