#ifndef MATH_Vec2d_3562362386348_HEADER
#define MATH_Vec2d_3562362386348_HEADER

#include "Common.h"

namespace solar
{
	//Two dimensional vector of doubles
	template<typename T>
	class Vec2
	{
	public:
		T x, y;
		Vec2() { x = y = T(); }
		constexpr explicit Vec2(T x, T y = T()) :x(x), y(y) {}
		Vec2(const Vec2<T>&) = default;
		Vec2(Vec2&&) = default;
		Vec2<T>& operator=(const Vec2&) = default;
		Vec2<T>& operator=(Vec2&&) = default;
		~Vec2() = default;

		T LengthSq() const
		{
			return static_cast<T>(x*x + y*y);
		}
		T Length() const
		{
			return static_cast<T>(sqrt(LengthSq()));
		}
		Vec2<T>& SetLength(T length)
		{
			this->Normalize();
			(*this) *= length;
			return *this;
		}
		Vec2<T>& Normalize()
		{
			T length = Length();
			if (length < epsilon<T>)
				throw Exception("Cannot normalize zero-length vector.");
			else
			{
				x /= length;
				y /= length;
			}
			return *this;
		}
		Vec2<T>& operator+=(T val)
		{
			x += val;
			y += val;
			return *this;
		}
		Vec2<T>& operator-=(T val)
		{
			x -= val;
			y -= val;
			return *this;
		}
		Vec2<T>& operator*=(T val)
		{
			x *= val;
			y *= val;
			return *this;
		}
		Vec2<T>& operator/=(T val)
		{
			if (std::abs(val) < epsilon<T>)
				throw Exception("Cannot divide by zero.");
			x /= val;
			y /= val;
			return *this;
		}
		Vec2<T>& operator+=(const Vec2<T>& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}
		Vec2<T>& operator-=(const Vec2<T>& other)
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}
		Vec2<T>& operator*=(const Vec2<T>& other)
		{
			x *= other.x;
			y *= other.y;
			return *this;
		}
		Vec2<T>& operator/=(const Vec2<T>& other)
		{
			if (std::abs(other.x) < epsilon<T> || std::abs(other.y) < epsilon<T>)
				throw Exception("Cannot divide by a vector containing zero element.");
			x /= other.x;
			y /= other.y;

			return *this;
		}
		template<typename U>
		explicit operator Vec2<U>() const
		{
			return Vec2<U>(static_cast<U>(x), static_cast<U>(y));
		}
	};

	template<typename T>
	void swap(Vec2<T>& a, Vec2<T>& b) noexcept
	{
		using std::swap;
		swap(a.x, b.x);
		swap(a.y, b.y);
	}
	template<typename T>
	T DotProduct(const Vec2<T>& a, const Vec2<T>& b)
	{
		return a.x*b.x + a.y*b.y;
	}
	template<typename T>
	Vec2<T> operator+(const Vec2<T>& a, const Vec2<T>& b)
	{
		Vec2<T> temp(a);
		return temp += b;
	}
	template<typename T>
	Vec2<T> operator-(const Vec2<T>& a, const Vec2<T>& b)
	{
		Vec2<T> temp(a);
		return temp -= b;
	}
	template<typename T>
	Vec2<T> operator*(const Vec2<T>& a, const Vec2<T>& b)
	{
		Vec2<T> temp(a);
		return temp *= b;
	}
	template<typename T>
	Vec2<T> operator/(const Vec2<T>& a, const Vec2<T>& b)
	{
		Vec2<T> temp(a);
		return temp /= b;
	}
	template<typename T>
	Vec2<T> operator+(const Vec2<T>& a, T b)
	{
		Vec2<T> temp(a);
		return temp += b;
	}
	template<typename T>
	Vec2<T> operator-(const Vec2<T>& a, T b)
	{
		Vec2<T> temp(a);
		return temp -= b;
	}
	template<typename T>
	Vec2<T> operator*(const Vec2<T>& a, T b)
	{
		Vec2<T> temp(a);
		return temp *= b;
	}
	template<typename T>
	Vec2<T> operator/(const Vec2<T>& a, T b)
	{
		Vec2<T> temp(a);
		return temp /= b;
	}
	template<typename T>
	Vec2<T> operator+(T a, const Vec2<T>& b)
	{
		return b + a;
	}
	template<typename T>
	Vec2<T> operator-(T a, const Vec2<T>& b)
	{
		return b - a;
	}
	template<typename T>
	Vec2<T> operator*(T a, const Vec2<T>& b)
	{
		return b*a;
	}
}
#endif
