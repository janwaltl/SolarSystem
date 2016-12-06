#ifndef MATH_VEC2_HEADER
#define MATH_VEC2_HEADER


//Two dimensional vector of doubles
struct Vec2
{
	//Default ctor is fine

	Vec2& operator+=(const Vec2& other)
	{
		this->x += other.x;
		this->y += other.y;
		return *this;
	}
	Vec2 operator+(const Vec2& other)
	{
		Vec2 tmp;
		return tmp += other;
	}

	Vec2& operator-=(const Vec2& other)
	{
		this->x -= other.x;
		this->y -= other.y;
		return *this;
	}
	Vec2 operator-(const Vec2& other)
	{
		Vec2 tmp;
		return tmp -= other;
	}
	template< typename T>
	Vec2& operator*=(T val)
	{
		this->x *= val;
		this->y *= val;
		return *this;
	}
	template<typename T>
	Vec2 operator*(T val)
	{
		Vec2 tmp;
		retun tmp *= val;
	}
private:
	double x {}, y {};
};


#endif