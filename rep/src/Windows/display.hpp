/*
 *                  ____ display ____ 
 *              windows     |        |
 *                       gdiplus     |
 *                             ___ mloop ___
 *                       __ paint           |
 *                ____ core____             |
 *             string   |      |            |
 *                   gl'pack   |            |
 *                          me'pack         |
 *                                    __ widgets
 *                                 events
 */

#pragma once
#include<windows.h>
#include<gdiplus.h>

#pragma comment (lib, "Gdiplus.lib")
using namespace Gdiplus;

#include"mloop.hpp"

 // check on exception
void WndExce(bool condition, LPCWSTR exceptionText)
{
	if (condition)
	{
		MessageBox(
			NULL,
			exceptionText,
			TEXT("Exception"),
			NULL);
		PostQuitMessage(0);
	}
}

class Display
{
public:
	// the correct entry WinMain
	int WINAPI WinMain(
		_In_     HINSTANCE hInstance,
		_In_opt_ HINSTANCE hPrevInstance,
		_In_     LPSTR     lpCmdLine,
		_In_     int       nCmdShow)
	{
		// Initialize GDI+.
		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR           gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		// Create window class
		WNDCLASSEX wcex{ sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = mloop; // message handler
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_DESKTOP);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = TEXT("Test class WNDCLASSEX");
		wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);
		WndExce(!RegisterClassEx(&wcex), TEXT("RegisterClassEx failed!"));

		// Create main window
		HWND hWnd = CreateWindowEx(
			WS_EX_LEFT,
			wcex.lpszClassName,
			TEXT("Empty window"),
			WS_VISIBLE | WS_OVERLAPPEDWINDOW,
			10,
			10,
			400,
			400,
			NULL,
			NULL,
			wcex.hInstance,
			NULL);
		WndExce(!hWnd, TEXT("CreateWindowEx failed!"));

		// Create General Timer
		SetTimer(hWnd, gl_timer::main_id, gl_timer::main_interval, NULL);

		// Set start system
		gc.set_display_limit(150., 50.);
		gc.set_reference_point(0, 0);
		gc.update_expand_scale();

		// Main loop window 
		MSG message{};
		while (GetMessage(&message, NULL, 0, 0))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		// Delete General Timer
		KillTimer(hWnd, gl_timer::main_id);

		GdiplusShutdown(gdiplusToken);
		return 0;
	}
};
