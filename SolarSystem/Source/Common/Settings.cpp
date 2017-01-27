#include "Settings.h"

namespace solar
{
	const size_t settings::lineTrail::maxLength = 40'000;
	const size_t settings::lineTrail::resolution = 10;
	const bool settings::lineTrail::enabledByDefault = true;

	const size_t settings::circleBuffer::resolution = 12;
	const float settings::circleBuffer::radius = 0.005f;
}