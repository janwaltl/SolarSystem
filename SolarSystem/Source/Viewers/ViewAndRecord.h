#ifndef VIEWERS_VIEW_AND_RECORD_HEADER
#define VIEWERS_VIEW_AND_RECORD_HEADER

#include "Viewer.h"
#include <fstream>
namespace solar
{
	//Records simulation
	//-Wrap any viewer and pass it as ordinary viewer to simulation
	class ViewAndRecord : public Viewer
	{
	public:
		ViewAndRecord(const std::string& outFileName, std::unique_ptr<Viewer> viewer);
		~ViewAndRecord()override final;
		void Prepare() override final;
		void operator()() override final;
	private:
		void CreateHeader();
		void Record();
		std::unique_ptr<Viewer> viewer;
		std::string outFile;
		std::ofstream out;
		uint32_t numRecords;
	};
}
#endif