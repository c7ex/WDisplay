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



#define DEFAULT_PLOT_SIZE_X        270.
#define DEFAULT_PLOT_SIZE_Y        360.
#define DEFAULT_PLOT_CENTRE_X      160.
#define DEFAULT_PLOT_CENTRE_Y      200.

#define DEFAULT_ABSTRUCT_SIZE_X    100.
#define DEFAULT_ABSTRUCT_SIZE_Y    100.
#define DEFAULT_ABSTRUCT_CENTRE_X  0.
#define DEFAULT_ABSTRUCT_CENTRE_Y  0.

#define DEFAULT_SCALE_MULTIPLIER   1.05

 // plot
 // paint bounds and graph area
class chart
{
private:
	rectpropf _plot;
	rectpropf _bounds[4];
	
public:
	paramf abstruct_size;
	paramf abstruct_centre;
	paramf abstruct_expand;
	
	pointf state_hold;
	paramf state_current_offset;
	paramf state_last_offset;

	paramf state_scale;
	paramf state_scale_last;
	paramf state_scale_counter;

public:
	paramf get_chart_centre()
	{
		return _plot.get_centre().get_current();
	}

	paramf get_chart_size()
	{
		return _plot.get_size().get_current();
	}

public:
	// abstruct = (window - middleplot) * expand + centre
	pointf get_abstruct(pointf pos)
	{
		return pointf{
			(pos.get_x() - _plot.get_centre().get_current().get_x()) * abstruct_expand.get_x() + abstruct_centre.get_x(),
			(_plot.get_centre().get_current().get_y() - pos.get_y()) * abstruct_expand.get_y() - abstruct_centre.get_y()};
	}

	// window = (abstruct - centre) / expand + middleplot
	pointf get_window(pointf pos)
	{
		return pointf{
			((pos.get_x() - abstruct_centre.get_x()) / abstruct_expand.to_pointf().get_x()) + _plot.get_centre().get_current().get_x(),
			_plot.get_centre().get_current().get_y() - ((pos.get_y() + abstruct_centre.get_y()) / abstruct_expand.to_pointf().get_y())};
	}

public:

	void update(const paramf& expand)
	{
		_plot.update(expand);

		abstruct_expand = abstruct_size / _plot.get_size().get_current();

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
		paint_line(hdc, get_window(pointf(-30, -30)), get_window(pointf(30, 30)));

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

		abstruct_size.set(DEFAULT_ABSTRUCT_SIZE_X, DEFAULT_ABSTRUCT_SIZE_Y);
		abstruct_centre.set(DEFAULT_ABSTRUCT_CENTRE_X, DEFAULT_ABSTRUCT_CENTRE_Y);
		abstruct_expand.set(
			DEFAULT_ABSTRUCT_SIZE_X / DEFAULT_PLOT_SIZE_X,
			DEFAULT_ABSTRUCT_SIZE_Y / DEFAULT_PLOT_SIZE_Y);

		state_last_offset.set(abstruct_centre);

		state_scale_last.set(1, 1);
		state_scale.set(1., 1.);
		state_scale_counter.set(0, 0);
	}
};



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

	chart& plot()
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
#define hd_form_mouse   wds.form().mouse()
#define hf_form_size    wds.form().size().get_current()
#define hf_form_expand  wds.form().size().get_expand()