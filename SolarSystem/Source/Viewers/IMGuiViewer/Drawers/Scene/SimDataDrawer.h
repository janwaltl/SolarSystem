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
		//Draws simulated units on screen as spheres
		// Convention is that first unit is the star which lights all other objects
		///IMPROVE add type(planet/star...) to parser for precise control
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
			void SetMinScreenRadius(float newRadius) { minRadius = newRadius; }
			float GetMinScreenRadius() { return minRadius; }
			bool IsRealScaleEnabled() { return realScale; }
			void SetRealScale(bool enable) { realScale = enable; }
		private:
			void CreateShader(const Camera& cam);
			std::unique_ptr<openGL::Shader> shader;
			std::unique_ptr<openGL::Sphere> sphere;
			//
			bool realScale;
			float minRadius;
		};
	}
}
#endif