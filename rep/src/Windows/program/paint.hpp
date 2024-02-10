#pragma once
#include"core.hpp"

// graphic handler
LRESULT WmPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	double mouse_x = LOWORD(lParam);
	double mouse_y = HIWORD(lParam);

	hdc = BeginPaint(hWnd, &ps);
	GetWindowRect(hWnd, &rt);

	double current_width_working_area = (rt.right - rt.left) - 16;
	double current_height_working_area = (rt.bottom - rt.top) - 39;

	HDC hMemDc;
	HBITMAP hMemBmp;
	hMemDc = CreateCompatibleDC(hdc);
	hMemBmp = CreateCompatibleBitmap(hdc,
		GetDeviceCaps(hdc, HORZRES),
		GetDeviceCaps(hdc, VERTRES));
	SelectObject(hMemDc, hMemBmp);

	/**********************************************************************************************/
	// Setting for painting text
	HFONT hFont = CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 2, 0, L"SYSTEM_FIXED_FONT");
	HFONT hTmp = (HFONT)SelectObject(hMemDc, hFont);
	SetBkMode(hMemDc, TRANSPARENT);
	SetTextColor(hMemDc, TEXT_MOUSE_CURRENT_POSITION);
	
	// Paint main background
	SelectObject(hMemDc, reinterpret_cast<HGDIOBJ>(BACKGROUND_BRUSH));
	SelectObject(hMemDc, reinterpret_cast<HGDIOBJ>(BACKGROUND_PEN));
	Rectangle(hMemDc, 0, 0, current_width_working_area, current_height_working_area);
	
	std::wstring wstr_current_x_position = L"Mouse working area x(" + std::to_wstring(mouse_x) + L")";
	std::wstring wstr_current_y_position = L"Mouse working area y(" + std::to_wstring(mouse_y) + L")";
	TextOut(hMemDc, 5, current_height_working_area - 30, wstr_current_x_position.c_str(), wstr_current_x_position.size());
	TextOut(hMemDc, 5, current_height_working_area - 15, wstr_current_y_position.c_str(), wstr_current_y_position.size());
	/**********************************************************************************************/

	SelectObject(hMemDc, hFont);
	DeleteObject(hFont);
	SelectObject(hMemDc, hTmp);
	DeleteObject(hTmp);

	BitBlt(hdc, 0, 0, GetDeviceCaps(hdc, HORZRES), GetDeviceCaps(hdc, VERTRES), hMemDc, 0, 0, SRCCOPY);

	DeleteDC(hMemDc);
	DeleteObject(hMemBmp);
	EndPaint(hWnd, &ps);

	return 0;
}