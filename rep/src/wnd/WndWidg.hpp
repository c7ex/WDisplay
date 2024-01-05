#pragma once
#include"WmProc.hpp"

// create widgets in main window
void CreateWidgets(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hButton = CreateWindow(
		TEXT("BUTTON"),
		TEXT("Button"),
		WS_VISIBLE | WS_CHILD | ES_CENTER,
		10, 10,
		100, 20,
		hWnd,
		(HMENU)WM_COMMAND_BUTTON,
		nullptr,
		nullptr);

	HWND hLable = CreateWindow(
		TEXT("static"),
		TEXT("Lable"),
		WS_VISIBLE | WS_CHILD | ES_CENTER,
		10, 30,
		100, 20,
		hWnd,
		nullptr,
		nullptr,
		nullptr);
}