#ifndef MATH_32572305239_HEADER
#define MATH_32572305239_HEADER


#include <cmath>
#include "Vec2.h"

const double PI = 4 * atan(1.0);
const double G = 6.67191e-11;

inline double lengthsq(const Vec2& vec)
{
	return vec.X()*vec.X() + vec.Y()*vec.Y();
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
#endif