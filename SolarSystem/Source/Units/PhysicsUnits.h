#ifndef COMMON_1345325_HEADER
#define COMMON_1345325_HEADER


#include <cmath>

namespace solar
{
	/*
	//Multiplication of ratios is ratio
	//Multiplication with value is value
	class Ratio
	{
	public:
		using type = double;

		constexpr Ratio(type r = type(1.0)) :ratio(r) {}
		constexpr Ratio(const Ratio&) = default;
		constexpr Ratio(Ratio&&) = default;
		Ratio& operator=(const Ratio&) = default;
		Ratio& operator=(Ratio&&) = default;
		~Ratio() = default;

		void operator*=(const Ratio& r) { ratio *= r.ratio; }
		void operator/=(const Ratio& r) { ratio /= r.ratio; }
		//Apply ratio to value = post-multiplication
		template<typename U>
		auto Mult(const U& value) ->decltype(U()*type()) { return value*ratio; }
		template<typename U>
		auto Div(const U& value)  ->decltype(U() / type()) { return value / ratio; }
		type Get() { return ratio; }
	private:
		type ratio;
	};

	inline Ratio operator*(const Ratio& left, const Ratio& right)
	{
		auto res = left;
		res *= right;
		return res;
	}
	inline Ratio operator/(const Ratio& left, const Ratio& right)
	{
		auto res = left;
		res /= right;
		return res;
	}
	template<typename T>
	Ratio operator*(const Ratio& ratio, const T& val)
	{
		auto res = ratio;
		res.apply(val);
		return res;
	}
	template<typename T>
	Ratio operator*(const T& val, const Ratio& ratio)
	{
		auto res = ratio;
		res.Mult(val);
		return res;
	}
	template<typename T>
	Ratio operator/(const Ratio& ratio, const T& val)
	{
		auto res = ratio;
		res.Div(val);
		return res;
	}
	template<typename T>
	void operator *=(T& val, const Ratio& ratio)
	{
		val = ratio.Mult(val);
	}
	template<typename T>
	void operator /=(T& val, const Ratio& ratio)
	{
		val = ratio.Div(val);
	}
	template<typename T>
	Ratio
		pow(const Ratio& ratio, const T& val)
	{
		return pow(ratio.Get(), val);
	}*/

	class PhysUnits
	{
	public:
		//Cannot use num/den because there isn't enough precision(especially for mass)
		using ratio = double;
		//Time
		static constexpr ratio second {1.0};
		static constexpr ratio minute {60.0};
		static constexpr ratio hour {3'600.0};
		static constexpr ratio day {86'400.0};
		static constexpr ratio year {31'536'000.0};
		//Mass
		static constexpr ratio kilogram {1.0};
		static constexpr ratio earth {5.9722e24};
		static constexpr ratio sun {1.988435e30};
		//Distance
		static constexpr ratio meter {1.0};
		static constexpr ratio kilometer {1e3};
		static constexpr ratio AU {149'597'870'700.0};

		ratio mass, time, dist;
		PhysUnits(ratio mass = kilogram, ratio time = second, ratio dist = meter) :mass(mass), time(time), dist(dist) {}
	};
}


#endif