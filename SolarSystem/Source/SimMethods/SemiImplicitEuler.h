#ifndef SIMMETHODS_SEMIIMPLICITEULER_HEADER
#define SIMMETHODS_SEMIIMPLICITEULER_HEADER

#include "SimMethod.h"
#include "Source/Common/Timing.h"
namespace solar
{
	//Simulates data using semi-implicit Euler integration method
	//Expects data to be in following units:
	//		-pos in AU units
	//		-vel in AU/Earth year
	//		-mass in multiples of Sun's masses
	//http://www.math.kit.edu/ianm3/lehre/geonumint2009s/media/gni_by_stoermer-verlet.pdf
	class SemiImplicitEuler :public SimMethod
	{
	public:
		SemiImplicitEuler() { timing.Reset(); }
		void operator()(double step) override final;
	private:
		TimeMeasurement timing;
		size_t numTimeSamples;
	};
}


#endif