#pragma once

void paint_line(HDC& hMemDc, int x0, int y0, int x1, int y1)
{
	MoveToEx(hMemDc, x0, y0, NULL);
	LineTo(hMemDc, x1, y1);
}

// graphic core update
void GraphicHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, HDC& hMemDc,HBITMAP& hMemBmp)
{
	HFONT hFont = CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 2, 0, L"SYSTEM_FIXED_FONT");
	HFONT hTmp = (HFONT)SelectObject(hMemDc, hFont);
	SetBkMode(hMemDc, TRANSPARENT);

	//SelectObject(hMemDc, reinterpret_cast<HGDIOBJ>(stock_objects::pen.white));

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
