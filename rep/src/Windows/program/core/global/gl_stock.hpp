namespace gl_stock
{
	namespace brush
	{
		HBRUSH BACKGROUND = CreateSolidBrush(gl_color::black);
	}

	namespace pen
	{
		HPEN BACKGROUND = CreatePen(PS_SOLID, 1, gl_color::black);
		HPEN AXIS = CreatePen(PS_SOLID, 1, gl_color::white);
		HPEN TEST_OBJ = CreatePen(PS_SOLID, 1, gl_color::red);
	}
}