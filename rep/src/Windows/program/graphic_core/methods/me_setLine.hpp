void me_setLine(int x0, int y0, int x1, int y1)
{
	MoveToEx(gl_paint::hMemDc, x0, y0, NULL);
	LineTo(gl_paint::hMemDc, x1, y1);
}