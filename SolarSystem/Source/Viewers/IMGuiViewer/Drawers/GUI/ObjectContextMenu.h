#ifndef GUI_OBJECT_CONTEXT_MENU_HEADER
#define GUI_OBJECT_CONTEXT_MENU_HEADER

#include "Source/Units/Unit.h"
namespace solar
{
	namespace drawers
	{
		class SceneDrawer;
		class SimDataDrawer;
	}
	namespace gui
	{
		//Shows names of hovered simulated objects and adds context menu when right clicked on objects.
		class ObjectContextMenu
		{
		public:
			ObjectContextMenu();
			void Draw(const SimData& data, const drawers::SceneDrawer& scene);
		private:
			bool open;
		};
	}
}

#endif