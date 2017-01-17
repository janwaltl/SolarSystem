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

		in.seekg(22);//Skip to offset
		in.read(reinterpret_cast<char*>(&offset), sizeof(offset));
		//Initial units' pos&vels are set by parser

		//Keep file open for reading
	}
	void ReplayerSimMethod::operator()(double)
	{
		this->GetSimTime();
		size_t recordNum = this->GetRunTime() / this->GetDtime();

		in.seekg(offset + recordNum * 32);
		//Read vals...

		
	}
}
