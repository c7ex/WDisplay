#include"WinEntry.hpp"

HINSTANCE arg_hInstance;
HINSTANCE arg_hPrevInstance;
LPSTR     arg_lpCmdLine;
int       arg_nCmdShow;

int main()
{
	WinMain(
		arg_hInstance,
		arg_hPrevInstance,
		arg_lpCmdLine,
		arg_nCmdShow);
}