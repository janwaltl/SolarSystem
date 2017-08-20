#ifndef OPENGL_GRID_4242351870_HEADER
#define OPENGL_GRID_4242351870_HEADER

#include <cstdint>
namespace solar
{
	namespace openGL
	{
		class Grid
		{
		public:
			//Number of lines in both axes
			//Needs OpenGL context created
			Grid(size_t gridRes);
			~Grid();

			void Draw() const;
		private:
			unsigned int VBO,VAO;
			size_t gridRes;
		};
	}
}
#endif
