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

			case WM_LBUTTONDOWN:
			{
				SetCapture(hWnd);
				
				buttondown_save_x_pos = xPos;
				buttondown_save_y_pos = yPos;
				
				LBUTTON_MOUSE_ACTIVE = true;

				std::cout << buttondown_save_x_pos << " " << buttondown_save_y_pos << "down\n";
				return 0;
			}

			case WM_LBUTTONUP:
			{
				ReleaseCapture();
				
				memory_buttondown_save_x_pos = shift_x;
				memory_buttondown_save_y_pos = shift_y;

				LBUTTON_MOUSE_ACTIVE = false;
				
				std::cout << "up\n";
				return 0;
			}

		case WM_MOUSEMOVE: // mouse events
		{
			if (LBUTTON_MOUSE_ACTIVE)
			{
				shift_x = xPos - buttondown_save_x_pos + memory_buttondown_save_x_pos;
				shift_y = yPos - buttondown_save_y_pos + memory_buttondown_save_y_pos;
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