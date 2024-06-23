#pragma once

void paint_text(HDC& hMemDc, int x, int y, std::wstring lable, double value, COLORREF color = stock_objects::color.white)
{
	SetTextColor(hMemDc, color);
	std::wstring gl_wstr = (lable)+L"(" + std::to_wstring(value) + L")";
	TextOut(hMemDc, x, y, gl_wstr.c_str(), (int)gl_wstr.size());
}

// graphic core update
void GraphicHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, HDC& hMemDc,HBITMAP& hMemBmp)
{
	HFONT hFont = CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 2, 0, L"SYSTEM_FIXED_FONT");
	HFONT hTmp = (HFONT)SelectObject(hMemDc, hFont);
	SetBkMode(hMemDc, TRANSPARENT);

	wds.update();
	wds.paint(hMemDc);

	wds.AddLable(hMemDc, pointf(310, 30),  L"current mouse x:", Form_Mouse.get_x());
	wds.AddLable(hMemDc, pointf(310, 40),  L"current mouse y:", Form_Mouse.get_y());
	wds.AddLable(hMemDc, pointf(310, 50), L"current abstruct x:", Engine.get_abstruct(Form_Mouse).get_x());
	wds.AddLable(hMemDc, pointf(310, 60), L"current abstruct y:", Engine.get_abstruct(Form_Mouse).get_y());

	SelectObject(hMemDc, hFont);
	DeleteObject(hFont);
	SelectObject(hMemDc, hTmp);
	DeleteObject(hTmp);
}

LRESULT rendering(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto startTime = std::chrono::high_resolution_clock::now();

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

	auto endTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> Time = endTime - startTime;
	//std::cout << std::endl << "paint_compressed_mode: " << Time.count() << " s" << std::endl;

	return 0;
}
