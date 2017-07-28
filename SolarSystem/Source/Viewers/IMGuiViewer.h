#ifndef VIEWERS_IMGUI_VIEWER_HEADER
#define VIEWERS_IMGUI_VIEWER_HEADER

#include "Viewer.h"
#include "IMGuiViewer/OpenGLBackEnd.h"
#include "IMGuiViewer/ImGuiBackend.h"
#include "IMGuiViewer/Drawers/GUIDrawer.h"
#include "IMGuiViewer/Drawers/SimDataDrawer.h"
#include "IMGuiViewer/Drawers/LineTrailsDrawer.h"
#include "IMGuiViewer/Drawers/GridDrawer.h"
#include "IMGuiViewer/Camera.h"
//#include "IMGuiViewer/Drawers/TestDrawer.h"

namespace solar
{
	//Renders Graphical interface that allows controlling the simulation
	//Also shows 2D visualization of simulated data
	class IMGuiViewer :public Viewer
	{
	public:
		// Properties of created window
		IMGuiViewer(size_t width, size_t height, const std::string& title);
		~IMGuiViewer() = default;

		void Prepare() override final;
		void operator()() override final;

		void DrawGrid();

		Camera& GetCamera();
	private:
		size_t w, h;
		OpenGLBackend openGL;	  //ORDER-DEPENDENT
		IMGuiBackend imguiBackend;//ORDER-DEPENDENT
		Camera cam;
		std::unique_ptr<drawers::GUIDrawer> GUIDrawer;
		std::unique_ptr<drawers::SimDataDrawer> simDataDrawer;
		std::unique_ptr<drawers::GridDrawer> gridDrawer;
		//std::unique_ptr<drawers::TestDrawer> testDrawer;
		std::unique_ptr<drawers::LineTrailsDrawer> lineTrailsDrawer;
	};
}


#endif