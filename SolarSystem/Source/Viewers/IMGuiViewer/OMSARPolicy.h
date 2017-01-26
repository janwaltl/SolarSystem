#ifndef OFFSET_MOVE_SCALE_AR_POLICY_HEADER
#define OFFSET_MOVE_SCALE_AR_POLICY_HEADER

#include "Source/Math/Math.h"
#include "Source/Common/Common.h"
namespace solar
{
	//Policy that provides Offset-Move-Scale-AR behaviour to child class
	class OMSAR
	{
	public:
		OMSAR(double scaleFactor, double AR, Vec2 offset) :
			scaleFactor(scaleFactor), AR(AR), offset(offset)
		{
		}
		double ScaleFactor() { return scaleFactor; }
		void ScaleFactor(double newFactor) { scaleFactor = newFactor; }

		void Move(const Vec2 newOffset) { offset = newOffset; }
		Vec2 GetOffset() { return offset; }
		//Sets scaleFactor to such value, that all units' positions*scaleFactor falls into <-scale,scale> range
		void ResetZoom(const simData_t& data, double scale);

		double AspectRatio() { return AR; }
		void AspectRatio(double newAR) { AR = newAR; }
	private:
		double scaleFactor;
		Vec2 offset;
		double AR;
	};
}


#endif