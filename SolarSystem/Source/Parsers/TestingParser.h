#ifndef PARSERS_TESTING_PARSER_HEADER
#define PARSERS_TESTING_PARSER_HEADER

#include "Parser.h"
#include <iostream>

class TestingParser :public Parser
{
public:
	simData_t Load() override final
	{
		std::cout << "Creating hardcoded data - Earth, Sun.\n";
		Unit earth, sun;
		earth.pos = {149'597'871'000.0,0.0};
		earth.vel = 2 * PI / (365 * 24 * 3600) * earth.pos;//One rotation per year
		earth.mass = 5.9736e24;

		sun.mass = 332'950* earth.mass;//Sun is 332 950 times heavier than Earth
		return {earth,sun};
	}
	void Save(const simData_t& data) override final
	{
		std::cout << "Saving data.\n";
	}
};


#endif