#pragma once

#define types_ZERO    0
#define types_HALF    0.5

// "x" and "y" conteiner
class simplest_unit
{
protected:	
	double _x;
	double _y;

public:
	double get_x(){return _x;}
	double get_y(){return _y;}

public:
	void y_invert()
	{
		_y = -_y;
	}

	void modulus()
	{
		_x = abs(_x);
		_y = abs(_y);
	}

	void integer()
	{
		_x = round(_x);
		_y = round(_y);
	}

public:
	simplest_unit operator+ (const simplest_unit& rigth)
	{
		return simplest_unit{ _x + rigth._x, _y + rigth._y };
	}

	simplest_unit operator- (const simplest_unit& rigth)
	{
		return simplest_unit{ _x - rigth._x, _y - rigth._y };
	}

	simplest_unit operator* (const simplest_unit& rigth)
	{
		return simplest_unit{ _x * rigth._x, _y * rigth._y };
	}

	simplest_unit operator/ (const simplest_unit& rigth)
	{
		if ((rigth._x == 0) || (rigth._y == 0))
			return simplest_unit{ 0,0 };

		return simplest_unit{ _x / rigth._x, _y / rigth._y };
	}

public:
	simplest_unit operator+ (double rigth)
	{
		return simplest_unit{ _x + rigth, _y + rigth };
	}

	simplest_unit operator- (double rigth)
	{
		return simplest_unit{ _x - rigth, _y - rigth };
	}

	simplest_unit operator* (double rigth)
	{
		return simplest_unit{ _x * rigth, _y * rigth };
	}

	simplest_unit operator/ (double rigth)
	{
		if (rigth == 0)
			return simplest_unit{ 0,0 };

		return simplest_unit{ _x / rigth, _y / rigth };
	}

public:
	bool operator> (const simplest_unit& rigth)
	{
		return ((_x > rigth._x) && (_y > rigth._y));
	}

	bool operator< (const simplest_unit& rigth)
	{
		return ((_x < rigth._x) && (_y < rigth._y));
	}

	bool operator>= (const simplest_unit& rigth)
	{
		return ((_x >= rigth._x) && (_y >= rigth._y));
	}

	bool operator<= (const simplest_unit& rigth)
	{
		return ((_x <= rigth._x) && (_y <= rigth._y));
	}

	bool operator== (const simplest_unit& rigth)
	{
		return ((_x == rigth._x) && (_y == rigth._y));
	}

public:
	void set(double new_x, double new_y)
	{
		_x = new_x;
		_y = new_y;
	}

	void set(const simplest_unit& other)
	{
		_x = other._x;
		_y = other._y;
	}

public:
	simplest_unit(double new_x, double new_y)
	{
		_x = new_x;
		_y = new_y;
	}

	simplest_unit()
	{ 
		_x = types_ZERO;
		_y = types_ZERO;
	}
};

typedef simplest_unit xy_point; // point (x,y)
typedef simplest_unit xy_param; // parameters for (x,y)

// parameters with reference/current/expand values
// control for slaveparam
// 1. init reference - not change
// 2. load new current or expand parameters
// 3. update state: expand and current
class master_param
{
private:
	xy_param _reference;
	xy_param _current;
	xy_param _expand;

public:
	xy_param get_current()
	{
		return _current;
	}

	xy_param get_expand()
	{
		return _expand;
	}

public:
	void update_from_current(const xy_param& new_current)
	{
		_current = new_current;
		_expand = _current / _reference;
	}

	void update_from_expand(const xy_param& new_expand)
	{
		_expand = new_expand;
		_current = _reference * _expand;
	}

public:
	void set(double new_x, double new_y)
	{
		_reference.set(new_x, new_y);
		_current.set(new_x, new_y);
	}

	void set(xy_param reference)
	{
		_reference.set(reference);
		_current.set(reference);
	}

public:
	master_param(xy_param reference)
	{
		_reference.set(reference);
		_current.set(reference);
		_expand = _reference / _current;
	}

	master_param(){}
};

// parameters with reference/current values
// 1. init reference - not change
// 2. load expand parameter for change current parameter
class slave_param
{
protected:
	xy_param _reference;
	xy_param _current;

public:
	xy_param get_current()
	{
		return _current;
	}

public:
	void update_from_expand(const xy_param& new_expand)
	{
		_current = _reference * new_expand;
	}

public:
	void set(double new_x, double new_y)
	{
		_reference.set(new_x, new_y);
		_current.set(new_x, new_y);
	}

	void set(xy_param reference)
	{
		_reference.set(reference);
		_current.set(reference);
	}

public:
	slave_param(xy_param reference)
	{
		_reference.set(reference);
		_current.set(reference);
	}

	slave_param() {}
};

// slavepoint
// 1. init reference point - not change
// 2. load expand parameter for change current point
typedef slave_param slave_point;

// rect_prop
// 1. recrangle with maintaining proportions in form
// 2. update and paint rect in form
class rect_prop
{
private:
	slave_param _size;
	slave_point _centre;
	xy_point _spoint;
	xy_point _epoint;

public:
	slave_param get_size()
	{
		return _size;
	}

	slave_point get_centre()
	{
		return _centre;
	}

public:
	xy_point& get_start_point()
	{
		return _spoint;
	}

	xy_point& get_end_point()
	{
		return _epoint;
	}

public:
	void update(const xy_param& expand)
	{
		_size.update_from_expand(expand);
		_centre.update_from_expand(expand);

		_spoint = _centre.get_current() - (_size.get_current() * types_HALF);
		_epoint = _centre.get_current() + (_size.get_current() * types_HALF);
	}

	void paint(HDC& hdc, HPEN& pen, HBRUSH& brush)
	{
		SelectObject(hdc, pen);
		SelectObject(hdc, brush);
		Rectangle(hdc,
			static_cast<int>(_spoint.get_x()),
			static_cast<int>(_spoint.get_y()),
			static_cast<int>(_epoint.get_x()),
			static_cast<int>(_epoint.get_y()));
	}

public:

	void init_reference(xy_param init_size, xy_param init_centre)
	{
		_size.set(init_size);
		_centre.set(init_centre);

		_spoint = _centre.get_current() - (_size.get_current() * types_HALF);
		_epoint = _centre.get_current() + (_size.get_current() * types_HALF);
	}

	rect_prop()
	{
		_centre.set(
			types_ZERO,
			types_ZERO);
		_size.set(
			types_ZERO,
			types_ZERO);
	}
};