#ifndef SIMMETHODS_EXPLICITEULER_HEADER
#define SIMMETHODS_EXPLICITEULER_HEADER

#include "SimMethod.h"
#include "Source/Common/Timing.h"
namespace solar
{
	//Simulates data using semi-implicit Euler integration method
	//Expects data to be in following units:
	//		-pos in AU units
	//		-vel in AU/Earth year
	//		-mass in multiples of Sun's masses
	class ExplicitEuler :public SimMethod
	{
	public:
		//ExplicitEuler() { timing.Reset(); }
		void operator()(double step) override final;
	private:
		/*TimeMeasurement timing;
		size_t numTimeSamples;*/
	};
}


#endif