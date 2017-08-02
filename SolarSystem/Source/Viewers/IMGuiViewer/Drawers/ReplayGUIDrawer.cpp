#include "ReplayGUIDrawer.h"
#include "Source/Viewers/ReplayerViewer.h"
#include "GUI/Visuals.h"
#include "SceneDrawer.h"

namespace solar
{
	namespace drawers
	{
		ReplayGUIDrawer::ReplayGUIDrawer(const std::string& replayFileName) :
			unitsProps(Vec2d(10, 10)), replayControls(replayFileName)
		{}

		void ReplayGUIDrawer::Draw(SimData & data, ReplayerViewer & viewer, SceneDrawer& scene,
								   size_t w, size_t h)
		{
			camControls(scene.GetCam(), data);
			unitsProps(data);
			replayControls(viewer, &scene.GetLineTrails(), w, h);
			gui::Visuals(scene.GetLineTrails(), data, w, h);
		}
	}
}
