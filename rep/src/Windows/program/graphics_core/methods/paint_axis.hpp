#pragma once

// abstract coordination
typedef double _Acrd;
// mouse coordination
typedef double _Mcrd;

#define ZEROVALUE 0
#define BASELOG10 10

#define SIPPORTIVE_AXIS_MIN 10.
#define SIPPORTIVE_AXIS_MAX 25.
#define SIPPORTIVE_AXIS_BACKLASH_START 15
#define SIPPORTIVE_AXIS_BACKLASH_END 15

// calculation optimal discretization for supportive axis
double get_discretization_supportive_axis(_Acrd limit)
{
	// max abstract discretization
	double coarse_discretization = limit / SIPPORTIVE_AXIS_MIN;
	double order_scale = round(log10(coarse_discretization));
	double discretization = pow(BASELOG10, order_scale);
	double number_of_axes = limit / discretization;

	// search optimal discretization
	while ((number_of_axes > SIPPORTIVE_AXIS_MAX) || (number_of_axes < SIPPORTIVE_AXIS_MIN))
	{
		if (number_of_axes >= SIPPORTIVE_AXIS_MAX)
			discretization *= SIPPORTIVE_AXIS_MIN;
		else if (number_of_axes <= SIPPORTIVE_AXIS_MIN)
			discretization /= (SIPPORTIVE_AXIS_MAX / SIPPORTIVE_AXIS_MIN);
		number_of_axes = (limit / discretization);
	}

	return discretization;
}

void paint_once_supportive_axis_label(HDC& hMemDc, _Acrd x, _Acrd y, double value)
{
	if (value == ZEROVALUE) return;

	_Mcrd coordinate_x = get_mouse_coordinate_x(x);

	if (coordinate_x < SIPPORTIVE_AXIS_BACKLASH_START)
		coordinate_x = SIPPORTIVE_AXIS_BACKLASH_START;
	else if (coordinate_x > gc.window.count_pixels.x - SIPPORTIVE_AXIS_BACKLASH_END)
		coordinate_x = gc.window.count_pixels.x - SIPPORTIVE_AXIS_BACKLASH_END;

	_Mcrd coordinate_y = get_mouse_coordinate_y(y);

	if (coordinate_y < SIPPORTIVE_AXIS_BACKLASH_START)
		coordinate_y = SIPPORTIVE_AXIS_BACKLASH_START;
	else if (coordinate_y > gc.window.count_pixels.y - SIPPORTIVE_AXIS_BACKLASH_END)
		coordinate_y = gc.window.count_pixels.y - SIPPORTIVE_AXIS_BACKLASH_END;

	paint_text(hMemDc, coordinate_x, coordinate_y, value);
}

void paint_once_horizontal_axis(HDC& hMemDc, _Acrd y)
{
	_Mcrd coordinate_y = get_mouse_coordinate_y(y);
	if (coordinate_y > ZEROVALUE)
		paint_line(hMemDc, ZEROVALUE, coordinate_y, gc.window.count_pixels.x, coordinate_y);
}

void paint_once_vertical_axis(HDC& hMemDc, _Acrd x)
{
	_Mcrd coordinate_x = get_mouse_coordinate_x(x);
	if (coordinate_x >= ZEROVALUE)
		paint_line(hMemDc, coordinate_x, NULL, coordinate_x, gc.window.count_pixels.y);
}

void paint_main_axis(HDC& hMemDc)
{
	SelectObject(hMemDc, reinterpret_cast<HGDIOBJ>(stock_objects::pen.main_axis));
	paint_once_horizontal_axis(hMemDc, ZEROVALUE);
	paint_once_vertical_axis(hMemDc, ZEROVALUE);
}

void paint_supportive_axis(HDC& hMemDc)
{
	_Acrd discret_x = get_discretization_supportive_axis(gc.window.coordinates_limit.x);
	_Acrd discret_y = get_discretization_supportive_axis(gc.window.coordinates_limit.y);

	int round_count_x = round(gc.window.coordinates_begin.x / discret_x);
	int round_count_y = round(gc.window.coordinates_end.y / discret_y);

	if ((round_count_x * discret_x) < gc.window.coordinates_begin.x) round_count_x++;

	while (round_count_x * discret_x <= gc.window.coordinates_end.x)
	{
		paint_once_vertical_axis(hMemDc, round_count_x * discret_x);
		round_count_x++;
	}

	if ((round_count_y * discret_y) < gc.window.coordinates_end.y) round_count_y++;

	while (round_count_y * discret_y <= gc.window.coordinates_begin.y)
	{
		paint_once_horizontal_axis(hMemDc, round_count_y * discret_y);
		round_count_y++;
	}
}

void paint_supportive_axis_labels(HDC& hMemDc)
{
	SetTextColor(hMemDc, stock_objects::color.white);

	_Acrd discret_x = get_discretization_supportive_axis(gc.window.coordinates_limit.x);
	_Acrd discret_y = get_discretization_supportive_axis(gc.window.coordinates_limit.y);

	int round_count_x = round(gc.window.coordinates_begin.x / discret_x);
	int round_count_y = round(gc.window.coordinates_end.y / discret_y);

	while (round_count_x * discret_x <= gc.window.coordinates_end.x)
	{
		paint_once_supportive_axis_label(hMemDc, round_count_x * discret_x, ZEROVALUE, round_count_x * discret_x);
		round_count_x++;
	}

	while (round_count_y * discret_y <= gc.window.coordinates_begin.y)
	{
		paint_once_supportive_axis_label(hMemDc, ZEROVALUE, round_count_y * discret_y, round_count_y * discret_y);
		round_count_y++;
	}
}