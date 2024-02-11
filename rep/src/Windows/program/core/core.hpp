#pragma once
#include"string"

std::wstring gl_wstr;

// global data
#include"global/gl_color.hpp"
#include"global/gl_data.hpp"
#include"global/gl_mouse.hpp"
#include"global/gl_paint.hpp"
#include"global/gl_stock.hpp"
#include"global/gl_timer.hpp"
#include"global/gl_wheel.hpp"
#include"global/gl_widgets.hpp"
#include"global/gl_windows.hpp"

// methods
#include"methods/me_setLine.hpp"
#include"methods/me_setText.hpp"

struct _coord
{
	double x = 0;
	double y = 0;
};

// rules:
//	1. One pixel expand N point coordinates, var: expand_N

double acoord_x(double);
double acoord_y(double);

struct graphic_core
{
	double scale = 1.;

	void update_scale()
	{
		display_limit.x /=  scale;
		display_limit.y /=  scale;

		scale = 1. * pow(1.1, gl_wheel::counter);

		display_limit.x *= scale;
		display_limit.y *= scale;

		update_expand_scale();
	}

	_coord reference_point;

	void set_reference_point(double x_reference, double y_reference)
	{
		reference_point.x = x_reference;
		reference_point.y = y_reference;
	}

	void update_reference_point()
	{
		reference_point.x = last_reference_point.x + shift.x;
		reference_point.y = last_reference_point.y + shift.y;
	}

	_coord last_reference_point;

	void update_last_reference_point()
	{
		last_reference_point.x = reference_point.x;
		last_reference_point.y = reference_point.y;
	}

	_coord hold_mouse;

	void update_hold()
	{
		hold_mouse.x = gl_mouse::position_x;
		hold_mouse.y = gl_mouse::position_y;
		update_last_reference_point();
	}

	_coord shift;

	void update_shift()
	{
		shift.x = (hold_mouse.x - gl_mouse::position_x)/ expand_scale.x;
		shift.y = (gl_mouse::position_y - hold_mouse.y)/ expand_scale.y;
	}

	_coord display_limit;

	void set_display_limit(double x_limit, double y_limit)
	{
		display_limit.x = x_limit * scale;
		display_limit.y = y_limit * scale;
	}

	_coord expand_scale;

	void update_expand_scale()
	{
		expand_scale.x = gl_windows::width / display_limit.x;
		expand_scale.y = gl_windows::height / display_limit.y;
	}
} gc;

//	mouse -> coord
//	Transform mouse coordinates in abstract coordinates
//	Reference_point - centre windows
double acoord_x(double pixel_x)
{
	return gc.reference_point.x - gc.display_limit.x / 2 + pixel_x / gc.expand_scale.x;
}

double acoord_y(double pixel_y)
{
	return gc.reference_point.y + gc.display_limit.y / 2 - pixel_y / gc.expand_scale.y;
}

//	coord -> mouse
//	Transform abstract coordinates in mouse coordinates 
//	Reference_point - centre windows
double mcoord_x(double coord_x)
{
	return (coord_x - gc.reference_point.x + gc.display_limit.x / 2) * gc.expand_scale.x;
}

double mcoord_y(double coord_y)
{
	return (-coord_y + gc.reference_point.y + gc.display_limit.y / 2) * gc.expand_scale.y;
}