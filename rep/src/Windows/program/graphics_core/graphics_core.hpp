#pragma once

void paint_line(HDC& hMemDc, double x0, double y0, double x1, double y1)
{
	MoveToEx(hMemDc, (int)x0, (int)y0, NULL);
	LineTo(hMemDc, (int)x1, (int)y1);
}

void paint_line(HDC& hMemDc, pointf p1, pointf p2)
{
	MoveToEx(hMemDc, (int)p1.get_x(), (int)p1.get_y(), NULL);
	LineTo(hMemDc, (int)p2.get_x(), (int)p2.get_y());
}

// form:
// 1. update and save state mouse 
// 2. update and save expand form
class form
{
private:
	masterparam _size;
	pointf _mouse;

public:
	pointf& mouse()
	{
		return _mouse;
	}

	masterparam& size()
	{
		return _size;
	}

public:
	void update_size(const paramf& size)
	{
		_size.update_from_current(size);
	}
	
	void update_mouse(const pointf& position)
	{
		_mouse.set(position);
	}

public:
	form()
	{
		_size.set(Init::form.size);
	}
};

// states for chart: abstract coordinations
// 1. Change offset - mouse move
// 2. Change scale - mousewheel scroll
class graphics_engine
{
private:
	paramf size;
	paramf centre;
	paramf expand;
	pointf hold;
	paramf current_offset;
	paramf last_offset;
	paramf scale;
	paramf scale_counter;

	paramf plot_centre;
	paramf plot_size;

	bool   hold_state   = false;
	bool   active_state = false;

public:
	// abstruct = (window - middleplot) * expand + centre
	pointf get_abstruct(pointf pos)
	{
		return pointf{
			(pos.get_x() - plot_centre.get_x()) * expand.get_x() + centre.get_x(),
			(plot_centre.get_y() - pos.get_y()) * expand.get_y() - centre.get_y() };
	}

	// window = (abstruct - centre) / expand + middleplot
	pointf get_window(pointf pos)
	{
		return pointf{
			((pos.get_x() - centre.get_x()) / expand.get_x()) + plot_centre.get_x(),
			plot_centre.get_y() - ((pos.get_y() + centre.get_y()) / expand.get_y()) };
	}

	paramf& get_size()
	{
		return size;
	}

	paramf& get_centre()
	{
		return centre;
	}

public:
	// update states if [WM_MOUSEWHEEL]
	void event_rescale(pointf pos, double inc)
	{
		if (active_state == false) return;
		if (hold_state == true) return;

		check_scale_limit(inc);

		paramf last_abstr_expand = expand;
		paramf last_abstr_centre = centre;

		scale.set(
			pow(DEFAULT_SCALE_MULTIPLIER, scale_counter.get_x()),
			pow(DEFAULT_SCALE_MULTIPLIER, scale_counter.get_y()));

		size = size * scale;
		expand = size / plot_size;

		paramf delta_expand = last_abstr_expand - expand;
		paramf curr_pixel_offset = pos - plot_centre;

		centre = paramf{
			curr_pixel_offset.get_x() * delta_expand.get_x() + last_abstr_centre.get_x(),
			curr_pixel_offset.get_y() * delta_expand.get_y() + last_abstr_centre.get_y() };

		last_offset = centre;
	}

	// update states if [WM_MOUSEMOVE]
	void event_mousemove(pointf pos)
	{
		if (active_state == false) return;

		if (hold_state == true)
		{
			current_offset = last_offset + (paramf{ hold - pos }) * expand;
			centre = current_offset;
		}
	}

	void event_hold_start(pointf pos)
	{ 
		if (active_state == false) return;

		hold = pos;
		hold_state = true;
	}

	void event_hold_stop()
	{
		if (active_state == false) return;

		last_offset = centre;
		hold_state = false;
	}

	void event_active(pointf pos)
	{
		pointf variance = plot_centre - pos;
		variance.mod();

		if ((plot_size.to_pointf() / 2) > variance)
		{
			active_state = true;
		}
		else
		{
			// if hold and out in acrive area
			if (hold_state == true)
			{
				last_offset = centre;
				hold_state = false;
			}
			active_state = false;
		}
	}

