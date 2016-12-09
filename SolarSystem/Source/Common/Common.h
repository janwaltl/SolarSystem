#ifndef COMMON_1345325_HEADER
#define COMMON_1345325_HEADER

#include <memory>
#include <vector>
#include <cassert>
#include "../Units/Unit.h"

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
}


#endif