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

	while ((lines > 10) || (lines < 5))
	{
		if (lines >= 10)
		{
			quant *= 5;
		}
		else if (lines <= 5)
		{
			quant /= 2;
		}
		lines = (number / quant);
		//std::cout << std::endl << "r" << lines << std::endl;
	}

	return quant;
}

void axis_label_x(double a_pos_x, double a_pos_y, double var)
{
	double m_pos_x = mcoord_x(a_pos_x);
	double m_pos_y = mcoord_y(a_pos_y);

	if (m_pos_y < 5)
		m_pos_y = 5;
	else if (m_pos_y > gl_windows::height - 15)
		m_pos_y = gl_windows::height - 15;

	me_setText(m_pos_x, m_pos_y, var);
}

void axis_label_y(double a_pos_x, double a_pos_y, double var)
{
	if (var == 0) return;

	double m_pos_x = mcoord_x(a_pos_x);
	double m_pos_y = mcoord_y(a_pos_y);

	if (m_pos_x < 5)
		m_pos_x = 5;
	else if (m_pos_x > gl_windows::width - 15)
		m_pos_x = gl_windows::width - 15;

	me_setText(m_pos_x, m_pos_y, var);
}

void hMainAxis(double y)
{
	double my = mcoord_y(y);
	if (my > 0)
		me_setLine(0, my, gl_windows::width, my);
}

void vMainAxis(double x)
{
	double mx = mcoord_x(x);
	if (mx >= 0)
		me_setLine(mx, 0, mx, gl_windows::height);
}

void hSupportiveAxis(double y)
{
	double my = mcoord_y(y);
	if (my > 0)
		me_setLine(0, my, gl_windows::width, my);
}

void vSupportiveAxis(double x)
{
	double mx = mcoord_x(x);
	if (mx >= 0)
		me_setLine(mx, 0, mx, gl_windows::height);
}

// graphic core update
void GraphicHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HFONT hFont = CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 2, 0, L"SYSTEM_FIXED_FONT");
	HFONT hTmp = (HFONT)SelectObject(gl_paint::hMemDc, hFont);
	SetBkMode(gl_paint::hMemDc, TRANSPARENT);
	SetTextColor(gl_paint::hMemDc, gl_color::wtext);

	gc.update_workspace();

	/**********************************************************************************************/

	// Paint main background
	SelectObject(gl_paint::hMemDc, reinterpret_cast<HGDIOBJ>(gl_stock::brush::BACKGROUND));
	SelectObject(gl_paint::hMemDc, reinterpret_cast<HGDIOBJ>(gl_stock::pen::BACKGROUND));
	Rectangle(gl_paint::hMemDc, 0, 0, gl_windows::width, gl_windows::height);



		/*---                              SupportiveAxis                             ---*/
		SelectObject(gl_paint::hMemDc, reinterpret_cast<HGDIOBJ>(gl_stock::pen::AXIS));

		double d_x = quantum(gc.display_limit.x);
		double d_y = quantum(gc.display_limit.y);

		double r_x = round(gc.workspace.coord_begin.x / d_x);
		double r_y = round(gc.workspace.coord_end.y / d_y);

		std::cout << r_x << " " << d_x << " " << r_x*d_x << std::endl;

		vSupportiveAxis(r_x * d_x);

		if ((r_x * d_x) < gc.workspace.coord_begin.x) r_x++;
		if ((r_y * d_y) < gc.workspace.coord_end.y) r_y++;

		while (r_x * d_x <= gc.workspace.coord_end.x)
		{
			vSupportiveAxis(r_x * d_x);
			axis_label_x(r_x * d_x, 0, r_x * d_x);
			r_x++;
		}

		while (r_y * d_y <= gc.workspace.coord_begin.y)
		{
			hSupportiveAxis(r_y * d_y);
			axis_label_y(0, r_y * d_y, r_y * d_y);
			r_y++;
		}
		/*---                                     ---*/



	SelectObject(gl_paint::hMemDc, reinterpret_cast<HGDIOBJ>(gl_stock::pen::MAINAXIS));

	hMainAxis(0);
	vMainAxis(0);


	SelectObject(gl_paint::hMemDc, reinterpret_cast<HGDIOBJ>(gl_stock::pen::BACKGROUND));

	// show data for current mouse position
	me_setText(5, gl_windows::height - 30, L"Mouse working area x", gl_mouse::position_x);
	me_setText(5, gl_windows::height - 15, L"Mouse working area y", gl_mouse::position_y);

	// show data for current abstruct coord
	me_setText(5, 5, L"x ", acoord_x(gl_mouse::position_x));
	me_setText(5, 15, L"y ", acoord_y(gl_mouse::position_y));

	// show data for limit area
	me_setText(5, 30, L"x ", gc.display_limit.x);
	me_setText(5, 40, L"y ", gc.display_limit.y);

	// show data for workspace area
	me_setText(5, 55, L"x ", gc.workspace.coord_begin.x);
	me_setText(5, 65, L"y ", gc.workspace.coord_begin.y);
	me_setText(5, 75, L"x ", gc.workspace.coord_end.x);
	me_setText(5, 85, L"y ", gc.workspace.coord_end.y);

	SelectObject(gl_paint::hMemDc, reinterpret_cast<HGDIOBJ>(gl_stock::pen::TEST_OBJ));

	if (gl_data::content.size() != 0)
	{
		for (int i = 0; i < gl_data::content.size() - 1; i++)
		{
			double x0 = mcoord_x(i);
			double x1 = mcoord_x(i+1);

			double y0 = mcoord_y(gl_data::content[i]);
			double y1 = mcoord_y(gl_data::content[i+1]);

			me_setLine(x0, y0, x1, y1);
		}
	}


	/**********************************************************************************************/

	SelectObject(gl_paint::hMemDc, hFont);
	DeleteObject(hFont);
	SelectObject(gl_paint::hMemDc, hTmp);
	DeleteObject(hTmp);
}

LRESULT rendering(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	gl_paint::hdc = BeginPaint(hWnd, &gl_paint::ps);

	GetWindowRect(hWnd, &gl_paint::rt);

	gl_paint::hMemDc = CreateCompatibleDC(gl_paint::hdc);
	gl_paint::hMemBmp = CreateCompatibleBitmap(gl_paint::hdc,
		GetDeviceCaps(gl_paint::hdc, HORZRES),
		GetDeviceCaps(gl_paint::hdc, VERTRES));

	SelectObject(gl_paint::hMemDc, gl_paint::hMemBmp);

	GraphicHandler(hWnd, message, wParam, lParam);

	BitBlt(gl_paint::hdc, 0, 0,
		GetDeviceCaps(gl_paint::hdc, HORZRES),
		GetDeviceCaps(gl_paint::hdc, VERTRES),
		gl_paint::hMemDc, 0, 0, SRCCOPY);

	DeleteDC(gl_paint::hMemDc);
	DeleteObject(gl_paint::hMemBmp);

	EndPaint(hWnd, &gl_paint::ps);

	return 0;
}
