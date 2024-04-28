std::wstring double2string(double value)
{
	if (value == 0)
		return std::to_wstring(0);

	std::wstring result = std::to_wstring(value);

	int size = result.size();
	int k = 0;

	while (k != size - 1)
	{
		int current_index = size - 1 - k;

		k++;
		if (result[current_index] == L'.')
			break;

		k--;
		if (result[current_index] != L'0')
			break;

		k++;
	}

	result.resize(size - k);

	return result;
}

void me_setText(int x, int y, double value)
{
	gl_wstr = double2string(value);
	TextOut(gl_paint::hMemDc, x, y, gl_wstr.c_str(), gl_wstr.size());
}

void me_setText(int x, int y, std::wstring lable, double value)
{
	gl_wstr = (lable) + L"(" + std::to_wstring(value) + L")";
	TextOut(gl_paint::hMemDc, x, y, gl_wstr.c_str(), gl_wstr.size());
}