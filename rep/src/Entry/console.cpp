#include"iostream"
#include"iomanip"
#include"random"

#include"display.hpp"

int main()
{
	std::random_device device;
	std::mt19937 gen(device());
	std::normal_distribution<double> law(0, 10);
	double amplitude = 1000;
	double tau = 100;
	
	display display;
	size_t size = 1e4;
	std::vector<double> data_x(size);
	std::vector<double> data_y(size);
	for (auto i = 0; i < size; i++)
	{
		data_x[i] = i;
		data_y[i] = amplitude * sin((double)i / tau) / ((double)i / tau);// + law(gen);
	}

	display.set_display_limit(size, amplitude);
	display.load_data(data_x, data_y);
	display.run();

	return 0;
}