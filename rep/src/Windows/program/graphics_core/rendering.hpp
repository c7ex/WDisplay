#pragma once

double quantum(double number)
{
	double part = number / 10;
	//std::cout << part << std::endl;

	double ecl = round(log10(part));
	//std::cout << ecl << std::endl;

	double quant = pow(10, ecl);
	//std::cout << quant << std::endl;

	double lines = number / quant;

	while ((lines > 20) || (lines < 10))
	{
		if (lines >= 20)
		{
			quant *= 10;
		}
		else if (lines <= 10)
		{
			quant /= 2;
		}
		lines = (number / quant);
		//std::cout << std::endl << "r" << lines << std::endl;
	}

	return quant;
}

void axis_label_x(HDC& hMemDc, double a_pos_x, double a_pos_y, double var)
{
	double m_pos_x = get_mouse_coordinate_x(a_pos_x);
	double m_pos_y = get_mouse_coordinate_y(a_pos_y);

	if (m_pos_y < 5)
		m_pos_y = 5;
	else if (m_pos_y > gc.window.count_pixels.y - 15)
		m_pos_y = gc.window.count_pixels.y - 15;

	me_setText(hMemDc, m_pos_x, m_pos_y, var);
}

void axis_label_y(HDC& hMemDc, double a_pos_x, double a_pos_y, double var)
{
	if (var == 0) return;

	double m_pos_x = get_mouse_coordinate_x(a_pos_x);
	double m_pos_y = get_mouse_coordinate_y(a_pos_y);

	if (m_pos_x < 5)
		m_pos_x = 5;
	else if (m_pos_x > gc.window.count_pixels.x - 15)
		m_pos_x = gc.window.count_pixels.x - 15;

	me_setText(hMemDc, m_pos_x, m_pos_y, var);
}

void hMainAxis(HDC& hMemDc, double y)
{
	double my = get_mouse_coordinate_y(y);
	if (my > 0)
		me_setLine(hMemDc, 0, my, gc.window.count_pixels.x, my);
}

void vMainAxis(HDC& hMemDc, double x)
{
	double mx = get_mouse_coordinate_x(x);
	if (mx >= 0)
		me_setLine(hMemDc, mx, 0, mx, gc.window.count_pixels.y);
}

void hSupportiveAxis(HDC& hMemDc, double y)
{
	double my = get_mouse_coordinate_y(y);
	if (my > 0)
		me_setLine(hMemDc, 0, my, gc.window.count_pixels.x, my);
}

void vSupportiveAxis(HDC& hMemDc, double x)
{
	double mx = get_mouse_coordinate_x(x);
	if (mx >= 0)
		me_setLine(hMemDc, mx, 0, mx, gc.window.count_pixels.y);
}

