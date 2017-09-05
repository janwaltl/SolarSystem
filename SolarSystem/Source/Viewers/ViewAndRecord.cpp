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

		std::cout << "Created replay: " << outFile << "\n" << "Total run time: " << GetRunTimeSecs() << "s\n";
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

		uint32_t numUnits = data->Get().size();
		out.write(reinterpret_cast<char*>(&numUnits), sizeof(numUnits));

		double distUnits = data->GetPhysUnits().dist;
		double massUnits = data->GetPhysUnits().mass;
		double timeUnits = data->GetPhysUnits().time;
		out.write(reinterpret_cast<char*>(&distUnits), sizeof(distUnits));
		out.write(reinterpret_cast<char*>(&massUnits), sizeof(massUnits));
		out.write(reinterpret_cast<char*>(&timeUnits), sizeof(timeUnits));


		uint32_t offset = 0;//No offset yet
		auto offsetPos = out.tellp();
		out.write(reinterpret_cast<char*>(&offset), sizeof(offset));

		const auto& simData = data->Get();
		for (decltype(numUnits) i = 0; i < numUnits; ++i)
		{
			auto nameL = static_cast<uint8_t>(simData[i].name.length());
			out.write(reinterpret_cast<char*>(&nameL), sizeof(nameL));
			out.write(simData[i].name.c_str(), nameL * sizeof(std::string::value_type));//Do not encode '\'

			out.write(reinterpret_cast<const char*>(&simData[i].color.x), sizeof(simData[i].color.x));
			out.write(reinterpret_cast<const char*>(&simData[i].color.y), sizeof(simData[i].color.y));
			out.write(reinterpret_cast<const char*>(&simData[i].color.z), sizeof(simData[i].color.z));
			out.write(reinterpret_cast<const char*>(&simData[i].color.w), sizeof(simData[i].color.w));

			out.write(reinterpret_cast<const char*>(&simData[i].mass), sizeof(simData[i].mass));
			out.write(reinterpret_cast<const char*>(&simData[i].radius), sizeof(simData[i].radius));
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
		timeStep = simulatedTime(viewer->GetDtime()*viewer->GetDTMultiplier()*viewer->GetRawMultiplier());

		while (curSimTime - timeStep > simTime)
		{
			SavePosVel();
			simTime += timeStep;
		}

	}
	void ViewAndRecord::SavePosVel()
	{
		//This function works with replay files whose format is documented in FileFormats/ReplayerFile.txt

		const auto& simData = data->Get();
		for (size_t i = 0; i < data->Get().size(); ++i)
		{
			out.write(reinterpret_cast<const char*>(&simData[i].pos.x), sizeof(simData[i].pos.x));
			out.write(reinterpret_cast<const char*>(&simData[i].pos.y), sizeof(simData[i].pos.y));
			out.write(reinterpret_cast<const char*>(&simData[i].pos.z), sizeof(simData[i].pos.z));
			out.write(reinterpret_cast<const char*>(&simData[i].vel.x), sizeof(simData[i].vel.x));
			out.write(reinterpret_cast<const char*>(&simData[i].vel.y), sizeof(simData[i].vel.y));
			out.write(reinterpret_cast<const char*>(&simData[i].vel.z), sizeof(simData[i].vel.z));
		}
		++numRecords;
	}
}