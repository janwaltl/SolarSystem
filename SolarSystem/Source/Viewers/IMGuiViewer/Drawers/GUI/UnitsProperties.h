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
			explicit UnitsProperties(Vec2d winPos = Vec2d(10, 310), Vec2d winSize = Vec2d(500, 400));
			void operator()(SimData& data);
			void UnitDetails(const size_t &i);
			void UnitColorPopUp(solar::SimData & data, const size_t &i);
			void ListHeader(solar::SimData & data);
			void SetFrameOfRef(solar::SimData & data);
			void SetMassUnits(solar::SimData & data);
			void SetLenUnits(solar::SimData & data);
			void SetTimeUnits(solar::SimData & data);
		private:
			static bool UnitsProperties::UnitNameGetter(void * data, int index, const char ** result);
			Vec2d winPos, winSize;
			int frameOfRef;//0=center, 1+ index of Unit in SimData counting from 1
			int distPosCombo;//Distance=0, Position=1
			int speedVelCombo;//Speed=0, Velocity=1
			int timeCombo, lenCombo, massCombo;
			PhysUnits::ratio massRatio, timeRatio, lenRatio;
			char * massSuff, *timeSuff, *lenSuff;
			const static  Unit center;//Center of coordinate
			const Unit* refUnit;
			float tempCol[3];
		};

	}
}


#endif