#pragma once
namespace stock_objects
{
	struct
	{
		COLORREF white        = RGB(0xFF, 0xFF, 0xFF);
		COLORREF black        = RGB(0x00, 0x00, 0x00);
		COLORREF green        = RGB(0x00, 0xFF, 0x00);
		COLORREF red          = RGB(0xFF, 0x00, 0x00);
		COLORREF main_axis    = RGB(0xF5, 0xA5, 0x94);
		COLORREF sup_axis     = RGB(0x15, 0x15, 0x14);
		COLORREF labels_axis  = RGB(0xF5, 0x85, 0x85);
		COLORREF data_style_1 = RGB(0x65, 0x05, 0x44);
	} color;
	
	struct
	{
		HPEN background   = CreatePen(PS_SOLID, 1, color.black);
		HPEN main_axis    = CreatePen(PS_SOLID, 1, color.main_axis);
		HPEN sup_axis     = CreatePen(PS_SOLID, 1, color.sup_axis);
		HPEN labels_axis  = CreatePen(PS_SOLID, 1, color.labels_axis);
		HPEN data_style_1 = CreatePen(PS_SOLID, 2, color.data_style_1);
	} pen;

	struct
	{
		HBRUSH background = CreateSolidBrush(color.black);
	} brush;
}

namespace key_is_active
{
	bool shift;
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