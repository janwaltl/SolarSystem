#include "ReplayerParser.h"
#include <fstream>
#include "Source/Common/Exception.h"

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
		in.close();//Close for now
	}

	SimData ReplayerParser::Load()
	{
		//This function works with replay files whose format is documented in FileFormats/ReplayerFile.txt

		std::ifstream in(inFile, std::ios::in | std::ios::binary);

		if (!in.is_open())//If someone managed to delete file between constructor and start of simulation
			throw Exception("Cannot open replay file: \'" + inFile + "\'.");

		//Throw on any Error, also EOF must not be reached
		in.exceptions(std::ifstream::failbit | std::ifstream::eofbit | std::ifstream::badbit);
		in.seekg(2);//Skip magic

		double deltaT = 0.0;
		in.read(reinterpret_cast<char*>(&deltaT), sizeof(deltaT));
		uint32_t multiplier = 0;
		in.read(reinterpret_cast<char*>(&multiplier), sizeof(multiplier));

		this->SetDTime(deltaT);
		this->SetDTMultiplier(multiplier);
		this->SetRawMultiplier(1);

		in.seekg(sizeof(uint32_t), std::ios::cur);//Skip numRecords

		uint32_t numUnits = 0;
		in.read(reinterpret_cast<char*>(&numUnits), sizeof(numUnits));

		in.seekg(sizeof(uint32_t), std::ios::cur);//Skip byteOffset


		SimData data;
		*data = SimData::units_t(numUnits, Unit());

		//Read units' properties
		for (decltype(numUnits) i = 0; i < numUnits; ++i)
		{
			uint8_t nameL;
			char name[256];
			in.read(reinterpret_cast<char*>(&nameL), sizeof(nameL));
			in.read(name, nameL * sizeof(name[0]));

			data.Get()[i].name = std::string(name, nameL);

			double color[4];
			in.read(reinterpret_cast<char*>(&color), sizeof(color));
			data.Get()[i].color = Vec4d(color[0], color[1], color[2], color[3]);

			double mass;
			in.read(reinterpret_cast<char*>(&mass), sizeof(mass));
			data.Get()[i].mass = mass;
		}

		//Initialize them to their initial value = first record
		for (decltype(numUnits) i = 0; i < numUnits; ++i)
		{
			//PosX, PosY, PosZ, VelX, VelY, VelZ
			double posVel[5];

			in.read(reinterpret_cast<char*>(&posVel), sizeof(posVel));
			data.Get()[i].pos.x = posVel[0];
			data.Get()[i].pos.y = posVel[1];
			data.Get()[i].pos.z = posVel[2];
			data.Get()[i].vel.x = posVel[3];
			data.Get()[i].vel.y = posVel[4];
			data.Get()[i].vel.z = posVel[5];
		}

		in.close();

		return data;
	}

}
