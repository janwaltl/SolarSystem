#ifndef COMMON_1345325_HEADER
#define COMMON_1345325_HEADER


#include <cmath>

namespace solar
{

	//Used physics units: AU,years,Sun's weight
	//Conversions between used units and SIUnits
	namespace physicsUnits
	{
		static constexpr double AUtoM = 149'597'870'700.0; // Astronomical units to meters
		static constexpr double YtoS = 31'536'000.0; // Years to seconds
		static constexpr double SMtoKG = 1.988435e30; // Solar masses to kilograms

		static constexpr double AUpYtoMpS = AUtoM / YtoS;//AUs per Year to Meters per Second
		static const double PI = 4 * atan(1.0);
		static const double G = 4 * PI*PI; // Gravitational constant is 4PI^2 in used units
	};
}


#endif