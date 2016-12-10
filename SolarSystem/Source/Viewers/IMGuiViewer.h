#ifndef VIEWERS_IMGUI_VIEWER_HEADER
#define VIEWERS_IMGUI_VIEWER_HEADER

#include "Viewer.h"

struct GLFWwindow;

namespace solar
{
	class IMGuiViewer :public Viewer
	{
	public:
		IMGuiViewer();
		~IMGuiViewer();

		void operator()(simData_t& data) override final;
		void Prepare(const simData_t& data) override final;
	private:
	};
}


#endif