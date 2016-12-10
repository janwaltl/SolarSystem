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
			//They are not needed anymore
			glDeleteShader(fragShader);
			glDeleteShader(vertShader);
		}
		Shader::~Shader()
		{
			glDeleteProgram(programID);
		}
		void Shader::Bind()
		{
			glUseProgram(programID);
		}
		void Shader::UnBind()
		{
			glUseProgram(0);
		}
	}
}
