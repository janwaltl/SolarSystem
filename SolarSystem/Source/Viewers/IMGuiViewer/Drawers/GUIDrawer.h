#ifndef DRAWERS_GUIDRAWER_HEADER
#define DRAWERS_GUIDRAWER_HEADER


#include "GUI/UnitsProperties.h"
#include "GUI/CameraControls.h"
#include "GUI/Graphs.h"
#include "Source/Units/Unit.h"
#include "Source/Viewers/Viewer.h"
namespace solar
{
	namespace drawers
	{
		class SceneDrawer;
			//Renders User interface for simulation via ImGUI
		class GUIDrawer
		{
		public:
			void Draw(SimData& data,Viewer& viewer, SceneDrawer& scene, size_t w, size_t h);
		private:
			gui::UnitsProperties unitsProps;
			gui::CameraControls camControls;
			gui::Graphs graphs;
		};
	}
}


#endif