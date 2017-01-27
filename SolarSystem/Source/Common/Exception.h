#ifndef EXCEPTION_523908571309_HEADER
#define EXCEPTION_523908571309_HEADER

#include <stdexcept>

namespace solar
{
	//Exception class used by this program
	//Derived from std::runtime_error(so from  std::exception too)
	class Exception : public std::runtime_error
	{
		using std::runtime_error::runtime_error;
	};

}

#endif