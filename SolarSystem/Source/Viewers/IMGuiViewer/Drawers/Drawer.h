#ifndef DRAWER_324235325235_HEADER
#define DRAWER_324235325235_HEADER

#include "Source/Common/Common.h"

namespace solar
{
	class IMGuiViewer;

	namespace drawers
	{
		//Abstract class
		//Draws part of simulation, stored in ImGuiViewer
		class Drawer
		{
		public:
			Drawer(IMGuiViewer* parent) :viewer(parent) { assert(viewer); }
			virtual void Draw() = 0;
		protected:
			IMGuiViewer* viewer;
		};
	}
}


#endif