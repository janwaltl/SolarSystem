#ifndef OPENGLBACKEND_SHADER_HEADER
#define OPENGLBACKEND_SHADER_HEADER

#include <string>

namespace solar
{
	namespace openGLBackend
	{
		class Shader
		{
		public:
			Shader(const std::string& vertexSource, const std::string& fragSource);
			//Moving can be enabled, but meh, not needed anyway
			Shader(const Shader& shader) = delete;
			Shader(Shader&& shader) = delete;
			Shader& operator=(const Shader& shader) = delete;
			Shader& operator=(Shader&& shader) = delete;

			~Shader();
			void Bind();
			void UnBind();
		private:
			unsigned int programID;
		};
	}
}

#endif