#ifndef VIEWERS_IMGUI_VIEWER_HEADER
#define VIEWERS_IMGUI_VIEWER_HEADER

#include "Viewer.h"
#include "IMGuiViewer/OpenGLBackEnd.h"
#include "IMGuiViewer/ImGuiBackend.h"
#include "IMGuiViewer/Drawers/GUIDrawer.h"
#include "IMGuiViewer/Drawers/SimDataDrawer.h"
#include "IMGuiViewer/Drawers/LineTrailsDrawer.h"

struct GLFWwindow;

namespace solar
{
	class IMGuiViewer :public Viewer
	{
	public:
		// Properties of created window
		IMGuiViewer(int width, int height, const std::string& title = "Simulation");
		~IMGuiViewer() = default;

		void operator()() override final;
		void Prepare() override final;

		double ScaleFactor();
		void ScaleFactor(double newFactor);

		void Move(const Vec2 newOffset);
		Vec2 GetOffset();
		// Zooms enough to see whole solar system
		// Sets such scale factor, which when applied on all positions, yields positions between <-1,1>
		void ResetZoom();

		double GetAspectRatio();
	private:
		OpenGLBackend openGL;	  //ORDER-DEPENDENT
		IMGuiBackend imguiBackend;//ORDER-DEPENDENT
		std::unique_ptr<drawers::GUIDrawer> GUIDrawer;
		std::unique_ptr<drawers::SimDataDrawer> simDataDrawer;
		std::unique_ptr<drawers::LineTrailsDrawer> lineTrailsDrawer;
		double scaleFactor;
		Vec2 offset;
		double AR;
	};
}


#endif