#ifndef MATH_VEC3_325723905794386_HEADER
#define MATH_VEC3_325723905794386_HEADER

#include "Common.h"

namespace solar
{
	template<typename T>
	class Vec3
	{
	public:
		T x, y, z;
		Vec3() { x = y = z = T(); }
		constexpr explicit Vec3(T x, T y = T(), T z = T()) :x(x), y(y), z(z) {}
		Vec3(const Vec3<T>&) = default;
		Vec3(Vec3&&) = default;
		Vec3<T>& operator=(const Vec3&) = default;
		Vec3<T>& operator=(Vec3&&) = default;
		~Vec3() = default;

		T LengthSq() const
		{
			return static_cast<T>(x*x + y*y + z*z);
		}
		T Length() const
		{
			return static_cast<T>(sqrt(LengthSq()));
		}
		Vec3<T>& SetLength(T length)
		{
			this->Normalize();
			(*this) *= length;
			return *this;
		}
		Vec3<T>& Normalize()
		{
			T length = Length();
			if (length < epsilon<T>)
				throw Exception("Cannot normalize zero-length vector.");
			else
			{
				x /= length;
				y /= length;
				z /= length;
			}
			return *this;
		}
		Vec3<T>& operator+=(T val)
		{
			x += val;
			y += val;
			z += val;
			return *this;
		}
		Vec3<T>& operator-=(T val)
		{
			x -= val;
			y -= val;
			z -= val;
			return *this;
		}
		Vec3<T>& operator*=(T val)
		{
			x *= val;
			y *= val;
			z *= val;
			return *this;
		}
		Vec3<T>& operator/=(T val)
		{
			if (abs(val) < epsilon<T>)
				throw Exception("Cannot divide by zero.");
			x /= val;
			y /= val;
			z /= val;
			return *this;
		}
		Vec3<T>& operator+=(const Vec3<T>& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}
		Vec3<T>& operator-=(const Vec3<T>& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}
		Vec3<T>& operator*=(const Vec3<T>& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
		}
		Vec3<T>& operator/=(const Vec3<T>& other)
		{
			if (abs(x) < epsilon<T> || abs(y) < epsilon<T> || abs(z) < epsilon<T>)
				throw Exception("Cannot divide by a vector containing zero element.");
			x /= other.x;
			y /= other.y;
			z /= other.z;
			return *this;
		}
	};

	template<typename T>
	void swap(Vec3<T>& a, Vec3<T>& b) noexcept
	{
		using std::swap;
		swap(a.x, b.x);
		swap(a.y, b.y);
		swap(a.z, b.z);
	}
	template<typename T>
	T DotProduct(const Vec3<T>& a, const Vec3<T>& b)
	{
		return a.x*b.x + a.y*b.y + a.z*b.z;
	}
	template<typename T>
	Vec3<T> CrossProduct(const Vec3<T>& a, const Vec3<T>& b)
	{
		return Vec3<T>(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
	}
	template<typename T>
	Vec3<T> operator+(const Vec3<T>& a, const Vec3<T>& b)
	{
		Vec3<T> temp(a);
		return temp += b;
	}
	template<typename T>
	Vec3<T> operator-(const Vec3<T>& a, const Vec3<T>& b)
	{
		Vec3<T> temp(a);
		return temp -= b;
	}
	template<typename T>
	Vec3<T> operator*(const Vec3<T>& a, const Vec3<T>& b)
	{
		Vec3<T> temp(a);
		return temp *= b;
	}
	template<typename T>
	Vec3<T> operator/(const Vec3<T>& a, const Vec3<T>& b)
	{
		Vec3<T> temp(a);
		return temp /= b;
	}

}


#endif