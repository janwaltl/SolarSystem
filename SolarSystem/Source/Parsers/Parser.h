#ifndef PARSERS_PARSER_HEADER
#define PARSERS_PARSER_HEADER

#include "../Common/SystemUnit.h"

//Cares about input and output
class Parser : public SystemUnit
{
public:
	Parser();
	//Loads input from whethever
	virtual elements_t Load() = 0;
	//Save them after end of simulation
	virtual void Save(const elements_t& elements) {};
};


#endif