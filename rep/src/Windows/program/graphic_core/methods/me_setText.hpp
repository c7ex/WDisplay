void me_setText(int x, int y, std::wstring lable, double value)
{
	gl_wstr = (lable) + L"(" + std::to_wstring(value) + L")";
	TextOut(gl_paint::hMemDc, x, y, gl_wstr.c_str(), gl_wstr.size());
}