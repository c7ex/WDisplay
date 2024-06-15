#pragma once


// // // // // // // // //      main timer     // // // // // // // // // //

namespace main_timer
{
	size_t id = 0;
	size_t interval = 2 * USER_TIMER_MINIMUM;
}



// // // // // // // // //         keys        // // // // // // // // // //

namespace keys
{
	bool shift = false;
	bool ctrl = false;
}


// // // // // // // // //    stock objects    // // // // // // // // // //

namespace stock_objects
{
	struct
	{
		COLORREF white          = RGB(0xFF, 0xFF, 0xFF);
		COLORREF black          = RGB(0x00, 0x00, 0x00);
		COLORREF blue           = RGB(0x00, 0x00, 0xFF);
		COLORREF green          = RGB(0x00, 0xFF, 0x00);
		COLORREF red            = RGB(0xFF, 0x00, 0x00);

		COLORREF chart          = RGB(0x0E, 0x1A, 0x0E);
		COLORREF bound          = RGB(0x0E, 0x0E, 0x0E);
		COLORREF axis           = RGB(0x35, 0x35, 0x35);
		COLORREF labels         = RGB(0x85, 0x75, 0x55);
	} color;
	
	struct
	{
		HPEN white              = CreatePen(PS_SOLID, 1, color.white);
		HPEN black              = CreatePen(PS_SOLID, 1, color.black);
		HPEN blue               = CreatePen(PS_SOLID, 1, color.blue);
		HPEN green              = CreatePen(PS_SOLID, 1, color.green);
		HPEN red                = CreatePen(PS_SOLID, 1, color.red);

		HPEN chart              = CreatePen(PS_SOLID, 1, color.chart);
		HPEN bound              = CreatePen(PS_SOLID, 1, color.bound);
		HPEN axis               = CreatePen(PS_SOLID, 1, color.axis);
		HPEN labels             = CreatePen(PS_SOLID, 1, color.labels);
	} pen;

	struct
	{
		HBRUSH white            = CreateSolidBrush(color.white);
		HBRUSH black            = CreateSolidBrush(color.black);
		HBRUSH blue             = CreateSolidBrush(color.blue);
		HBRUSH green            = CreateSolidBrush(color.green);
		HBRUSH red              = CreateSolidBrush(color.red);

		HBRUSH chart            = CreateSolidBrush(color.chart);
		HBRUSH bound            = CreateSolidBrush(color.bound);
	} brush;
}