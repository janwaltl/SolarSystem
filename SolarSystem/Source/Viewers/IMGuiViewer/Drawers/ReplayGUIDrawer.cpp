#include "ReplayGUIDrawer.h"
#include "Source/Viewers/ReplayerViewer.h"
#include "GUI/VisualPreferences.h"
#include "SceneDrawer.h"

namespace solar
{
	namespace drawers
	{
		ReplayGUIDrawer::ReplayGUIDrawer(const std::string& replayFileName, const SimData& data) :
			gui(data), replayControls(replayFileName)
		{}

		void ReplayGUIDrawer::Draw(SimData & data, ReplayerViewer & viewer, SceneDrawer& scene,
								   size_t w, size_t h)
		{
			gui.Draw(data, viewer, scene, w, h);
			replayControls(viewer, &scene.GetLineTrails(), w, h);
		}
	}
}
