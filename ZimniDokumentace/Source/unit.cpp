class Unit
{
public:
	Unit(const Vec2& pos, const Vec2&vel, double mass,
		 const Vec47 color, const std::string& name)
		:pos(pos), vel(vel), mass(mass), name(name), color(color)
	{}

	Vec2 pos, vel;
	double mass;
	Vec4 color;
	std::string name;
};