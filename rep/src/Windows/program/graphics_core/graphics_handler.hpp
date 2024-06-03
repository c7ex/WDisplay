#pragma once
#include"packs.hpp"
#include"rendering.hpp"

LRESULT MainGraphicHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_MOUSEWHEEL:
		{

			return 0;
		}

		case WM_LBUTTONDOWN:
		{

			return 0;
		}

		case WM_LBUTTONUP:
		{

			return 0;
		}

		case WM_MOUSEMOVE:
		{
			gc.window_mouse_position_in_pixels.set(LOWORD(lParam), HIWORD(lParam));

			return 0;
		}

		case WM_SIZE:
		{
			RECT rt;
			GetWindowRect(hWnd, &rt);

			double window_size_x = rt.right - rt.left - 17;
			double window_size_y = rt.bottom - rt.top - 38;

			gc.window_size_in_pixels.set(
				window_size_x,
				window_size_y);

			gc.compression_factors.set(
				window_size_x / DEFAULT_SIZE_WINDOW_X,
				window_size_y / DEFAULT_SIZE_WINDOW_Y);

			gc.ch.update();

			return 0;
		}

		case WM_KEYDOWN:
		{
			switch (wParam)
			{
				case VK_SHIFT:
				{
					keys::shift = true;
					return 0;
				}

				case VK_CONTROL:
				{
					keys::ctrl = true;
					return 0;
				}
			}
		}

		case WM_KEYUP:
		{
			switch (wParam)
			{
				case VK_SHIFT:
				{
					keys::shift = false;
					return 0;
				}

				case VK_CONTROL:
				{
					keys::ctrl = false;
					return 0;
				}
			}
		}
	}

	return 0;
}