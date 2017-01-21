#ifndef VIEWERS_REPLAY_VIEWER_HEADER
#define VIEWERS_REPLAY_VIEWER_HEADER

#include "Viewer.h"
#include "IMGuiViewer/OMSARPolicy.h"
#include "IMGuiViewer/OpenGLBackEnd.h"
#include "IMGuiViewer/ImGuiBackend.h"
#include "IMGuiViewer/Drawers/SimDataDrawer.h"
#include "IMGuiViewer/Drawers/LineTrailsDrawer.h"
#include "IMGuiViewer/Drawers/ReplayGUIDrawer.h"
namespace solar
{
	class ReplayerViewer : public Viewer,public OMSAR
	{
	public:
		ReplayerViewer(size_t width = settings::win::size::x,
					   size_t height = settings::win::size::y,
					   const std::string& title = settings::win::title);
		void Prepare() override final;
		void operator()() override final;
	private:
		OpenGLBackend openGL;	  //ORDER-DEPENDENT
		IMGuiBackend imguiBackend;//ORDER-DEPENDENT
		std::unique_ptr<drawers::ReplayGUIDrawer> replayGUIDrawer;
		std::unique_ptr<drawers::SimDataDrawer> simDataDrawer;
		std::unique_ptr<drawers::LineTrailsDrawer> lineTrailsDrawer;
	};
}


#endif