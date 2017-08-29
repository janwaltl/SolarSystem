#ifndef COMMON_1345325_HEADER
#define COMMON_1345325_HEADER


#include <cmath>

namespace solar
{
	//Class that acompanies SimData class and stores in which units(distance/time/mass) those SimDatas' objects' variables are.
	class PhysUnits
	{
	public:
		//Cannot use num/den integers because there isn't enough precision(especially for mass)
		using ratio = double;
		//Time
		static constexpr ratio second {1.0};
		static constexpr ratio minute {60.0};
		static constexpr ratio hour {3'600.0};
		static constexpr ratio day {86'400.0};
		static constexpr ratio year {31'536'000.0};
		//Mass
		static constexpr ratio kilogram {1.0};
		static constexpr ratio earth {5.9722e24};
		static constexpr ratio sun {1.988435e30};
		//Distance
		static constexpr ratio meter {1.0};
		static constexpr ratio kilometer {1e3};
		static constexpr ratio AU {149'597'870'700.0};
		static constexpr ratio lightYear {9'460'730'472'580'800};
		//Number of kilograms in one unit of mass
		ratio mass;
		//Number of seconds in one unit of time
		ratio time;
		//Number of meters in one unit of distance
		ratio dist;
		PhysUnits(ratio mass = kilogram, ratio time = second, ratio dist = meter) :mass(mass), time(time), dist(dist) {}
	};
}


#endif