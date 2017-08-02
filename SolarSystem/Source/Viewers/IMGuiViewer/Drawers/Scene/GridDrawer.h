#ifndef DRAWERS_GRID_DRAWER_HEADER
#define DRAWERS_GRID_DRAWER_HEADER

#include <memory>
#include "Source/Math/Math.h"
#include "Source/Units/Unit.h"

namespace solar
{
	namespace openGL
	{
		class Grid;
		class Shader;
		class Pinheads;
	}
	class Camera;
	namespace drawers
	{
		//Needs OpenGL Context at creation
		//Class that draws dynamic grid at given plane and position
		class GridDrawer
		{
		public:
			enum plane :size_t
			{
				XY = 0, XZ = 1, YZ = 2,
			};
			//gridRes - number of gridSquare for bigger grid in each dimension, should be suffenciently large to cover whole screen
			//smallToBig - number of small grid's squares that fit into bigger grid's square 
			GridDrawer(const SimData& data, const Camera& cam, size_t gridRes, size_t smallToBig);
			~GridDrawer();
			//Draws the grid
			//In which plane should the grid be drawn
			//With given scale (size of bigger square) and offset in world coordinates
			void Draw(const SimData& data, const Camera& cam, plane p, const Vec2f& scale, float offset);
		private:
			void Draw(const std::unique_ptr<openGL::Grid>& grid, plane p, const Vec2f& scale, const Vec4f& col, const Vec3f& offset, float fadeRange, size_t gridResolution);

			std::unique_ptr<openGL::Shader> CreateShader(const Camera& cam, plane p);
			std::unique_ptr<openGL::Grid> smallerGrid, biggerGrid;
			std::unique_ptr<openGL::Pinheads> pinheads;
			size_t smallToBig;
			float invSTB;
			size_t gridRes;
			//Indexed by 'plane' enum
			std::unique_ptr<openGL::Shader> shaders[3];
		};
	}
}


#endif
