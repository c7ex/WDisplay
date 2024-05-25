#pragma once
#include"chrono"

void paint_compressed_mode(
	HDC& hMemDc,
	std::vector<double>& values,
	_Cindx& start_index,
	_Cindx& end_index,
	double compressed_scale)
{
	_Cindx current_index = start_index;
	_Cindx next_index = current_index + compressed_scale;
	_Acrd data_delta_value = gl_data::content_seod._dx;

	while (next_index < end_index)
	{
		// search extremums
		_Cindx local_current_index = current_index;

		_Acrd extremum_min = values[current_index];
		_Acrd extremum_max = values[current_index];

		while (local_current_index <= next_index)
		{
			if (gl_data::content_seod._data[local_current_index] < extremum_min)
			{
				extremum_min = values[local_current_index];
				local_current_index++;
				continue;
			}

			if (gl_data::content_seod._data[local_current_index] > extremum_max)
			{
				extremum_max = values[local_current_index];
			}

			local_current_index++;
		}
		//

		_Wcrd x0 = get_window_coordinate_x(current_index * data_delta_value);
		_Wcrd x1 = get_window_coordinate_x(current_index * data_delta_value);

		_Wcrd y0 = get_window_coordinate_y(extremum_min);
		_Wcrd y1 = get_window_coordinate_y(extremum_max);

		paint_line(hMemDc, x0, y0, x1, y1);

		current_index = next_index;
		next_index = current_index + compressed_scale;
	}
}

void paint_default_mode(HDC& hMemDc,
	std::vector<double>& y_values,
	_Cindx& start_index,
	_Cindx& end_index)
{
	{
		_Acrd data_delta_value = gl_data::content_seod._dx;

		for (_Cindx i = start_index; i < end_index; i++)
		{
			_Wcrd x0 = get_window_coordinate_x(i * data_delta_value);
			_Wcrd x1 = get_window_coordinate_x((i + 1) * data_delta_value);

			_Wcrd y0 = get_window_coordinate_y(y_values[i]);
			_Wcrd y1 = get_window_coordinate_y(y_values[i + 1]);

			paint_line(hMemDc, x0, y0, x1, y1);
		}
	}
}

void paint_data(HDC& hMemDc)
{
	if (gl_data::content_seod._data.size() > 1)
	{
		SelectObject(hMemDc, reinterpret_cast<HGDIOBJ>(stock_objects::pen.data_style_1));

		// abstruct window range
		_Acrd first_abstruct_value_in_window = gc.window.coordinates_begin.x;
		_Acrd last_abstruct_value_in_window = gc.window.coordinates_end.x;

		_Csize data_size = gl_data::content_seod._data.size();
		_Acrd data_first_value = gl_data::content_seod._first_x;
		_Acrd data_last_value = gl_data::content_seod._first_x + gl_data::content_seod._dx * (data_size - 1);
		_Acrd data_delta_value = gl_data::content_seod._dx;

		// if data in window
		if ((data_last_value > first_abstruct_value_in_window) && (data_first_value < last_abstruct_value_in_window))
		{
			_Cindx start_index = 0;
			_Cindx end_index = data_size - 1;

			std::vector<double>& values = gl_data::content_seod._data;

			// first visible index
			if (data_first_value < first_abstruct_value_in_window)
			{
				start_index = first_abstruct_value_in_window / data_delta_value;
				if (start_index < 0)
					start_index = 0;
			}

			// last visible index
			if (data_last_value > last_abstruct_value_in_window)
			{
				end_index = last_abstruct_value_in_window / data_delta_value + 1;
				if (start_index > data_size - 1)
					start_index = data_size - 1;
			}

			_Wcrd window_value_data_start = get_window_coordinate_x(start_index * data_delta_value);
			_Wcrd window_value_data_end = get_window_coordinate_x(end_index * data_delta_value);
			_Wcrd window_width = window_value_data_end - window_value_data_start;

			_Csize size_data_in_window = end_index - start_index;
			double compressed_scale = size_data_in_window / window_width;

			if (compressed_scale > 2)
				// if in one pixel 2 points and more
				paint_compressed_mode(hMemDc, values, start_index, end_index, compressed_scale);
			else
				paint_default_mode(hMemDc, values, start_index, end_index);
		}
	}
}