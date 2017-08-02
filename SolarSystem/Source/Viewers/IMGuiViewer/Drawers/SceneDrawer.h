#ifndef DRAWERS_SCENE_DRAWER_HEADER
#define DRAWERS_SCENE_DRAWER_HEADER

#include "Scene/GridDrawer.h"
#include "Scene/LineTrailsDrawer.h"
#include "Scene/SimDataDrawer.h"
#include "Scene/TestDrawer.h"
#include "Scene/Camera.h"
namespace solar
{
	class SimData;
	namespace drawers
	{
		//Renders 3D scene of simulated objects
		class SceneDrawer
		{
		public:
			SceneDrawer(const SimData& data);
			void Draw(const SimData& data);
			Camera& GetCam() { return cam; }
			LineTrailsDrawer& GetLineTrails() { return lineTrails; }
		private:
			Camera cam;//ORDER DEPENDENT - must before all the drawers
			GridDrawer grid;
			LineTrailsDrawer lineTrails;
			SimDataDrawer simData;
			TestDrawer test;
		};
	}
}

#endif