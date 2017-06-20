#ifndef MATH_COMMON_3525209479124107_HEADER
#define MATH_COMMON_3525209479124107_HEADER

#include <cmath>
#include <cassert>
#include "Source/Common/Exception.h"

namespace solar
{
	template<typename T> constexpr T epsilon = T(1.0e-6);
	template<typename T> constexpr T pi = T(3.1415926535);
}


#endif