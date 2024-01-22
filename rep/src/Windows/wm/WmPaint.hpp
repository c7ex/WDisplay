#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

// need BitBlt for fast paint (create copy devise in memory)

HDC          hdc;
PAINTSTRUCT  ps;
RECT         rt;

int xPos;
int yPos;

// graphic handler
LRESULT WmPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	hdc = BeginPaint(hWnd, &ps);
	GetWindowRect(hWnd, &rt);

	HDC hMemDc;
	HBITMAP hMemBmp;

	hMemDc = CreateCompatibleDC(hdc);
	hMemBmp = CreateCompatibleBitmap(hdc,
		GetDeviceCaps(hdc, HORZRES),
		GetDeviceCaps(hdc, VERTRES));

	SelectObject(hMemDc, hMemBmp);


		// Paint background
		HGDIOBJ pen = SelectObject(hMemDc, GetStockObject(BLACK_PEN));
		HGDIOBJ brush = SelectObject(hMemDc, GetStockObject(SYSTEM_FONT));

		int x_pos_end = (rt.right - rt.left) - 16;
		int y_pos_end = (rt.bottom - rt.top) - 39;

		Rectangle(hMemDc, 0, 0, x_pos_end, y_pos_end);

		SelectObject(hMemDc, pen);
		DeleteObject(pen);
		SelectObject(hMemDc, brush);
		DeleteObject(brush);


		// Mouse info
		std::wstring x_message = L"x(" + std::to_wstring(xPos) + L")";
		std::wstring y_message = L"y(" + std::to_wstring(yPos) + L")";

		HFONT hFont = CreateFont(11, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 2, 0, L"SYSTEM_FIXED_FONT");
		HFONT hTmp = (HFONT)SelectObject(hMemDc, hFont);
		SetBkMode(hMemDc, TRANSPARENT);

		TextOut(hMemDc, 20 + xPos, yPos - 10, x_message.c_str(), x_message.size());
		TextOut(hMemDc, 20 + xPos, yPos - 0 , y_message.c_str(), y_message.size());

		SelectObject(hMemDc, hFont);
		DeleteObject(hFont);
		SelectObject(hMemDc, hTmp);
		DeleteObject(hTmp);


	BitBlt(hdc, 0, 0, GetDeviceCaps(hdc, HORZRES), GetDeviceCaps(hdc, VERTRES), hMemDc, 0, 0, SRCCOPY);

	DeleteDC(hMemDc);
	DeleteObject(hMemBmp);
	EndPaint(hWnd, &ps);

	return static_cast<LRESULT>(0);
}