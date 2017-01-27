#ifndef MATH_VEC2_3562362386348_HEADER
#define MATH_VEC2_3562362386348_HEADER

namespace solar
{
	//Two dimensional vector of doubles
	struct Vec2
	{
	public:
		double x {}, y {};

		explicit Vec2(double x = 0.0, double y = 0.0) :x(x), y(y) {}
		Vec2& operator+=(const Vec2& other)
		{
			this->x += other.x;
			this->y += other.y;
			return *this;
		}
		Vec2 operator+(const Vec2& other) const
		{
			Vec2 tmp = *this;
			tmp += other;
			return tmp;
		}

		Vec2& operator-=(const Vec2& other)
		{
			this->x -= other.x;
			this->y -= other.y;
			return *this;
		}
		Vec2 operator-(const Vec2& other) const
		{
			Vec2 tmp = *this;
			tmp -= other;
			return tmp;
		}
		template< typename T>
		Vec2& operator*=(T val)
		{
			this->x *= val;
			this->y *= val;
			return *this;
		}
	};

	template<typename T>
	Vec2 operator*(Vec2 vec, T val)
	{
		vec *= val;
		return vec;
	}
	template<typename T>
	Vec2 operator*(T val, Vec2 vec)
	{
		vec *= val;
		return vec;
	}
}
#endif