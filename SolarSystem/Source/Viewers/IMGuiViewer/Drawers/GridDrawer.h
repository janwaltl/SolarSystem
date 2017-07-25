#ifndef DRAWERS_GRID_DRAWER_HEADER
#define DRAWERS_GRID_DRAWER_HEADER

#include <memory>
#include "Source/Math/Math.h"

namespace solar
{
	namespace openGL
	{
		class Grid;
		class Shader;
	}
	class Camera;
	namespace drawers
	{
		class GridDrawer
		{
		public:
			enum plane :size_t
			{
				XY = 0, XZ = 1, YZ = 2,
			};
			GridDrawer(const Camera& cam, size_t gridRes);
			~GridDrawer();
			//In which plane should the grid be drawn
			void Draw(plane p, const Vec2f& scale, const Vec4f& col,float fade);
		private:
			std::unique_ptr<openGL::Shader> CreateShader(const Camera& cam, plane p, size_t gridRes);
			std::unique_ptr<openGL::Grid> grid;
			//Indexed by 'plane' enum
			std::unique_ptr<openGL::Shader> shaders[3];
		};
	}
}


#endif
