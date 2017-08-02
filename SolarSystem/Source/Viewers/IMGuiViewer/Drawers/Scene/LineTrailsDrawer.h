#ifndef DRAWERS_LINE_TRAILS_DRAWER_HEADER
#define DRAWERS_LINE_TRAILS_DRAWER_HEADER

#include <memory>
#include "Source/Units/Unit.h"
#include "Source/Math/Math.h"
#include "Source/Viewers/IMGuiViewer/OpenGL/UnitTrail.h"

namespace solar
{
	namespace openGL
	{
		class Shader;
	}
	
	class Camera;

	namespace drawers
	{
		//Draws line trails behind the simulated Units
		//Require GL context at the point of creation
		class LineTrailsDrawer
		{
		public:
			//Throws GLError on OpenGL error(i.e out of GPU memory)
			LineTrailsDrawer(size_t dataSize, const Camera& cam);
			~LineTrailsDrawer();
			void Draw(const SimData& data);
			//Enables/disables simData[index] unit's trail
			// - disabling also clears the trail
			void SwitchTrail(size_t index, bool enable);
			//Enables/disables all units' trails
			void SwitchAll(bool enable);
			//Clears all units' trails
			void ClearAll();
			bool IsTrailEnabled(size_t index);
		private:
			void CreateShader(const Camera& cam);
			void CreateTrails(size_t dataSize);
			//Adds new points(from current units' positions) to trails every 'trailRes' frames
			void UpdateTrails(const SimData& data);

			size_t frameCounter;

			std::unique_ptr<openGL::Shader> shader;
			std::vector<openGL::UnitTrail> trails;
			std::vector<bool> trailsControls;
		};
	}
}

#endif