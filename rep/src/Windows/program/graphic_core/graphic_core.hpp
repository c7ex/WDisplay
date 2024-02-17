#pragma once
#include"packs.hpp"
#include"rendering.hpp"

LRESULT MainGraphicHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_MOUSEWHEEL:
		{
			if (gl_mouse::hold == true)
				return 0;

			if (active_shift_key)
			{
				gl_wheel::counter_w -= GET_WHEEL_DELTA_WPARAM(wParam) / 120.;
				gc.update_scale_w();
			}
			else
			{
				gl_wheel::counter -= GET_WHEEL_DELTA_WPARAM(wParam) / 120.;
				gc.update_scale();
			}

			return 0;
		}

		case WM_LBUTTONDOWN:
		{
			gl_mouse::hold = true;
			gc.update_hold();
			//SetCapture(hWnd);
			return 0;
		}

		case WM_LBUTTONUP:
		{
			gl_mouse::hold = false;
			//ReleaseCapture();
			return 0;
		}

		case WM_MOUSEMOVE:
		{
			gl_mouse::position_x = LOWORD(lParam);
			gl_mouse::position_y = HIWORD(lParam);

			if (gl_mouse::hold == true)
			{
				gc.update_shift();
				gc.update_reference_point();
			}
			return 0;
		}

		case WM_SIZE:
		{
			GetWindowRect(hWnd, &gl_paint::rt);
			gl_windows::width = (gl_paint::rt.right - gl_paint::rt.left) - 16;
			gl_windows::height = (gl_paint::rt.bottom - gl_paint::rt.top) - 39;

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