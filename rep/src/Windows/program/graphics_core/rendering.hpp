#pragma once

// graphic core rendering
void wds_render(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, HDC& hMemDc,HBITMAP& hMemBmp)
{
	wds.update();
	wds.paint(hMemDc);

	HFONT hFont = CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 2, 0, L"SYSTEM_FIXED_FONT");
	HFONT hTmp = (HFONT)SelectObject(hMemDc, hFont);
	SetBkMode(hMemDc, TRANSPARENT);

	wds.AddLable(hMemDc, xy_point(310, 30), L"current mouse x:", Form_Mouse.get_x());
	wds.AddLable(hMemDc, xy_point(310, 40), L"current mouse y:", Form_Mouse.get_y());
	wds.AddLable(hMemDc, xy_point(310, 50), L"current abstruct x:", Engine.get_abstruct(Form_Mouse).get_x());
	wds.AddLable(hMemDc, xy_point(310, 60), L"current abstruct y:", Engine.get_abstruct(Form_Mouse).get_y());
	wds.AddLable(hMemDc, xy_point(310, 70), L"limit s abstruct x:", Engine.get_limit_left_down().get_x());
	wds.AddLable(hMemDc, xy_point(310, 80), L"limit s abstruct y:", Engine.get_limit_left_down().get_y());
	wds.AddLable(hMemDc, xy_point(310, 90), L"limit e abstruct x:", Engine.get_limit_right_up().get_x());
	wds.AddLable(hMemDc, xy_point(310,100), L"limit e abstruct y:", Engine.get_limit_right_up().get_y());

	SelectObject(hMemDc, hFont);
	DeleteObject(hFont);
	SelectObject(hMemDc, hTmp);
	DeleteObject(hTmp);
}

LRESULT paint_frame(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

	wds_render(hWnd, message, wParam, lParam, hMemDc, hMemBmp);

	BitBlt(hdc, 0, 0,
		GetDeviceCaps(hdc, HORZRES),
		GetDeviceCaps(hdc, VERTRES),
		hMemDc, 0, 0, SRCCOPY);

	DeleteDC(hMemDc);
	DeleteObject(hMemBmp);

	EndPaint(hWnd, &ps);

	auto endTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> Time = endTime - startTime;
	//std::cout << std::endl << "time paint frame: " << Time.count() << " s" << std::endl;

	return 0;
}
