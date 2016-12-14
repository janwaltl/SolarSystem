#ifndef OPENGLBACKEND_SHADER_HEADER
#define OPENGLBACKEND_SHADER_HEADER

#include <string>
#include <map>
#include "../../../Math/Math.h"

namespace solar
{
	namespace openGLBackend
	{
		//Manager for OpenGL's shader
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

			void SetUniform1i(const std::string& name, int i) const;
			void SetUniform2f(const std::string& name, float x, float y) const;
			void SetUniform2f(const std::string& name, const Vec2& vec) const;
			void SetUniform4f(const std::string& name, float x, float y, float z, float w) const;
			void SetUniform4f(const std::string& name, const Vec4& vec) const;
			void SetUniform4Mat(const std::string& name, const float mat[4][4]) const;
		private:
			//Obtains locations of shader's uniforms and stores them in the map
			void LoadUniforms();
			unsigned int programID;

			// Map that holds all uniforms and their locations
			//Location is guaranteed not to change,
			//so this way we do not need to look it up everytime
			std::map<std::string, unsigned int> uniforms;
		};
	}
}

#endif