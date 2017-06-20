#ifndef DRAWERS_SIMDATA_DRAWER_HEADER
#define DRAWERS_SIMDATA_DRAWER_HEADER

#include <memory>
#include "Source/Math/Math.h"
#include "Source/Units/Unit.h"

namespace solar
{
	namespace openGL
	{
		class Shader;
		class CircleBuffer;
	}
	namespace drawers
	{
		//Draws simulated units on screen
		//Require GL context at the point of creation
		class SimDataDrawer
		{
		public:
			SimDataDrawer(double aspectRatio);
			~SimDataDrawer();
			void Draw(const simData_t& data, double scaleFactor, const Vec2d& offset);
		private:
			void CreateShader(double aspectRatio);
			std::unique_ptr<openGL::Shader> shader;
			std::unique_ptr<openGL::CircleBuffer> circle;
		};
	}
}
#endif