	void check_scale_limit(double inc)
	{
		paramf last_scale_counter = scale_counter;

		size = size / scale;

		if (keys::shift == true)
			scale_counter = scale_counter + paramf(inc, 0);
		else
			scale_counter = scale_counter + inc;

		paramf next_scale = size * paramf(
			pow(DEFAULT_SCALE_MULTIPLIER, scale_counter.get_x()),
			pow(DEFAULT_SCALE_MULTIPLIER, scale_counter.get_y()));

		std::cout << next_scale.get_x() << "\n";

		double limit_scale_x = scale_counter.get_x();
		if ((next_scale.get_x() > Init::engine.upper_limit_size.get_x()) || next_scale.get_x() < Init::engine.lower_limit_size.get_x())
			limit_scale_x = last_scale_counter.get_x();

		double limit_scale_y = scale_counter.get_y();
		if ((next_scale.get_y() > Init::engine.upper_limit_size.get_y()) || next_scale.get_y() < Init::engine.lower_limit_size.get_y())
			limit_scale_y = last_scale_counter.get_y();

		scale_counter.set(limit_scale_x, limit_scale_y);
	}

public:
	bool is_active()
	{
		return active_state;
	}

	void update(rectpropf& plot)
	{
		plot_size = plot.get_size().get_current();
		plot_centre = plot.get_centre().get_current();
		expand = size / plot_size;

		if (active_state == true) return;
	}

public:
	void Init()
	{
		active_state = false;
		hold_state = false;

		size.set(Init::engine.size);
		centre.set(Init::engine.centre);
		expand.set(Init::engine.size / Init::plot.size);

		last_offset.set(Init::engine.centre);

		scale.set(1., 1.);
		scale_counter.set(0, 0);
	}

	graphics_engine() {};
};

// axis
// 1. paint axes
// 2. paint labels for axis
class axes
{
private:
	int      range_size = 3;
	double   reference_range[3] = { 1, 2, 5};

public:
	paramf size;
	paramf centre;
	pointf spoint;
	pointf epoint;

	paramf count;
	paramf discret;

public:
	void update(paramf new_size, paramf new_centre)
	{
		size = new_size;
		centre = new_centre;

		spoint = centre.to_pointf() - (size.to_pointf() * types_HALF);
		epoint = centre.to_pointf() + (size.to_pointf() * types_HALF);

		spoint.y_inv();
		epoint.y_inv();

		paramf coarse_discret = size / count;

		double level_x = exclusive::range_level(coarse_discret.get_x());
		double level_y = exclusive::range_level(coarse_discret.get_y());

		double discret_x = exclusive::nearest_in_range(coarse_discret.get_x(), level_x, &reference_range[0], range_size);
		double discret_y = exclusive::nearest_in_range(coarse_discret.get_y(), level_y, &reference_range[0], range_size);

		discret.set(discret_x, discret_y);
	}

	void paint_axes(HDC& hdc, graphics_engine& engine)
	{
		SelectObject(hdc, reinterpret_cast<HGDIOBJ>(stock_objects::pen.axis));

		double horisontal_axes = round(spoint.get_y() / discret.get_y()) * discret.get_y();

		while (horisontal_axes >= epoint.get_y())
		{

			paint_line(hdc,
				engine.get_window(pointf(spoint.get_x(), horisontal_axes)),
				engine.get_window(pointf(epoint.get_x(), horisontal_axes)));

			horisontal_axes -= discret.get_y();
		}

		double vertical_axes = round(spoint.get_x() / discret.get_x()) * discret.get_x();

		while (vertical_axes <= epoint.get_x())
		{

			paint_line(hdc,
				engine.get_window(pointf(vertical_axes, spoint.get_y())),
				engine.get_window(pointf(vertical_axes, epoint.get_y())));

			vertical_axes += discret.get_x();
		}
	}

