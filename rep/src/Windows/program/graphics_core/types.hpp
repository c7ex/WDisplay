#pragma once

#define tyzero    0
#define tyhalf    0.5

class pointf;
class paramf;
class masterparam;
class slaveparam;
class slavepoint;
class rectpropf;

// pointf
// point on form (x,y)
class pointf
{
protected:	
	double _x;
	double _y;

public:
	double get_x()
	{
		return _x;
	}

	double get_y()
	{
		return _y;
	}

	void y_inv()
	{
		_y = -_y;
	}

public:
	pointf operator+ (const pointf& rigth)
	{
		return pointf{ _x + rigth._x, _y + rigth._y };
	}

	pointf operator- (const pointf& rigth)
	{
		return pointf{ _x - rigth._x, _y - rigth._y };
	}

	pointf operator* (const pointf& rigth)
	{
		return pointf{ _x * rigth._x, _y * rigth._y };
	}

	pointf operator*  (double rigth)
	{
		return pointf{ _x * rigth, _y * rigth };
	}

	pointf operator/ (const pointf& rigth)
	{
		if ((rigth._x == 0) || (rigth._y == 0))
			return pointf{ 0,0 };

		return pointf{ _x / rigth._x, _y / rigth._y };
	}

	pointf operator/ (double rigth)
	{
		if (rigth == 0)
			return pointf{ 0,0 };

		return pointf{ _x / rigth, _y / rigth };
	}

public:
	void set(double new_x, double new_y)
	{
		_x = new_x;
		_y = new_y;
	}

	void set(const pointf& other)
	{
		_x = other._x;
		_y = other._y;
	}

	pointf(double new_x, double new_y)
	{
		_x = new_x;
		_y = new_y;
	}

	pointf()
	{ 
		_x = tyzero;
		_y = tyzero;
	}
};

// paramf
// parameters of form (x,y)
class paramf : public pointf
{
public:
	pointf to_pointf()
	{
		return pointf{_x, _y};
	}

public:
	paramf operator+ (const paramf& rigth)
	{
		return paramf{ _x + rigth._x, _y + rigth._y };
	}

	paramf operator+ (double rigth)
	{
		return paramf{ _x + rigth, _y + rigth};
	}

	paramf operator- (const paramf& rigth)
	{
		return paramf{ _x - rigth._x, _y - rigth._y };
	}

	paramf operator* (const paramf& rigth)
	{
		return paramf{ _x * rigth._x, _y * rigth._y };
	}

	paramf operator/ (const paramf& rigth)
	{
		if ((rigth._x == 0) || (rigth._y == 0))
			return paramf{ 0,0 };

		return paramf{ _x / rigth._x, _y / rigth._y };
	}

public:
	paramf(pointf point)
	{
		_x = point.get_x();
		_y = point.get_y();
	}

	paramf(double new_x, double new_y)
	{
		_x = new_x;
		_y = new_y;
	}

	paramf() : pointf(){}
};

// parameters with reference/current/expand values
// control for slaveparam
// 1. init reference - not change
// 2. load new current or expand parameters
// 3. update state: expand and current
class masterparam
{
private:
	paramf _reference;
	paramf _current;
	paramf _expand;

public:

	paramf get_current()
	{
		return _current;
	}

	paramf get_expand()
	{
		return _expand;
	}

	void update_from_current(const paramf& new_current)
	{
		_current = new_current;
		_expand = _current / _reference;
	}

	void update_from_expand(const paramf& new_expand)
	{
		_expand = new_expand;
		_current = _reference * _expand;
	}

	void set(double new_x, double new_y)
	{
		_reference.set(new_x, new_y);
		_current.set(new_x, new_y);
	}

	void set(paramf reference)
	{
		_reference.set(reference);
		_current.set(reference);
	}

	masterparam(paramf reference)
	{
		_reference.set(reference);
		_current.set(reference);
		_expand = _reference / _current;
	}

	masterparam(){}
};

// parameters with reference/current values
// 1. init reference - not change
// 2. load expand parameter for change current parameter
class slaveparam
{
protected:
	paramf _reference;
	paramf _current;

public:

	paramf get_current()
	{
		return _current;
	}

	void update_from_expand(const paramf& new_expand)
	{
		_current = _reference * new_expand;
	}

	void set(double new_x, double new_y)
	{
		_reference.set(new_x, new_y);
		_current.set(new_x, new_y);
	}

	void set(paramf reference)
	{
		_reference.set(reference);
		_current.set(reference);
	}

	slaveparam(paramf reference)
	{
		_reference.set(reference);
		_current.set(reference);
	}

	slaveparam() {}
};

// slavepoint
// 1. init reference point - not change
// 2. load expand parameter for change current point
class slavepoint : public slaveparam {};

// rectf
// 1. recrangle with maintaining proportions in form
// 2. update and paint rect in form
class rectpropf
{
private:
	slaveparam _size;
	slavepoint _centre;
	pointf _spoint;
	pointf _epoint;

public:
	slaveparam get_size()
	{
		return _size;
	}

	slavepoint get_centre()
	{
		return _centre;
	}

public:
	void update(const paramf& expand)
	{
		_size.update_from_expand(expand);
		_centre.update_from_expand(expand);

		_spoint = _centre.get_current().to_pointf() - (_size.get_current().to_pointf() * tyhalf);
		_epoint = _centre.get_current().to_pointf() + (_size.get_current().to_pointf() * tyhalf);
	}

	void paint(HDC& hdc, HPEN& pen, HBRUSH& brush)
	{
		SelectObject(hdc, pen);
		SelectObject(hdc, brush);
		Rectangle(hdc,
			_spoint.get_x(),
			_spoint.get_y(),
			_epoint.get_x(),
			_epoint.get_y());
	}

public:

	void init_reference(paramf init_size, paramf init_centre)
	{
		_size.set(init_size);
		_centre.set(init_centre);

		_spoint = _centre.get_current().to_pointf() - (_size.get_current().to_pointf() * tyhalf);
		_epoint = _centre.get_current().to_pointf() + (_size.get_current().to_pointf() * tyhalf);
	}

	rectpropf()
	{
		_centre.set(
			tyzero,
			tyzero);
		_size.set(
			tyzero,
			tyzero);
	}
};