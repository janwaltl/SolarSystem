#ifndef COMMON_TIMING_HEADER
#define COMMON_TIMING_HEADER

#include <chrono>

namespace solar
{
	using clock_t = std::chrono::steady_clock;
	using stepTime_t = clock_t::duration;

	//Structure used for storing simulated time
	//Splits time into whole seconds and fraction, thus achieving higher precision
	struct simulatedTime
	{
		using secsType = std::chrono::seconds;
		using fracType = std::chrono::steady_clock::duration;
		secsType seconds;
		fracType fraction;

		explicit simulatedTime(double secs = 0.0);
		bool operator<(const simulatedTime& right) const;
		bool operator>(const simulatedTime& right) const;
		bool operator==(const simulatedTime& right) const;
		simulatedTime& operator+=(const simulatedTime& right);
		simulatedTime& operator-=(const simulatedTime& right);
		double operator/=(double factor);
	};

	inline simulatedTime operator+(const simulatedTime& left, const simulatedTime& right)
	{
		simulatedTime temp(left);
		return temp += right;
	}

	inline simulatedTime operator-(const simulatedTime& left, const simulatedTime& right)
	{
		simulatedTime temp(left);
		return temp -= right;
	}
	inline double operator/(const simulatedTime& left, double factor)
	{
		return left.seconds.count() / factor + left.fraction.count() / simulatedTime::fracType::period::den / factor;
	}

	//Converts time duration to seconds
	template<typename Rep, typename Per>
	inline double ToSecs(const std::chrono::duration<Rep, Per>& time)
	{
		using type = std::chrono::duration<Rep, Per>;
		return  time.count() * type::period::num / double(type::period::den);
	}
	//Converts time duration to seconds
	inline double ToSecs(const simulatedTime& time)
	{
		return ToSecs(time.seconds) + ToSecs(time.fraction);
	}

	// Years,Days,Hours,Minutes,Seconds
	struct SplittedTime
	{
		uint32_t Y, D, H, M, S;
	};
	SplittedTime SplitTime(const simulatedTime& time);
	SplittedTime SplitTime(double seconds);
	SplittedTime SplitTime(uint32_t seconds);
}


#endif