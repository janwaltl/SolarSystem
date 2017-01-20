#ifndef IMGUIVIEWER_GUI_SIM_PROPERTIES_HEADER
#define IMGUIVIEWER_GUI_SIM_PROPERTIES_HEADER


namespace solar
{
	class SystemUnit;

	namespace gui
	{
		//Window containing SimControls and SimMetrics
		void SimProperties(SystemUnit& sys);

		//Collection of buttons that allow control over simulation
		//	- resume/pause/step
		//	- change raw,DT speed
		void SimControls(SystemUnit& sys);

		//Two columns that show various information about current simulation
		void SimMetrics(const SystemUnit& sys);
	}
}


#endif