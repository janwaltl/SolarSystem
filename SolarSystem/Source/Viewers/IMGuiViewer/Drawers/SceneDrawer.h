#ifndef DRAWERS_SCENE_DRAWER_HEADER
#define DRAWERS_SCENE_DRAWER_HEADER

#include "Scene/GridDrawer.h"
#include "Scene/LineTrailsDrawer.h"
#include "Scene/SimDataDrawer.h"
#include "Scene/TestDrawer.h"
#include "Scene/Camera.h"
#include "Scene/CameraControls.h"

namespace solar
{
	class SimData;
	namespace drawers
	{
		//Renders 3D scene of simulated objects
		//Needs OpenGL context when constructed.
		class SceneDrawer
		{
		public:
			SceneDrawer(const SimData& data);
			void Draw(const SimData& data);
			Camera& GetCam() { return cam; }
			const Camera& GetCam() const { return cam; }
			LineTrailsDrawer& GetLineTrails() { return lineTrails; }
			SimDataDrawer& GetSimDataDrawer() { return simData; }
			const SimDataDrawer& GetSimDataDrawer() const { return simData; }
			GridDrawer& GetGrid() { return grid; }
			//Return scale of smaller grid in meters from last draw call
			PhysUnits::ratio GetGridScale();
			void SwitchGrid(bool enable) { gridEnabled = enable; }
			bool IsGridEnabled() { return gridEnabled; }
			void SwitchLineTrails(bool enable) { lineTrails.SwitchAll(enable); }
		private:
			Camera cam;//ORDER DEPENDENT - must be before all the drawers
			GridDrawer grid;
			LineTrailsDrawer lineTrails;
			SimDataDrawer simData;
			TestDrawer test;
			CameraControls camControls;

			PhysUnits::ratio gridScale;
			bool gridEnabled;
		};
	}
}

#endif