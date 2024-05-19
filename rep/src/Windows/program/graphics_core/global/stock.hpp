#pragma once
namespace stock_objects
{
	struct
	{
		COLORREF white          = RGB(0xFF, 0xFF, 0xFF);
		COLORREF black          = RGB(0x00, 0x00, 0x00);
		COLORREF green          = RGB(0x00, 0xFF, 0x00);
		COLORREF red            = RGB(0xFF, 0x00, 0x00);
		COLORREF bound          = RGB(0x0E, 0x0E, 0x0E);
		COLORREF plot           = RGB(0x0E, 0x1A, 0x0E);
		COLORREF sup_axis       = RGB(0x35, 0x35, 0x35);
		COLORREF labels_axis    = RGB(0x85, 0x75, 0x55);
		COLORREF data_style_1   = RGB(0x45, 0x75, 0x24);
	} color;
	
	struct
	{
		HPEN bound              = CreatePen(PS_SOLID, 1, color.bound);
		HPEN sup_axis           = CreatePen(PS_SOLID, 1, color.sup_axis);
		HPEN labels_axis        = CreatePen(PS_SOLID, 1, color.labels_axis);
		HPEN data_style_1       = CreatePen(PS_SOLID, 3, color.data_style_1);
	} pen;

	struct
	{
		HBRUSH plot             = CreateSolidBrush(color.plot);
		HBRUSH bound            = CreateSolidBrush(color.bound);
	} brush;
}

namespace key_is_active
{
	bool shift;
	bool ctrl;
}

namespace gl_timer
{
	size_t main_id = 0;
	size_t main_interval = USER_TIMER_MINIMUM;
}

namespace gl_data
{
	content data_content;
}

bool FullScreen = false;
WINDOWPLACEMENT wpc;