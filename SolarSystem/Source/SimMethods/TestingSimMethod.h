#ifndef SIMMETHODS_TESTING_SIM_METHOD_HEADER
#define SIMMETHODS_TESTING_SIM_METHOD_HEADER

#include "SimMethod.h"
#include <iostream>
class TestingSimMethod :public SimMethod
{
public:
	TestingSimMethod(double st) :state(st) {};

	void operator()(elements_t& elements, double step) override final
	{
		std::cout << "Simulating " << elements.size() << " elements\n";
		for (auto& e : elements)
		{
			e.mass += state;
		}
	}
private:
	double state;
};


#endif