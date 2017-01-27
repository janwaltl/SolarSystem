#ifndef MATH_VEC4_4257923752391_HEADER
#define MATH_VEC4_4257923752391_HEADER

namespace solar
{
	//Four-dimensional vector of doubles
	struct Vec4
	{
	public:
		double x, y, z, w;

		constexpr explicit Vec4(double x = 0.0, double y = 0.0, double z = 0.0, double w = 0.0) :x(x), y(y), z(z), w(w) {}
		Vec4& operator+=(const Vec4& other)
		{
			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			this->w += other.w;
			return *this;
		}
		Vec4 operator+(const Vec4& other) const
		{
			Vec4 tmp = *this;
			tmp += other;
			return tmp;
		}

		Vec4& operator-=(const Vec4& other)
		{
			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
			this->w -= other.w;
			return *this;
		}
		Vec4 operator-(const Vec4& other) const
		{
			Vec4 tmp = *this;
			tmp -= other;
			return tmp;
		}
		template< typename T>
		Vec4& operator*=(T val)
		{
			this->x *= val;
			this->y *= val;
			this->z *= val;
			this->w *= val;
			return *this;
		}
	};

	template<typename T>
	Vec4 operator*(Vec4 vec, T val)
	{
		vec *= val;
		return vec;
	}
	template<typename T>
	Vec4 operator*(T val, Vec4 vec)
	{
		vec *= val;
		return vec;
	}
}
#endif
