#ifndef OPENGL_SPHERE_234724901641_HEADER
#define OPENGL_SPHERE_234724901641_HEADER


namespace solar
{
	namespace openGL
	{
		//VAO buffer containing centered sphere of given resolution(number of rings) and radius
		class Sphere
		{
		public:
			Sphere(size_t resolution, float radius);
			~Sphere();
			//Needs appropiate shader with following inputs:
			//location=0 vec3 **position and also normals**
			void Draw();
		private:
			unsigned int VBO, VAO, IBO;
			size_t numIndices;
		};
	}
}


#endif