#ifndef OPENGL_PINHEADS_HEADER
#define OPENGL_PINHEADS_HEADER

#include <vector>
#include <memory>
#include "Source/Units/Unit.h"
#include "Source/Viewers/IMGuiViewer/Drawers/Scene/GridDrawer.h"
namespace solar
{
	class Camera;
	namespace openGL
	{
		class Shader;
		//Projects simulated data onto given plane and connects them via pinheads
		//Needs OpenGL context
		class Pinheads
		{
		public:
			using plane = drawers::GridDrawer::plane;
			Pinheads(const Camera& cam, size_t dataSize);
			~Pinheads();
			//Plane offset from origin in world coordinates
			void Draw(const SimData& data, plane p, double planeOffset, float baseSize, const Camera& cam);
		private:
			std::unique_ptr<Shader>  CreateShader(const solar::Camera & cam, drawers::GridDrawer::plane p);
			//Calculates plane offset relative to the camera
			float CalcRelPlaneOffset(const solar::Camera & cam, drawers::GridDrawer::plane p, double planeOffset);
			unsigned int VBO, VAO;
			size_t dataSize;
			//Positions of the objects relative to the camera
			std::vector<Vec3f> dataPos;
			//Shader to render the pinheads, indexed by plane enum
			std::unique_ptr<Shader> shaders[3];
		};
	}
}

#endif 