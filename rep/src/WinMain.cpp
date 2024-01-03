#include"windows.h"

LRESULT CALLBACK WndProcUpdate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
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
	// Create window class

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProcUpdate; // message handler
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = TEXT("Test class WNDCLASSEX");
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



	// Create main window
	// https://learn.microsoft.com/en-us/previous-versions/ms960010(v=msdn.10)

	HWND hWndMain = CreateWindowEx(
		WS_EX_LEFT,
		TEXT("Test class WNDCLASSEX"),
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

	if (!hWndMain)
	{
		MessageBox(
			NULL,
			TEXT("CreateWindowEx failed!"),
			TEXT("Exception"),
			NULL);

		return 1;
	}



	// infinite cycle
	MSG message;
	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return 0;
}

