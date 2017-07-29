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
	//Floors given value to multiple of 'multiple' number
	//e.g floorToMult(9.2,10.0)=10.0
	//e.g floorToMult(9.2,100.0)=100.0
	//e.g floorToMult(9.2,5.0)=5.0
	//e.g floorToMult(9.2,0)=9.2 = UNCHANGED
	//e.g floorToMult(9.2,-10)=-10

	template<typename T>
	T floorToMult(T value, T multiple)
	{
		if (multiple == T(0))
			return value;
		else
			return std::floor(value / abs(multiple))*multiple;
	}
}


#endif