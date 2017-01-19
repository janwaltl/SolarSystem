#include "ReplayerMethod.h"

#include "Source/Exception.h"

namespace solar
{
	ReplayerSimMethod::ReplayerSimMethod(const std::string & inFileName) :
		inFile(inFileName)
	{
		in.open(inFile, std::ios::in | std::ios::binary);
		if (!in.is_open())
			throw Exception("Cannot open replay file: \'" + inFile + "\'.");

		//Read magic constant
		uint8_t magic[2];
		in.read(reinterpret_cast<char*>(magic), sizeof(magic));

		if (magic[0] != 'R' || magic[1] != 'E')
			throw Exception("Replay file: \'" + inFile + "\' does not contain a replay.");

		//Close for now
		in.close();
	}
	void ReplayerSimMethod::Prepare()
	{
		in.open(inFile, std::ios::in | std::ios::binary);
		if (!in.is_open())
			throw Exception("Cannot open replay file: \'" + inFile + "\'.");

		in.seekg(10);//Skip to multiplier
		in.read(reinterpret_cast<char*>(&multiplier), sizeof(multiplier));
		in.read(reinterpret_cast<char*>(&numRecords), sizeof(numRecords));
		in.seekg(22);//Skip to offset
		in.read(reinterpret_cast<char*>(&offset), sizeof(offset));

		this->SetDTMultiplier(this->GetDTMultiplier() / 1);
		//Initial units' pos&vels are set by parser
		//Keep file open for reading
	}
	void ReplayerSimMethod::operator()(double)
	{
		auto tmp = this->GetSimTime() / this->GetDtime() / double(multiplier);
		uint32_t recordNum = tmp;
		double lambda = tmp - recordNum;//For interpolation
		if (recordNum >= numRecords)//Stop at the end, obviously...
			recordNum = numRecords - 1;//Because 'recordNum' starts at zero and ends at 'numRecords-1'
		///Maybe even Pause

		//Reads record number 'recordNum' and the next one.
		//Then interpolates between them
		in.seekg(offset + recordNum * 32 * data->size());

		double posVel[4];//pos.x,pos.y,vel.x,vel.y

		for (size_t i = 0; i < data->size(); ++i)
		{
			in.read(reinterpret_cast<char*>(posVel), sizeof(posVel));
			(*data)[i].pos.x = (1 - lambda)*posVel[0];
			(*data)[i].pos.y = (1 - lambda)*posVel[1];
			(*data)[i].vel.x = (1 - lambda)*posVel[2];
			(*data)[i].vel.y = (1 - lambda)*posVel[3];
		}
		bool last = numRecords - recordNum == 1;
		for (size_t i = 0; i < data->size() && !last; ++i)
		{
			in.read(reinterpret_cast<char*>(posVel), sizeof(posVel));
			(*data)[i].pos.x += lambda*posVel[0];
			(*data)[i].pos.y += lambda*posVel[1];
			(*data)[i].vel.x += lambda*posVel[2];
			(*data)[i].vel.y += lambda*posVel[3];
		}

		//std::cout << recordNum << "/" << numRecords << "/" << lambda << std::endl;
	}
}
