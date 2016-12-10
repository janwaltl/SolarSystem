#ifndef OPENGLBACKEND_SHADER_HEADER
#define OPENGLBACKEND_SHADER_HEADER

#include <string>
#include <map>
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
			void Bind() const;
			void UnBind() const;

			void SetUniform2f(const std::string& name, const float &x, const float &y) const;
			void SetUniform4f(const std::string& name, const float &x, const float &y, const float &z, const float &w) const;

		private:
			void LoadUniforms();
			unsigned int programID;

			/* Map that holds all uniforms and their location
			Location is guaranteed not to change,
			so this way we do not need to look it up everytime*/
			std::map<std::string, unsigned int> uniforms;
		};
	}
}

#endif