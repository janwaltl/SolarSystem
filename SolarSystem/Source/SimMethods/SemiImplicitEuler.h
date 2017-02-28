#ifndef SIMMETHODS_SEMIIMPLICITEULER_HEADER
#define SIMMETHODS_SEMIIMPLICITEULER_HEADER

#include "SimMethod.h"
#include "Source/Common/Timing.h"
#include <fstream>

namespace solar
{
	//Simulates data using semi-implicit Euler integration method
	//Expects data to be in following units:
	//		-pos in AU units
	//		-vel in AU/Earth year
	//		-mass in multiples of Sun's masses
	class SemiImplicitEuler :public SimMethod
	{
	public:
		SemiImplicitEuler();
		void operator()(double step) override final;
	private:
		/*TimeMeasurement timing;
		size_t numTimeSamples;
		size_t maxSamples;
		std::ofstream out;*/
	};
}


#endif