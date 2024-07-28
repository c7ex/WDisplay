#pragma once


// // // // // // // // //   fast performance   // // // // // // // // // //
namespace fast_performance
{
	double fps;
}


// // // // // // // // //      main timer     // // // // // // // // // //
namespace main_timer
{
	size_t id = 0;
	size_t interval = 1 * USER_TIMER_MINIMUM;
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

		COLORREF bound          = RGB(0x0E, 0x0E, 0x0E);
		COLORREF axis           = RGB(0x30, 0x35, 0x50);
		COLORREF labels         = RGB(0x40, 0x35, 0x70);

		COLORREF chart          = RGB(0x1A, 0x15, 0x35);
		COLORREF chart_enable   = RGB(0x15, 0x15, 0x3E);
		COLORREF chart_disable  = RGB(0x80, 0x1A, 0x0E);
	} color;
	
	struct
	{
		HPEN white              = CreatePen(PS_SOLID, 1, color.white);
		HPEN black              = CreatePen(PS_SOLID, 1, color.black);
		HPEN blue               = CreatePen(PS_SOLID, 1, color.blue);
		HPEN green              = CreatePen(PS_SOLID, 1, color.green);
		HPEN red                = CreatePen(PS_SOLID, 1, color.red);

		HPEN bound              = CreatePen(PS_SOLID, 1, color.bound);
		HPEN axis               = CreatePen(PS_SOLID, 1, color.axis);
		HPEN labels             = CreatePen(PS_SOLID, 1, color.labels);

		HPEN chart              = CreatePen(PS_SOLID, 1, color.chart);
		HPEN chart_enable       = CreatePen(PS_SOLID, 1, color.chart_enable);
		HPEN chart_disable      = CreatePen(PS_SOLID, 1, color.chart_disable);

		HPEN test               = CreatePen(PS_SOLID, 2, color.white);
		HPEN test2              = CreatePen(PS_SOLID, 2, color.red);
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


// // // // // // // // //      init data      // // // // // // // // // //

#define DEFAULT_SCALE_MULTIPLIER   1.05
#define SCALE_DIVIDER_DEFAULT      -120. // 1 tick = 1 count
#define SCALE_DIVIDER_FAST          -40. // 1 tick = 3 count

namespace Init
{
	struct
	{
		xy_param size             = xy_param{ 400. ,400. };
	}form;

	struct
	{
		xy_param size             = xy_param{ 270., 360. };
		xy_point centre           = xy_point{ 160., 200. };
	}plot;

	struct
	{
		xy_param size             = xy_param{ 100.,  100.};
		xy_param upper_limit_size = xy_param{ 10e6, 10e4 };
		xy_param lower_limit_size = xy_param{ 1e-3, 1e-3 };
		xy_point centre           = xy_point{   0.,    0.};
	}engine;

	struct
	{
		xy_param size             = engine.size;
		xy_point centre           = engine.centre;
		xy_param coarse_count     = xy_param{  18.,  18. };
	}axes;

}


// // // // // // // // //        methods      // // // // // // // // // //

namespace exclusive
{
	// return range
	// ...
	// value in range [ 0.1 ;   1)  return -1
	// value in range [   1 ;  10)  return  0
	// value in range [  10 ; 100)  return  1
	// ...
	int range_level(double value)
	{
		double level = log10(value);
		if (level < 0) level -= 1;
		return int(level);
	}

	// return nearest value from range
	double nearest_in_range(double input_value, int level, double* range, int size_range)
	{
		double nearest;

		double* current_range = (double*)malloc(sizeof(double) * size_range);

		if (current_range)
		{
			for (int index = 0; index < size_range; index++)
				current_range[index] = range[index] * pow(10., level);
		}

		double* difference = (double*)malloc(sizeof(double) * size_range);

		if (difference)
		{
			for (int index = 0; index < size_range; index++)
				difference[index] = abs(current_range[index] - input_value);


			int min_index;
			double min_dif = current_range[size_range - 1];
			for (int index = 0; index < size_range; index++)
			{
				if (difference[index] < min_dif)
				{
					min_index = index;
					min_dif = difference[index];
				}
			}

			nearest = current_range[min_index];
		}

		free(difference);
		free(current_range);

		return nearest;
	}

	std::wstring double2wstring(double value, double norm = 3)
	{
		if (value == 0)
			return std::to_wstring(0);

		std::wstring result = std::to_wstring(value);

		int size = static_cast<int>(result.size());
		int k = 0;

		while (k != size - 1)
		{
			int current_index = size - 1 - k;

			k++;
			if (result[current_index] == L'.')
				break;

			k--;
			if (result[current_index] != L'0')
				break;

			k++;
		}

		result.resize(size - k);

		return result;
	}

	COLORREF change_ref_color(COLORREF& reference_color, COLORREF& background_color, double coefficient)
	{
		int ref_r = (reference_color & 0x0000ff);
		int ref_g = (reference_color & 0x00ff00) >> 8;
		int ref_b = (reference_color & 0xff0000) >> 16;

		int gnd_r = (background_color & 0x0000ff);
		int gnd_g = (background_color & 0x00ff00) >> 8;
		int gnd_b = (background_color & 0xff0000) >> 16;

		int del_r = ref_r - gnd_r;
		int del_g = ref_g - gnd_g;
		int del_b = ref_b - gnd_b;

		std::size_t r = coefficient * del_r + gnd_r;
		std::size_t g = coefficient * del_g + gnd_g;
		std::size_t b = coefficient * del_b + gnd_b;

		return RGB(r,g,b);
	}
}

namespace paint
{
	void line(HDC& hMemDc, double x0, double y0, double x1, double y1)
	{
		MoveToEx(hMemDc, (int)x0, (int)y0, NULL);
		LineTo(hMemDc, (int)x1, (int)y1);
	}

	void line(HDC& hMemDc, xy_point p1, xy_point p2)
	{
		MoveToEx(hMemDc, (int)p1.get_x(), (int)p1.get_y(), NULL);
		LineTo(hMemDc, (int)p2.get_x(), (int)p2.get_y());
	}

	inline void rect(HDC& hMemDc, xy_point p, int size)
	{
		Rectangle(hMemDc,
			p.get_x() - size,
			p.get_y() - size,
			p.get_x() + size,
			p.get_y() + size);
	}
}