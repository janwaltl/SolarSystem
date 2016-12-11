#ifndef VIEWERS_IMGUI_VIEWER_HEADER
#define VIEWERS_IMGUI_VIEWER_HEADER

#include "Viewer.h"
#include "IMGuiViewer/OpenGLBackEnd.h"

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
		// Returns such scale factor, which when applied on all positions, yields positions between <-1,1>
		double NormalizeData(const simData_t& data);
		OpenGLBackend openGL;
		double scaleFactor;
	};
}


#endif