#include "OpenGLBackEnd.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../../Exception.h"

namespace solar
{
	GLFWwindow* OpenGLBackEnd::win = nullptr;
	std::string OpenGLBackEnd::error = "";
	unsigned int OpenGLBackEnd::shader;

	void OpenGLBackEnd::Init(int width, int height, const std::string & title)
	{
		if (glfwInit() == GL_FALSE)
			throw Exception("Cannot initialize GLFW library.");

		glfwSetErrorCallback(ErrorCallback);

		// Uses OpenGL 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//Do not allow rezising, because it's too much work to make it nice
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		win = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr); // creates actual window
		if (win == nullptr) // if that fails
			throw Exception("Unable to create Window,reason: " + error);

		glfwSetWindowPos(win, 5, 20);
		glfwMakeContextCurrent(win);
		glewExperimental = GL_TRUE; // on some machines OpenGL crashes when this is at false

		if (glewInit() != GLEW_OK) // tries to initialize glew
			throw Exception("GLEW initialization failed.");

		glViewport(0, 0, width, height); // sets correct coordinate viewport
		//glfwSwapInterval(0);

		LoadShaders();
		CreateBufferObjects();
	}

	void OpenGLBackEnd::Destroy()
	{
		glfwDestroyWindow(win);
		glfwTerminate();

		win = nullptr;
	}

	GLFWwindow * OpenGLBackEnd::GetWin()
	{
		return win;
	}

	void OpenGLBackEnd::ErrorCallback(int err, const char * description)
	{
		error = std::string(description);
	}

	void OpenGLBackEnd::LoadShaders()
	{
		//One Shader for Units, one Shader for line trails
		const std::string vertexSource = R"(
			#version 330 core
			layout(location = 0) in vec2 position;

			uniform vec2 offset;

			void main()
			{
				gl_Position = vec4(position.x + offset.x, position.y + offset.y, 0.0, 1.0);
			})";
		const std::string fragSource = R"(
			#version 330 core
			out vec4 color;

			uniform vec3 col;

			void main()
			{
				color = vec4(col,1.0f);
			})";

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
			GLchar log[1024];
			glGetShaderInfoLog(fragShader, 512, NULL, log);
			throw Exception("Error:Fragment Shader's compilation failed,reason:\n" + std::string(log));
		}

		//Link together

		shader = glCreateProgram();
		glAttachShader(shader, vertShader);
		glAttachShader(shader, fragShader);
		glLinkProgram(shader);
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLchar log[1024];
			glGetProgramInfoLog(shader, 512, NULL, log);
			throw Exception("Error:Shader's linkage has failed,reason:\n" + std::string(log));
		}

		glDeleteShader(fragShader);
		glDeleteShader(vertShader);

	}

	void OpenGLBackEnd::CreateBufferObjects()
	{
		//One for Units, one for line trails
		
	}

}
