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
			struct
			{
				int camType;
				int camTarget;
				int camPos;
			} combo;
		};

		//Draws window containing all visuals functions:
		//		- LineTrailsControls
		//		- LineTrailsBoxes
		void Visuals(drawers::LineTrailsDrawer& lineTrails, const SimData& data, size_t w, size_t h);
		//Three buttons that offer controls about lineTrails
		//	-switch on/off or reset all trails
		void LineTrailsControls(drawers::LineTrailsDrawer& lineTrails);
		//Child Window containing checkoboxes for each unit offering to switch on/off the trails
		void LineTrailsBoxes(drawers::LineTrailsDrawer& lineTrails, const SimData& data);
	}
}


#endif