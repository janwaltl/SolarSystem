#ifndef VIEWERS_EMPTY_VIEWER_HEADER
#define VIEWERS_EMPTY_VIEWER_HEADER


#include "Viewer.h"

namespace solar
{
	//Does nothing
	class EmptyViewer :public Viewer
	{
	public:
		void operator()() override final {}
	};
}
#endif