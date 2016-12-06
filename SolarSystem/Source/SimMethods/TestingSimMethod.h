#ifndef SIMMETHODS_TESTING_SIM_METHOD_HEADER
#define SIMMETHODS_TESTING_SIM_METHOD_HEADER

#include "SimMethod.h"
#include <iostream>

class TestingSimMethod :public SimMethod
{
public:
	TestingSimMethod(double st) :state(st) {};

	void operator()(simData_t& data, double step) override final
	{
		std::cout << "Simulating " << data.size() << " data\n";
		for (auto& e : data)
		{
			e.mass += state;
		}
	}
private:
	double state;
};


#endif