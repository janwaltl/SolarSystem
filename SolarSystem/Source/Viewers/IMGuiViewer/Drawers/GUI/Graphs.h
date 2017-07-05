#ifndef IMGUIVIEWER_GUI_GRAPHS_HEADER
#define IMGUIVIEWER_GUI_GRAPHS_HEADER


#include "Source/Units/Unit.h"
#include "Source/Math/Math.h"
#include "Source/Viewers/IMGuiViewer/IMGuiLibrary/imguiExtended.h"
#include <vector>
#include <memory>
namespace solar
{
	namespace gui
	{
		class Graphs
		{
		public:
			Graphs();
			void operator()(SimData& data);
			void Preview();
			void NewGraphPopUp();
			void ListOfGraphs();
		private:
			ImGuiE::LineGraph graph;
			bool showPreview;
			std::vector<std::unique_ptr<ImGuiE::LineGraph>> graphs;
			size_t selected;//Index to vector of graphs
		};

	}
}

#endif