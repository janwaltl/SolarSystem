#ifndef VIEWERS_REPLAY_VIEWER_HEADER
#define VIEWERS_REPLAY_VIEWER_HEADER

#include "IMGuiViewer.h"

namespace solar
{
	class ReplayerViewer : public Viewer
	{
	public:

		void Prepare() override final;
		void operator()() override final;
	private:
		IMGuiViewer viewer;
	};
}


#endif