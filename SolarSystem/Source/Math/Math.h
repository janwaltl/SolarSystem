#ifndef MATH_32572305239_HEADER
#define MATH_32572305239_HEADER


#include <cmath>
#include "Vec2.h"
#include "Vec4.h"
namespace solar
{
	inline double lengthsq(const Vec2& vec)
	{
		return vec.x*vec.x + vec.y*vec.y;
	}
	inline double length(const Vec2& vec)
	{
		return sqrt(lengthsq(vec));
	}
	inline double distsq(const Vec2& left, const Vec2& right)
	{
		return lengthsq(left - right);
	}
	inline double dist(const Vec2& left, const Vec2& right)
	{
		return sqrt(distsq(left, right));
	}
}
#endif