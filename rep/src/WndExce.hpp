#pragma once
#include"windows.h"

// check on exception
void WndExce(bool condition, LPCWSTR exceptionText)
{
	if (condition)
	{
		MessageBox(
			NULL,
			exceptionText,
			TEXT("Exception"),
			NULL);
		PostQuitMessage(0);
	}
}