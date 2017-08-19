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
			enum camType
			{
				perspective, ortho
			};
			using plane = GridDrawer::plane;
			//Initializes all drawers and sets both cameras, sets perspective as active one by default
			SceneDrawer(const SimData& data);
			void Draw(const SimData& data);
			Camera& GetActiveCam();
			const Camera& GetActiveCam() const;
			//If sync, then this function will call new camera's LookAt with old cam's arguments and copies its target
			// - Set as false to make cameras independent - i.e ortho can draw view pro top and perpsective can be used to fly around
			void SetActiveCam(camType cam, bool sync = true);
			LineTrailsDrawer& GetLineTrails() { return lineTrails; }
			SimDataDrawer& GetSimDataDrawer() { return simData; }
			const SimDataDrawer& GetSimDataDrawer() const { return simData; }
			GridDrawer& GetGrid() { return grid; }
			//Return scale of smaller grid in meters from last draw call
			PhysUnits::ratio GetGridScale();
			void SwitchGrid(bool enableGrid, bool enablePinHeads=true) { gridEnabled = enableGrid; grid.SwitchPinHeads(enablePinHeads); }
			bool ArePinHeadsEnabled() { return grid.ArePinHeadsEnabled(); }
			bool IsGridEnabled() { return gridEnabled; }
			void SetGridPlane(plane p) { gridPlane = p; }
			//Sets grid offset in woorld coordinates
			void SetGridOffset(float offset) { gridOffset = offset; }
			float GetGridOffset() { return gridOffset; }
			void SwitchLineTrails(bool enable) { lineTrails.SwitchAll(enable); }
		private:
			struct
			{
				PerspectiveCamera perspective;
				ScaledOrthoCamera scaledOrtho;
				//OrthographicCamera ortho;
			}camera;
			Camera* activeCamera;
			GridDrawer grid;
			LineTrailsDrawer lineTrails;
			SimDataDrawer simData;
			TestDrawer test;
			CameraControls camControls;

			PhysUnits::ratio gridScale;
			bool gridEnabled;
			GridDrawer::plane gridPlane;
			float gridOffset;
		};
	}
}

#endif