#include"iostream"
#include"iomanip"
#include"random"

#include"display.hpp"

int main()
{
	display display;

	// 10 test-graphs ~ 4 fps
	for (int h = 0; h < 10; h++)
	{
		std::random_device device;
		std::mt19937 gen(device());
		std::normal_distribution<double> law(0, 10);
		double amplitude = 2;
		double tau = 50 + law(gen) * 10.;

		size_t size = (size_t)1e5;
		std::vector<double> data(size);
		for (auto i = 0; i < size; i++)

			data[i] =
			amplitude * amplitude * amplitude * sin(double(i + law(gen)) / (tau * tau * tau)) +
			amplitude * amplitude * cos(double(i + law(gen)) / (tau * tau)) +
			amplitude * sin(double(i + law(gen)) / tau);

		COLORREF color = RGB(
			(unsigned int)(law(gen)) & 0xff, 
			(unsigned int)(law(gen)) & 0xff, 
			(unsigned int)(law(gen)) & 0xff);

		double step = 0.4445; //abs(law(gen));
		double offset = law(gen);

		std::cout << "i:\t" << h << "\tstep: " << step << "\toffset: " << offset << std::endl;

		display.load_data(data, color, step, offset);
	}

	display.run();

	return 0;
}