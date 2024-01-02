#include"windows.h"

LRESULT CALLBACK WndProcUpdate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

// the correct entry WinMain
int WINAPI WinMain(
	_In_     HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_     LPSTR     lpCmdLine,
	_In_     int       nCmdShow)
{
	// Create main window
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

	if (!hMain)
	{
		MessageBox(
			NULL,
			TEXT("CreateWindowEx failed!"),
			TEXT("Exception"),
			NULL);

		return 1;
	}

	// Create window class

	WNDCLASSEX wcex;

	wcex.lpfnWndProc = WndProcUpdate; // message handler
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = TEXT("Empty window");
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(
			NULL,
			TEXT("RegisterClassEx failed!"),
			TEXT("Exception"),
			NULL);

		return 1;
	}

	// infinite cycle
	while (1);

	return 0;
}

