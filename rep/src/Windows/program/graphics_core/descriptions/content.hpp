#pragma once
#include<vector>

// content type 1
// sorted equal-ordered data <seod>
// y       - data   - set user
// dx      - const  - set user
// first_x - const  - set user

#define SEOD_DEFAULT_FIRST_X 0
#define SEOD_DEFAULT_dX 1

class seod
{
public:
	std::vector<double> _data;
	double _dx;
	double _first_x;

	void set(std::vector<double>& data, double dx = SEOD_DEFAULT_dX, double first_x = SEOD_DEFAULT_FIRST_X)
	{
		_data = data;
		_dx = dx;
		_first_x = first_x;
	}

	seod( std::vector<double>& data)
	{
		_data = data;
		_dx = SEOD_DEFAULT_dX;
		_first_x = SEOD_DEFAULT_FIRST_X;
	}

	seod() 
	{
		_dx = SEOD_DEFAULT_dX;
		_first_x = SEOD_DEFAULT_FIRST_X;
	}
};