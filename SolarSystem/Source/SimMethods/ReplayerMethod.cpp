#include "ReplayerMethod.h"

#include "Source/Common/Exception.h"

namespace solar
{
	ReplayerSimMethod::ReplayerSimMethod(const std::string & inFileName) :
		inFile(inFileName)
	{
		//This function works with replay files whose format is documented in FileFormats/ReplayerFile.txt

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
		//This function works with replay files whose format is documented in FileFormats/ReplayerFile.txt

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
		//This function works with replay files whose format is documented in FileFormats/ReplayerFile.txt

		double tmp = this->GetSimTime() / this->GetDtime() / double(multiplier);
		uint32_t recordNum = static_cast<uint32_t>(tmp);
		double lambda = tmp - recordNum;//For interpolation
		if (recordNum >= numRecords)//Stop at the end, obviously...
			recordNum = numRecords - 1;//Because 'recordNum' starts at zero and ends at 'numRecords-1'

		double posVel[6];//pos.x,pos.y,pos.z,vel.x,vel.y,vel.z
		//Reads record number 'recordNum' and the next one.
		//Then interpolates between them
		in.seekg(offset + recordNum * sizeof(posVel) * data->Get().size());


		bool last = numRecords - recordNum == 1;
		if (last)//Do not interpolate on last record
			lambda = 0.0;

		for (size_t i = 0; i < data->Get().size(); ++i)
		{
			in.read(reinterpret_cast<char*>(posVel), sizeof(posVel));
			data->Get()[i].pos.x = (1 - lambda)*posVel[0];
			data->Get()[i].pos.y = (1 - lambda)*posVel[1];
			data->Get()[i].pos.z = (1 - lambda)*posVel[2];
			data->Get()[i].vel.x = (1 - lambda)*posVel[3];
			data->Get()[i].vel.y = (1 - lambda)*posVel[4];
			data->Get()[i].vel.z = (1 - lambda)*posVel[5];
		}
		for (size_t i = 0; !last && i < data->Get().size(); ++i)
		{
			in.read(reinterpret_cast<char*>(posVel), sizeof(posVel));
			data->Get()[i].pos.x = lambda*posVel[0];
			data->Get()[i].pos.y = lambda*posVel[1];
			data->Get()[i].pos.z = lambda*posVel[2];
			data->Get()[i].vel.x = lambda*posVel[3];
			data->Get()[i].vel.y = lambda*posVel[4];
			data->Get()[i].vel.z = lambda*posVel[5];
		}
	}
}
