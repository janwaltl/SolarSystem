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
		class Sphere;
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
			void Draw(const SimData& dataconst, const Camera & cam);
			//Returns [-1,1] screen position of passed object's center on the screen.
			Vec2f GetScreenPos(const Unit& object, const Camera& cam) const;
			//Returns radius of object (its circle or enclosing circle for other shapes) in screen coordinates.
			float GetScreenRadius(const Unit& object, const Camera& cam) const;
			void CreateShader(const Camera& cam);
			std::unique_ptr<openGL::Shader> shader;
			std::unique_ptr<openGL::Sphere> sphere;
		};
	}
}
#endif