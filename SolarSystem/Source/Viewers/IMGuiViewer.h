#ifndef VIEWERS_IMGUI_VIEWER_HEADER
#define VIEWERS_IMGUI_VIEWER_HEADER

#include "Viewer.h"
#include "IMGuiViewer/OpenGLBackEnd.h"
#include "IMGuiViewer/ImGuiBackend.h"
#include "IMGuiViewer/Drawers/GUIDrawer.h"
#include "IMGuiViewer/Drawers/SceneDrawer.h"

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
	private:
		size_t w, h;
		OpenGLBackend openGL;	  //ORDER-DEPENDENT
		IMGuiBackend imguiBackend;//ORDER-DEPENDENT
		Camera cam;
		std::unique_ptr<drawers::GUIDrawer> GUI;
		std::unique_ptr<drawers::SceneDrawer> scene;
	};
}


#endif