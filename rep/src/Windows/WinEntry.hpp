#pragma once
/*
*                __ WinEntry __ 
*               |              |
*         __ WndExce        WndProc __
*        |                            |
*     windows                   __ WndWidg __
*                              |             |
*                        __ WmPaint        WmProc __
*                       |                           |
*                    gdiplus                     WmList __
*                                                         |
*                                                   __ WndGlob __
*                                                  |             |
*                                               windows        string
*
*                                          ===============================
*                                         |                               |
*                                         |   *Widgets for create form*   |
*                                         |      *Data for Display*       |
*                                         |                               |
*                                          ===============================
*/ 
#include"WndExce.hpp"
#include"WndProc.hpp"

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
		wcex.lpfnWndProc = WndProc; // message handler
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
		SetTimer(hWnd, ID_SENIOR_TIMER, INTERVAL_TIMER, NULL);
		
		// Main loop window 
		MSG message{};
		while (GetMessage(&message, NULL, 0, 0))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		// Delete General Timer
		KillTimer(hWnd, ID_SENIOR_TIMER);

		GdiplusShutdown(gdiplusToken);
		return 0;
	}
};
