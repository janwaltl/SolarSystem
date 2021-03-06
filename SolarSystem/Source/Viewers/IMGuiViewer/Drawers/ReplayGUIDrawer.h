#ifndef DRAWERS_REPLAY_GUIDRAWER_HEADER
#define DRAWERS_REPLAY_GUIDRAWER_HEADER


#include "GUIDrawer.h"
#include "GUI/ReplayControls.h"

namespace solar
{
	class ReplayerViewer;

	namespace drawers
	{
		class SceneDrawer;
		//Renders UserInface for replayed simulation
		class ReplayGUIDrawer
		{
		public:
			ReplayGUIDrawer(const std::string& replayFileName, const SimData& data);
			void Draw(SimData& data, ReplayerViewer& viewer, SceneDrawer& scene,
					  size_t w, size_t h);
		private:
			//Standard graphics interface
			drawers::GUIDrawer gui;
			gui::ReplayControls replayControls;
			
		};
	}
}

#endif