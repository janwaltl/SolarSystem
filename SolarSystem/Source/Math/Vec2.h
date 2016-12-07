#ifndef MATH_VEC2_HEADER
#define MATH_VEC2_HEADER


//Two dimensional vector of doubles
struct Vec2
{
	Vec2(double x, double y) :x(x), y(y) {}
	Vec2() :Vec2(0.0, 0.0) {}
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
		return tmp ;
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

	double X() const { return x; }
	double Y() const { return y; }
	void X(double newX) { x = newX; }
	void Y(double newY) { y = newY; }
private:
	double x {}, y {};
};

template<typename T>
Vec2 operator*(Vec2 vec, T val)
{
	vec *= val;
	return vec;
}
template<typename T>
Vec2 operator*( T val, Vec2 vec)
{
	vec *= val;
	return vec;
}
#endif