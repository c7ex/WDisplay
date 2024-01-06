#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

HDC          hdc;
PAINTSTRUCT  ps;

// graphic handler
LRESULT WmPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	std::wstring widestr = std::wstring(str.begin(), str.end());

	hdc = BeginPaint(hWnd, &ps);
	Graphics    graphics(hdc);
	SolidBrush  brush(Color(255, 0, 0, 0));
	FontFamily  fontFamily(L"Times New Roman");
	Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
	PointF      pointF(100.0f, 200.0f);
	graphics.DrawString(widestr.c_str(), -1, &font, pointF, &brush);
	EndPaint(hWnd, &ps);

	return static_cast<LRESULT>(0);
}