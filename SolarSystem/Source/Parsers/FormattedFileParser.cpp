#include "FormattedFileParser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Source/Common/Exception.h"
#include "Source/Units/PhysicsUnits.h"

namespace solar
{
	//To print unit in formatted way, valid only for this parser
	namespace
	{
		std::ostream& operator<< (std::ostream& out, const Unit& unit)
		{
			out << "name<" << unit.name << ">\n";
			out << "color<" << unit.color.x << " " << unit.color.y << " " << unit.color.z << " " << unit.color.w << ">\n";
			out << "position<" << unit.pos.x*physicsUnits::AUtoM << " " << unit.pos.y*physicsUnits::AUtoM << ">\n";
			out << "velocity<" << unit.vel.x*physicsUnits::AUpYtoMpS << " " << unit.vel.y*physicsUnits::AUpYtoMpS << ">\n";
			out << "mass<" << unit.mass*physicsUnits::SMtoKG << ">";//No new line

			return out;
		}
	}

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
		if (!outFileName.empty())
		{
			std::ofstream outputF(outFileName, std::ios::trunc | std::ios::out);
			if (!outputF.is_open())
				throw Exception("Output file \'" + outFileName + "\' could not be created/opened.");

			std::cout << "Saving simulated data to file: " << outFileName << '\n';
			for (const auto& unit : data)
			{
				outputF << "{ " << unit << "}\n\n";
			}
			std::cout << data.size() << " units saved.\n";
		}
	}

	Unit FormattedFileParser::ParseUnit(const std::string& str)
	{
		Unit unit;
		auto parse = [str = str, &unit = unit](auto fnc, const std::string& token) {
			try
			{
				fnc(unit, ParseToken(str, token));
			}
			catch (const std::logic_error& e)//std::stod threw out-of-range or invalid-argument
			{ throw Exception("Invalid Format: Unit's " + token + "could not be parsed, reason: " + std::string(e.what())); }
		};

		parse(ParsePosition, "position");
		parse(ParseVelocity, "velocity");
		parse(ParseMass, "mass");
		parse(ParseColor, "color");
		parse(ParseName, "name");

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
			unit.pos.x = std::stod(val, &pos) / physicsUnits::AUtoM;
			unit.pos.y = std::stod(val.substr(pos)) / physicsUnits::AUtoM;
		}
	}

	void FormattedFileParser::ParseVelocity(Unit & unit, const std::string & val)
	{
		if (!val.empty())
		{
			size_t pos {};
			unit.vel.x = std::stod(val, &pos) / physicsUnits::AUpYtoMpS;
			unit.vel.y = std::stod(val.substr(pos)) / physicsUnits::AUpYtoMpS;
		}
	}

	void FormattedFileParser::ParseMass(Unit & unit, const std::string & val)
	{
		if (!val.empty())
			unit.mass = std::stod(val) / physicsUnits::SMtoKG;//Convert to Suns masses
	}

	void FormattedFileParser::ParseColor(Unit & unit, const std::string & val)
	{
		if (!val.empty())
		{
			size_t pos {};
			unit.color.x = std::stod(val, &pos);
			std::string tmp = val.substr(pos);
			unit.color.y = std::stod(tmp, &pos);
			tmp = tmp.substr(pos);
			unit.color.z = std::stod(tmp, &pos);
			tmp = tmp.substr(pos);
			unit.color.w = std::stod(tmp);
		}
		else
			unit.color = Vec4 {1.0,1.0,1.0,1.0};
	}

	void FormattedFileParser::ParseName(Unit & unit, const std::string & val)
	{
		unit.name = val;
	}

}
