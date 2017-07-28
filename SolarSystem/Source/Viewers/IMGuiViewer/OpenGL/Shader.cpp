#include "Shader.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Source/Common/Exception.h"
#include "Error.h"

namespace solar
{
	namespace
	{
		constexpr size_t logBufSize = 1024;
	}
	namespace openGL
	{
		Shader::Shader(const std::string & vertexSource, const std::string & fragSource)
		{
			//Compile vertexShader 

			GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
			const GLchar* vSource = vertexSource.c_str();
			glShaderSource(vertShader, 1, &vSource, NULL);
			glCompileShader(vertShader);
			GLint success = false;
			glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				GLchar log[logBufSize];
				glGetShaderInfoLog(vertShader, logBufSize, NULL, log);
				throw Exception("Error:Vertex Shader's compilation failed,reason:\n" + std::string(log));
			}

			//Compile fragmentShader

			GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
			const GLchar* fSource = fragSource.c_str();
			glShaderSource(fragShader, 1, &fSource, NULL);
			glCompileShader(fragShader);
			glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glDeleteShader(vertShader);//Because we will throw
				GLchar log[logBufSize];
				glGetShaderInfoLog(fragShader, logBufSize, NULL, log);
				throw Exception("Error:Fragment Shader's compilation failed, reason:\n" + std::string(log));
			}

			//Link shaders together

			programID = glCreateProgram();
			glAttachShader(programID, vertShader);
			glAttachShader(programID, fragShader);
			glLinkProgram(programID);
			glGetProgramiv(programID, GL_LINK_STATUS, &success);
			if (!success)
			{
				glDeleteShader(vertShader);//Because we will throw
				glDeleteShader(fragShader);//Because we will throw
				GLchar log[logBufSize];
				glGetProgramInfoLog(programID, logBufSize, NULL, log);
				throw Exception("Error:Shader's linkage has failed, reason:\n" + std::string(log));
			}
			LoadUniforms();
			//They are not needed anymore
			glDeleteShader(fragShader);
			glDeleteShader(vertShader);
			
			//Throws if something failed, but it mostly should be covered by above checks...
			ThrowOnError([this]() {glDeleteProgram(this->programID); });
		}
		Shader::~Shader()
		{
			glDeleteProgram(programID);
		}
		void Shader::Bind() const
		{
			glUseProgram(programID);
		}
		void Shader::UnBind() const
		{
			glUseProgram(0);
		}
		void Shader::SetUniform1f(const std::string & name, float x) const
		{
			this->Bind();
			glUniform1f(uniforms.at(name), x);
		}
		void Shader::SetUniform1i(const std::string & name, int i) const
		{
			this->Bind();
			glUniform1i(uniforms.at(name), i);
		}
		void Shader::SetUniform2f(const std::string& name, float x, float y) const
		{
			this->Bind();
			glUniform2f(uniforms.at(name), x, y);
		}
		void Shader::SetUniform2f(const std::string & name, const Vec2d & vec) const
		{
			SetUniform2f(name, static_cast<float>(vec.x), static_cast<float>(vec.y));
		}
		void Shader::SetUniform2f(const std::string & name, const Vec2f & vec) const
		{
			SetUniform2f(name, vec.x, vec.y);
		}
		void Shader::SetUniform3f(const std::string& name, float x, float y, float z) const
		{
			this->Bind();
			glUniform3f(uniforms.at(name), x, y, z);
		}
		void Shader::SetUniform3f(const std::string & name, const Vec3d & vec) const
		{
			SetUniform3f(name,
						 static_cast<float>(vec.x),
						 static_cast<float>(vec.y),
						 static_cast<float>(vec.z));
		}
		void Shader::SetUniform3f(const std::string & name, const Vec3f & vec) const
		{
			SetUniform3f(name, vec.x, vec.y, vec.z);
		}
		void Shader::SetUniform4f(const std::string& name, float x, float y, float z, float w) const
		{
			this->Bind();
			glUniform4f(uniforms.at(name), x, y, z, w);
		}
		void Shader::SetUniform4f(const std::string & name, const Vec4d & vec) const
		{
			SetUniform4f(name,
						 static_cast<float>(vec.x),
						 static_cast<float>(vec.y),
						 static_cast<float>(vec.z),
						 static_cast<float>(vec.w));
		}
		void Shader::SetUniform4f(const std::string & name, const Vec4f & vec) const
		{
			SetUniform4f(name, vec.x, vec.y, vec.z, vec.w);
		}
		void Shader::SetUniform4Mat(const std::string & name, const Mat4f& mat) const
		{
			this->Bind();
			glUniformMatrix4fv(uniforms.at(name), 1, false, mat.Data());
		}
		size_t Shader::GetUniformBlockIndex(const std::string & blockName) const
		{
			return glGetUniformBlockIndex(programID, blockName.c_str());
		}
		size_t Shader::GetID() const
		{
			return programID;
		}
		void Shader::LoadUniforms()
		{
			// go through all uniforms and store their location
			GLint num, loc, size;
			GLenum type;
			GLchar name[512];
			glGetProgramiv(this->programID, GL_ACTIVE_UNIFORMS, &num); // get number of uniforms
			for (int i = 0; i < num; i++)
			{
				glGetActiveUniform(this->programID, i, 512, NULL, &size, &type, name);
				loc = glGetUniformLocation(this->programID, name);
				uniforms.emplace(name, loc);
			}
		}
	}
}
