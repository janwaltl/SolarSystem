#ifndef SIMMETHODS_SEMIIMPLICITEULER_HEADER
#define SIMMETHODS_SEMIIMPLICITEULER_HEADER

#include "SimMethod.h"

class SemiImplicitEuler :public SimMethod
{
public:
	void operator()(simData_t& data, double step) override final;
private:
};


#endif