#ifndef VIEWERS_IMGUI_VIEWER_HEADER
#define VIEWERS_IMGUI_VIEWER_HEADER

#include "Viewer.h"
#include "IMGuiViewer/OpenGLBackEnd.h"
#include "IMGuiViewer/ImGuiBackend.h"
#include "IMGuiViewer/GUIDrawer.h"
struct GLFWwindow;

namespace solar
{
	class IMGuiViewer :public Viewer
	{
	public:
		// Properties of created window
		IMGuiViewer(int width, int height, const std::string& title = "Simulation", float circleSize = 0.01f,
					size_t circleRes = 32);
		~IMGuiViewer() = default;

		void operator()(simData_t& data) override final;
		void Prepare(const simData_t& data) override final;

		double ScaleFactor();
		void ScaleFactor(double newFactor);

		void Move(const Vec2 newOffset);
		Vec2 GetOffset();
		// Zooms enough to see whole solar system
		// Sets such scale factor, which when applied on all positions, yields positions between <-1,1>
		void ResetZoom(const simData_t& data);
	private:
		OpenGLBackend openGL;	  //ORDER-DEPENDENT
		IMGuiBackend imguiBackend;//ORDER-DEPENDENT
		GUIDrawer gui;
		double scaleFactor;
		Vec2 offset;
	};
}


#endif