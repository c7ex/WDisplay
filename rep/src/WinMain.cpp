#include"windows.h"

// the correct entry WinMain
int WINAPI WinMain(
	_In_     HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_     LPSTR     lpCmdLine,
	_In_     int       nCmdShow)
{
	// https://learn.microsoft.com/en-us/previous-versions/ms960010(v=msdn.10)
	HWND hMain = CreateWindowEx(
		WS_EX_LEFT,
		WC_DIALOG, 
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

	// infinite cycle
	while (1);

	return 0;
}

