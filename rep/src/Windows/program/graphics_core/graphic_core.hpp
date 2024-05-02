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

			if (active_shift_key)
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
			GetWindowRect(hWnd, &gl_paint::rt);

			gl_windows::width = (gl_paint::rt.right - gl_paint::rt.left) - GRAPHICS_CORE_WINDOW_CORRECTION_WIDTH;
			gl_windows::height = (gl_paint::rt.bottom - gl_paint::rt.top) - GRAPHICS_CORE_WINDOW_CORRECTION_HEIGHT;

			gc.update_expand_scale();

			return 0;
		}

		case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_SHIFT:
				active_shift_key = true;
				return 0;
			}
		}

		case WM_KEYUP:
		{
			switch (wParam)
			{
			case VK_SHIFT:
				active_shift_key = false;
				return 0;
			}
		}
	}

	return 0;
}