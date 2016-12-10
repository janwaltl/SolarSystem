#include "Shader.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../../../Exception.h"


namespace solar
{
	namespace openGLBackend
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
				GLchar log[1024];
				glGetShaderInfoLog(vertShader, 512, NULL, log);
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
				GLchar log[1024];
				glGetShaderInfoLog(fragShader, 512, NULL, log);
				throw Exception("Error:Fragment Shader's compilation failed,reason:\n" + std::string(log));
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
				GLchar log[1024];
				glGetProgramInfoLog(programID, 512, NULL, log);
				throw Exception("Error:Shader's linkage has failed,reason:\n" + std::string(log));
			}
			LoadUniforms();
			//They are not needed anymore
			glDeleteShader(fragShader);
			glDeleteShader(vertShader);
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
		void Shader::SetUniform2f(const std::string & name, const float & x, const float & y) const
		{
			this->Bind();
			glUniform2f(uniforms.at(name), x, y);
		}
		void Shader::SetUniform4f(const std::string& name, const float &x, const float &y, const float &z, const float &w) const
		{
			this->Bind();
			glUniform4f(uniforms.at(name), x, y, z, w);
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
