#include "FormattedFileParser.h"
#include <fstream>
#include <sstream>
#include "../Exception.h"
FormattedFileParser::FormattedFileParser(const std::string & inputFileName, const std::string & outputFileName) :
	inFileName(inputFileName), outFileName(outputFileName)
{
}

simData_t FormattedFileParser::Load()
{
	std::ifstream inputF(inFileName);

	if (!inputF.is_open())
		throw Exception("Input file \'" + inFileName + "\' could not be opened.");

	std::stringstream buffer;

	buffer << inputF.rdbuf();
	inputF.close();

	input = buffer.str();
	buffer.clear();

	simData_t data;

	//Read first unit

	auto beg = input.find_first_of('{');
	if (beg == input.npos)
		return data;

	auto end = input.find_first_of('}', beg);
	if (end == input.npos)
		throw Exception("Invalid Format: Closing bracket has not been found.");

	data.emplace_back(ParseUnit(input.substr(beg + 1, end - beg - 1)));
	//If there is more
	beg = input.find_first_of('{', end);
	while (beg != input.npos)
	{
		//Find the end of it
		end = input.find_first_of('}', beg);
		if (end == input.npos)
			throw Exception("Invalid Format: Closing bracket has not been found.");
		data.emplace_back(ParseUnit(input.substr(beg + 1, end - beg - 1)));
		//Try to find another
		beg = input.find_first_of('{', end);
	}

	return data;
}

void FormattedFileParser::Save(const simData_t & data)
{
	std::ofstream outputF(outFileName, std::ios::trunc | std::ios::out);
	if (!outputF.is_open())
		throw Exception("Output file \'" + inFileName + "\' could not be created/opened.");

	for (const auto& unit : data)
	{
		outputF << "{ " << SerializeUnit(unit) << " }\n\n";
	}
}

Unit FormattedFileParser::ParseUnit(const std::string& str)
{
	Unit unit;
	//auto name = ParseToken(str, "name");
	try
	{
		ParsePosition(unit, ParseToken(str, "position"));
	}
	catch (const std::logic_error& e)//std::stod threw out-of-range or invalid-argument
	{ throw Exception("Invalid Format: Unit's position could not be parsed, reason: " + std::string(e.what())); }

	try
	{
		ParseVelocity(unit, ParseToken(str, "velocity"));
	}
	catch (const std::logic_error& e)//std::stod threw out-of-range or invalid-argument
	{ throw Exception("Invalid Format: Unit's velocity could not be parsed, reason: " + std::string(e.what())); }

	try
	{
		ParseMass(unit, ParseToken(str, "mass"));
	}
	catch (const std::logic_error& e)//std::stod threw out-of-range or invalid-argument
	{ throw Exception("Invalid Format: Unit's mass could not be parsed, reason: " + std::string(e.what())); }


	return unit;
}
std::string FormattedFileParser::ParseToken(const std::string&str, const std::string& token)
{
	auto tokenPos = str.find(token);
	if (tokenPos != str.npos)
	{
		auto beg = str.find_first_of('<', tokenPos);
		if (beg == str.npos)
			throw Exception("Invalid Format: Opening \'" + token + "\'-bracket has not been found.");

		auto end = str.find_first_of('>', beg);
		if (end == str.npos)
			throw Exception("Invalid Format: Closing \'" + token + "\'-bracket has not been found.");

		return str.substr(beg + 1, end - beg - 1);
	}
	return {};
}

void FormattedFileParser::ParsePosition(Unit & unit, const std::string & val)
{
	if (!val.empty())
	{
		size_t pos {};
		unit.pos.X(std::stod(val, &pos));
		unit.pos.Y(std::stod(val.substr(pos)));
	}
}

void FormattedFileParser::ParseVelocity(Unit & unit, const std::string & val)
{
	if (!val.empty())
	{
		size_t pos {};
		unit.vel.X(std::stod(val, &pos));
		unit.vel.Y(std::stod(val.substr(pos)));
	}
}

void FormattedFileParser::ParseMass(Unit & unit, const std::string & val)
{
	if (!val.empty())
		unit.mass = std::stod(val);
}

std::string FormattedFileParser::SerializeUnit(const Unit & unit)
{
	std::string str;

	//str += "name<" + unit.name + ">\n";
	str += "position<" + std::to_string(unit.pos.X()) + " " + std::to_string(unit.pos.Y()) + ">\n";
	str += "  velocity<" + std::to_string(unit.vel.X()) + " " + std::to_string(unit.vel.Y()) + ">\n";
	str += "  mass<" + std::to_string(unit.mass) + ">";

	return str;
}
