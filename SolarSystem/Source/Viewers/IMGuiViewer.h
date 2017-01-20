#ifndef VIEWERS_IMGUI_VIEWER_HEADER
#define VIEWERS_IMGUI_VIEWER_HEADER

#include "Viewer.h"
#include "IMGuiViewer/OpenGLBackEnd.h"
#include "IMGuiViewer/ImGuiBackend.h"
#include "IMGuiViewer/Drawers/GUIDrawer.h"
#include "IMGuiViewer/Drawers/SimDataDrawer.h"
#include "IMGuiViewer/Drawers/LineTrailsDrawer.h"
#include "IMGuiViewer/OMSARPolicy.h"

namespace solar
{
	class IMGuiViewer :public Viewer, public OMSAR
	{
	public:
		// Properties of created window
		IMGuiViewer(size_t width = settings::win::size::x,
					size_t height = settings::win::size::y,
					const std::string& title = settings::win::title);
		~IMGuiViewer() = default;

		void Prepare() override final;
		void operator()() override final;

		//Gives access to drawer
		//-used mainly by other drawers(GUI)
		drawers::LineTrailsDrawer* GetTrailsDrawer();
	private:
		OpenGLBackend openGL;	  //ORDER-DEPENDENT
		IMGuiBackend imguiBackend;//ORDER-DEPENDENT
		std::unique_ptr<drawers::GUIDrawer> GUIDrawer;
		std::unique_ptr<drawers::SimDataDrawer> simDataDrawer;
		std::unique_ptr<drawers::LineTrailsDrawer> lineTrailsDrawer;
	};
}


#endif