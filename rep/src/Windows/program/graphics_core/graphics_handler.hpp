#pragma once
#include"packs.hpp"
#include"rendering.hpp"

LRESULT MainGraphicHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_MOUSEWHEEL:
		{
			if (keys::mouse == true) return 0;

			double val = GET_WHEEL_DELTA_WPARAM(wParam)/ -120.;

			pointf pos = wds.form().mouse();
			paramf last_abstr_expand = wds.plot().abstruct_expand;
			paramf last_abstr_centre = wds.plot().abstruct_centre;
			paramf last_chart_centre = wds.plot().get_chart_centre();

			wds.plot().abstruct_size = wds.plot().abstruct_size / wds.plot().state_scale;

			wds.plot().state_scale_counter = wds.plot().state_scale_counter + val;
			wds.plot().state_scale.set(
				pow(1.05, wds.plot().state_scale_counter.get_x()), 
				pow(1.05, wds.plot().state_scale_counter.get_y()));

			wds.plot().abstruct_size = wds.plot().abstruct_size * wds.plot().state_scale;
			wds.plot().abstruct_expand = wds.plot().abstruct_size / wds.plot().get_chart_size();

			paramf delta_expand = last_abstr_expand - wds.plot().abstruct_expand;
			paramf curr_pixel_offset = pos - wds.plot().get_chart_centre();

			wds.plot().abstruct_centre = paramf{
				curr_pixel_offset.get_x() * delta_expand.get_x() + last_abstr_centre.get_x(),
				curr_pixel_offset.get_y() * delta_expand.get_y() + last_abstr_centre.get_y()};

			wds.plot().state_last_offset = wds.plot().abstruct_centre;

			return 0;
		}

		case WM_LBUTTONDOWN:
		{
			pointf current_mouse_position = pointf(LOWORD(lParam), HIWORD(lParam));
			wds.plot().state_hold = current_mouse_position;
			keys::mouse = true;

			return 0;
		}

		case WM_LBUTTONUP:
		{
			pointf current_mouse_position = pointf(LOWORD(lParam), HIWORD(lParam));
			wds.plot().state_last_offset = wds.plot().abstruct_centre;
			keys::mouse = false;

			return 0;
		}

		case WM_MOUSEMOVE:
		{
			pointf current_mouse_position = pointf(LOWORD(lParam), HIWORD(lParam));

			wds.form().update_mouse(current_mouse_position);

			if (keys::mouse == true)
			{
				wds.plot().state_current_offset = wds.plot().state_last_offset + (paramf{wds.plot().state_hold - current_mouse_position}) * wds.plot().abstruct_expand;
				wds.plot().abstruct_centre = wds.plot().state_current_offset;
			}

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