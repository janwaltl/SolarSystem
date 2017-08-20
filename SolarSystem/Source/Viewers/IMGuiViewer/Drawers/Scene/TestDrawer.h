#ifndef DRAWERS_TEST_DRAWER_HEADER
#define DRAWERS_TEST_DRAWER_HEADER

#include "Source/Math/Math.h"
#include "Source/Units/Unit.h"
#include <memory>
#include <vector>

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

			void Draw(const SimData& data);
		private:
			void CreateShader(const Camera& cam);
			void MakeSphere(const size_t &resolution, std::vector<solar::Vec3f> &vertices, std::vector<uint32_t> &indices);
			std::unique_ptr<openGL::Shader> shader;
			uint32_t VBO,IBO,VAO;
			size_t numIndices;//In sphere
		};
	}
}


#endif