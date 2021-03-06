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
			//With given scale (size of smaller grid) and offset in SimData units when camera is one unit away from its target
			//Returns scale in which the small grid has been drawn adjusted to camera's zoom level(also in same unit as SimData are)
			double Draw(const SimData& data, const Camera& cam, plane p, const double scale, double offset);
			//Return number of smaller squares in a big one
			size_t SmallToBig();
			bool ArePinHeadsEnabled() { return pinHeadsEnabled; }
			void SwitchPinHeads(bool enable) { pinHeadsEnabled = enable; }
		private:
			//offset is position relative to the camera
			void Draw(const std::unique_ptr<openGL::Grid>& grid, plane p, const float scale, const Vec4f& col, const Vec3f& offset, float fadeRange, size_t gridResolution);

			std::unique_ptr<openGL::Shader> CreateShader(const Camera& cam, plane p);
			std::unique_ptr<openGL::Grid> smallerGrid, biggerGrid;
			std::unique_ptr<openGL::Pinheads> pinheads;
			size_t smallToBig;
			float invSTB;
			size_t gridRes;
			//Indexed by 'plane' enum
			std::unique_ptr<openGL::Shader> shaders[3];
			bool pinHeadsEnabled;
		};
	}
}


#endif
