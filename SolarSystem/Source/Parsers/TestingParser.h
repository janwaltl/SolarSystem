#ifndef PARSERS_TESTING_PARSER_HEADER
#define PARSERS_TESTING_PARSER_HEADER

#include "Parser.h"
#include <iostream>

class TestingParser :public Parser
{
public:
	simData_t Load() override final
	{
		std::cout << "Creating hardcoded data:\n";
		Unit earth, sun;
		/*earth.pos = {149'597'871'000.0,0.0};
		earth.vel = {0,2 * PI / (365 * 24 * 3600) * earth.pos.X()};//One rotation per year
		earth.mass = 5.9736e24;

		sun.mass = 332'950 * earth.mass;//Sun is 332 950 times heavier than Earth

		std::cout << "1. Earth: position(" << earth.pos.X() << ", " << earth.pos.Y() << ")\n";
		std::cout << "          velocity(" << earth.vel.X() << ", " << earth.vel.Y() << ")\n";
		std::cout << "          mass(" << earth.mass << ")\n";

		std::cout << "2. Sun: position(" << sun.pos.X() << ", " << sun.pos.Y() << ")\n";
		std::cout << "        velocity(" << sun.vel.X() << ", " << sun.vel.Y() << ")\n";
		std::cout << "        mass(" << sun.mass << ")\n";
		*/
		earth.pos = {1.0,0.0};
		earth.vel = {0.0,1.0};
		sun.pos = {0.0,0.0};
		sun.vel = {0.0,0.0};
		return {earth,sun};
	}
	void Save(const simData_t& data) override final
	{
		assert(data.size() == 2);
		auto& earth = data[0];
		auto& sun = data[1];
		std::cout << "1. Earth: position(" << earth.pos.X() << ", " << earth.pos.Y() << ")\n";
		std::cout << "          velocity(" << earth.vel.X() << ", " << earth.vel.Y() << ")\n";
		std::cout << "          mass(" << earth.mass << ")\n";

		std::cout << "2. Sun: position(" << sun.pos.X() << ", " << sun.pos.Y() << ")\n";
		std::cout << "        velocity(" << sun.vel.X() << ", " << sun.vel.Y() << ")\n";
		std::cout << "        mass(" << sun.mass << ")\n";
	}
};


#endif