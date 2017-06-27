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
	class Camera;
	namespace drawers
	{
		//Draws simulated units on screen
		//Require GL context at the point of creation
		class SimDataDrawer
		{
		public:
			SimDataDrawer(const Camera& cam);
			~SimDataDrawer();
			void Draw(const SimData& data);
		private:
			void CreateShader(const Camera& cam);
			std::unique_ptr<openGL::Shader> shader;
			std::unique_ptr<openGL::CircleBuffer> circle;
		};
	}
}
#endif