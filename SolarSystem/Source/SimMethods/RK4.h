#ifndef SIMMETHODS_RK4_HEADER
#define SIMMETHODS_RK4_HEADER

#include "SimMethod.h"

#include <vector>
#include <array>
namespace solar
{
	class RK4 : public SimMethod
	{
	public:
		void Prepare() override final;
		void operator()(double step) override final;
	private:
		struct AccVel//Helper class just to clear up notation, can be replaced with pair
		{
			Vec2 acc, vel;
		};
		struct VelPos//Helper clas just to clear up notation, can be replaced with pair
		{
			Vec2 vel, pos;
		};
		//Temporary object for each unit, holds vel,pos between individual RK4 steps
		std::vector<VelPos> temps;
		//Coefficients k1,k2,k3,k4 for each unit
		std::array<std::vector<AccVel>, 4> kXs;

	};
}

#endif