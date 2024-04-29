#pragma once
#include<vector>

class content
{
public:
	std::vector<double> _data_x;
	std::vector<double> _data_y;

	void set(std::vector<double>& data_x, std::vector<double>& data_y)
	{
		_data_x = data_x;
		_data_y = data_y;
	}

	content(std::vector<double>& data_x, std::vector<double>& data_y)
	{
		_data_x = data_x;
		_data_y = data_y;
	}

	content(){}
};

namespace gl_data
{
	content data_content;
}