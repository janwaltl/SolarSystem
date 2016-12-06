#ifndef PARSERS_TESTING_PARSER_HEADER
#define PARSERS_TESTING_PARSER_HEADER

#include "Parser.h"
#include <iostream>
class TestingParser :public Parser
{
public:
	elements_t Load() override final
	{
		std::cout << "Loading elements.\n";
		return {{},{},{}};
	}
	void Save(const elements_t& elements) override final
	{
		std::cout << "Saving elements.\n";
	}
private:
};


#endif