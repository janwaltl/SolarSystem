#include "Settings.h"

namespace solar
{
	const std::string settings::win::title = "Solar simulation";
	const size_t settings::win::pos::x = 5;
	const size_t settings::win::pos::y = 20;
	const size_t settings::win::size::x = 1200;
	const size_t settings::win::size::y = 700;

	const size_t settings::lineTrail::maxLength = 4'000;
	const size_t settings::lineTrail::resolution = 10;
	const bool settings::lineTrail::enabledByDefault = true;

	const size_t settings::circleBuffer::resolution = 12;
	const float settings::circleBuffer::radius = 0.005f;
}