#ifndef VIEWERS_VIEW_AND_RECORD_HEADER
#define VIEWERS_VIEW_AND_RECORD_HEADER

#include "Viewer.h"
#include <fstream>
namespace solar
{
	//Records simulation
	// -Wrap any viewer and pass it as ordinary viewer to simulation
	template<typename someViewer>
	class ViewAndRecord : public Viewer
	{
	public:
		template<typename... someViewerArgs>
		ViewAndRecord(const std::string& outFileName, someViewerArgs&&... args);
		~ViewAndRecord()override final;
		void Prepare() override final;
		void operator()() override final;
	private:
		void CreateHeader();
		void Record();
		someViewer viewer;
		std::string outFile;
		std::ofstream out;
		uint32_t numRecords;
	};
}

#include "../Simulation.h"
namespace solar
{
	template<typename someViewer>
	template<typename... someViewerArgs>
	ViewAndRecord<someViewer>::ViewAndRecord(const std::string & outFileName, someViewerArgs && ...args) :
		outFile(outFileName), viewer(std::forward<someViewerArgs...>(args...))
	{
		//Tests if the file exists and can be opened
		out.open(outFileName, std::ios::out | std::ios::binary);
		if (!out.is_open())
			throw Exception("Cannot open replay file: \'" + replayFileName + "\'.");
		out.close();//Close it for now
	}

	template<typename someViewer>
	ViewAndRecord<someViewer>::~ViewAndRecord()
	{
		//Write correct number of records
		out.seekp(14);//Offset in header(see fileFormat)
		out.write(reinterpret_cast<char*>(&numRecords), sizeof(numRecords));
		out.close();
	}

	template<typename someViewer>
	void ViewAndRecord<someViewer>::Prepare()
	{
		//Because viewer has not been linked to simulation, but this class(also a viewer) has been.
		LinkUnitAndSim(viewer, *this->sim);
		viewer.Prepare();

		out.open(outFile, std::ios::out | std::ios::binary | std::ios::trunc);
		if (!out.is_open())
			throw Exception("Cannot create replay file: \'" + replayFileName + "\'.");

		CreateHeader();
		//Keep file open for recording.
	}
	template<typename someViewer>
	void ViewAndRecord<someViewer>::operator()()
	{
		viewer();
		Record();
	}
	template<typename someViewer>
	void ViewAndRecord<someViewer>::CreateHeader()
	{
		//This function works with replay files whose format is documented in FileFormats/ReplayerFile.txt

		assert(out.is_open());
		out.write("RE", 2);

		double deltaT = sim->GetDtime();
		out.write(reinterpret_cast<char*>(&deltaT), sizeof(deltaT));
		uint32_t multiplier = sim->GetDTMultiplier()*sim->GetRawMultiplier();
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

			out.write(reinterpret_cast<char*>(&(*data)[i].color.x, sizeof((*data)[i].color.x));
			out.write(reinterpret_cast<char*>(&(*data)[i].color.y, sizeof((*data)[i].color.y));
			out.write(reinterpret_cast<char*>(&(*data)[i].color.z, sizeof((*data)[i].color.z));
			out.write(reinterpret_cast<char*>(&(*data)[i].color.w, sizeof((*data)[i].color.w));

			out.write(reinterpret_cast<char*>((*data)[i].mass, sizeof((*data)[i].mass));
		}
	}
	template<typename someViewer>
	void ViewAndRecord<someViewer>::Record()
	{
		assert(out.is_open());
		for (size_t i = 0; i < data->size(); ++i)
		{
			out.write(reinterpret_cast<char*>(&(*data)[i].pos.x, sizeof((*data)[i].pos.x));
			out.write(reinterpret_cast<char*>(&(*data)[i].pos.y, sizeof((*data)[i].pos.y));
			out.write(reinterpret_cast<char*>(&(*data)[i].vel.x, sizeof((*data)[i].vel.x));
			out.write(reinterpret_cast<char*>(&(*data)[i].pos.y, sizeof((*data)[i].pos.y));
		}
		++numRecords;
	}
}

#endif