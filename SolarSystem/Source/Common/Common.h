#ifndef COMMON_1345325_HEADER
#define COMMON_1345325_HEADER

#include <memory>
#include <vector>
#include <cassert>
#include "../Units/Unit.h"
#include <math.h>
namespace solar
{
	class Parser;
	class SimMethod;
	class Viewer;
	class Simulation;

	using simData_t = std::vector<Unit>;
	using parser_p = std::unique_ptr<Parser>;
	using simMethod_p = std::unique_ptr<SimMethod>;
	using viewer_p = std::unique_ptr<Viewer>;

	//Used physics units: AU,years,Sun's weight
	//Conversions between used units and SIUnits
	namespace physicsUnits
	{
		static constexpr long double AUtoM = 149'597'870'700.0; // Astronomical units to meters
		static constexpr long double YtoS = 31'536'000.0; // Years to seconds
		static constexpr long double SMtoKG = 1.988435e30; // Solar masses to kilograms

		static constexpr long double AUpYtoMpS = AUtoM / YtoS;//AUs per Year to Meters per Second
		static const long double PI = 4 * atanl(1.0);
		static const long double G = 4 * PI*PI; // Gravitational constant is 4PI^2 in used units
	};
}


#endif