	void paint_lables(HDC& hdc, graphics_engine& engine, COLORREF color = stock_objects::color.labels)
	{
		SelectObject(hdc, reinterpret_cast<HGDIOBJ>(stock_objects::pen.labels));
		SetTextColor(hdc, color);
		SetTextAlign(hdc, TA_CENTER);

		double vertical_lables = round(spoint.get_y() / discret.get_y()) * discret.get_y();
		if (vertical_lables > spoint.get_y())
			vertical_lables -= discret.get_y();

		// vertical
		// x - const
		// y - change
		while (vertical_lables >= epoint.get_y())
		{
			double x = engine.get_window(spoint).get_x() / 2.;
			double y = engine.get_window(pointf(x, vertical_lables)).get_y() - 6;
			
			std::wstring gl_wstr = exclusive::double2wstring(vertical_lables);

			pointf supline_point1 = engine.get_window(pointf(spoint.get_x(), vertical_lables));
			pointf supline_point2 = supline_point1 + pointf(5, 0);
			paint_line(hdc, supline_point1, supline_point2);

			TextOut(hdc, x, y, gl_wstr.c_str(), (int)gl_wstr.size());

			vertical_lables -= discret.get_y();
		}

		double horisontal_lables = round(spoint.get_x() / discret.get_x()) * discret.get_x();
		if (horisontal_lables < spoint.get_x())
			horisontal_lables += discret.get_x();

		// vertical
		// x - change
		// y - const
		while (horisontal_lables <= epoint.get_x())
		{
			double y = engine.get_window(epoint).get_y() + 5;
			double x = engine.get_window(pointf(horisontal_lables, y)).get_x() + 1;

			std::wstring gl_wstr = exclusive::double2wstring(horisontal_lables);

			pointf supline_point1 = engine.get_window(pointf(horisontal_lables, epoint.get_y()));
			pointf supline_point2 = supline_point1 + pointf(0, -5);
			paint_line(hdc, supline_point1, supline_point2);

			TextOut(hdc, x, y, gl_wstr.c_str(), (int)gl_wstr.size());

			horisontal_lables += discret.get_x();
		}

		SetTextAlign(hdc, VTA_BASELINE);
	}

public:
	void Init()
	{
		size.set(Init::axes.size);
		centre.set(Init::axes.centre);
		count.set(Init::axes.count);

		spoint = centre.to_pointf() - (size.to_pointf() * types_HALF);
		epoint = centre.to_pointf() + (size.to_pointf() * types_HALF);

		spoint.y_inv();
		epoint.y_inv();

		paramf coarse_discret = size / count;
		
		double level_x = exclusive::range_level(coarse_discret.get_x());
		double level_y = exclusive::range_level(coarse_discret.get_y());

		double discret_x = exclusive::nearest_in_range(coarse_discret.get_x(), level_x, &reference_range[0], range_size);
		double discret_y = exclusive::nearest_in_range(coarse_discret.get_x(), level_x, &reference_range[0], range_size);

		discret.set(discret_x, discret_y);
	}

	axes() {};
};

 // chart
 // paint bounds and graph area
class chart
{
private:
	// paint objects
	rectpropf _plot;
	rectpropf _bounds[4];
	axes      _axes;
	
private:
	graphics_engine _engine;

public:
	rectpropf& plot()
	{
		return _plot;
	}

	graphics_engine& engine()
	{
		return _engine;
	}

public:

	void update(const paramf& expand)
	{
		_plot.update(expand);

		_engine.update(_plot);

		_axes.update(
			_engine.get_size(), 
			_engine.get_centre());

		_bounds[0].update(expand);
		_bounds[1].update(expand);
		_bounds[2].update(expand);
		_bounds[3].update(expand);
	}

