#ifndef MATH_32572305239_HEADER
#define MATH_32572305239_HEADER


#include <cmath>
#include "Vec2.h"

inline double lengthsq(const Vec2& vec)
{
	return vec.X()*vec.X() + vec.Y()*vec.Y();
}
inline double length(const Vec2& vec)
{
	return sqrt(lengthsq(vec));
}
#endif