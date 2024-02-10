#pragma once

// command list
#define WM_COMMAND_BUTTON 0

// command handler
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
			return 0;;
		}
	}
}