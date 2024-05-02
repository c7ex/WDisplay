/*
 *                                      ____ display ____ 
 *                                     |                 |
 *                                  windows              |
 *                                                       |
 *                                          ______ message_loop ______
 *                                         |                          |
 *                                         |                          ^
 *                                         |                          ^
 *                                         |                          ^
 *                              _____ graphics_core __                ^
 *                             |                      |               ^
 *                   ______ packs _____               |               ^
 *                  |     |     |       |             |               ^
 *               string   |     |       |             |               ^   <- empty branch
 *                        |     |       |             |               ^
 *                     gl'pack  |       |             |               ^
 *                              |       |             |               ^
 *                            me'pack   |             |               ^
 *                                      |             |               ^
 *                                 description        |               ^
 *                                                    |               ^
 *                                                rendering           ^
 *                                                                    |
 *                                                               __ widgets
 *                                                              |
 *                                                            events
 */

#pragma once
#include<windows.h>
#include"message_loop.hpp"

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
	SetTimer(hWnd, gl_timer::main_id, gl_timer::main_interval, NULL);

	// Main loop window 
	MSG message{};
	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	// Delete General Timer
	KillTimer(hWnd, gl_timer::main_id);

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

public:
	void set_display_limit(double limit_x, double limit_y)
	{
		gc.set_display_limit(limit_x, limit_y);
	}

public:
	void load_data(std::vector<double>& data_x, std::vector<double>& data_y)
	{
		gl_data::data_content.set(data_x, data_y);
	}
};