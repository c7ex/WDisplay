#include"iostream"
#include"iomanip"
#include"random"

#include"display.hpp"

HINSTANCE arg_hInstance;
HINSTANCE arg_hPrevInstance;
LPSTR     arg_lpCmdLine;
int       arg_nCmdShow;

int main()
{
	std::random_device device;
	std::mt19937 gen(device());
	std::normal_distribution<double> law(10, 5);

	display display;
	size_t size = 1e6;
	std::vector<double> data_x(size);
	std::vector<double> data_y(size);
	for (auto i = 0; i < size; i++)
	{
		data_x[i] = 0.1*i;
		data_y[i] = law(gen) + (double)i / 10.;
	}

	display.set_display_limit(1000, 300);
	display.load_data(data_x, data_y);
	
	display.WinMain(
		arg_hInstance,
		arg_hPrevInstance,
		arg_lpCmdLine,
		arg_nCmdShow);

	return 0;
}