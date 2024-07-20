#pragma once

// form:
// 1. update and save state mouse 
// 2. update and save expand form
class form
{
private:
	master_param _size;
	xy_point _mouse;

public:
	xy_point& mouse()
	{
		return _mouse;
	}

	master_param& size()
	{
		return _size;
	}

public:
	void update_size(const xy_param& size)
	{
		_size.update_from_current(size);
	}
	
	void update_mouse(const xy_point& position)
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
	xy_param size;
	xy_point centre; // abstruct-coordinate in windows coordinate system

	xy_param expand;
	xy_point hold;
	xy_param current_offset;
	xy_param last_offset;
	xy_param scale;
	xy_param scale_counter;

	xy_point plot_centre;
	xy_param plot_size;

	xy_point limit_left_down;
	xy_point limit_right_up;

	bool   hold_state   = false;
	bool   active_state = false;

public:
	// abstruct = (window - middleplot) * expand + centre
	xy_point get_abstruct(xy_point pos)
	{
		return xy_point{
			(pos.get_x() - plot_centre.get_x()) * expand.get_x() + centre.get_x(),
			(plot_centre.get_y() - pos.get_y()) * expand.get_y() - centre.get_y() };
	}

	// window = (abstruct - centre) / expand + middleplot
	xy_point get_window(xy_point pos)
	{
		return xy_point{
			((pos.get_x() - centre.get_x()) / expand.get_x()) + plot_centre.get_x(),
			plot_centre.get_y() - ((pos.get_y() + centre.get_y()) / expand.get_y()) };
	}

public:
	xy_param& get_plot_size()
	{
		return plot_size;
	}

	xy_point& get_plot_centre()
	{
		return plot_centre;
	}

public:
	xy_param& get_size()
	{
		return size;
	}

	xy_point& get_centre()
	{
		return centre;
	}

public:
	xy_point& get_limit_left_down()
	{
		return limit_left_down;
	}

	xy_point& get_limit_right_up()
	{
		return limit_right_up;
	}

public:
	// update states if [WM_MOUSEWHEEL]
	void event_rescale(xy_point pos, double inc)
	{
		if (active_state == false) return;
		if (hold_state == true) return;

		check_scale_limit(inc);

		xy_param last_abstr_expand = expand;
		xy_point last_abstr_centre = centre;

		scale.set(
			pow(DEFAULT_SCALE_MULTIPLIER, scale_counter.get_x()),
			pow(DEFAULT_SCALE_MULTIPLIER, scale_counter.get_y()));

		size = size * scale;
		expand = size / plot_size;

		xy_param delta_expand = last_abstr_expand - expand;
		xy_param curr_pixel_offset = pos - plot_centre;

		centre = xy_param{
			curr_pixel_offset.get_x() * delta_expand.get_x() + last_abstr_centre.get_x(),
			curr_pixel_offset.get_y() * delta_expand.get_y() + last_abstr_centre.get_y() };

		last_offset = centre;
	}

	// update states if [WM_MOUSEMOVE]
	void event_mousemove(xy_point pos)
	{
		if (active_state == false) return;

		if (hold_state == true)
		{
			current_offset = last_offset + (xy_param{ hold - pos }) * expand;
			centre = current_offset;
		}
	}

	void event_hold_start(xy_point pos)
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

	void event_active(xy_point pos)
	{
		xy_point variance = plot_centre - pos;
		variance.modulus();

		if ((plot_size / 2) > variance)
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
		xy_param last_scale_counter = scale_counter;

		size = size / scale;

		if (keys::shift == true)
			scale_counter = scale_counter + xy_param(inc, 0);
		else
			scale_counter = scale_counter + inc;

		xy_param next_scale = size * xy_param(
			pow(DEFAULT_SCALE_MULTIPLIER, scale_counter.get_x()),
			pow(DEFAULT_SCALE_MULTIPLIER, scale_counter.get_y()));

		double limit_scale_x = scale_counter.get_x();
		if ((next_scale.get_x() > Init::engine.upper_limit_size.get_x()) || next_scale.get_x() < Init::engine.lower_limit_size.get_x())
			limit_scale_x = last_scale_counter.get_x();

		double limit_scale_y = scale_counter.get_y();
		if ((next_scale.get_y() > Init::engine.upper_limit_size.get_y()) || next_scale.get_y() < Init::engine.lower_limit_size.get_y())
			limit_scale_y = last_scale_counter.get_y();

		scale_counter.set(limit_scale_x, limit_scale_y);
	}

