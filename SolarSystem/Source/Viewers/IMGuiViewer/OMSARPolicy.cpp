#include "OMSARPolicy.h"
#include <numeric>
#include <algorithm>

void solar::OMSAR::ResetZoom(const simData_t& data, double scale)
{
	//Find the furthest position from center in all four directions
	
	Vec2 max {std::numeric_limits<double>::min(),std::numeric_limits<double>::min()};
	Vec2 min {std::numeric_limits<double>::max(),std::numeric_limits<double>::max()};
	
	auto find = [&](const Unit& u) {
		if (u.pos.x > max.x)
			max.x = u.pos.x;
		if (u.pos.y > max.y)
			max.y = u.pos.y;
		if (u.pos.x < min.x)
			min.x = u.pos.x;
		if (u.pos.y < min.y)
			min.y = u.pos.y;
	};
	std::for_each(data.begin(), data.end(), find);

	//Correct for aspect ratio
	max.y *= AR;
	min.y *= AR;

	auto maxL = length(max);
	auto minL = length(min);
	scaleFactor = 1.0 / (maxL > minL ? maxL : minL);
	scaleFactor *= scale;
}
