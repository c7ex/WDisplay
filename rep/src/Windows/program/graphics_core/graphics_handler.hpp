#pragma once
#include"packs.hpp"
#include"rendering.hpp"

LRESULT MainGraphicHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_MOUSEWHEEL:
		{
			double inc = GET_WHEEL_DELTA_WPARAM(wParam)/ -120.;
			wds.chart().get_engine().event_rescale(hd_form_mouse, inc);
			return 0;
		}

		case WM_LBUTTONDOWN:
		{
			pointf current_mouse_position = pointf(LOWORD(lParam), HIWORD(lParam));
			Engine_HoldStart(current_mouse_position);
			return 0;
		}

		case WM_LBUTTONUP:
		{
			Engine_HoldStop;
			return 0;
		}

		case WM_MOUSEMOVE:
		{
			pointf current_mouse_position = pointf(LOWORD(lParam), HIWORD(lParam));

			wds.form().update_mouse(current_mouse_position);
			wds.chart().get_engine().event_mousemove(hd_form_mouse);
			return 0;
		}

		case WM_SIZE:
		{
			RECT rt;
			GetWindowRect(hWnd, &rt);

			double form_size_x = rt.right - rt.left - 16;
			double form_size_y = rt.bottom - rt.top - 38;

			paramf current_form_size = paramf(form_size_x, form_size_y);
			wds.form().update_size(current_form_size);
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