	void update_limits()
	{
		xy_point acentre = centre;
		xy_param half = size / 2.;

		acentre.y_invert();

		limit_left_down = acentre - half;
		limit_right_up = acentre + half;
	}

public:
	bool is_active()
	{
		return active_state;
	}

	void update(rect_prop& plot)
	{
		plot_size = plot.get_size().get_current();
		plot_centre = plot.get_centre().get_current();
		expand = size / plot_size;

		update_limits();
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

		update_limits();
	}

	graphics_engine() {};
};

// figures for chart
class data
{
public:
	std::vector<double> content;
	double step;
	double offset;

public:
	void paint(HDC& hdc, graphics_engine& engine)
	{
		double x_data_begin = offset;
		double x_data_end = offset + step * (content.size() - 1);

		double x_left_limit = engine.get_limit_left_down().get_x();
		double x_right_limit = engine.get_limit_right_up().get_x();

		// data out plot
		if (x_data_begin > x_right_limit) return;
		if (x_data_end < x_left_limit) return;

		// search left data point
		unsigned int first_index = 0;

		if (x_left_limit > x_data_begin)
		{
			double delta = x_left_limit - x_data_begin;
			first_index = delta / step;
		}

		// search right data point
		unsigned int last_index = last_index = content.size() - 2;

		if (x_right_limit < x_data_end)
		{
			double delta = x_right_limit;
			last_index = delta / step;
		}

		// compressed solve
		unsigned int count_data = engine.get_size().get_x();
		unsigned int count_pixels = engine.get_plot_size().get_x();

		double coefficient_compressed = 
			static_cast<double>(count_data) / 
			static_cast<double>(count_pixels);

		std::cout << x_right_limit << "x\n";

		if (coefficient_compressed < 2.)
			paint_non_compressed_mode(hdc, engine, first_index, last_index);
		else
			paint_compressed_mode(hdc, engine, first_index, last_index, coefficient_compressed);
	}

private:
	void paint_non_compressed_mode(
		HDC& hdc, 
		graphics_engine& engine, 
		unsigned int first_index, 
		unsigned int last_index)
	{
		SelectObject(hdc, reinterpret_cast<HGDIOBJ>(stock_objects::pen.test));

		unsigned int index = first_index;

		double last_x = offset + step * index;
		xy_point last_point = xy_point{ last_x, content[index] };

		index++;
		double curr_x = last_x + step;
		xy_point curr_point = xy_point{ curr_x, content[index] };

		paint::line(hdc,
			engine.get_window(last_point),
			engine.get_window(curr_point));

		while (index <= last_index)
		{
			last_point = curr_point;

			index++;
			curr_x = curr_x + step;
			curr_point = xy_point{ curr_x, content[index] };

			paint::line(hdc,
				engine.get_window(last_point),
				engine.get_window(curr_point));
		}
	}

