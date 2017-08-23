#ifndef IMGUIVIEWER_GUI_VISUAL_PREFERENCES_HEADER
#define IMGUIVIEWER_GUI_VISUAL_PREFERENCES_HEADER


#include "Source/Units/Unit.h"

namespace solar
{
	namespace drawers
	{
		class LineTrailsDrawer;
		class SceneDrawer;
	}
	class Camera;
	namespace gui
	{
		//Shows settings for visual parts of simulation
		class VisualPreferences
		{
		public:
			void Draw(SimData& data, drawers::SceneDrawer& scene);
		private:
			void CameraControls(SimData& data, drawers::SceneDrawer & scene);
			void LineTrailsControls(drawers::LineTrailsDrawer & lineTrails);
			//Child Window containing checkoboxes for each unit offering to switch on/off the trails
			void LineTrailsPopUp(drawers::LineTrailsDrawer& lineTrails, const SimData& data);
			struct
			{
				int camType;
				int camTarget;
				int camPos;
				
				int gridPlane;
			} combo;
			struct
			{
				bool pinheads;
			}checkBox;
			float gridOffset;
			float minScreenRadius;//RealScale planets
		};
	}
}


#endif