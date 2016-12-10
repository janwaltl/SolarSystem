#ifndef OPENGLBACKEND_CIRCLE_BUFFER_HEADER
#define OPENGLBACKEND_CIRCLE_BUFFER_HEADER

namespace solar
{
	namespace openGLBackend
	{
		class CircleBuffer
		{
		public:
			CircleBuffer(size_t numVertices, float radius);
			~CircleBuffer();

			void Draw();
		private:
			unsigned int VBO, VAO, IBO;
			size_t numIndices;
		};
	}
}


#endif