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
		secsType seconds;//store whole seconds
		fracType fraction;//Store fraction of seconds 

		explicit simulatedTime(double secs = 0.0)
		{
			seconds = secsType(secsType::rep(secs));
			secs -= seconds.count();
			fraction = fracType(fracType::rep(secs*fracType::period::den / fracType::period::num));
		}
		bool operator<(const simulatedTime& right) const
		{
			return seconds < right.seconds && fraction < right.fraction;
		}
		bool operator>(const simulatedTime& right) const
		{
			return right < *this;
		}
		simulatedTime& operator+=(const simulatedTime& right)
		{
			using namespace std::chrono_literals;
			seconds += right.seconds;
			fraction += right.fraction;
			if (fraction > 1s)
			{
				auto secs = std::chrono::duration_cast<secsType>(fraction);
				seconds += secs;
				fraction -= secs;
			}
			return *this;
		}
		simulatedTime& operator-=(const simulatedTime& right)
		{
			using namespace std::chrono_literals;
			seconds -= right.seconds;
			fraction -= right.fraction;
			if (fraction < fraction.zero())
			{
				seconds -= 1s;
				fraction += 1s;
			}
			return *this;
		}
		double operator/=(double factor)
		{
			return seconds.count() / factor + fraction.count() / fracType::period::den / factor;
		}
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

	inline double ToSecs(const simulatedTime& time)
	{
		return ToSecs(time.seconds) + ToSecs(time.fraction);
	}

	struct SplittedTime
	{
		//Years,Days,Hours,Minutes,Seconds
		uint32_t Y, D, H, M, S;
	};
	inline SplittedTime SplitTime(const simulatedTime& time)
	{
		SplittedTime tmp;
		auto secs = time.seconds.count();
		tmp.Y = static_cast<uint32_t>(secs / 31'536'000);
		tmp.D = static_cast<uint32_t>((secs % 31'536'000) / 86'400);
		tmp.H = static_cast<uint32_t>((secs % 86'400) / 3600);
		tmp.M = static_cast<uint32_t>((secs % 3600) / 60);
		tmp.S = static_cast<uint32_t>(secs % 60);
		return tmp;
	}
	inline SplittedTime SplitTime(double seconds)
	{
		assert(seconds >= 0.0);
		SplittedTime tmp;
		uint32_t secs = static_cast<uint32_t>(seconds);//Sufficient precisison for days and lower...
		tmp.Y = static_cast<uint32_t>(seconds / 31'536'000);
		tmp.D = static_cast<uint32_t>((secs % 31'536'000) / 86'400);
		tmp.H = static_cast<uint32_t>((secs % 86'400) / 3600);
		tmp.M = static_cast<uint32_t>((secs % 3600) / 60);
		tmp.S = static_cast<uint32_t>(secs % 60);
		return tmp;
	}
	inline SplittedTime SplitTime(uint32_t seconds)
	{
		assert(seconds >= 0.0);
		SplittedTime tmp;
		tmp.Y = static_cast<uint32_t>(seconds / 31'536'000);
		tmp.D = static_cast<uint32_t>((seconds % 31'536'000) / 86'400);
		tmp.H = static_cast<uint32_t>((seconds % 86'400) / 3600);
		tmp.M = static_cast<uint32_t>((seconds % 3600) / 60);
		tmp.S = static_cast<uint32_t>(seconds % 60);
		return tmp;
	}
}


#endif