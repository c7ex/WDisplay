#pragma once

#define GRAPHICS_CORE_INIT_ZERO 0
#define GRAPHICS_CORE_WINDOW_START_X 0
#define GRAPHICS_CORE_WINDOW_START_Y 0
#define GRAPHICS_CORE_DEFAULT_TOTAL_SCALE 1
#define GRAPHICS_CORE_DEFAULT_WIDTH_SCALE 1
#define GRAPHICS_CORE_DEFAULT_DISPLAY_LIMIT_X 500
#define GRAPHICS_CORE_DEFAULT_DISPLAY_LIMIT_Y 250
#define GRAPHICS_CORE_WINDOW_CORRECTION_WIDTH 16
#define GRAPHICS_CORE_WINDOW_CORRECTION_HEIGHT 39

struct _xy
{
	double x, y = GRAPHICS_CORE_INIT_ZERO;
};

struct scales_settings
{
	double total = GRAPHICS_CORE_DEFAULT_TOTAL_SCALE;
	double total_counter = GRAPHICS_CORE_INIT_ZERO;

	double width = GRAPHICS_CORE_DEFAULT_WIDTH_SCALE;
	double width_counter = GRAPHICS_CORE_INIT_ZERO;

	_xy stretching;
};

struct window_settings
{
	_xy coordinates_begin;
	_xy coordinates_end;
	_xy coordinates_limit;
};

struct mouse_settings
{
	_xy current_position;
	_xy hold_position;
	bool is_hold = false;
};

struct picture_settings
{
	_xy current_reference_point;
	_xy last_reference_point;
	_xy offset_reference_point;
};

double get_abstract_coordinate_x(double);
double get_abstract_coordinate_y(double);

class graphics_core
{
public:
	scales_settings scales;
	window_settings window;
	mouse_settings mouse;
	picture_settings picture;

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
	graphics_core();
};

void graphics_core::update_scale()
{
	window.coordinates_limit.x /= scales.total;
	window.coordinates_limit.y /= scales.total;

	scales.total = 1. * pow(1.1, scales.total_counter);

	window.coordinates_limit.x *= scales.total;
	window.coordinates_limit.y *= scales.total;

	update_expand_scale();
}

void graphics_core::update_scale_w()
{
	window.coordinates_limit.x /= scales.width;

	scales.width = 1. * pow(1.1, scales.width_counter);

	window.coordinates_limit.x *= scales.width;

	update_expand_scale();
}

void graphics_core::update_reference_point()
{
	picture.current_reference_point.x = picture.last_reference_point.x + picture.offset_reference_point.x;
	picture.current_reference_point.y = picture.last_reference_point.y + picture.offset_reference_point.y;
}

void graphics_core::update_last_reference_point()
{
	picture.last_reference_point.x = picture.current_reference_point.x;
	picture.last_reference_point.y = picture.current_reference_point.y;
}

void graphics_core::update_hold()
{
	mouse.hold_position.x = mouse.current_position.x;
	mouse.hold_position.y = mouse.current_position.y;
	update_last_reference_point();
}

void graphics_core::update_shift()
{
	picture.offset_reference_point.x = (mouse.hold_position.x - mouse.current_position.x) / scales.stretching.x;
	picture.offset_reference_point.y = (mouse.current_position.y - mouse.hold_position.y) / scales.stretching.y;
}

void graphics_core::update_expand_scale()
{
	scales.stretching.x = gl_windows::width / window.coordinates_limit.x;
	scales.stretching.y = gl_windows::height / window.coordinates_limit.y;
}

void graphics_core::update_workspace()
{
	window.coordinates_begin.x = get_abstract_coordinate_x(GRAPHICS_CORE_WINDOW_START_X);
	window.coordinates_begin.y = get_abstract_coordinate_y(GRAPHICS_CORE_WINDOW_START_Y);

	window.coordinates_end.x = get_abstract_coordinate_x(gl_windows::width);
	window.coordinates_end.y = get_abstract_coordinate_y(gl_windows::height);
}

inline double graphics_core::get_ref_x()
{
	return picture.current_reference_point.x;
}

inline double graphics_core::get_ref_y()
{
	return picture.current_reference_point.y;
}

void graphics_core::set_reference_point(double x_reference, double y_reference)
{
	picture.current_reference_point.x = x_reference;
	picture.current_reference_point.y = y_reference;
}

void graphics_core::set_display_limit(double x_limit, double y_limit)
{
	window.coordinates_limit.x = x_limit * scales.total;
	window.coordinates_limit.y = y_limit * scales.total;
}

graphics_core::graphics_core()
{
	window.coordinates_limit.x = GRAPHICS_CORE_DEFAULT_DISPLAY_LIMIT_X;
	window.coordinates_limit.y = GRAPHICS_CORE_DEFAULT_DISPLAY_LIMIT_Y;
}

graphics_core gc;

//	mouse -> coord
//	Transform mouse coordinates in abstract coordinates
//	Reference_point - centre windows
double get_abstract_coordinate_x(double pixel_x)
{
	return gc.picture.current_reference_point.x - gc.window.coordinates_limit.x / 2 + pixel_x / gc.scales.stretching.x;
}

double get_abstract_coordinate_y(double pixel_y)
{
	return gc.picture.current_reference_point.y + gc.window.coordinates_limit.y / 2 - pixel_y / gc.scales.stretching.y;
}

//	coord -> mouse
//	Transform abstract coordinates in mouse coordinates 
//	Reference_point - centre windows
double get_mouse_coordinate_x(double coordinate_x)
{
	return (coordinate_x - gc.picture.current_reference_point.x + gc.window.coordinates_limit.x / 2) * gc.scales.stretching.x;
}

double get_mouse_coordinate_y(double coordinate_y)
{
	return (-coordinate_y + gc.picture.current_reference_point.y + gc.window.coordinates_limit.y / 2) * gc.scales.stretching.y;
}