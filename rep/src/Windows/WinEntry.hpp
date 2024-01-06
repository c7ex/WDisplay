#pragma once
/*
*           __ WinEntry __ 
*          |              |
*       WndExce        WndProc __
*                                |
*                             WndWidg __
*                                       |
*                                     WmProc __
*                                              |
*                                           WmList __
*                                                    |
*                                              __ WndGlob __
*                                             |             |
*                                          windows        string
* 
*                                     ===============================
*                                    |                               |
*                                    |   *Widgets for create form*   |
*                                    |      *Data for Display*       |
*                                    |                               |
*                                     ===============================
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
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
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
			WS_VISIBLE | WS_SYSMENU,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			NULL,
			NULL,
			hInstance,
			NULL);
		WndExce(!hWnd, TEXT("CreateWindowEx failed!"));

		// Main loop window 
		MSG message{};
		while (GetMessage(&message, NULL, 0, 0))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		GdiplusShutdown(gdiplusToken);
		return 0;
	}

	void updateString(std::string newstr)
	{
		str = newstr;
	}
};
