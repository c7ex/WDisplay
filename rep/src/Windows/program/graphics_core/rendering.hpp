#pragma once

void paint_text(HDC& hMemDc, int x, int y, std::wstring lable, double value, COLORREF color = stock_objects::color.white)
{
	SetTextColor(hMemDc, color);
	std::wstring gl_wstr = (lable)+L"(" + std::to_wstring(value) + L")";
	TextOut(hMemDc, x, y, gl_wstr.c_str(), (int)gl_wstr.size());
}

void paint_line(HDC& hMemDc, double x0, double y0, double x1, double y1)
{
	MoveToEx(hMemDc, (int)x0, (int)y0, NULL);
	LineTo(hMemDc, (int)x1, (int)y1);
}

void paint_pixel(HDC& hMemDc, int x, int y)
{
	MoveToEx(hMemDc, x, y, NULL);
	LineTo(hMemDc, x+1, y+1);
}

// graphic core update
void GraphicHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, HDC& hMemDc,HBITMAP& hMemBmp)
{
	HFONT hFont = CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 2, 0, L"SYSTEM_FIXED_FONT");
	HFONT hTmp = (HFONT)SelectObject(hMemDc, hFont);
	SetBkMode(hMemDc, TRANSPARENT);
	
	gc.ch.paint_chart(hMemDc,
		stock_objects::pen.chart,
		stock_objects::brush.chart,
		stock_objects::pen.bound,
		stock_objects::brush.bound);

	paint_text(hMemDc, 10, 10, L"window_size_in_pixels x:", gc.window_size_in_pixels.x);
	paint_text(hMemDc, 10, 20, L"window_size_in_pixels y:", gc.window_size_in_pixels.y);

	paint_text(hMemDc, 10, 30, L"window_mouse_position_in_pixels x:", gc.window_mouse_position_in_pixels.x);
	paint_text(hMemDc, 10, 40, L"window_mouse_position_in_pixels y:", gc.window_mouse_position_in_pixels.y);

	paint_text(hMemDc, 10, 50, L"compression_factors x:", gc.compression_factors.x);
	paint_text(hMemDc, 10, 60, L"compression_factors y:", gc.compression_factors.y);

	paint_text(hMemDc, 10, 70, L"middle x:", gc.ch.plot.middle.x);
	paint_text(hMemDc, 10, 80, L"middle y:", gc.ch.plot.middle.y);

	SelectObject(hMemDc, hFont);
	DeleteObject(hFont);
	SelectObject(hMemDc, hTmp);
	DeleteObject(hTmp);
}

LRESULT rendering(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//auto startTime = std::chrono::high_resolution_clock::now();

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

	//auto endTime = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double> Time = endTime - startTime;
	//std::cout << std::endl << "paint_compressed_mode: " << Time.count() << " s" << std::endl;

	return 0;
}
