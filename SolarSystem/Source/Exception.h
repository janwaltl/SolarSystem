#ifndef EXCEPTION_523908571309_HEADER
#define EXCEPTION_523908571309_HEADER

#include <stdexcept>

//Exception class to use in project
class Exception : public std::runtime_error
{
	using std::runtime_error::runtime_error;
};

#endif