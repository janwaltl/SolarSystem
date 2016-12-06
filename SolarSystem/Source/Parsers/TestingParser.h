#ifndef PARSERS_TESTING_PARSER_HEADER
#define PARSERS_TESTING_PARSER_HEADER

#include "Parser.h"
#include <iostream>
class TestingParser :public Parser
{
public:
	simData_t Load() override final
	{
		std::cout << "Loading data.\n";
		return {{},{},{}};
	}
	void Save(const simData_t& data) override final
	{
		std::cout << "Saving data.\n";
	}
private:
};


#endif