#pragma once
#include"core.hpp"

void hAxis(double y)
{
	double my = mcoord_y(y);
	if (my > 0)
		me_setLine(0, my, gl_windows::width, my);
}

void vAxis(double x)
{
	double mx = mcoord_x(x);
	if (mx >= 0)
		me_setLine(mx, 0, mx, gl_windows::height);
}

// graphic handler
void GraphicHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HFONT hFont = CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 2, 0, L"SYSTEM_FIXED_FONT");
	HFONT hTmp = (HFONT)SelectObject(gl_paint::hMemDc, hFont);
	SetBkMode(gl_paint::hMemDc, TRANSPARENT);
	SetTextColor(gl_paint::hMemDc, gl_color::wtext);

	/**********************************************************************************************/

	// Paint main background
	SelectObject(gl_paint::hMemDc, reinterpret_cast<HGDIOBJ>(gl_stock::brush::BACKGROUND));
	SelectObject(gl_paint::hMemDc, reinterpret_cast<HGDIOBJ>(gl_stock::pen::BACKGROUND));
	Rectangle(gl_paint::hMemDc, 0, 0, gl_windows::width, gl_windows::height);


	SelectObject(gl_paint::hMemDc, reinterpret_cast<HGDIOBJ>(gl_stock::pen::AXIS));
	
	hAxis(0);
	vAxis(0);

	me_setText(5, gl_windows::height - 30, L"Mouse working area x", gl_mouse::position_x);
	me_setText(5, gl_windows::height - 15, L"Mouse working area y", gl_mouse::position_y);

	me_setText(5, 5, L"x ", acoord_x(gl_mouse::position_x));
	me_setText(5, 15, L"y ", acoord_y(gl_mouse::position_y));

	me_setText(5, 30, L"x ", gc.display_limit.x);
	me_setText(5, 40, L"y ", gc.display_limit.y);

	SelectObject(gl_paint::hMemDc, reinterpret_cast<HGDIOBJ>(gl_stock::pen::TEST_OBJ));

	if (gl_data::content.size() != 0)
	{
		for (int i = 0; i < gl_data::content.size()-1; i++)
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

LRESULT repaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
