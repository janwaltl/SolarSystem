#ifndef DRAWERS_LINE_TRAILS_DRAWER_HEADER
#define DRAWERS_LINE_TRAILS_DRAWER_HEADER

#include "Drawer.h"
#include "Source/Viewers/IMGuiViewer/OpenGL/UnitTrail.h"

namespace solar
{
	namespace openGL
	{
		class Shader;
	}
	namespace drawers
	{
		//Draws line trails behind the simulated Units
		//Require GL context at the point of creation
		class LineTrailsDrawer :public Drawer
		{
		public:
			LineTrailsDrawer(IMGuiViewer* parent, simData_t* data);
			~LineTrailsDrawer();
			void Draw() override final;
			//Enables/disables simData[index] unit's trail
			// - disabling also clears the trail
			void SwitchTrail(size_t index, bool enable);
			//Enables/disables all units' trails
			void SwitchAll(bool enable);
			//Clears all units' trails
			void ClearAll();
			bool IsTrailEnabled(size_t index);
		private:
			void CreateShader();
			void CreateTrails();
			//Adds new points(from current units' positions) to trails every 'trailRes' frames
			void UpdateTrails();

			size_t frameCounter;
			
			std::unique_ptr<openGL::Shader> shader;
			std::vector<openGL::UnitTrail> trails;
			std::vector<bool> trailsControls;
			simData_t* simData;
		};
	}
}

#endif