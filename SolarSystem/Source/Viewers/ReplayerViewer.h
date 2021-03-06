#ifndef VIEWERS_REPLAY_VIEWER_HEADER
#define VIEWERS_REPLAY_VIEWER_HEADER

#include "Viewer.h"
#include "IMGuiViewer/OpenGLBackEnd.h"
#include "IMGuiViewer/ImGuiBackend.h"
#include "IMGuiViewer/Drawers/SceneDrawer.h"
#include "IMGuiViewer/Drawers/ReplayGUIDrawer.h"

namespace solar
{
	//Part of replaying a simulation mode.
	//Draws GUI that allows controlling the replaying of a replay
	class ReplayerViewer : public Viewer
	{
	public:
		// File including path and extension
		// Creates window with passed parametres
		ReplayerViewer(const std::string& replayFileName, size_t width, size_t height, const std::string& title);
		void Prepare() override final;
		void operator()() override final;
	private:
		size_t w, h;
		const std::string replayFileName;
		OpenGLBackend openGL;	  //ORDER-DEPENDENT
		IMGuiBackend imguiBackend;//ORDER-DEPENDENT

		std::unique_ptr<drawers::ReplayGUIDrawer> GUI;
		std::unique_ptr<drawers::SceneDrawer> scene;
	};
}


#endif