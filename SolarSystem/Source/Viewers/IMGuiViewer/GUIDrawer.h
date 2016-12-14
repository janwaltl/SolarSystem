#ifndef IMGUIVIEWER_GUIDRAWER_HEADER
#define IMGUIVIEWER_GUIDRAWER_HEADER


#include "../../Common/Common.h"

namespace solar
{
	//Renders User interface via ImGUI
	class GUIDrawer
	{
	public:
		void Draw(simData_t& data);
	};
}


#endif