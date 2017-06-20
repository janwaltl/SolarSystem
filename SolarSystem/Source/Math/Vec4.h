#ifndef MATH_VEC4_4257923752391_HEADER
#define MATH_VEC4_4257923752391_HEADER

#include "Common.h"

namespace solar
{
	template<typename T>
	class Vec4
	{
	public:
		T x, y, z, w;
		Vec4() { x = y = z = w = T(); }
		constexpr explicit Vec4(T x, T y = T(), T z = T(), T w = T()) :x(x), y(y), z(z), w(w) {}
		Vec4(const Vec4<T>&) = default;
		Vec4(Vec4&&) = default;
		Vec4<T>& operator=(const Vec4&) = default;
		Vec4<T>& operator=(Vec4&&) = default;
		~Vec4() = default;

		T LengthSq() const
		{
			return static_cast<T>(x*x + y*y + z*z + w*w);
		}
		T Length() const
		{
			return static_cast<T>(sqrt(LengthSq()));
		}
		Vec4<T>& SetLength(T length)
		{
			this->Normalize();
			(*this) *= length;
			return *this;
		}
		Vec4<T>& Normalize()
		{
			T length = Length();
			if (length < epsilon<T>)
				throw Exception("Cannot normalize zero-length vector.");
			else
			{
				x /= length;
				y /= length;
				z /= length;
				w /= length;
			}
			return *this;
		}
		Vec4<T>& operator+=(T val)
		{
			x += val;
			y += val;
			z += val;
			w += val;
			return *this;
		}
		Vec4<T>& operator-=(T val)
		{
			x -= val;
			y -= val;
			z -= val;
			w -= val;
			return *this;
		}
		Vec4<T>& operator*=(T val)
		{
			x *= val;
			y *= val;
			z *= val;
			w *= val;
			return *this;
		}
		Vec4<T>& operator/=(T val)
		{
			if (abs(val) < epsilon<T>)
				throw Exception("Cannot divide by zero.");
			x /= val;
			y /= val;
			z /= val;
			w /= val;
			return *this;
		}
		Vec4<T>& operator+=(const Vec4<T>& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
			return *this;
		}
		Vec4<T>& operator-=(const Vec4<T>& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
			return *this;
		}
		Vec4<T>& operator*=(const Vec4<T>& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			w *= other.w;
			return *this;
		}
		Vec4<T>& operator/=(const Vec4<T>& other)
		{
			if (abs(other.x) < epsilon<T> || abs(other.y) < epsilon<T> || abs(other.z) < epsilon<T> || abs(other.w) < epsilon<T>)
				throw Exception("Cannot divide by a vector containing zero element.");
			x /= other.x;
			y /= other.y;
			z /= other.z;
			w /= other.w;
			return *this;
		}
	};

	template<typename T>
	void swap(Vec4<T>& a, Vec4<T>& b) noexcept
	{
		using std::swap;
		swap(a.x, b.x);
		swap(a.y, b.y);
		swap(a.z, b.z);
		swap(a.w, b.w);
	}
	template<typename T>
	T DotProduct(const Vec4<T>& a, const Vec4<T>& b)
	{
		return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
	}
	template<typename T>
	Vec4<T> operator+(const Vec4<T>& a, const Vec4<T>& b)
	{
		Vec4<T> temp(a);
		return temp += b;
	}
	template<typename T>
	Vec4<T> operator-(const Vec4<T>& a, const Vec4<T>& b)
	{
		Vec4<T> temp(a);
		return temp -= b;
	}
	template<typename T>
	Vec4<T> operator*(const Vec4<T>& a, const Vec4<T>& b)
	{
		Vec4<T> temp(a);
		return temp *= b;
	}
	template<typename T>
	Vec4<T> operator/(const Vec4<T>& a, const Vec4<T>& b)
	{
		Vec4<T> temp(a);
		return temp /= b;
	}
	template<typename T>
	Vec4<T> operator+(const Vec4<T>& a, T b)
	{
		Vec4<T> temp(a);
		return temp += b;
	}
	template<typename T>
	Vec4<T> operator-(const Vec4<T>& a, T b)
	{
		Vec4<T> temp(a);
		return temp -= b;
	}
	template<typename T>
	Vec4<T> operator*(const Vec4<T>& a, T b)
	{
		Vec4<T> temp(a);
		return temp *= b;
	}
	template<typename T>
	Vec4<T> operator/(const Vec4<T>& a, T b)
	{
		Vec4<T> temp(a);
		return temp /= b;
	}
	template<typename T>
	Vec4<T> operator+(T a, const Vec4<T>& b)
	{
		return b + a;
	}
	template<typename T>
	Vec4<T> operator-(T a, const Vec4<T>& b)
	{
		return b - a;
	}
	template<typename T>
	Vec4<T> operator*(T a, const Vec4<T>& b)
	{
		return b*a;
	}
}
#endif