	void paint_compressed_mode(
		HDC& hdc, 
		graphics_engine& engine, 
		unsigned int first_index, 
		unsigned int last_index, 
		double coefficient_compressed)
	{
		SelectObject(hdc, reinterpret_cast<HGDIOBJ>(stock_objects::pen.test));

		double index = first_index;

		while (index <= last_index)
		{

			double local_start_index = index;
			double local_end_index = local_start_index + coefficient_compressed;

			double local_current_index = local_start_index;
			double local_min_extremum = content[local_start_index];
			double local_max_extremum = content[local_start_index];

			if (local_end_index > last_index)
				local_end_index = last_index;

			while (local_current_index <= local_end_index)
			{
				if (local_min_extremum > content[local_current_index])
				{
					local_min_extremum = content[local_current_index];
					local_current_index++;
					continue;
				}

				if (local_max_extremum < content[local_current_index])
					local_max_extremum = content[local_current_index];

				local_current_index++;
			}

			xy_point min_extremum = xy_point{ offset + step * local_start_index, local_min_extremum };
			xy_point max_extremum = xy_point{ offset + step * local_end_index, local_max_extremum };


			paint::line(hdc,
				engine.get_window(min_extremum),
				engine.get_window(max_extremum));

			index += coefficient_compressed;
		}
	
	}

public:
	void Init(std::vector<double>& load_content, double load_step = 1, double load_offset = 0)
	{
		content = load_content;
		step = load_step;
		offset = load_offset;
	}
	
	data() 
	{ 
		content.resize(0);
		offset = 0; 
		step = 0; 
	};
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
	xy_param size;
	xy_param centre;
	xy_point spoint;
	xy_point epoint;

	xy_param coarse_count;
	xy_param discret;

	xy_param saxes_point;
	xy_param eaxes_point;
	xy_param current_count;

public:
	void update(xy_param new_size, xy_param new_centre)
	{
		size = new_size;
		centre = new_centre;

		spoint = centre - (size * types_HALF);
		epoint = centre + (size * types_HALF);

		spoint.y_invert();
		epoint.y_invert();

		xy_param coarse_discret = size / coarse_count;

		int level_x = exclusive::range_level(coarse_discret.get_x());
		int level_y = exclusive::range_level(coarse_discret.get_y());

		double discret_x = exclusive::nearest_in_range(coarse_discret.get_x(), level_x, &reference_range[0], range_size);
		double discret_y = exclusive::nearest_in_range(coarse_discret.get_y(), level_y, &reference_range[0], range_size);

		discret.set(discret_x, discret_y);

		double saxes_point_y = round(spoint.get_y() / discret.get_y()) * discret.get_y();
		if (saxes_point_y > spoint.get_y())
			saxes_point_y -= discret.get_y();

		double eaxes_point_y = saxes_point_y;
		while (eaxes_point_y >= epoint.get_y())
			eaxes_point_y -= discret.get_y();

		double saxes_point_x = round(spoint.get_x() / discret.get_x()) * discret.get_x();
		if (saxes_point_x < spoint.get_x())
			saxes_point_x += discret.get_x();

		double eaxes_point_x = saxes_point_x;
		while (eaxes_point_x <= epoint.get_x())
			eaxes_point_x += discret.get_x();

		saxes_point.set(saxes_point_x, saxes_point_y);
		eaxes_point.set(eaxes_point_x, eaxes_point_y);

		current_count = (eaxes_point - saxes_point) / discret;
		current_count.modulus();
		current_count.integer(); // hard move wow
	}

