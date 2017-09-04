#ifndef IMGUIVIEWER_GUI_UNITS_PROPERTIES_HEADER
#define IMGUIVIEWER_GUI_UNITS_PROPERTIES_HEADER

#include "Source/Math/Math.h"
#include "Source/Units/Unit.h"

namespace solar
{
	class OMSAR;

	class Camera;
	namespace gui
	{
		//Window showing information about simulatedUnits.
		class UnitsProperties
		{
		public:
			explicit UnitsProperties();
			void Draw(SimData& data, Camera& cam);
		private:
			void UnitDetails(const size_t &i, Camera& cam, const Unit& object);
			void UnitColorPopUp(solar::SimData & data, const size_t &i);
			void ListHeader(solar::SimData & data);
			void SetFrameOfRef(solar::SimData & data);
			void SetMassUnits(solar::SimData & data);
			void SetLenUnits(solar::SimData & data);
			void SetTimeUnits(solar::SimData & data);
			static bool UnitNameGetter(void * data, int index, const char ** result);
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
