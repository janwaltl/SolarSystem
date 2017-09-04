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
				int camType = 0;
				int camTarget = 0;
				int camPos = 0;

				int gridPlane = 0;
			} combo;
			struct
			{
				bool pinheads = 0;
			}checkBox;
			float gridOffset = 0.0f;
			float minScreenRadius = 0.0f;//RealScale planets
		};
	}
}


#endif