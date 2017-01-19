#ifndef VIEWERS_REPLAY_VIEWER_HEADER
#define VIEWERS_REPLAY_VIEWER_HEADER

#include "IMGuiViewer.h"
#include "IMGuiViewer/IMGuiLibrary/imgui.h"
#include <GLFW/glfw3.h>

namespace solar
{
	class ReplayerViewer : public Viewer
	{
	public:

		void Prepare() override final
		{
			LinkUnitAndLinkedUnit(*this, viewer);
			viewer.Prepare();
		}
		void operator()() override final
		{
			viewer();
		}
	private:
		IMGuiViewer viewer;
	};
}


#endif