#pragma once

#define GRAPHIC_CORE_START_X 0
#define GRAPHIC_CORE_START_Y 0
#define GRAPHIC_CORE_DEFAULT_TOTAL_SCALE 1
#define GRAPHIC_CORE_DEFAULT_WIDTH_SCALE 1

struct xy_scales
{
	double total = GRAPHIC_CORE_DEFAULT_TOTAL_SCALE;
	double width = GRAPHIC_CORE_DEFAULT_WIDTH_SCALE;
};

struct xy_coordinates
{
	double x = 0;
	double y = 0;
};

struct xy_region
{
	xy_coordinates coord_begin;
	xy_coordinates coord_end;
};

double get_abstract_coordinate_x(double);
double get_abstract_coordinate_y(double);

class graphic_core
{
public:
	xy_scales scale;
	xy_coordinates reference_point;
	xy_coordinates last_reference_point;
	xy_coordinates hold_mouse;
	xy_coordinates shift;
	xy_coordinates display_limit;
	xy_coordinates expand_scale;
	xy_region workspace;

public:
	void update_scale();
	void update_scale_w();
	void update_reference_point();
	void update_last_reference_point();
	void update_hold();
	void update_shift();
	void update_expand_scale();
	void update_workspace();

public:
	double get_ref_x();
	double get_ref_y();

public:
	void set_reference_point(double, double);
	void set_display_limit(double, double);

public:
	graphic_core() {}
};

void graphic_core::update_scale()
{
	display_limit.x /= scale.total;
	display_limit.y /= scale.total;

	scale.total = 1. * pow(1.1, gl_wheel::counter);

	display_limit.x *= scale.total;
	display_limit.y *= scale.total;

	update_expand_scale();
}

void graphic_core::update_scale_w()
{
	display_limit.x /= scale.width;

	scale.width = 1. * pow(1.1, gl_wheel::counter_w);

	display_limit.x *= scale.width;

	update_expand_scale();
}

void graphic_core::update_reference_point()
{
	reference_point.x = last_reference_point.x + shift.x;
	reference_point.y = last_reference_point.y + shift.y;
}

void graphic_core::update_last_reference_point()
{
	last_reference_point.x = reference_point.x;
	last_reference_point.y = reference_point.y;
}

void graphic_core::update_hold()
{
	hold_mouse.x = gl_mouse::position_x;
	hold_mouse.y = gl_mouse::position_y;
	update_last_reference_point();
}

void graphic_core::update_shift()
{
	shift.x = (hold_mouse.x - gl_mouse::position_x) / expand_scale.x;
	shift.y = (gl_mouse::position_y - hold_mouse.y) / expand_scale.y;
}

void graphic_core::update_expand_scale()
{
	expand_scale.x = gl_windows::width / display_limit.x;
	expand_scale.y = gl_windows::height / display_limit.y;
}

void graphic_core::update_workspace()
{
	workspace.coord_begin.x = get_abstract_coordinate_x(GRAPHIC_CORE_START_X);
	workspace.coord_begin.y = get_abstract_coordinate_y(GRAPHIC_CORE_START_Y);

	workspace.coord_end.x = get_abstract_coordinate_x(gl_windows::width);
	workspace.coord_end.y = get_abstract_coordinate_y(gl_windows::height);
}

inline double graphic_core::get_ref_x()
{
	return reference_point.x;
}

inline double graphic_core::get_ref_y()
{
	return reference_point.y;
}

void graphic_core::set_reference_point(double x_reference, double y_reference)
{
	reference_point.x = x_reference;
	reference_point.y = y_reference;
}

void graphic_core::set_display_limit(double x_limit, double y_limit)
{
	display_limit.x = x_limit * scale.total;
	display_limit.y = y_limit * scale.total;
}

graphic_core gc;

//	mouse -> coord
//	Transform mouse coordinates in abstract coordinates
//	Reference_point - centre windows
double get_abstract_coordinate_x(double pixel_x)
{
	return gc.reference_point.x - gc.display_limit.x / 2 + pixel_x / gc.expand_scale.x;
}

double get_abstract_coordinate_y(double pixel_y)
{
	return gc.reference_point.y + gc.display_limit.y / 2 - pixel_y / gc.expand_scale.y;
}

//	coord -> mouse
//	Transform abstract coordinates in mouse coordinates 
//	Reference_point - centre windows
double get_mouse_coordinate_x(double coordinate_x)
{
	return (coordinate_x - gc.reference_point.x + gc.display_limit.x / 2) * gc.expand_scale.x;
}

double get_mouse_coordinate_y(double coordinate_y)
{
	return (-coordinate_y + gc.reference_point.y + gc.display_limit.y / 2) * gc.expand_scale.y;
}