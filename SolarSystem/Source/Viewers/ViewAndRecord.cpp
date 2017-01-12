#include "Source/Simulation.h"
#include "ViewAndRecord.h"
#include "Source/Exception.h"

namespace solar
{
	ViewAndRecord::ViewAndRecord(const std::string& outFileName, std::unique_ptr<Viewer> viewer) :
		outFile(outFileName), viewer(std::move(viewer))
	{
		assert(viewer);
	}

	ViewAndRecord::~ViewAndRecord()
	{
		assert(out.is_open());
		//Write correct number of records
		out.seekp(14);//Offset in header(see fileFormat)
		out.write(reinterpret_cast<char*>(&numRecords), sizeof(numRecords));
		out.close();
	}
	void ViewAndRecord::Prepare()
	{
		out.open(outFile, std::ios::out | std::ios::binary | std::ios::trunc);
		if (!out.is_open())
			throw Exception("Cannot create replay file: \'" + outFile + "\'.");

		CreateHeader();
		//Keep file open for recording.

		//Because viewer has not been linked to simulation, but this class(also a viewer) has been.
		LinkUnitAndLinkedUnit(*this, *viewer);
		viewer->Prepare();
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
		out.write(reinterpret_cast<char*>(&multiplier), sizeof(multiplier));
		numRecords = 0;
		out.write(reinterpret_cast<char*>(&numRecords), sizeof(numRecords));

		uint32_t numUnits = data->size();
		out.write(reinterpret_cast<char*>(&numUnits), sizeof(numUnits));
		for (decltype(numUnits) i = 0; i < numUnits; ++i)
		{
			uint8_t nameL = (*data)[i].name.length();
			out.write(reinterpret_cast<char*>(&nameL), sizeof(nameL));
			out.write((*data)[i].name.c_str(), nameL * sizeof(std::string::value_type));//Do not encode '\'

			out.write(reinterpret_cast<char*>(&(*data)[i].color.x), sizeof((*data)[i].color.x));
			out.write(reinterpret_cast<char*>(&(*data)[i].color.y), sizeof((*data)[i].color.y));
			out.write(reinterpret_cast<char*>(&(*data)[i].color.z), sizeof((*data)[i].color.z));
			out.write(reinterpret_cast<char*>(&(*data)[i].color.w), sizeof((*data)[i].color.w));

			out.write(reinterpret_cast<char*>(&(*data)[i].mass), sizeof((*data)[i].mass));
		}
	}
	void ViewAndRecord::Record()
	{
		assert(out.is_open());
		for (size_t i = 0; i < data->size(); ++i)
		{
			out.write(reinterpret_cast<char*>(&(*data)[i].pos.x), sizeof((*data)[i].pos.x));
			out.write(reinterpret_cast<char*>(&(*data)[i].pos.y), sizeof((*data)[i].pos.y));
			out.write(reinterpret_cast<char*>(&(*data)[i].vel.x), sizeof((*data)[i].vel.x));
			out.write(reinterpret_cast<char*>(&(*data)[i].pos.y), sizeof((*data)[i].pos.y));
		}
		++numRecords;
	}
}