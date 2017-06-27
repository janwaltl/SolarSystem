#ifndef IMGUIVIEWER_GUI_UNITS_PROPERTIES_HEADER
#define IMGUIVIEWER_GUI_UNITS_PROPERTIES_HEADER

#include "Source/Math/Math.h"
#include "Source/Units/Unit.h"

namespace solar
{
	class OMSAR;

	namespace gui
	{
		//Window showing information about simulatedUnits.
		//Also offers support for following a selected unit if OMSAR is passed
		//	following = sets such offset to center selectedUnit
		class UnitsProperties
		{
		public:
			explicit UnitsProperties(Vec2d winPos = Vec2d(10, 310), Vec2d winSize = Vec2d(250, 390));
			void operator()(SimData& data);
			void SelectedUnitTextLabel(solar::SimData & data);
		private:
			// Returns char* string representing name of the selected Unit by its index in simData vector
			// Helper functions for combo box with units' names
			static bool UnitNameGetter(void* data, int index, const char** result);
			// index=0 is center system, index>0 gets simData(index-1) unit's name
			// Helper functions for combo box with reference units' names
			static bool RefUnitNameGetter(void* data, int index, const char** result);
			// Sets such offset to center selectedUnit on the screen
			void Follow(const SimData& data, OMSAR& follow);

			//Name of center reference system
			constexpr static char* centerSystem = "(0,0) coordinates";
			//Unit for (0,0) coordinates , created for convience and simplicity
			const static Unit centerRefSystem;
			//index of selected unit in data's vector
			int selectedUnit {0};
			//(index-1) of unit in data vector, relative to which are all values shown.
			// 0 is reserved for 0,0 coordiantes
			int selectedRefUnit {0};
			//whether the selectedUnit is being followed
			bool following {false};
			Vec2d winPos, winSize;
		};

	}
}


#endif