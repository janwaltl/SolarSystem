#ifndef DRAWERS_TEST_DRAWER_HEADER
#define DRAWERS_TEST_DRAWER_HEADER

#include <memory>

namespace solar
{
	namespace openGL
	{
		class Shader;
	}
	class Camera;
	namespace drawers
	{
		//Draws a cube
		//Require GL context at the point of creation
		class TestDrawer
		{
		public:
			TestDrawer(const Camera& cam);
			~TestDrawer();

			void Draw();
		private:
			void CreateShader(const Camera& cam);
			std::unique_ptr<openGL::Shader> shader;
			uint32_t VBO,VAO;
		};
	}
}


#endif