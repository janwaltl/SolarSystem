#include "Timing.h"
#include <cassert>

namespace solar
{
	simulatedTime::simulatedTime(double secs)
	{
		seconds = secsType(secsType::rep(secs));
		secs -= seconds.count();
		fraction = fracType(fracType::rep(secs*fracType::period::den / fracType::period::num));
	}
	bool simulatedTime::operator<(const simulatedTime & right) const
	{
		return seconds < right.seconds || (seconds == right.seconds && fraction < right.fraction);
	}
	bool simulatedTime::operator>(const simulatedTime & right) const
	{
		return right < *this;
	}
	bool simulatedTime::operator==(const simulatedTime & right) const
	{
		return seconds == right.seconds && fraction == right.fraction;
	}
	simulatedTime & simulatedTime::operator+=(const simulatedTime & right)
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
	simulatedTime & simulatedTime::operator-=(const simulatedTime & right)
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
	double simulatedTime::operator/=(double factor)
	{
		return seconds.count() / factor + fraction.count() / fracType::period::den / factor;
	}
	SplittedTime SplitTime(const simulatedTime & time)
	{
		return SplitTime(static_cast<uint64_t>(time.seconds.count()));
	}
	SplittedTime SplitTime(double seconds)
	{
		assert(seconds >= 0.0);
		return SplitTime(static_cast<uint64_t>(seconds));
	}
	SplittedTime SplitTime(uint64_t seconds)
	{
		SplittedTime tmp;
		tmp.Y = static_cast<uint32_t>(seconds / 31'536'000);
		tmp.D = static_cast<uint32_t>((seconds % 31'536'000) / 86'400);
		tmp.H = static_cast<uint32_t>((seconds % 86'400) / 3600);
		tmp.M = static_cast<uint32_t>((seconds % 3600) / 60);
		tmp.S = static_cast<uint32_t>(seconds % 60);
		return tmp;
	}
}