// graphic core update
void GraphicHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, HDC& hMemDc,HBITMAP& hMemBmp)
{
	HFONT hFont = CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 2, 0, L"SYSTEM_FIXED_FONT");
	HFONT hTmp = (HFONT)SelectObject(hMemDc, hFont);
	SetBkMode(hMemDc, TRANSPARENT);

	gc.update_workspace();

	/**********************************************************************************************/

	// Paint main background
	SelectObject(hMemDc, reinterpret_cast<HGDIOBJ>(stock_objects::brush.background));
	Rectangle(hMemDc, 0, 0, gc.window.count_pixels.x, gc.window.count_pixels.y);

	/*---                                 Axis                               ---*/
	SelectObject(hMemDc, reinterpret_cast<HGDIOBJ>(stock_objects::pen.sup_axis));

	double d_x = quantum(gc.window.coordinates_limit.x);
	double d_y = quantum(gc.window.coordinates_limit.y);

	double s_x = round(gc.window.coordinates_begin.x / d_x);
	double s_y = round(gc.window.coordinates_end.y / d_y);

	double r_x = s_x;
	double r_y = s_y;

	if ((r_x * d_x) < gc.window.coordinates_begin.x) r_x++;
	if ((r_y * d_y) < gc.window.coordinates_end.y) r_y++;

	while (r_x * d_x <= gc.window.coordinates_end.x)
	{
		vSupportiveAxis(hMemDc, r_x * d_x);
		r_x++;
	}

	while (r_y * d_y <= gc.window.coordinates_begin.y)
	{
		hSupportiveAxis(hMemDc, r_y * d_y);
		r_y++;
	}

	r_x = s_x;
	r_y = s_y;

	SelectObject(hMemDc, reinterpret_cast<HGDIOBJ>(stock_objects::pen.main_axis));

	hMainAxis(hMemDc, 0);
	vMainAxis(hMemDc, 0);

	/*---                              [end] Axis                               ---*/


	SetTextColor(hMemDc, stock_objects::color.white);
	/*---                              Data render                              ---*/
	if (gl_data::data_content._data_x.size() != 0)
	{
		SelectObject(hMemDc, reinterpret_cast<HGDIOBJ>(stock_objects::pen.test_object2));

		long long data_size = gl_data::data_content._data_x.size();
		double start_data_x =(gl_data::data_content._data_x[0]);
		double end_data_x = (gl_data::data_content._data_x[data_size - 1]);

		if ((end_data_x > gc.window.coordinates_begin.x) && (start_data_x < gc.window.coordinates_end.x))
		{
			long long start_index = 0;
			long long end_index = data_size - 1;

			if (start_data_x < gc.window.coordinates_begin.x)
			{
				while (gl_data::data_content._data_x[start_index] < gc.window.coordinates_begin.x)
					start_index++;
				
				if (start_index != 0)
					start_index--;
			}

			if (end_data_x > gc.window.coordinates_end.x)
			{
				while (gl_data::data_content._data_x[end_index] > gc.window.coordinates_end.x)
					end_index--;

				if (end_index > (data_size - 1))
					end_index = data_size - 1;
			}

			double mouse_coord_data_start = get_mouse_coordinate_x(gl_data::data_content._data_x[start_index]);
			double mouse_coord_data_end = get_mouse_coordinate_x(gl_data::data_content._data_x[end_index]);
			double mouse_width = mouse_coord_data_end - mouse_coord_data_start;
			long long count = end_index - start_index;
			//me_setText(hMemDc, 5, 25, L"width pixels", mouse_width);
			//me_setText(hMemDc, 5, 35, L"count data", count);

			double compressed_scale = count / mouse_width;
			/*me_setText(hMemDc, 5, 45, L"compressed", compressed_scale);*/
			
			if (compressed_scale > 2)
			{
				double c = start_index;
				double n = c + compressed_scale;

				while (n < end_index)
				{
					// search extremums
					long long local_current_index = c;

					double extremum_min = gl_data::data_content._data_y[c];
					double extremum_max = gl_data::data_content._data_y[c];

					while (local_current_index <= n)
					{
						if (gl_data::data_content._data_y[local_current_index] < extremum_min)
						{
							extremum_min = gl_data::data_content._data_y[local_current_index];
							local_current_index++;
						}

						if (gl_data::data_content._data_y[local_current_index] > extremum_max)
						{
							extremum_max = gl_data::data_content._data_y[local_current_index];
						}

						local_current_index++;
					}
					//

					double x0 = get_mouse_coordinate_x(gl_data::data_content._data_x[c]);
					double x1 = get_mouse_coordinate_x(gl_data::data_content._data_x[n]);

					double y0 = get_mouse_coordinate_y(extremum_min);
					double y1 = get_mouse_coordinate_y(extremum_max);

					me_setLine(hMemDc, x0, y0, x1, y1);

					c = n;
					n = c + compressed_scale;
				}
			}
			else
			{
				for (int i = start_index; i < end_index; i++)
				{
					double x0 = get_mouse_coordinate_x(gl_data::data_content._data_x[i]);
					double x1 = get_mouse_coordinate_x(gl_data::data_content._data_x[i + 1]);

					double y0 = get_mouse_coordinate_y(gl_data::data_content._data_y[i]);
					double y1 = get_mouse_coordinate_y(gl_data::data_content._data_y[i + 1]);

					me_setLine(hMemDc, x0, y0, x1, y1);
				}
			}
		}

	}
	/*---                           [end] Data render                           ---*/

	hFont = CreateFont(16, 0, 0, 0, FW_BLACK, 0, 0, 0, 0, 0, 0, 2, 0, L"SYSTEM_FIXED_FONT");
	hTmp = (HFONT)SelectObject(hMemDc, hFont);
	//SetTextColor(hMemDc, stock_objects::color.rtext);

	while (r_x * d_x <= gc.window.coordinates_end.x)
	{
		axis_label_x(hMemDc, r_x * d_x, 0, r_x * d_x);
		r_x++;
	}

	while (r_y * d_y <= gc.window.coordinates_begin.y)
	{
		axis_label_y(hMemDc, 0, r_y * d_y, r_y * d_y);
		r_y++;
	}


	SelectObject(hMemDc, reinterpret_cast<HGDIOBJ>(stock_objects::pen.background));

	// show data for current mouse position
	//me_setText(hMemDc, 5, gc.window.count_pixels.y - 30, L"Mouse working area x", gc.mouse.current_position.x);
	//me_setText(hMemDc, 5, gc.window.count_pixels.y - 15, L"Mouse working area y", gc.mouse.current_position.y);

	// show data for current abstruct coord
	//me_setText(hMemDc, 5, 5, L"x ", get_abstract_coordinate_x(gc.mouse.current_position.x));
	//me_setText(hMemDc, 5, 15, L"y ", get_abstract_coordinate_y(gc.mouse.current_position.y));

	// show data for limit area
	//me_setText(5, 30, L"x ", gc.display_limit.x);
	//me_setText(5, 40, L"y ", gc.display_limit.y);

	// show data for workspace area
	//me_setText(5, 55, L"x ", gc.workspace.coord_begin.x);
	//me_setText(5, 65, L"y ", gc.workspace.coord_begin.y);
	//me_setText(5, 75, L"x ", gc.workspace.coord_end.x);
	//me_setText(5, 85, L"y ", gc.workspace.coord_end.y);

	/**********************************************************************************************/

	SelectObject(hMemDc, hFont);
	DeleteObject(hFont);
	SelectObject(hMemDc, hTmp);
	DeleteObject(hTmp);
}

LRESULT rendering(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC          hMemDc;
	HBITMAP      hMemBmp;
	PAINTSTRUCT  ps;
	RECT         rt;
	HDC          hdc = BeginPaint(hWnd, &ps);

	GetWindowRect(hWnd, &rt);

	hMemDc = CreateCompatibleDC(hdc);
	hMemBmp = CreateCompatibleBitmap(hdc,
		GetDeviceCaps(hdc, HORZRES),
		GetDeviceCaps(hdc, VERTRES));

	SelectObject(hMemDc, hMemBmp);

	GraphicHandler(hWnd, message, wParam, lParam, hMemDc, hMemBmp);

	BitBlt(hdc, 0, 0,
		GetDeviceCaps(hdc, HORZRES),
		GetDeviceCaps(hdc, VERTRES),
		hMemDc, 0, 0, SRCCOPY);

	DeleteDC(hMemDc);
	DeleteObject(hMemBmp);

	EndPaint(hWnd, &ps);

	return 0;
}
