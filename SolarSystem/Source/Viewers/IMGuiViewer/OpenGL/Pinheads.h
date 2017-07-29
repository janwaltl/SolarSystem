#ifndef OPENGL_PINHEADS_HEADER
#define OPENGL_PINHEADS_HEADER

#include <memory>
#include "Source/Units/Unit.h"
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
			enum plane :size_t//On which plane to project the pinheads
			{
				XY = 0, XZ = 1, YZ = 2,
			};
			Pinheads(const Camera& cam, size_t dataSize);
			~Pinheads();

			void Draw(const SimData& data, plane p, float planeOffset, const Vec2f& baseSize);
		private:
			std::unique_ptr<Shader>  CreateShader(const solar::Camera & cam, plane p);
			size_t dataSize;
			unsigned int VBO,VAO;
			//Shader to render the pinheads, indexed by plane enum
			std::unique_ptr<Shader> shaders[3];
		};
	}
}

#endif 