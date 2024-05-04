void me_setLine(HDC& hMemDc, int x0, int y0, int x1, int y1)
{
	MoveToEx(hMemDc, x0, y0, NULL);
	LineTo(hMemDc, x1, y1);
}