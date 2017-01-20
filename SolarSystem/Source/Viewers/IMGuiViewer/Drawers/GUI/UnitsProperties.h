#ifndef IMGUIVIEWER_GUI_UNITS_PROPERTIES_HEADER
#define IMGUIVIEWER_GUI_UNITS_PROPERTIES_HEADER

#include "Source/Common/Common.h"
#include <Source/Viewers/IMGuiViewer/OMSARPolicy.h>
namespace solar
{
	class SystemUnit;

	namespace gui
	{

		//Window showing information about simulatedUnits.
		//Also offers support for following a selected unit if OMSAR is passed
		//	following = sets such offset to center selectedUnit
		class UnitsViewer
		{
		public:
			void operator()(simData_t& data, OMSAR* follow = nullptr);
		private:
			// Returns char* string representing name of the selected Unit by its index in simData vector
			// Helper functions for combo box with units' names
			static bool UnitNameGetter(void* data, int index, const char** result);
			// index=0 is center system, index>0 gets simData(index-1) unit's name
			// Helper functions for combo box with reference units' names
			static bool RefUnitNameGetter(void* data, int index, const char** result);

			void Follow(const simData_t& data, OMSAR& follow);
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
		};

	}
}


#endif