#ifndef VIEWERS_IMGUI_VIEWER_HEADER
#define VIEWERS_IMGUI_VIEWER_HEADER

#include "Viewer.h"
#include "IMGuiViewer/OpenGLBackEnd.h"
#include "IMGuiViewer/ImGuiBackend.h"
struct GLFWwindow;

namespace solar
{
	class IMGuiViewer :public Viewer
	{
	public:
		// Properties of created window
		IMGuiViewer(int width, int height, const std::string& title = "Simulation", float circleSize = 0.01f,
					size_t circleRes = 32);
		~IMGuiViewer();

		void operator()(simData_t& data) override final;
		void Prepare(const simData_t& data) override final;
	private:
		// Returns such scale factor, which when applied on all positions, yields positions between <-1,1>
		double NormalizeData(const simData_t& data);
		OpenGLBackend openGL;	  //ORDER-DEPENDENT
		IMGuiBackend imguiBackend;//ORDER-DEPENDENT
		double scaleFactor;
	};
}


#endif