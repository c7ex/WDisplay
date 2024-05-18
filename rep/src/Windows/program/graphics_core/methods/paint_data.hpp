#pragma once

// search: data[index_result] <= reference_value < index_result[index_result + 1]
void binary_search(std::vector<double>& data, _Acrd reference_value, _Cindx& index_result)
{
	_Csize volume = data.size();
	_Cindx r_index = volume - 1;
	_Cindx l_index = 0;

	// неоднозначность округления при делении на 2
	// сужает поиск до 3 элементов в худшем случае
	while (r_index - l_index > 3)
	{
		index_result = (l_index + r_index) / 2;

		if (data[index_result] > reference_value)
			r_index = index_result + 1;
		else
			l_index = index_result - 1;
	}

	index_result = l_index;

	while (data[index_result] < reference_value)
		index_result++;

	index_result--;
}

void paint_compressed_mode(
	HDC& hMemDc,
	std::vector<double>& x_values, 
	std::vector<double>& y_values,
	_Cindx& start_index,
	_Cindx& end_index,
	double compressed_scale)
{
	_Cindx current_index = start_index;
	_Cindx next_index = current_index + compressed_scale;

	while (next_index < end_index)
	{
		// search extremums
		_Cindx local_current_index = current_index;

		_Acrd extremum_min = y_values[current_index];
		_Acrd extremum_max = y_values[current_index];

		while (local_current_index <= next_index)
		{
			if (gl_data::data_content._data_y[local_current_index] < extremum_min)
			{
				extremum_min = y_values[local_current_index];
				local_current_index++;
				continue;
			}

			if (gl_data::data_content._data_y[local_current_index] > extremum_max)
			{
				extremum_max = y_values[local_current_index];
			}

			local_current_index++;
		}
		//

		_Wcrd x0 = get_window_coordinate_x(gl_data::data_content._data_x[current_index]);
		_Wcrd x1 = get_window_coordinate_x(gl_data::data_content._data_x[next_index]);

		_Wcrd y0 = get_window_coordinate_y(extremum_min);
		_Wcrd y1 = get_window_coordinate_y(extremum_max);

		paint_line(hMemDc, x0, y0, x1, y1);

		current_index = next_index;
		next_index = current_index + compressed_scale;
	}
}

void paint_default_mode(HDC& hMemDc,
	std::vector<double>& x_values,
	std::vector<double>& y_values,
	_Cindx& start_index,
	_Cindx& end_index)
{
	{
		for (_Cindx i = start_index; i < end_index; i++)
		{
			_Wcrd x0 = get_window_coordinate_x(x_values[i]);
			_Wcrd x1 = get_window_coordinate_x(x_values[i + 1]);

			_Wcrd y0 = get_window_coordinate_y(y_values[i]);
			_Wcrd y1 = get_window_coordinate_y(y_values[i + 1]);

			paint_line(hMemDc, x0, y0, x1, y1);
		}
	}
}

void paint_data(HDC& hMemDc)
{
	if (gl_data::data_content._data_x.size() > 1)
	{
		SelectObject(hMemDc, reinterpret_cast<HGDIOBJ>(stock_objects::pen.data_style_1));

		// abstruct window range
		_Acrd first_abstruct_value_in_window = gc.window.coordinates_begin.x;
		_Acrd last_abstruct_value_in_window = gc.window.coordinates_end.x;

		_Csize data_size = gl_data::data_content._data_x.size();
		_Acrd data_first_value = gl_data::data_content._data_x[0];
		_Acrd data_last_value = gl_data::data_content._data_x[data_size - 1];

		// if data in window
		if ((data_last_value > first_abstruct_value_in_window) && (data_first_value < last_abstruct_value_in_window))
		{
			_Cindx start_index = 0;
			_Cindx end_index = data_size - 1;

			std::vector<double>& x_values = gl_data::data_content._data_x;
			std::vector<double>& y_values = gl_data::data_content._data_y;

			// first visible index
			if (data_first_value < first_abstruct_value_in_window)
				binary_search(x_values, first_abstruct_value_in_window, start_index);

			// last visible index
			if (data_last_value > last_abstruct_value_in_window)
			{
				binary_search(x_values, last_abstruct_value_in_window, end_index);
				end_index++;
			}

			_Wcrd window_value_data_start = get_window_coordinate_x(x_values[start_index]);
			_Wcrd window_value_data_end = get_window_coordinate_x(x_values[end_index]);
			_Wcrd window_width = window_value_data_end - window_value_data_start;

			_Csize size_data_in_window = end_index - start_index;
			double compressed_scale = size_data_in_window / window_width;

			if (compressed_scale > 2)
				// if in one pixel 2 points and more
				paint_compressed_mode(hMemDc, x_values, y_values, start_index, end_index, compressed_scale);
			else
				paint_default_mode(hMemDc, x_values, y_values, start_index, end_index);
		}
	}
}