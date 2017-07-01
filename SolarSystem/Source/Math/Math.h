#ifndef MATH_32572305239_HEADER
#define MATH_32572305239_HEADER

#include <cmath>
#include "Common.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Mat4.h"

namespace solar
{
	using Vec2d = Vec2<double>;
	using Vec2f = Vec2<float>;
	using Vec2i = Vec2<int>;
	using Vec3d = Vec3<double>;
	using Vec3f = Vec3<float>;
	using Vec3i = Vec3<int>;
	using Vec4d = Vec4<double>;
	using Vec4f = Vec4<float>;
	using Vec4i = Vec4<int>;
	using Mat4d = Mat4<double>;
	using Mat4f = Mat4<float>;
	using Mat4i = Mat4<float>;

	template<typename T, typename J>
	auto operator*(const Mat4<T>& mat, const Vec4<J>& vec) -> Vec4<decltype(T()*J())>
	{
		Vec4<decltype(T()*J())> res;

		res.x = mat[0] * vec.x + mat[4] * vec.y + mat[8] * vec.z + mat[12] * vec.w;
		res.y = mat[1] * vec.x + mat[5] * vec.y + mat[9] * vec.z + mat[13] * vec.w;
		res.z = mat[2] * vec.x + mat[6] * vec.y + mat[10] * vec.z + mat[14] * vec.w;
		res.w = mat[3] * vec.x + mat[7] * vec.y + mat[11] * vec.z + mat[15] * vec.w;

		return res;
	}
}
#endif