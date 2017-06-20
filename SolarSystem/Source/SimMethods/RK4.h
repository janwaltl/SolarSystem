#ifndef SIMMETHODS_RK4_HEADER
#define SIMMETHODS_RK4_HEADER

#include "SimMethod.h"

#include <vector>
#include <array>
#include "Source/Common/Timing.h"
#include <fstream>

namespace solar
{
	//Simulates data using fourth order Runge-Kutta integration method
	//Expects data to be in following units:
	//		-pos in AU units
	//		-vel in AU/Earth year
	//		-mass in multiples of Sun's masses
	class RK4 : public SimMethod
	{
	public:
		void Prepare() override final;
		void operator()(double step) override final;
	private:
		struct AccVel//Helper class just to clear up notation, can be replaced with pair
		{
			Vec2d acc, vel;
		};
		struct VelPos//Helper clas just to clear up notation, can be replaced with pair
		{
			Vec2d vel, pos;
		};
		//Temporary object for each unit, holds vel, pos for individual RK4 substeps
		std::vector<VelPos> temps;
		//Coefficients k1,k2,k3,k4 for each unit
		std::array<std::vector<AccVel>, 4> kXs;
	};
}

#endif