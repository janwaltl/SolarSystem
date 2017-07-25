#ifndef OPENGLBACKEND_SHADER_HEADER
#define OPENGLBACKEND_SHADER_HEADER

#include <string>
#include <map>
#include "Source/Math/Math.h"

namespace solar
{
	namespace openGL
	{
		//Wrapper for OpenGL's shader
		class Shader
		{
		public:
			Shader(const std::string& vertexSource, const std::string& fragSource);
			//Moving can be enabled,but not needed anyway
			//Either way, just swap programIDs and uniforms
			Shader(const Shader& shader) = delete;
			Shader(Shader&& shader) = delete;
			Shader& operator=(const Shader& shader) = delete;
			Shader& operator=(Shader&& shader) = delete;

			~Shader();

			void Bind() const;
			void UnBind() const;
			void SetUniform1f(const std::string& name, float x) const;
			void SetUniform1i(const std::string& name, int i) const;
			void SetUniform2f(const std::string& name, float x, float y) const;
			//Still sets vec2 of floats, just less casting
			void SetUniform2f(const std::string& name, const Vec2d& vec) const;
			void SetUniform2f(const std::string& name, const Vec2f& vec) const;
			void SetUniform3f(const std::string& name, float x, float y, float z) const;
			//Still sets vec3 of floats, just less casting
			void SetUniform3f(const std::string& name, const Vec3d& vec) const;
			void SetUniform3f(const std::string& name, const Vec3f& vec) const;
			void SetUniform4f(const std::string& name, float x, float y, float z, float w) const;
			//Still sets vec4 of floats, just less casting
			void SetUniform4f(const std::string& name, const Vec4d& vec) const;
			void SetUniform4Mat(const std::string& name, const Mat4f& mat) const;
			size_t GetUniformBlockIndex(const std::string& blockName) const;
			size_t GetID() const;
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