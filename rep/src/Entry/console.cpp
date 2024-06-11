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
	size_t size = (size_t)100; // 1M points
	std::vector<double> data(size);
	for (auto i = 0; i < size; i++)

		data[i] = 10*law(gen);
		            //amplitude * amplitude * amplitude * sin(double(i + law(gen)) / (tau * tau * tau)) +
		            //amplitude * amplitude * cos(double(i + law(gen)) / (tau * tau)) +
		            //amplitude * sin(double(i + law(gen))/tau);

	display.run();

	return 0;
}