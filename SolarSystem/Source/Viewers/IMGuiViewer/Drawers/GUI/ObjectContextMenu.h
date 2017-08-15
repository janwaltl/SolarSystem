#ifndef GUI_OBJECT_CONTEXT_MENU_HEADER
#define GUI_OBJECT_CONTEXT_MENU_HEADER

#include "Source/Units/Unit.h"
#include <map>
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
			ObjectContextMenu(const SimData& data);
			void Draw(const SimData& data, const drawers::SceneDrawer& scene);
		private:
			//Returns whether any object is hovered
			bool SetObjectsStates(const SimData& data, const drawers::SceneDrawer& scene);
			enum objectState
			{
				nothing, hovered, inContext
			};
			std::vector<objectState> objectsStates;
			Vec2f contextWinPos;
			bool contextWinOpened;
			
		};
	}
}

#endif