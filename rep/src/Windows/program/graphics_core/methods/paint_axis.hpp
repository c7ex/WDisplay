#pragma once
#define START_MAIN_AXIS 0
#define MIN_SIPPORTIVE_AXIS 10.
#define MAX_SIPPORTIVE_AXIS 20.

// calculation optimal discretization
double get_discretization_supportive_axis(double abstract_limit)
{
	double coarse_discretization = abstract_limit / MIN_SIPPORTIVE_AXIS;
	double order = round(log10(coarse_discretization));
	double discretization = pow(10, order);
	double number_of_axes = abstract_limit / discretization;

	while ((number_of_axes > MAX_SIPPORTIVE_AXIS) || (number_of_axes < MIN_SIPPORTIVE_AXIS))
	{
		if (number_of_axes >= MAX_SIPPORTIVE_AXIS)
			discretization *= MIN_SIPPORTIVE_AXIS;
		else if (number_of_axes <= MIN_SIPPORTIVE_AXIS)
			discretization /= (MAX_SIPPORTIVE_AXIS/ MIN_SIPPORTIVE_AXIS);
		number_of_axes = (abstract_limit / discretization);
	}

	return discretization;
}

void paint_once_supportive_axis_label_x(HDC& hMemDc, double a_pos_x, double a_pos_y, double var)
{
	double m_pos_x = get_mouse_coordinate_x(a_pos_x);
	double m_pos_y = get_mouse_coordinate_y(a_pos_y);

	if (m_pos_y < 5)
		m_pos_y = 5;
	else if (m_pos_y > gc.window.count_pixels.y - 15)
		m_pos_y = gc.window.count_pixels.y - 15;

	paint_text(hMemDc, m_pos_x, m_pos_y, var);
}

void paint_once_supportive_axis_label_y(HDC& hMemDc, double a_pos_x, double a_pos_y, double var)
{
	if (var == 0) return;

	double m_pos_x = get_mouse_coordinate_x(a_pos_x);
	double m_pos_y = get_mouse_coordinate_y(a_pos_y);

	if (m_pos_x < 5)
		m_pos_x = 5;
	else if (m_pos_x > gc.window.count_pixels.x - 15)
		m_pos_x = gc.window.count_pixels.x - 15;

	paint_text(hMemDc, m_pos_x, m_pos_y, var);
}

void hMainAxis(HDC& hMemDc, double y)
{
	double my = get_mouse_coordinate_y(y);
	if (my > 0)
		paint_line(hMemDc, 0, my, gc.window.count_pixels.x, my);
}

void vMainAxis(HDC& hMemDc, double x)
{
	double mx = get_mouse_coordinate_x(x);
	if (mx >= 0)
		paint_line(hMemDc, mx, 0, mx, gc.window.count_pixels.y);
}

void horizontal_supportive_axis(HDC& hMemDc, double y)
{
	double my = get_mouse_coordinate_y(y);
	if (my > 0)
		paint_line(hMemDc, 0, my, gc.window.count_pixels.x, my);
}

void vertical_supportive_axis(HDC& hMemDc, double x)
{
	double mx = get_mouse_coordinate_x(x);
	if (mx >= 0)
		paint_line(hMemDc, mx, 0, mx, gc.window.count_pixels.y);
}

void paint_main_axis(HDC& hMemDc)
{
	SelectObject(hMemDc, reinterpret_cast<HGDIOBJ>(stock_objects::pen.main_axis));
	hMainAxis(hMemDc, START_MAIN_AXIS);
	vMainAxis(hMemDc, START_MAIN_AXIS);
}

void paint_supportive_axis(HDC& hMemDc)
{
	double d_x = get_discretization_supportive_axis(gc.window.coordinates_limit.x);
	double d_y = get_discretization_supportive_axis(gc.window.coordinates_limit.y);

	double r_x = round(gc.window.coordinates_begin.x / d_x);
	double r_y = round(gc.window.coordinates_end.y / d_y);

	if ((r_x * d_x) < gc.window.coordinates_begin.x) r_x++;

	while (r_x * d_x <= gc.window.coordinates_end.x)
	{
		vertical_supportive_axis(hMemDc, r_x * d_x);
		r_x++;
	}

	if ((r_y * d_y) < gc.window.coordinates_end.y) r_y++;

	while (r_y * d_y <= gc.window.coordinates_begin.y)
	{
		horizontal_supportive_axis(hMemDc, r_y * d_y);
		r_y++;
	}
}

void paint_supportive_axis_labels(HDC& hMemDc)
{
	SetTextColor(hMemDc, stock_objects::color.white);

	double d_x = get_discretization_supportive_axis(gc.window.coordinates_limit.x);
	double d_y = get_discretization_supportive_axis(gc.window.coordinates_limit.y);

	double r_x = round(gc.window.coordinates_begin.x / d_x);
	double r_y = round(gc.window.coordinates_end.y / d_y);

	while (r_x * d_x <= gc.window.coordinates_end.x)
	{
		paint_once_supportive_axis_label_x(hMemDc, r_x * d_x, 0, r_x * d_x);
		r_x++;
	}

	while (r_y * d_y <= gc.window.coordinates_begin.y)
	{
		paint_once_supportive_axis_label_y(hMemDc, 0, r_y * d_y, r_y * d_y);
		r_y++;
	}
}