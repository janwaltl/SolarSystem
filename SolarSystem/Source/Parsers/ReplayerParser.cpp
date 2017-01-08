#include "ReplayerParser.h"
#include <fstream>
#include "Source/Exception.h"

namespace solar
{
	ReplayerParser::ReplayerParser(const std::string & replayFileName)
	{
		//This function works with replay files whose format is documented in FileFormats/ReplayerFile.txt

		std::ifstream in(replayFileName, std::ios::in | std::ios::binary);
		if (!in.is_open())
			throw Exception("Cannot open replay file: \'" + replayFileName + "\'.");

		//Read magic constant
		uint8_t magic[2];
		in.read(reinterpret_cast<char*>(magic), sizeof(magic));

		if (magic[0] != 'R' || magic[1] != 'E')
			throw Exception("Replay file: \'" + replayFileName + "\' does not contain a replay.");

		//Correct file
		inFile = replayFileName;
	}

	simData_t ReplayerParser::Load()
	{
		//This function works with replay files whose format is documented in FileFormats/ReplayerFile.txt

		std::ifstream in(inFile, std::ios::in | std::ios::binary);

		if (!in.is_open())//If someone managed to delete file between constructor and start of simulation
			throw Exception("Cannot open replay file: \'" + inFile + "\'.");

		//Throw on any Error, also EOF must not be reached
		in.exceptions(std::ifstream::failbit | std::ifstream::eofbit | std::ifstream::badbit);
		in.seekg(2);//Skip magic

		uint32_t numUnits = 0;
		in.read(reinterpret_cast<char*>(&numUnits), sizeof(numUnits));

		simData_t data (numUnits,Unit());

		for (decltype(numUnits) i = 0; i < numUnits; ++i)
		{
			uint8_t nameL;
			char name[256];
			in.read(reinterpret_cast<char*>(&nameL), sizeof(nameL));
			in.read(name, nameL * sizeof(name[0]));

			data[i].name = std::string(name, nameL);

			///Might use Vec4, but padding, constructors, C-casts....
			double color[4];
			in.read(reinterpret_cast<char*>(&color), sizeof(color));
			data[i].color = Vec4(color[0], color[1], color[2], color[3]);

			double mass;
			in.read(reinterpret_cast<char*>(&mass), sizeof(mass));
			data[i].mass = mass;
		}

		//Skip parameters and go to first time tick
		//in.seekg(XXX);

		//Initialize them to their intial value
		for (decltype(numUnits) i = 0; i < numUnits; ++i)
		{
			//PosX,PosY,VelX,VelY
			double posVel[4];

			in.read(reinterpret_cast<char*>(&posVel), sizeof(posVel));
			data[i].pos.x = posVel[0];
			data[i].pos.y = posVel[1];
			data[i].vel.x = posVel[2];
			data[i].vel.y = posVel[3];
		}
		return data;
	}

}
