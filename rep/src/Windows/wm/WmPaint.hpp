#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

HDC          hdc;
PAINTSTRUCT  ps;

Color mainOutline(100, 0, 0, 0);
Color mainBackground(255, 141, 231, 237);

void PaintRectangle(Graphics& graphics, const Color& penColor, float widthPen, const Color& brushColor, RECT rect)
{
	Pen pen(penColor, 3);
	SolidBrush solidBrush(brushColor);
	RectF rectangle(
		0, 0, 
		static_cast<float>(rect.right - rect.left - 17),
		static_cast<float>(rect.bottom - rect.top - 40));
	graphics.FillRectangle(&solidBrush, rectangle);
	graphics.DrawRectangle(&pen, rectangle);
}

void PaintRectangle(Graphics& graphics, const Color& penColor, float widthPen, const Color& brushColor, float x, float y, float width, float heigh)
{
	Pen pen(penColor, 3);
	SolidBrush solidBrush(brushColor);
	RectF rectangle(x, y, width, heigh);
	graphics.FillRectangle(&solidBrush, rectangle);
	graphics.DrawRectangle(&pen, rectangle);
}

// graphic handler
LRESULT WmPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT clientRect;
	GetWindowRect(hWnd, &clientRect);
	hdc = BeginPaint(hWnd, &ps);
	
	Graphics graphics(hdc);
	PaintRectangle(
		graphics, 
		mainOutline,
		3.0f,
		mainBackground,
		clientRect);

	EndPaint(hWnd, &ps);
	return static_cast<LRESULT>(0);
}