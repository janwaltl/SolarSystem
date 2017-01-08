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
		ViewAndRecord(const std::string& outFileName, someViewerArgs&&... args) :
			outFile(outFileName), viewer(std::forward<someViewerArgs...>(args...)) {}

		void Prepare() override final
		{
			//Because viewer has not been linked to simulation, but this class(also a viewer) has been.
			LinkUnitAndSim(viewer, *this->sim);
			viewer.Prepare();
		}
		void operator()() override final;
	private:
		someViewer viewer;
		std::string outFile;
		std::ofstream out;
	};
}
#endif