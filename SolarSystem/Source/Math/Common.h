#ifndef MATH_COMMON_3525209479124107_HEADER
#define MATH_COMMON_3525209479124107_HEADER

#include <cmath>
#include <cassert>
#include "Source/Common/Exception.h"

namespace solar
{
	template<typename T> constexpr T epsilon = T(1.0e-6);
	template<> constexpr double epsilon<double> = 0.0;
	template<typename T> constexpr T PI = T(3.1415926535);
	template<typename T> constexpr T G = T(6.67408e-11);
	template<typename T>
	T DegToRad(T degs)
	{
		return degs*PI<T> / T(180);
	}
	template<typename T>
	T RadToDeg(T rad)
	{
		return rad* T(180) / PI<T>;
	}
}


#endif