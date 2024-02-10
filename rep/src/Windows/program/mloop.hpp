#pragma once
#include"paint.hpp"
#include"guitool.hpp"

// message handler
LRESULT CALLBACK mloop(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
		{
			CreateWidgets(hWnd, message, wParam, lParam);
			return 0;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_COMMAND: // events widgets
		{
			return events(hWnd, message, wParam, lParam);
		}
				
				
				
				//===================GRAPH CORE====================//

				case (WM_MOUSEWHEEL):
				{
					return 0;
				}

				case WM_LBUTTONDOWN:
				{
					SetCapture(hWnd);
					return 0;
				}

				case WM_LBUTTONUP:
				{
					ReleaseCapture();
					return 0;
				}

				case WM_MOUSEMOVE:
				{
					return 0;
				}

				//=================================================//



		case WM_PAINT: // graphics
		{
			return WmPaint(hWnd, message, wParam, lParam);
		}

		case WM_TIMER:
		{
			InvalidateRect(hWnd, NULL, FALSE);
		}

		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}

	return 0;
}