#ifndef SIMMETHODS_SEMIIMPLICITEULER_HEADER
#define SIMMETHODS_SEMIIMPLICITEULER_HEADER

#include "SimMethod.h"

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
		void operator()(double step) override final;
	private:
	};
}


#endif