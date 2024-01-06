#pragma once
#include"WndWidg.hpp"

#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

// message handler
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
		{
			CreateWidgets(hWnd, message, wParam, lParam);
			return static_cast<LRESULT>(0);
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return static_cast<LRESULT>(0);
		}

		case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			Graphics    graphics(hdc);
			SolidBrush  brush(Color(255, 0, 0, 0));
			FontFamily  fontFamily(L"Times New Roman");
			Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
			PointF      pointF(100.0f, 200.0f);
			graphics.DrawString(L"String", -1, &font, pointF, &brush);
			EndPaint(hWnd, &ps);

			return static_cast<LRESULT>(0);
		}

		case WM_COMMAND:
		{
			return WmProc(hWnd, message, wParam, lParam);
		}

		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}

	return static_cast<LRESULT>(0);
}