#ifndef COMMON_1345325_HEADER
#define COMMON_1345325_HEADER


#include <cmath>
#include <ratio>


namespace solar
{

	//Used physics units: AU, Earth's years, Sun's weight
	//Conversions between used units and SIUnits
	namespace physicsUnits
	{
		static constexpr double AUtoM = 149'597'870'700.0; // Astronomical units to meters
		static constexpr double YtoS = 31'536'000.0; // Years to seconds
		static constexpr double SMtoKG = 1.988435e30; // Solar masses to kilograms

		static constexpr double AUpYtoMpS = AUtoM / YtoS;//AUs per Year to Meters per Second
	};
	class PhysUnits
	{
	public:
		//Cannot use num/den because there isn't enough precision(especially for mass)
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

		ratio mass, time, dist;
		PhysUnits(ratio mass = kilogram, ratio time = second, ratio dist = meter) :mass(mass), time(time), dist(dist) {}
	};
}


#endif