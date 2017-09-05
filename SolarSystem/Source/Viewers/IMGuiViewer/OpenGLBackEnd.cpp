#include "OpenGLBackEnd.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Source/Common/Exception.h"
#include "Source/Math/Math.h"
#include "OpenGL/Error.h"
#include <algorithm>

namespace solar
{
	namespace
	{
		//Stores error message
		std::string error;
		bool errTrigger = false;
		constexpr int winPosX = 5;
		constexpr int winPosY = 20;
		//Background color
		constexpr Vec4d bgColor(0 / 255.0, 0 / 255.0, 0 / 255.0, 1.0);
		constexpr size_t samples = 1;
	}

	//Following usage of GLFW library is  based on their tutorial
	// at http://www.glfw.org/docs/latest/quick.html .
	OpenGLBackend::OpenGLBackend(size_t width, size_t height, const std::string & title) :
		w(width), h(height)
	{
		if (glfwInit() == GL_FALSE)
			throw Exception("Cannot initialize GLFW library.");

		glfwSetErrorCallback(ErrorCallback);
		// Uses OpenGL 1.0 to atleast create contex
		// will automatically use the newest available or throw on some error
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		//Do not allow rezising
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		glfwWindowHint(GLFW_SAMPLES, samples);//Anti-aliasing
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		win = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr); // create actual window
		if (win == nullptr) // if that fails
			throw Exception("Unable to create Window, reason: " + error);

		glfwSetWindowPos(win, winPosX, winPosY);
		glfwMakeContextCurrent(win);

		auto add = glfwGetProcAddress("glGetString");
		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) // tries to initialize glad
		{
			DestroyGLFW();
			throw Exception("GLAD initialization failed.");
		}

		CheckVersionAndExtensions();

		glEnable(GL_MULTISAMPLE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_GEQUAL);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glViewport(0, 0, width, height); // sets correct coordinate viewport
		glClearColor((GLclampf)bgColor.x, (GLclampf)bgColor.y, (GLclampf)bgColor.z, (GLclampf)bgColor.w);
		glClearDepth(0.0f);

		CreateFBO();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glfwSwapInterval(0);
	}

	void OpenGLBackend::CreateFBO()
	{
		GLint colMaxSamples = 0, depthMaxSamples = 0;
		glGetIntegerv(GL_MAX_DEPTH_TEXTURE_SAMPLES, &depthMaxSamples);
		glGetIntegerv(GL_MAX_COLOR_TEXTURE_SAMPLES, &colMaxSamples);
		if (samples <= 0 || samples > colMaxSamples || samples > depthMaxSamples)
			throw Exception("This machine does not support this amount(" + std::to_string(samples) + ") of samples. Maximum is " + std::to_string(std::min(colMaxSamples, depthMaxSamples)) + '.' +
							"Amount can be changed in file OpenGLBackEnd.cpp");

		glGenTextures(1, &FBOColTex);
		glGenTextures(1, &FBODepthTex);
		glGenFramebuffers(1, &FBO);

		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, FBOColTex);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA, w, h, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, FBODepthTex);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_DEPTH_COMPONENT32F, w, h, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, FBOColTex, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, FBODepthTex, 0);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			DeleteFBO();
			DestroyGLFW();
			throw Exception("Could not create OpenGL Framebuffer.");
		}
	}

	void OpenGLBackend::DeleteFBO()
	{
		glDeleteTextures(1, &FBOColTex);
		glDeleteTextures(1, &FBODepthTex);
		glDeleteFramebuffers(1, &FBO);
	}

	OpenGLBackend::~OpenGLBackend()
	{
		DestroyGLFW();
	}

	void OpenGLBackend::DestroyGLFW()
	{
		if (win)
		{
			glfwDestroyWindow(win);
			glfwTerminate();

			win = nullptr;
		}
	}

	GLFWwindow * OpenGLBackend::GetWin()
	{
		return win;
	}

	bool OpenGLBackend::NewFrame()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glfwSwapBuffers(win);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glfwPollEvents();//Get system events, calls registered callbacks
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (errTrigger)
		{
			errTrigger = false;
			throw Exception(error);
		}

		return glfwWindowShouldClose(win) != 0;
	}

	void OpenGLBackend::Render()
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // default FBO
		glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLBackend::ErrorCallback(int err, const char * description)
	{
		error = "GLFW error: " + std::to_string(err) + ": " + description;
		if(err==65543)//Failed to create context - version might be too low
			error+="\n Make sure your version of OpenGL is atleast 3.3";			
		errTrigger = true;
	}
	void OpenGLBackend::CheckVersionAndExtensions()
	{
		if(!GLAD_GL_VERSION_3_3)
			throw Exception("This application requires OpenGL 3.3");
		//Is needed to implement reversed depth buffer.
		if (GLAD_GL_VERSION_4_5 || GLAD_GL_ARB_clip_control)
			//Sources: http://dev.theomader.com/depth-precision/ last paragraph 'Reverse depth on OpenGL'
			//			issues #14 and #20 https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_clip_control.txt
			glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);//Overwrites standard -1,1 clip space
		else if (GLAD_GL_NV_depth_buffer_float)
			//Source: following two links
			//Overwrites standard -1,1 clip space, but http://dev.theomader.com/depth-precision/ says it also needs custom far clipping plane
			// http://outerra.blogspot.cz/2012/11/maximizing-depth-buffer-range-and.html paragraph 'DirectX vs. OpenGL' also mentions custom far clipping plane
			//TODO Resolve: Not so sure about that, it clips fine here, but might be driver specific?
			//	- Does it matter? Far plane is set to very far,basically infinity, anyway
			glDepthRangedNV(-1.0, 1.0);
		else
		{
			GLint major, minor;
			glGetIntegerv(GL_MAJOR_VERSION, &major);
			glGetIntegerv(GL_MINOR_VERSION, &minor);
			DestroyGLFW();
			throw Exception("This application requires OpenGL 4.5 or either of the two following extensions GL_ARB_clip_control or GL_NV_depth_buffer_float.\n"
							"Neither of those extensions have been found and OpenGL's detected version: " + std::to_string(major) + '.' + std::to_string(minor));
		}
	}
}
