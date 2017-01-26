#ifndef IMGUIVIEWER_GUI_VISUALS_HEADER
#define IMGUIVIEWER_GUI_VISUALS_HEADER

#include "Source/Common/Common.h"
namespace solar
{
	namespace drawers
	{
		class LineTrailsDrawer;
	}
	namespace gui
	{
		//Draws window containing all visuals functions:
		//		- LineTrailsControls
		//		- LineTrailsBoxes
		void Visuals(drawers::LineTrailsDrawer& lineTrails, const simData_t& data);
		//Three buttons that offer controls about lineTrails
		//	-switch on/off or reset all trails
		void LineTrailsControls(drawers::LineTrailsDrawer& lineTrails);
		//Child Window containing checkoboxes for each unit offering to switch on/off the trails
		void LineTrailsBoxes(drawers::LineTrailsDrawer& lineTrails, const simData_t& data);
	}
}


#endif