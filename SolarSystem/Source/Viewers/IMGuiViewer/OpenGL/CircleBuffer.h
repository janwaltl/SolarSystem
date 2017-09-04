#ifndef OPENGL_CIRCLE_BUFFER_HEADER
#define OPENGL_CIRCLE_BUFFER_HEADER

#include <cstddef>

namespace solar
{
	namespace openGL
	{
		//VAO buffer containing centered circle of given resolution and radius
		class CircleBuffer
		{
		public:
			CircleBuffer(size_t numVertices, float radius);
			~CircleBuffer();

			void Draw();
		private:
			void DeleteBuffers();
			unsigned int VBO, VAO, IBO;
			size_t numIndices;
		};
	}
}


#endif
