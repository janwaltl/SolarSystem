#ifndef UNITS_TYPES_HEADER
#define UNITS_TYPES_HEADER


#include <memory>

namespace solar
{
	class Parser;
	class SimMethod;
	class Viewer;

	using parser_p = std::unique_ptr<Parser>;
	using simMethod_p = std::unique_ptr<SimMethod>;
	using viewer_p = std::unique_ptr<Viewer>;

}

#endif