	void paint_axes(HDC& hdc, graphics_engine& engine)
	{
		xy_point plot_spoint = engine.get_window(spoint);
		xy_point plot_epoint = engine.get_window(epoint);
		xy_point plot_centre = engine.get_plot_centre();
		xy_param plot_size   = engine.get_plot_size();

		double horisontal_axes = saxes_point.get_y();

		for(auto count_y = 0; count_y < current_count.get_y(); count_y++)
		{
			double y_axes_window = engine.get_window(xy_point(0, horisontal_axes)).get_y();
			double delta = sqrt(1. -  abs(plot_centre.get_y() - y_axes_window) / (plot_size.get_y() / 2.));

			COLORREF color = 
				exclusive::change_ref_color(
					stock_objects::color.axis,
					stock_objects::color.chart,
					delta);

			HPEN pen = CreatePen(PS_SOLID, 1, color);
			SelectObject(hdc, reinterpret_cast<HGDIOBJ>(pen));

			paint::line(hdc,
				(xy_point(plot_spoint.get_x(), y_axes_window)),
				(xy_point(plot_epoint.get_x(), y_axes_window)));

			DeleteObject(pen);

			horisontal_axes -= discret.get_y();
		}

		SelectObject(hdc, reinterpret_cast<HGDIOBJ>(stock_objects::pen.axis));

		double vertical_axes = saxes_point.get_x();

		for (int count_x = 0; count_x < static_cast<int>(current_count.get_x()); count_x++)
		{
			double x_axes_window = engine.get_window(xy_point(vertical_axes, 0)).get_x();
			double delta = sqrt(1. - abs(plot_centre.get_x() - x_axes_window) / (plot_size.get_x() / 2.));

			COLORREF color =
				exclusive::change_ref_color(
					stock_objects::color.axis,
					stock_objects::color.chart,
					delta);

			HPEN pen = CreatePen(PS_SOLID, 1, color);
			SelectObject(hdc, reinterpret_cast<HGDIOBJ>(pen));

			paint::line(hdc,
				(xy_point(x_axes_window, plot_spoint.get_y())),
				(xy_point(x_axes_window, plot_epoint.get_y())));

			DeleteObject(pen);

			vertical_axes += discret.get_x();
		}
	}

