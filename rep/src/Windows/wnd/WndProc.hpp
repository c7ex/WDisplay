#pragma once
#include"WndWidg.hpp"

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

		case WM_COMMAND: // events
		{
			return WmProc(hWnd, message, wParam, lParam);
		}

		//======================MOUSE======================//

			case (WM_MOUSEWHEEL):
			{
				scale_rate += GET_WHEEL_DELTA_WPARAM(wParam)/120.;
				scale = pow(1.03, scale_rate);
				std::cout << scale << "WM_MOUSEHWHEEL\n";
				return 0;
			}

			case WM_LBUTTONDOWN:
			{
				SetCapture(hWnd);
				
				buttondown_save_x_pos = xPos/scale;
				buttondown_save_y_pos = yPos/scale;
				
				LBUTTON_MOUSE_ACTIVE = true;

				return 0;
			}

			case WM_LBUTTONUP:
			{
				ReleaseCapture();
				
				memory_buttondown_save_x_pos = shift_x;
				memory_buttondown_save_y_pos = shift_y;

				LBUTTON_MOUSE_ACTIVE = false;

				return 0;
			}

		case WM_MOUSEMOVE: // mouse events
		{
			if (LBUTTON_MOUSE_ACTIVE)
			{
				shift_x = xPos/scale - buttondown_save_x_pos + memory_buttondown_save_x_pos;
				shift_y = yPos/scale - buttondown_save_y_pos + memory_buttondown_save_y_pos;
			}

			xPos = LOWORD(lParam);
			yPos = HIWORD(lParam);
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

	return static_cast<LRESULT>(0);
}