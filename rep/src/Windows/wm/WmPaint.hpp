#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

// need BitBlt for fast paint (create copy devise in memory)

HDC          hdc;
PAINTSTRUCT  ps;
RECT         rt;

int xPos;
int yPos;

// graphic handler
LRESULT WmPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GetWindowRect(hWnd, &rt);
	
	int x_pos_end = (rt.right - rt.left) - 17;
	int y_pos_end = (rt.bottom - rt.top) - 40;

	hdc = BeginPaint(hWnd, &ps);

	// Paint background
	HGDIOBJ pen = SelectObject(hdc, GetStockObject(BLACK_PEN));
	HGDIOBJ brush = SelectObject(hdc, GetStockObject(SYSTEM_FONT));
	Rectangle(hdc, 1, 2, x_pos_end, y_pos_end);

	//?
	//SelectObject(hdc, pen); 
	//SelectObject(hdc, brush);
	DeleteObject(pen);
	DeleteObject(brush);

	// Mouse info
	std::wstring x_message = L"x(" + std::to_wstring(xPos) + L")";
	std::wstring y_message = L"y(" + std::to_wstring(yPos) + L")";

	HFONT hFont = CreateFont(11, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 2, 0, L"SYSTEM_FIXED_FONT");
	HFONT hTmp = (HFONT)SelectObject(hdc, hFont);
	SetBkMode(hdc, TRANSPARENT);

	TextOut(hdc, 20 + xPos, yPos - 10, x_message.c_str(), x_message.size());
	TextOut(hdc, 20 + xPos, yPos - 0 , y_message.c_str(), y_message.size());

	DeleteObject(hFont);
	DeleteObject(hTmp);

	EndPaint(hWnd, &ps);
	return static_cast<LRESULT>(0);
}