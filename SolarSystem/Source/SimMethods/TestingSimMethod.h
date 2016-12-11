#ifndef SIMMETHODS_TESTING_SIM_METHOD_HEADER
#define SIMMETHODS_TESTING_SIM_METHOD_HEADER

#include "SimMethod.h"
#include <iostream>

namespace solar
{
	//Circular motion
	class TestingSimMethod :public SimMethod
	{
	public:
		TestingSimMethod() {};

		void operator()(simData_t& data, double step) override final
		{
			for (auto& e : data)
			{
				e.vel += step*Vec2{-e.pos.X(), -e.pos.Y()};
				e.pos += step*e.vel;
			}
		}
	private:
		double state;
	};
}


#endif