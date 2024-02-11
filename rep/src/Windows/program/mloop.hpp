#pragma once
#include"paint.hpp"
#include"widgets.hpp"

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
					gl_wheel::counter += GET_WHEEL_DELTA_WPARAM(wParam) / 120.;
					gc.update_scale();
					//std::cout << gl_wheel::counter << std::endl;
					return 0;
				}

				case WM_LBUTTONDOWN:
				{
					gl_mouse::hold = true;
					gc.update_hold();
					//SetCapture(hWnd);
					return 0;
				}

				case WM_LBUTTONUP:
				{
					gl_mouse::hold = false;

					//ReleaseCapture();
					return 0;
				}

				case WM_MOUSEMOVE:
				{
					gl_mouse::position_x = LOWORD(lParam);
					gl_mouse::position_y = HIWORD(lParam);

					if (gl_mouse::hold == true)
					{
						gc.update_shift();
						gc.update_reference_point();
					}

					return 0;
				}

				case (WM_SIZE):
				{
					GetWindowRect(hWnd, &gl_paint::rt);
					gl_windows::width = (gl_paint::rt.right - gl_paint::rt.left) - 16;
					gl_windows::height = (gl_paint::rt.bottom - gl_paint::rt.top) - 39;
					
					gc.update_expand_scale();
					
					return 0;
				}

				//=================================================//



		case WM_PAINT: // graphics
		{
			return repaint(hWnd, message, wParam, lParam);
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