	void paint(HDC& hdc)
	{
		_plot.paint(hdc, stock_objects::pen.chart, stock_objects::brush.chart);

		_axes.paint_axes(hdc, _engine);

		// test obj
		SelectObject(hdc, reinterpret_cast<HGDIOBJ>(stock_objects::pen.white));
		paint_line(hdc,
			_engine.get_window(pointf(-30, -30)),
			_engine.get_window(pointf(30, 30)));

		_bounds[0].paint(hdc, stock_objects::pen.bound, stock_objects::brush.bound);
		_bounds[1].paint(hdc, stock_objects::pen.bound, stock_objects::brush.bound);
		_bounds[2].paint(hdc, stock_objects::pen.bound, stock_objects::brush.bound);
		_bounds[3].paint(hdc, stock_objects::pen.bound, stock_objects::brush.bound);

		_axes.paint_lables(hdc, _engine);

		if (_engine.is_active())
			SelectObject(hdc, reinterpret_cast<HGDIOBJ>(stock_objects::pen.chart_enable));
		else
			SelectObject(hdc, reinterpret_cast<HGDIOBJ>(stock_objects::pen.chart_disable));

		pointf spoint = _plot.get_start_point();
		pointf epoint = _plot.get_end_point();

		paint_line(hdc, spoint.get_x(), spoint.get_y(), epoint.get_x(), spoint.get_y());
		paint_line(hdc, epoint.get_x(), spoint.get_y(), epoint.get_x(), epoint.get_y());
		paint_line(hdc, epoint.get_x(), epoint.get_y(), spoint.get_x(), epoint.get_y());
		paint_line(hdc, spoint.get_x(), epoint.get_y(), spoint.get_x(), spoint.get_y());
	}

public:
	chart()
	{
		_plot.init_reference(Init::plot.size, Init::plot.centre);

		double x0size   = Init::form.size.get_x();
		double y0size   = Init::plot.centre.get_y() - Init::plot.size.get_y() * types_HALF;
		double x0centre = x0size * types_HALF;
		double y0centre = y0size * types_HALF;

		_bounds[0].init_reference(
			paramf(x0size, y0size),
			paramf(x0centre, y0centre));

		double x1size = Init::plot.centre.get_x() - Init::plot.size.get_x() * types_HALF;
		double y1size = Init::form.size.get_y();
		double x1centre = x1size * types_HALF;
		double y1centre = y1size * types_HALF;

		_bounds[1].init_reference(
			paramf(x1size, y1size),
			paramf(x1centre, y1centre));

		double d_y = Init::plot.centre.get_y() + Init::plot.size.get_y() * types_HALF;
		double x2size = Init::form.size.get_x();
		double y2size = Init::form.size.get_y() - d_y;
		double x2centre = x2size * types_HALF;
		double y2centre = (Init::form.size.get_y() + d_y) * types_HALF;

		_bounds[2].init_reference(
			paramf(x2size, y2size),
			paramf(x2centre, y2centre));

		double d_x = Init::plot.centre.get_x() + Init::plot.size.get_x() * types_HALF;
		double x3size = Init::form.size.get_x() - d_x;
		double y3size = Init::form.size.get_y();
		double x3centre = (Init::form.size.get_x() + d_x) * types_HALF;
		double y3centre = y3size * types_HALF;

		_bounds[3].init_reference(
			paramf(x3size, y3size),
			paramf(x3centre, y3centre));

		_engine.Init();
		_axes.Init();
	}
};

// window
// all objects
class wdisplay
{
private:
	form   _form;
	chart  _chart;

public:
	form& form()
	{
		return _form;
	}

	chart& chart()
	{
		return _chart;
	}

public:
	void update()
	{
		_chart.update(_form.size().get_expand());
	}

	void AddLable(HDC& hdc, pointf position, std::wstring text, double value, COLORREF color = stock_objects::color.white)
	{
		SetTextColor(hdc, color);
		std::wstring gl_wstr = (text)+L"(" + std::to_wstring(value) + L")";
		pointf current = position * _form.size().get_expand();
		TextOut(hdc, current.get_x(), current.get_y(), gl_wstr.c_str(), (int)gl_wstr.size());
	}

	void paint(HDC& hdc)
	{
		_chart.paint(hdc);
	}

public:
	wdisplay(){}
};

wdisplay wds;

// hot define
#define Form_Mouse                         wds.form().mouse()
#define Form_Size                          wds.form().size().get_current()
#define Form_Expand                        wds.form().size().get_expand()
#define Engine                             wds.chart().engine()

#define Engine_Active(pos)                 wds.chart().engine().event_active(pos)
#define Engine_Mouse_Wheel(pos, inc)       wds.chart().engine().event_rescale(pos, inc)
#define Engine_Mouse_Move(pos)             wds.chart().engine().event_mousemove(pos)
#define Engine_Hold_Start(pos)             wds.chart().engine().event_hold_start(pos)
#define Engine_Hold_Stop                   wds.chart().engine().event_hold_stop()