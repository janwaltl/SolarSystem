#include "FormattedFileParser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Source/Common/Exception.h"
#include "Source/Units/PhysicsUnits.h"
#include "SerializeUnit.h"
namespace solar
{

	FormattedFileParser::FormattedFileParser(const std::string & inputFileName, const std::string & outputFileName) :
		inFileName(inputFileName), outFileName(outputFileName)
	{}

	SimData FormattedFileParser::Load()
	{
		//This function works with formatted text files whose format is documented in FileFormats/FormattedTextFile.txt

		std::ifstream inputF(inFileName);

		if (!inputF.is_open())
			throw Exception("Input file \'" + inFileName + "\' could not be opened.");

		std::stringstream buffer;

		buffer << inputF.rdbuf();
		inputF.close();

		input = buffer.str();
		buffer.clear();

		SimData data;
		//Read first unit
		auto beg = input.find_first_of('{');
		if (beg == input.npos)
			return data;

		auto end = input.find_first_of('}', beg);
		if (end == input.npos)
			throw Exception("Invalid Format: Closing bracket has not been found.");

		data->emplace_back(ParseUnit(input.substr(beg + 1, end - beg - 1)));
		//If there are more units
		beg = input.find_first_of('{', end);
		while (beg != input.npos)
		{
			//Find the ending bracket
			end = input.find_first_of('}', beg);
			if (end == input.npos)
				throw Exception("Invalid Format: Closing bracket has not been found.");

			data->emplace_back(ParseUnit(input.substr(beg + 1, end - beg - 1)));
			//Try to find another unit
			beg = input.find_first_of('{', end);
		}

		data.ConvertTo(PhysUnits::sun, PhysUnits::AU, PhysUnits::year);
		return data;
	}

	void FormattedFileParser::Save(const SimData & data)
	{
		//This function works with formatted text files whose format is documented in FileFormats/FormattedTextFile.txt

		if (!outFileName.empty())
		{
			std::ofstream outputF(outFileName, std::ios::trunc | std::ios::out);
			if (!outputF.is_open())
				throw Exception("Output file \'" + outFileName + "\' could not be created/opened.");

			std::cout << "Saving simulated data to file: " << outFileName << '\n';
			for (const auto& unit : data.Get())
				PrintUnit(outputF, unit, data.GetPhysUnits(), PhysUnits {PhysUnits::kilogram,PhysUnits::second,PhysUnits::meter});

			std::cout << data->size() << " units saved.\n";
		}
	}

	Unit FormattedFileParser::ParseUnit(const std::string& str)
	{
		//This function works with formatted text files whose format is documented in FileFormats/FormattedTextFile.txt

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
		parse(ParseRadius, "radius");

		return unit;
	}
	std::string FormattedFileParser::ParseToken(const std::string&str, const std::string& token)
	{
		//This function works with formatted text files whose format is documented in FileFormats/FormattedTextFile.txt

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
		//This function works with formatted text files whose format is documented in FileFormats/FormattedTextFile.txt

		if (!val.empty())
		{
			size_t pos1, pos2 {};
			unit.pos.x = std::stod(val, &pos1);
			unit.pos.y = std::stod(val.substr(pos1), &pos2);
			unit.pos.z = std::stod(val.substr(pos2 + pos1));
		}
	}

	void FormattedFileParser::ParseVelocity(Unit & unit, const std::string & val)
	{
		//This function works with formatted text files whose format is documented in FileFormats/FormattedTextFile.txt

		if (!val.empty())
		{
			size_t pos1, pos2 {};
			unit.vel.x = std::stod(val, &pos1);
			unit.vel.y = std::stod(val.substr(pos1), &pos2);
			unit.vel.z = std::stod(val.substr(pos2 + pos1));
		}
	}

	void FormattedFileParser::ParseMass(Unit & unit, const std::string & val)
	{
		//This function works with formatted text files whose format is documented in FileFormats/FormattedTextFile.txt

		if (!val.empty())
			unit.mass = std::stod(val);
	}
	void FormattedFileParser::ParseRadius(Unit & unit, const std::string & val)
	{
		//This function works with formatted text files whose format is documented in FileFormats/FormattedTextFile.txt

		if (!val.empty())
			unit.radius = std::stod(val);
		else
			unit.radius = 1.0;
	}
	void FormattedFileParser::ParseColor(Unit & unit, const std::string & val)
	{
		//This function works with formatted text files whose format is documented in FileFormats/FormattedTextFile.txt

		if (!val.empty())
		{
			size_t pos1, pos2, pos3 {};
			unit.color.x = std::stod(val, &pos1);
			unit.color.y = std::stod(val.substr(pos1), &pos2);
			unit.color.z = std::stod(val.substr(pos2 + pos1), &pos3);
			unit.color.w = std::stod(val.substr(pos1 + pos2 + pos3));
		}
		else
			unit.color = Vec4d {1.0,1.0,1.0,1.0};
	}

	void FormattedFileParser::ParseName(Unit & unit, const std::string & val)
	{
		//This function works with formatted text files whose format is documented in FileFormats/FormattedTextFile.txt

		unit.name = val;
	}

}
