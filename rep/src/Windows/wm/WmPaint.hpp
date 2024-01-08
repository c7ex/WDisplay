#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

HDC          hdc;
PAINTSTRUCT  ps;
RECT         rt;

Color mainOutline(100, 0, 0, 0);
Color mainBackground(55, 141, 231, 237);

// graphic handler
LRESULT WmPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GetWindowRect(hWnd, &rt);
	int x_pos_end = (rt.right - rt.left) - 17;
	int y_pos_end = (rt.bottom - rt.top) - 40;

	hdc = BeginPaint(hWnd, &ps);
	HGDIOBJ pen = SelectObject(hdc, GetStockObject(BLACK_PEN));
	HGDIOBJ brush = SelectObject(hdc, GetStockObject(SYSTEM_FONT));

	// Paint background
	Rectangle(hdc, 1, 2, x_pos_end, y_pos_end);

	SelectObject(hdc, pen);
	SelectObject(hdc, brush);
	DeleteObject(pen);
	DeleteObject(brush);

	EndPaint(hWnd, &ps);
	return static_cast<LRESULT>(0);
}