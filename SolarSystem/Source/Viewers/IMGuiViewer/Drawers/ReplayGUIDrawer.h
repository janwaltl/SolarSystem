#ifndef DRAWERS_REPLAY_GUIDRAWER_HEADER
#define DRAWERS_REPLAY_GUIDRAWER_HEADER


#include "GUI/UnitsProperties.h"
#include "GUI/ReplayControls.h"
#include "GUI/VisualPreferences.h"
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
			ReplayGUIDrawer(const std::string& replayFileName);
			void Draw(SimData& data, ReplayerViewer& viewer, SceneDrawer& scene,
					  size_t w, size_t h);
		private:
			gui::UnitsProperties unitsProps;
			gui::ReplayControls replayControls;
			gui::VisualPreferences visualPrefs;
		};
	}
}

#endif