#pragma once
void paint_data(HDC& hMemDc)
{
	if (gl_data::data_content._data_x.size() != 0)
	{
		SelectObject(hMemDc, reinterpret_cast<HGDIOBJ>(stock_objects::pen.test_object2));

		long long data_size = gl_data::data_content._data_x.size();
		double start_data_x = (gl_data::data_content._data_x[0]);
		double end_data_x = (gl_data::data_content._data_x[data_size - 1]);

		if ((end_data_x > gc.window.coordinates_begin.x) && (start_data_x < gc.window.coordinates_end.x))
		{
			long long start_index = 0;
			long long end_index = data_size - 1;

			if (start_data_x < gc.window.coordinates_begin.x)
			{
				while (gl_data::data_content._data_x[start_index] < gc.window.coordinates_begin.x)
					start_index++;

				if (start_index != 0)
					start_index--;
			}

			if (end_data_x > gc.window.coordinates_end.x)
			{
				while (gl_data::data_content._data_x[end_index] > gc.window.coordinates_end.x)
					end_index--;

				if (end_index > (data_size - 1))
					end_index = data_size - 1;
			}

			double mouse_coord_data_start = get_mouse_coordinate_x(gl_data::data_content._data_x[start_index]);
			double mouse_coord_data_end = get_mouse_coordinate_x(gl_data::data_content._data_x[end_index]);
			double mouse_width = mouse_coord_data_end - mouse_coord_data_start;
			long long count = end_index - start_index;
			double compressed_scale = count / mouse_width;

			if (compressed_scale > 2)
			{
				double c = start_index;
				double n = c + compressed_scale;

				while (n < end_index)
				{
					// search extremums
					long long local_current_index = c;

					double extremum_min = gl_data::data_content._data_y[c];
					double extremum_max = gl_data::data_content._data_y[c];

					while (local_current_index <= n)
					{
						if (gl_data::data_content._data_y[local_current_index] < extremum_min)
						{
							extremum_min = gl_data::data_content._data_y[local_current_index];
							local_current_index++;
						}

						if (gl_data::data_content._data_y[local_current_index] > extremum_max)
						{
							extremum_max = gl_data::data_content._data_y[local_current_index];
						}

						local_current_index++;
					}
					//

					double x0 = get_mouse_coordinate_x(gl_data::data_content._data_x[c]);
					double x1 = get_mouse_coordinate_x(gl_data::data_content._data_x[n]);

					double y0 = get_mouse_coordinate_y(extremum_min);
					double y1 = get_mouse_coordinate_y(extremum_max);

					paint_line(hMemDc, x0, y0, x1, y1);

					c = n;
					n = c + compressed_scale;
				}
			}
			else
			{
				for (int i = start_index; i < end_index; i++)
				{
					double x0 = get_mouse_coordinate_x(gl_data::data_content._data_x[i]);
					double x1 = get_mouse_coordinate_x(gl_data::data_content._data_x[i + 1]);

					double y0 = get_mouse_coordinate_y(gl_data::data_content._data_y[i]);
					double y1 = get_mouse_coordinate_y(gl_data::data_content._data_y[i + 1]);

					paint_line(hMemDc, x0, y0, x1, y1);
				}
			}
		}
	}
}