	void paint_lables(HDC& hdc, graphics_engine& engine, rect_prop* bounds, COLORREF color = stock_objects::color.labels)
	{
		SelectObject(hdc, reinterpret_cast<HGDIOBJ>(stock_objects::pen.labels));
		SetTextColor(hdc, color);
		SetTextAlign(hdc, TA_CENTER);

		double left_bound_size = bounds[1].get_size().get_current().get_x();
		double left_bound_centre = bounds[1].get_centre().get_current().get_x();
		int nWidth = static_cast<int>(left_bound_size/9.);

		if (nWidth > 5) nWidth = 5;

		double down_bound_size = bounds[2].get_size().get_current().get_y();
		double down_bound_centre = bounds[2].get_centre().get_current().get_y();
		int nHeight = static_cast<int>(down_bound_size / 2.);

		if (nHeight > 14) nHeight = 14;

		SetGraphicsMode(hdc, GM_COMPATIBLE);
		HFONT hFont = CreateFont(nHeight, nWidth, 300, 300, 900, 0, 0, 0, 0, 0, 0, 2, 0, L"SYSTEM_FIXED_FONT");
		HFONT hTmp = (HFONT)SelectObject(hdc, hFont);
		SetBkMode(hdc, TRANSPARENT);

		xy_point plot_centre = engine.get_plot_centre();
		xy_param plot_size = engine.get_plot_size();

		double vertical_lables = saxes_point.get_y();

		// vertical
		// x - const
		// y - change
		for (auto count_y = 0; count_y < current_count.get_y(); count_y++)
		{
			double x = left_bound_centre;
			double y = engine.get_window(xy_point(x, vertical_lables)).get_y() - 6;
			
			double delta = sqrt(1. - abs(plot_centre.get_y() - (y + 6)) / (plot_size.get_y() / 2.));

			COLORREF color_pen =
				exclusive::change_ref_color(
					stock_objects::color.labels,
					stock_objects::color.chart,
					delta);

			COLORREF color_labels =
				exclusive::change_ref_color(
					stock_objects::color.labels,
					stock_objects::color.bound,
					delta);

			HPEN pen = CreatePen(PS_SOLID, 1, color_pen);
			SelectObject(hdc, reinterpret_cast<HGDIOBJ>(pen));

			std::wstring gl_wstr = exclusive::double2wstring(vertical_lables);

			xy_point supline_point1 = engine.get_window(xy_point(spoint.get_x(), vertical_lables));
			xy_point supline_point2 = supline_point1 + xy_point(0.007*plot_size.get_x(), 0);
			paint::line(hdc, supline_point1, supline_point2);

			DeleteObject(pen);

			SetTextColor(hdc, color_labels);
			TextOut(hdc, static_cast<int>(x), static_cast<int>(y), gl_wstr.c_str(), (int)gl_wstr.size());

			vertical_lables -= discret.get_y();
		}

		double horisontal_lables = saxes_point.get_x();

		// vertical
		// x - change
		// y - const
		for (auto count_x = 0; count_x < current_count.get_x(); count_x++)
		{
			double y = down_bound_centre - 5;
			double x = engine.get_window(xy_point(horisontal_lables, y)).get_x() - 4;

			double delta = pow((1. - abs(plot_centre.get_x() - (x + 4)) / (plot_size.get_x() / 2.)), 0.33);

			COLORREF color_pen =
				exclusive::change_ref_color(
					stock_objects::color.labels,
					stock_objects::color.chart,
					delta);

			COLORREF color_labels =
				exclusive::change_ref_color(
					stock_objects::color.labels,
					stock_objects::color.bound,
					delta);

			HPEN pen = CreatePen(PS_SOLID, 1, color_pen);
			SelectObject(hdc, reinterpret_cast<HGDIOBJ>(pen));

			std::wstring gl_wstr = exclusive::double2wstring(horisontal_lables);

			xy_point supline_point1 = engine.get_window(xy_point(horisontal_lables, epoint.get_y()));
			xy_point supline_point2 = supline_point1 + xy_point(0, -0.01 * plot_size.get_y());
			paint::line(hdc, supline_point1, supline_point2);

			DeleteObject(pen);

			SetTextColor(hdc, color_labels);
			TextOut(hdc, static_cast<int>(x), static_cast<int>(y), gl_wstr.c_str(), (int)gl_wstr.size());

			horisontal_lables += discret.get_x();
		}

		SetTextAlign(hdc, VTA_BASELINE);

		SelectObject(hdc, hFont);
		DeleteObject(hFont);
	}

public:
	void Init()
	{
		size.set(Init::axes.size);
		centre.set(Init::axes.centre);
		coarse_count.set(Init::axes.coarse_count);

		spoint = centre - (size * types_HALF);
		epoint = centre + (size * types_HALF);

		spoint.y_invert();
		epoint.y_invert();

		xy_param coarse_discret = size / coarse_count;
		
		int level_x = exclusive::range_level(coarse_discret.get_x());
		int level_y = exclusive::range_level(coarse_discret.get_y());

		double discret_x = exclusive::nearest_in_range(coarse_discret.get_x(), level_x, &reference_range[0], range_size);
		double discret_y = exclusive::nearest_in_range(coarse_discret.get_x(), level_x, &reference_range[0], range_size);

		discret.set(discret_x, discret_y);

		double saxes_point_x = round(spoint.get_y() / discret.get_y()) * discret.get_y();
		if (saxes_point_x > spoint.get_y())
			saxes_point_x -= discret.get_y();

		double eaxes_point_x = saxes_point_x;
		while (eaxes_point_x >= epoint.get_y())
			eaxes_point_x -= discret.get_y();

		double saxes_point_y = round(spoint.get_x() / discret.get_x()) * discret.get_x();
		if (saxes_point_y < spoint.get_x())
			saxes_point_y += discret.get_x();

		double eaxes_point_y = saxes_point_y;
		while (eaxes_point_y <= epoint.get_x())
			eaxes_point_y += discret.get_x();

		saxes_point.set(saxes_point_x, saxes_point_y);
		eaxes_point.set(eaxes_point_x, eaxes_point_y);

		current_count = (eaxes_point - saxes_point) / discret;
		current_count.modulus();
		current_count.integer();
	}

	axes() {};
};

 // chart
 // paint bounds and graph area
class chart
{
private:
	// paint objects
	rect_prop _plot;
	rect_prop _bounds[4];
	data      _data;
	axes      _axes;

private:
	graphics_engine _engine;

public:
	rect_prop& plot()
	{
		return _plot;
	}

	graphics_engine& engine()
	{
		return _engine;
	}

