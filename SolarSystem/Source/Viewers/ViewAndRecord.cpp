#include "ViewAndRecord.h"

#include <iostream>
#include "Source/Simulation.h"
#include "Source/Common/Exception.h"

namespace solar
{
	ViewAndRecord::ViewAndRecord(const std::string& outFileName, viewer_p viewer) :
		outFile(outFileName), viewer(std::move(viewer))
	{
		assert(this->viewer);
	}

	ViewAndRecord::~ViewAndRecord()
	{
		//This function works with replay files whose format is documented in FileFormats/ReplayerFile.txt

		assert(out.is_open());
		//Write correct number of records
		out.seekp(14);//Offset in header(see fileFormat)
		out.write(reinterpret_cast<char*>(&numRecords), sizeof(numRecords));
		out.close();

		std::cout << "Created replay: " << outFile << "\n" << "Total run time: " << GetRunTime() << "s\n";
		std::cout << "Total sim time: " << GetSimTimeSecs() << "s\n";
		std::cout << "Total number of records: " << numRecords << "\n";
	}
	void ViewAndRecord::Prepare()
	{
		//This function works with replay files whose format is documented in FileFormats/ReplayerFile.txt

		out.open(outFile, std::ios::out | std::ios::binary | std::ios::trunc);
		if (!out.is_open())
			throw Exception("Cannot create replay file: \'" + outFile + "\'.");

		//Save simulation's parametres
		CreateHeader();
		//Save initial positions and velocities of each unit
		SavePosVel();
		//Keep file open for recording.

		//Because viewer has not been linked to simulation(and needs to be), but this class(also a viewer) has been.
		LinkUnitAndLinkedUnit(*this, *viewer);
		viewer->Prepare();

		simTime = this->GetSimTime();
	}
	void ViewAndRecord::operator()()
	{
		(*viewer)();
		Record();
	}
	void ViewAndRecord::CreateHeader()
	{
		//This function works with replay files whose format is documented in FileFormats/ReplayerFile.txt

		assert(out.is_open());

		out.write("RE", 2);

		double deltaT = this->GetDtime();
		out.write(reinterpret_cast<char*>(&deltaT), sizeof(deltaT));
		uint32_t multiplier = this->GetDTMultiplier()*this->GetRawMultiplier();

		timeStep = simulatedTime(deltaT*multiplier);

		out.write(reinterpret_cast<char*>(&multiplier), sizeof(multiplier));
		numRecords = 0;
		out.write(reinterpret_cast<char*>(&numRecords), sizeof(numRecords));

		uint32_t numUnits = data->size();
		out.write(reinterpret_cast<char*>(&numUnits), sizeof(numUnits));

		uint32_t offset = 0;//No offset yet
		auto offsetPos = out.tellp();
		out.write(reinterpret_cast<char*>(&offset), sizeof(offset));

		for (decltype(numUnits) i = 0; i < numUnits; ++i)
		{
			auto nameL = static_cast<uint8_t>((*data)[i].name.length());
			out.write(reinterpret_cast<char*>(&nameL), sizeof(nameL));
			out.write((*data)[i].name.c_str(), nameL * sizeof(std::string::value_type));//Do not encode '\'

			out.write(reinterpret_cast<char*>(&(*data)[i].color.x), sizeof((*data)[i].color.x));
			out.write(reinterpret_cast<char*>(&(*data)[i].color.y), sizeof((*data)[i].color.y));
			out.write(reinterpret_cast<char*>(&(*data)[i].color.z), sizeof((*data)[i].color.z));
			out.write(reinterpret_cast<char*>(&(*data)[i].color.w), sizeof((*data)[i].color.w));

			out.write(reinterpret_cast<char*>(&(*data)[i].mass), sizeof((*data)[i].mass));
		}
		//Write offset now
		offset = static_cast<uint32_t>(out.tellp());
		out.seekp(offsetPos);
		out.write(reinterpret_cast<char*>(&offset), sizeof(offset));
		out.seekp(offset);//Return back

	}
	void ViewAndRecord::Record()
	{
		assert(out.is_open());
		//For each Dtime there should be simulated multiplier*dTime time.
		auto curSimTime = this->GetSimTime();
		while (curSimTime - timeStep > simTime)
		{
			SavePosVel();
			simTime += timeStep;
		}

	}
	void ViewAndRecord::SavePosVel()
	{
		//This function works with replay files whose format is documented in FileFormats/ReplayerFile.txt

		for (size_t i = 0; i < data->size(); ++i)
		{
			out.write(reinterpret_cast<char*>(&(*data)[i].pos.x), sizeof((*data)[i].pos.x));
			out.write(reinterpret_cast<char*>(&(*data)[i].pos.y), sizeof((*data)[i].pos.y));
			out.write(reinterpret_cast<char*>(&(*data)[i].vel.x), sizeof((*data)[i].vel.x));
			out.write(reinterpret_cast<char*>(&(*data)[i].vel.y), sizeof((*data)[i].vel.y));
		}
		++numRecords;
	}
}