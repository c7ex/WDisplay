#pragma once
#include"packs.hpp"
#include"rendering.hpp"

LRESULT MainGraphicHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_MOUSEWHEEL:
		{
			if (gc.mouse.is_hold == true)
				return 0;

			if (key_is_active::shift)
			{
				gc.scales.width_counter -= GET_WHEEL_DELTA_WPARAM(wParam) / 120.;
				gc.update_scale_w();
			}
			else
			{
				gc.scales.total_counter -= GET_WHEEL_DELTA_WPARAM(wParam) / 120.;
				gc.update_scale();
			}

			return 0;
		}

		case WM_LBUTTONDOWN:
		{
			gc.mouse.is_hold = true;
			gc.update_hold();
			//SetCapture(hWnd);
			return 0;
		}

		case WM_LBUTTONUP:
		{
			gc.mouse.is_hold = false;
			//ReleaseCapture();
			return 0;
		}

		case WM_MOUSEMOVE:
		{
			gc.mouse.current_position.x = LOWORD(lParam);
			gc.mouse.current_position.y = HIWORD(lParam);

			if (gc.mouse.is_hold == true)
			{
				gc.update_shift();
				gc.update_reference_point();
			}
			return 0;
		}

		case WM_SIZE:
		{
			RECT rt;
			GetWindowRect(hWnd, &rt);

			gc.window.count_pixels.x = (rt.right - rt.left) - GRAPHICS_CORE_WINDOW_CORRECTION_WIDTH;
			gc.window.count_pixels.y = (rt.bottom - rt.top) - GRAPHICS_CORE_WINDOW_CORRECTION_HEIGHT;

			gc.update_expand_scale();

			return 0;
		}

		case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_SHIFT:
				key_is_active::shift = true;
				return 0;
			}
		}

		case WM_KEYUP:
		{
			switch (wParam)
			{
			case VK_SHIFT:
				key_is_active::shift = false;
				return 0;
			}
		}
	}

	return 0;
}