	data& data()
	{
		return _data;
	}

public:

	void update(const xy_param& expand)
	{
		_plot.update(expand);

		_engine.update(_plot);

		_bounds[0].update(expand);
		_bounds[1].update(expand);
		_bounds[2].update(expand);
		_bounds[3].update(expand);

		_axes.update(
			_engine.get_size(),
			_engine.get_centre());
	}

	void paint(HDC& hdc)
	{
		_plot.paint(hdc, stock_objects::pen.chart, stock_objects::brush.chart);

		_axes.paint_axes(hdc, _engine);

		// test obj
		_data.paint(hdc, _engine);

		_bounds[0].paint(hdc, stock_objects::pen.bound, stock_objects::brush.bound);
		_bounds[1].paint(hdc, stock_objects::pen.bound, stock_objects::brush.bound);
		_bounds[2].paint(hdc, stock_objects::pen.bound, stock_objects::brush.bound);
		_bounds[3].paint(hdc, stock_objects::pen.bound, stock_objects::brush.bound);

		_axes.paint_lables(hdc, _engine, _bounds);

		if (_engine.is_active())
			SelectObject(hdc, reinterpret_cast<HGDIOBJ>(stock_objects::pen.chart_enable));
		else
			SelectObject(hdc, reinterpret_cast<HGDIOBJ>(stock_objects::pen.chart_disable));

		xy_point spoint = _plot.get_start_point();
		xy_point epoint = _plot.get_end_point();

		paint::line(hdc, spoint.get_x(), spoint.get_y(), epoint.get_x(), spoint.get_y());
		paint::line(hdc, epoint.get_x(), spoint.get_y(), epoint.get_x(), epoint.get_y());
		paint::line(hdc, epoint.get_x(), epoint.get_y(), spoint.get_x(), epoint.get_y());
		paint::line(hdc, spoint.get_x(), epoint.get_y(), spoint.get_x(), spoint.get_y());
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
			xy_param(x0size, y0size),
			xy_param(x0centre, y0centre));

		double x1size = Init::plot.centre.get_x() - Init::plot.size.get_x() * types_HALF;
		double y1size = Init::form.size.get_y();
		double x1centre = x1size * types_HALF;
		double y1centre = y1size * types_HALF;

		_bounds[1].init_reference(
			xy_param(x1size, y1size),
			xy_param(x1centre, y1centre));

		double d_y = Init::plot.centre.get_y() + Init::plot.size.get_y() * types_HALF;
		double x2size = Init::form.size.get_x();
		double y2size = Init::form.size.get_y() - d_y;
		double x2centre = x2size * types_HALF;
		double y2centre = (Init::form.size.get_y() + d_y) * types_HALF;

		_bounds[2].init_reference(
			xy_param(x2size, y2size),
			xy_param(x2centre, y2centre));

		double d_x = Init::plot.centre.get_x() + Init::plot.size.get_x() * types_HALF;
		double x3size = Init::form.size.get_x() - d_x;
		double y3size = Init::form.size.get_y();
		double x3centre = (Init::form.size.get_x() + d_x) * types_HALF;
		double y3centre = y3size * types_HALF;

		_bounds[3].init_reference(
			xy_param(x3size, y3size),
			xy_param(x3centre, y3centre));

		_engine.Init();
		_axes.Init();
		
		//_data.Init(stock_objects::tst_data);
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

	void AddLable(HDC& hdc, simplest_unit position, std::wstring text, double value, COLORREF color = stock_objects::color.white)
	{
		SetTextColor(hdc, color);
		std::wstring gl_wstr = (text)+L"(" + std::to_wstring(value) + L")";
		xy_point current = position * _form.size().get_expand();
		TextOut(hdc, static_cast<int>(current.get_x()), static_cast<int>(current.get_y()), gl_wstr.c_str(), (int)gl_wstr.size());
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

#define Data_Init(content, step, offset)   wds.chart().data().Init(content, step, offset)