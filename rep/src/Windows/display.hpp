/*
 *                                      ____ display ____ 
 *                                     |                 |
 *                                  windows              |
 *                                                       |
 *                                           ________ winloop ________
 *                                          |                         |
 *                                          |                         ^
 *                                          |                         ^
 *                                          |                         ^
 *                            _____ graphics_handler __               ^
 *                           |                         |              ^
 *                    ______ packs _____               |              ^
 *                   |     |     |       |             |              ^
 *                string   |     |       |             |              ^   <- empty branch
 *                         |     |       |             |              ^
 *                      gl'pack  |       |             |              ^
 *                               |       |             |              ^
 *                             me'pack   |             |              ^
 *                                       |             |              ^
 *                                  description        |              ^
 *                                                     |              ^
 *                                                 rendering          ^
 *                                                                    |
 *                                                               __ widgets
 *                                                              |
 *                                                            events
 */

#pragma once
#include<windows.h>
#include"winloop.hpp"

void WndExce(bool, LPCWSTR);

int WINAPI WinMain(
	_In_     HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_     LPSTR     lpCmdLine,
	_In_     int       nCmdShow)
{

	// Create window class
	WNDCLASSEX wcex{ sizeof(WNDCLASSEX) };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MainMessageHandler; // load main loop
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
	SetTimer(hWnd, main_timer::id, main_timer::interval, NULL);

	// Main loop window 
	MSG message{};
	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	// Delete General Timer
	KillTimer(hWnd, main_timer::id);

	return 0;
}

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

HINSTANCE arg_hInstance;
HINSTANCE arg_hPrevInstance;
LPSTR     arg_lpCmdLine;
int       arg_nCmdShow;

class display
{
public:
	void run()
	{
		WinMain(arg_hInstance,arg_hPrevInstance,arg_lpCmdLine,arg_nCmdShow);
	}
};