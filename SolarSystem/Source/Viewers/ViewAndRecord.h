#ifndef VIEWERS_VIEW_AND_RECORD_HEADER
#define VIEWERS_VIEW_AND_RECORD_HEADER

#include <fstream>
#include "Viewer.h"
#include "Source/Units/SystemUnitTypes.h"

namespace solar
{
	//Records simulation
	class ViewAndRecord : public Viewer
	{
	public:
		//Passed viewer will be used as ordinary viewer
		//Recorded simulation is saved in 'outFileName' replay file, including path and extension
		ViewAndRecord(const std::string& outFileName, viewer_p viewer);
		~ViewAndRecord()override final;
		void Prepare() override final;
		void operator()() override final;
	private:
		//Header of replay file
		void CreateHeader();
		//Makes record each DTime
		void Record();
		//Write position and velocity of each unit into file
		void SavePosVel();
		viewer_p viewer;
		std::string outFile;
		std::ofstream out;

		uint32_t numRecords;
		simulatedTime simTime;
		simulatedTime timeStep;
	};
}
#endif