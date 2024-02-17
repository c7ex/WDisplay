#include"iostream"
#include"display.hpp"
#include"random"

HINSTANCE arg_hInstance;
HINSTANCE arg_hPrevInstance;
LPSTR     arg_lpCmdLine;
int       arg_nCmdShow;

int main()
{
	std::random_device device;
	std::mt19937 gen(device());
	std::normal_distribution<double> low(10, 5);

	Display display;
	size_t size = 1e3;
	std::vector<double> data(size);
	for (auto i = 0; i < size; i++)
		data[i] = low(gen);

	display.load_data(data);
	
	display.WinMain(
		arg_hInstance,
		arg_hPrevInstance,
		arg_lpCmdLine,
		arg_nCmdShow);

	return 0;
}