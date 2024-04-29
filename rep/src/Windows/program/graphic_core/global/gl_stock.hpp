namespace gl_stock
{
	namespace brush
	{
		HBRUSH BACKGROUND = CreateSolidBrush(gl_color::black);
	}

	namespace pen
	{
		HPEN BACKGROUND = CreatePen(PS_SOLID, 1, gl_color::black);
		HPEN MAINAXIS = CreatePen(PS_SOLID, 1, gl_color::white);
		
		HPEN TEST_OBJ1 = CreatePen(PS_SOLID, 1, gl_color::red);
		HPEN TEST_OBJ2 = CreatePen(PS_SOLID, 1, gl_color::green);

		HPEN AXIS = CreatePen(PS_SOLID, 1, gl_color::axis);
	}
}