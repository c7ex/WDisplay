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

	wds.AddLable(hMemDc, pointf(310, 30),  L"current mouse x:", hd_form_mouse.get_x());
	wds.AddLable(hMemDc, pointf(310, 40),  L"current mouse y:", hd_form_mouse.get_y());
	wds.AddLable(hMemDc, pointf(310, 50),  L"size x:", hf_form_size.get_x());
	wds.AddLable(hMemDc, pointf(310, 60),  L"size y:", hf_form_size.get_y());
	wds.AddLable(hMemDc, pointf(310, 70),  L"expand x:", hf_form_expand.get_x());
	wds.AddLable(hMemDc, pointf(310, 80),  L"expand y:", hf_form_expand.get_y());
	wds.AddLable(hMemDc, pointf(310, 90),  L"abstruct expand x:", wds.plot().abstruct_expand.to_pointf().get_x());
	wds.AddLable(hMemDc, pointf(310, 100), L"abstruct expand y:", wds.plot().abstruct_expand.to_pointf().get_y());
	wds.AddLable(hMemDc, pointf(310, 110), L"current abstruct x:", wds.plot().get_abstruct(hd_form_mouse).get_x());
	wds.AddLable(hMemDc, pointf(310, 120), L"current abstruct y:", wds.plot().get_abstruct(hd_form_mouse).get_y());
	wds.AddLable(hMemDc, pointf(310, 130), L"current offset x:", wds.plot().state_current_offset.get_x());
	wds.AddLable(hMemDc, pointf(310, 140), L"current offset y:", wds.plot().state_current_offset.get_y());
	wds.AddLable(hMemDc, pointf(310, 150), L"last offset x:", wds.plot().state_last_offset.get_x());
	wds.AddLable(hMemDc, pointf(310, 160), L"last offset y:", wds.plot().state_last_offset.get_y());
	wds.AddLable(hMemDc, pointf(310, 170), L"abstruct scale x:", wds.plot().state_scale.get_x());
	wds.AddLable(hMemDc, pointf(310, 180), L"abstruct scale y:", wds.plot().state_scale.get_y());
	wds.AddLable(hMemDc, pointf(310, 190), L"abstruct centre x:", wds.plot().abstruct_centre.get_x());
	wds.AddLable(hMemDc, pointf(310, 200), L"abstruct centre y:", wds.plot().abstruct_centre.get_y());
	wds.AddLable(hMemDc, pointf(310, 210), L"chart centre x:", wds.plot().get_chart_centre().get_x());
	wds.AddLable(hMemDc, pointf(310, 220), L"chart centre y:", wds.plot().get_chart_centre().get_y());

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
