#ifndef SIMMETHODS_RK4_HEADER
#define SIMMETHODS_RK4_HEADER

#include "SimMethod.h"

namespace solar
{
	class RK4 : public SimMethod
	{
	public:
		void operator()(simData_t& data,double step);
	};
}

#endif