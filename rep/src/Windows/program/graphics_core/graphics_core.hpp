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

#define DEFAULT_FORM_SIZE_X      400.
#define DEFAULT_FORM_SIZE_Y      400.

#define DEFAULT_PLOT_SIZE_X        270.
#define DEFAULT_PLOT_SIZE_Y        360.
#define DEFAULT_PLOT_CENTRE_X      160.
#define DEFAULT_PLOT_CENTRE_Y      200.

#define DEFAULT_ABSTRUCT_SIZE_X    100.
#define DEFAULT_ABSTRUCT_SIZE_Y    100.
#define DEFAULT_ABSTRUCT_CENTRE_X  0.
#define DEFAULT_ABSTRUCT_CENTRE_Y  0.

#define DEFAULT_SCALE_MULTIPLIER   1.05

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
		_size.set(
			DEFAULT_FORM_SIZE_X, 
			DEFAULT_FORM_SIZE_Y);
	}
};

// states for chart
// 1. Change offset - mouse move
// 2. Change scale - mousewheel scroll
class graphics_engine
{
public:
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
	bool   hold_state;

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

public:
	// update states if [WM_MOUSEWHEEL]
	void event_rescale(pointf pos, double inc)
	{
		if (hold_state == true) return;

		paramf last_abstr_expand = expand;
		paramf last_abstr_centre = centre;
		paramf last_chart_centre = plot_centre;

		size = size / scale;

		scale_counter = scale_counter + inc;
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
		if (hold_state == true)
		{
			current_offset = last_offset + (paramf{ hold - pos }) * expand;
			centre = current_offset;
		}
	}

	void event_hold_start(pointf pos)
	{ 
		hold = pos;
		hold_state = true;
	}

	void event_hold_stop()
	{
		last_offset = centre;
		hold_state = false;
	}

public:
	void update(rectpropf& plot)
	{
		plot_size = plot.get_size().get_current();
		plot_centre = plot.get_centre().get_current();
		expand = size / plot_size;
	}

public:
	void Init()
	{
		hold_state = false;

		size.set(DEFAULT_ABSTRUCT_SIZE_X, DEFAULT_ABSTRUCT_SIZE_Y);
		centre.set(DEFAULT_ABSTRUCT_CENTRE_X, DEFAULT_ABSTRUCT_CENTRE_Y);
		expand.set(
			DEFAULT_ABSTRUCT_SIZE_X / DEFAULT_PLOT_SIZE_X,
			DEFAULT_ABSTRUCT_SIZE_Y / DEFAULT_PLOT_SIZE_Y);

		last_offset.set(centre);

		scale.set(1., 1.);
		scale_counter.set(0, 0);
	}

	graphics_engine() {};
};

 // plot
 // paint bounds and graph area
class chart
{
private:
	// paint objects
	rectpropf _plot;
	rectpropf _bounds[4];
	
private:
	graphics_engine _engine;

public:
	paramf get_chart_centre()
	{
		return _plot.get_centre().get_current();
	}

	paramf get_chart_size()
	{
		return _plot.get_size().get_current();
	}

	graphics_engine& get_engine()
	{
		return _engine;
	}

public:

	void update(const paramf& expand)
	{
		_plot.update(expand);

		_engine.update(_plot);

		_bounds[0].update(expand);
		_bounds[1].update(expand);
		_bounds[2].update(expand);
		_bounds[3].update(expand);
	}

	void paint(HDC& hdc)
	{
		_plot.paint(hdc, stock_objects::pen.chart, stock_objects::brush.chart);

		// test obj
		SelectObject(hdc, reinterpret_cast<HGDIOBJ>(stock_objects::pen.white));
		paint_line(hdc, 
			_engine.get_window(pointf(-30, -30)),
			_engine.get_window(pointf( 30,  30)));

		_bounds[0].paint(hdc, stock_objects::pen.bound, stock_objects::brush.bound);
		_bounds[1].paint(hdc, stock_objects::pen.bound, stock_objects::brush.bound);
		_bounds[2].paint(hdc, stock_objects::pen.bound, stock_objects::brush.bound);
		_bounds[3].paint(hdc, stock_objects::pen.bound, stock_objects::brush.bound);
	}

public:
	chart()
	{
		_plot.init_reference(
			paramf(DEFAULT_PLOT_SIZE_X, DEFAULT_PLOT_SIZE_Y),
			paramf(DEFAULT_PLOT_CENTRE_X, DEFAULT_PLOT_CENTRE_Y));

		double x0size   = DEFAULT_FORM_SIZE_X;
		double y0size   = DEFAULT_PLOT_CENTRE_Y - DEFAULT_PLOT_SIZE_Y * tyhalf;
		double x0centre = DEFAULT_FORM_SIZE_X * tyhalf;
		double y0centre = (DEFAULT_PLOT_CENTRE_Y - DEFAULT_PLOT_SIZE_Y * tyhalf) * tyhalf;

		_bounds[0].init_reference(
			paramf(x0size, y0size),
			paramf(x0centre, y0centre));

		double x1size = DEFAULT_PLOT_CENTRE_X - DEFAULT_PLOT_SIZE_X * tyhalf;
		double y1size = DEFAULT_FORM_SIZE_Y;
		double x1centre = (DEFAULT_PLOT_CENTRE_X - DEFAULT_PLOT_SIZE_X * tyhalf) * tyhalf;
		double y1centre = DEFAULT_FORM_SIZE_Y * tyhalf;

		_bounds[1].init_reference(
			paramf(x1size, y1size),
			paramf(x1centre, y1centre));

		double x2size = DEFAULT_FORM_SIZE_X;
		double y2size = DEFAULT_FORM_SIZE_Y - (DEFAULT_PLOT_CENTRE_Y + DEFAULT_PLOT_SIZE_Y * tyhalf);
		double x2centre = DEFAULT_FORM_SIZE_X * tyhalf;
		double y2centre = (DEFAULT_FORM_SIZE_Y + (DEFAULT_PLOT_CENTRE_Y + DEFAULT_PLOT_SIZE_Y * tyhalf)) * tyhalf;

		_bounds[2].init_reference(
			paramf(x2size, y2size),
			paramf(x2centre, y2centre));

		double x3size = DEFAULT_FORM_SIZE_X - (DEFAULT_PLOT_CENTRE_X + DEFAULT_PLOT_SIZE_X * tyhalf);
		double y3size = DEFAULT_FORM_SIZE_Y;
		double x3centre = (DEFAULT_FORM_SIZE_X + (DEFAULT_PLOT_CENTRE_X + DEFAULT_PLOT_SIZE_X * tyhalf)) * tyhalf;
		double y3centre = DEFAULT_FORM_SIZE_Y * tyhalf;

		_bounds[3].init_reference(
			paramf(x3size, y3size),
			paramf(x3centre, y3centre));

		_engine.Init();
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
	wdisplay()
	{

	}
};

wdisplay wds;

// hot define
#define hd_form_mouse           wds.form().mouse()
#define hd_form_size            wds.form().size().get_current()
#define hd_form_expand          wds.form().size().get_expand()
#define hd_engine               wds.chart().get_engine()

#define Engine_MouseWheel       wds.chart().get_engine().event_rescale()
#define Engine_MouseMove        wds.chart().get_engine().event_mousemove()
#define Engine_HoldStart(pos)   wds.chart().get_engine().event_hold_start(pos)
#define Engine_HoldStop         wds.chart().get_engine().event_hold_stop()