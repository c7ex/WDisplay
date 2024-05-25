#include"iostream"
#include"iomanip"
#include"random"

#include"display.hpp"

int main()
{
	std::random_device device;
	std::mt19937 gen(device());
	std::normal_distribution<double> law(0, 10);
	double amplitude = 2;
	double tau = 50;
	
	display display;
	size_t size = 1e6; // 1M points
	std::vector<double> data_y(size);
	for (auto i = 0; i < size; i++)
		
		data_y[i] = 
		            amplitude * amplitude * amplitude * sin(double(i + law(gen)) / (tau * tau * tau)) +
		            amplitude * amplitude * cos(double(i + law(gen)) / (tau * tau)) +
		            amplitude * sin(double(i + law(gen))/tau);

	display.set_display_limit(size, amplitude);
	display.load_data(data_y);
	display.run();

	return 0;
}