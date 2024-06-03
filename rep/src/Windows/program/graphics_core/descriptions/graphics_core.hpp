#pragma once

// rules:
// scales - scroll
// moving - mouse



// // // // // // // // //      main timer     // // // // // // // // // //

namespace main_timer
{
	size_t id = 0;
	size_t interval = 2 * USER_TIMER_MINIMUM;
}



// // // // // // // // //         keys        // // // // // // // // // //

namespace keys
{
	bool shift;
	bool ctrl;
}

// point
struct _xy
{
	double x, y = 0;

	void set(double new_x, double new_y)
	{
		x = new_x;
		y = new_y;
	}

	void set(_xy& other)
	{
		x = other.x;
		y = other.y;
	}

	void set_end(_xy start, _xy size)
	{
		x = start.x + size.x;
		y = start.y + size.y;
	}

	void set_middle(_xy start, _xy size)
	{
		x = start.x + size.x/2;
		y = start.y + size.y/2;
	}

	_xy(double new_x, double new_y)
	{
		x = new_x;
		y = new_y;
	}

	_xy(void) { x, y = 0; }
};

// point form-proportion
struct _fxy
{
	_xy cur;
	_xy ref;

	void set(_xy ref_init)
	{
		cur.set(ref_init);
		ref.set(ref_init);
	}

	void refresh(_xy& new_scales)
	{
		cur.set(
			ref.x * new_scales.x,
			ref.y * new_scales.y);
	}
};

// form proportion rect
struct _frect
{
	_fxy start;
	_fxy size;
	_xy end;
	_xy middle;

	void update(_xy& compression)
	{
		start.refresh(compression);
		size.refresh(compression);
		end.set_end(start.cur, size.cur);
		middle.set_middle(start.cur, size.cur);
	}

	void paint(HDC& hMemDc)
	{
		Rectangle(hMemDc,
			(int)start.cur.x,
			(int)start.cur.y,
			(int)end.x,
			(int)end.y);
	}

	void set(_xy init_start, _xy init_size)
	{
		start.set(init_start);
		size.set(init_size);
		end.set_end(init_start, init_size);
		middle.set_middle(init_start, init_size);
	}
};

#define DEFAULT_SIZE_WINDOW_X 400
#define DEFAULT_SIZE_WINDOW_Y 400

#define CHART_START_POINT _xy(30,20)
#define CHART_SIZE        _xy(300,350)

struct wdisplay
{
	_xy window_size_in_pixels;
	_xy window_mouse_position_in_pixels;
	_xy compression_factors;

	struct chart
	{
		_frect plot;
		_frect bound[4];

		void paint_chart(HDC& hMemDc, HPEN& pen, HBRUSH& brush, HPEN& bpen, HBRUSH& bbrush)
		{
			SelectObject(hMemDc, reinterpret_cast<HGDIOBJ>(pen));
			SelectObject(hMemDc, reinterpret_cast<HGDIOBJ>(brush));
			plot.paint(hMemDc);

			SelectObject(hMemDc, reinterpret_cast<HGDIOBJ>(bpen));
			SelectObject(hMemDc, reinterpret_cast<HGDIOBJ>(bbrush));
			bound[0].paint(hMemDc);
			bound[1].paint(hMemDc);
			bound[2].paint(hMemDc);
			bound[3].paint(hMemDc);
		}

		void update()
		{
			gc.ch.plot.update(gc.compression_factors);

			gc.ch.bound[0].update(gc.compression_factors);
			gc.ch.bound[1].update(gc.compression_factors);
			gc.ch.bound[2].update(gc.compression_factors);
			gc.ch.bound[3].update(gc.compression_factors);
		}

		chart()
		{
			plot.set(CHART_START_POINT, CHART_SIZE);
			
			_xy start_point_bound0 = _xy(0, 0);
			_xy size_bound0 = _xy(CHART_START_POINT.x, DEFAULT_SIZE_WINDOW_Y);
			bound[0].set(start_point_bound0, size_bound0);
		
			_xy start_point_bound1 = _xy(CHART_START_POINT.x, 0);
			_xy size_bound1 = _xy(DEFAULT_SIZE_WINDOW_X, CHART_START_POINT.y);
			bound[1].set(start_point_bound1, size_bound1);

			_xy start_point_bound2 = _xy(0, CHART_START_POINT.y + CHART_SIZE.y);
			_xy size_bound2 = _xy(DEFAULT_SIZE_WINDOW_X, DEFAULT_SIZE_WINDOW_Y);
			bound[2].set(start_point_bound2, size_bound2);

			_xy start_point_bound3 = _xy(CHART_START_POINT.x + CHART_SIZE.x, 0);
			_xy size_bound3 = _xy(DEFAULT_SIZE_WINDOW_X, DEFAULT_SIZE_WINDOW_Y);
			bound[3].set(start_point_bound3, size_bound3);
		}

		// 1 plot          +
		// 2 bound         +
		// 3 axis
		// 4 data <-load
		// 5 lables

	}ch;

} gc;