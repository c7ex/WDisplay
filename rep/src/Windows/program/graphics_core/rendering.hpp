#pragma once

// graphic core update
void GraphicHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, HDC& hMemDc,HBITMAP& hMemBmp)
{
	HFONT hFont = CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 2, 0, L"SYSTEM_FIXED_FONT");
	HFONT hTmp = (HFONT)SelectObject(hMemDc, hFont);
	SetBkMode(hMemDc, TRANSPARENT);

	gc.update_window();

	// Paint main background
	SelectObject(hMemDc, reinterpret_cast<HGDIOBJ>(stock_objects::brush.plot));
	Rectangle(hMemDc, 0, 0, gc.window.count_pixels.x, gc.window.count_pixels.y);

	// Paint supportive axis
	SelectObject(hMemDc, reinterpret_cast<HGDIOBJ>(stock_objects::pen.sup_axis));
	paint_supportive_axis(hMemDc);

	// Paint graphs
	paint_data(hMemDc);

	SelectObject(hMemDc, reinterpret_cast<HGDIOBJ>(stock_objects::pen.bound));
	SelectObject(hMemDc, reinterpret_cast<HGDIOBJ>(stock_objects::brush.bound));
	Rectangle(hMemDc, 0, 0, gc.window.count_pixels.x, gc.window.count_pixels.y * GRAPH_BGN_Y);
	Rectangle(hMemDc, 0, gc.window.count_pixels.y * GRAPH_END_Y, gc.window.count_pixels.x, gc.window.count_pixels.y);
	Rectangle(hMemDc, 0, 0, gc.window.count_pixels.x * GRAPH_BGN_X, gc.window.count_pixels.y);
	Rectangle(hMemDc, gc.window.count_pixels.x * GRAPH_END_X, 0, gc.window.count_pixels.x, gc.window.count_pixels.y);

	// Paint axis labels
	SetTextAlign(hMemDc, TA_CENTER);
	//std::cout << gc.window.count_pixels.x << std::endl;
	hFont = CreateFont((sqrt(1.3*gc.window.count_pixels.x)/2.6), 0, 0, 0, FW_BLACK, 0, 0, 0, 0, 0, 0, 2, 0, L"SYSTEM_FIXED_FONT");
	hTmp = (HFONT)SelectObject(hMemDc, hFont);
	SelectObject(hMemDc, reinterpret_cast<HGDIOBJ>(stock_objects::pen.labels_axis));
	paint_supportive_axis_labels(hMemDc);

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
