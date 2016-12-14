#include "ImGuiBackend.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "IMGuiLibrary/imgui.h"

#include "OpenGLBackend/Shader.h"

namespace solar
{
	unsigned int IMGuiBackend::textID, IMGuiBackend::VAO, IMGuiBackend::IBO, IMGuiBackend::VBO;
	std::unique_ptr<openGLBackend::Shader> IMGuiBackend::shader;

	IMGuiBackend::IMGuiBackend(GLFWwindow * win)
	{
		assert(win);//If nullptr, that means this is initializing first, which should not. Or OpenGLBackend failed, which
					//shouldve thrown.
		this->win = win;
		SetImGUISettings();
		LoadShader();
		CreateBuffers();
		LoadFontTexture();
		SetCallbacks();

	}
	void IMGuiBackend::NewFrame()
	{
		ImGui::NewFrame();
	}
	void IMGuiBackend::Render()
	{
		ImGui::Render();
	}
	IMGuiBackend::~IMGuiBackend()
	{
		glDeleteTextures(1, &textID);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &IBO);
		glDeleteVertexArrays(1, &VAO);
		ImGui::Shutdown();
	}
	void IMGuiBackend::DrawGUI()
	{
		NewFrame();

		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_Once);
		ImGui::SetNextWindowSize(ImVec2(150, 400), ImGuiSetCond_Once);
		ImGui::Begin("Window");
		ImGui::Text("Text");
		ImGui::Button("Button");
		ImGui::End();
		ImGui::Render();

		Render();
	}
	void IMGuiBackend::SetImGUISettings()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		int w, h;
		glfwGetWindowSize(win, &w, &h);
		io.DisplaySize = {static_cast<float>(w),static_cast<float>(h)};

		//Register Render function
		io.RenderDrawListsFn = IMGuiBackend::RenderFnc;

	}

	void IMGuiBackend::LoadShader()
	{
		const std::string vertSource = R"(
			#version 330
			layout(location=0)in vec2 pos;
			layout(location=1)in vec4 color;
			layout(location=2)in vec2 uv;
			
			//Pass them to fragment shader
			out vec4 fragCol;
			out vec2 fragUV;

			uniform mat4 orthoProjMat;
			void main()
			{
				fragCol=color;
				fragUV=uv;
				gl_Position = orthoProjMat*vec4(pos.xy,0.0,1.0);
			})";

		const std::string fragSource = R"(
			#version 330
			in vec2 fragUV;
			in vec4 fragCol;

			out vec4 outCol;

			uniform sampler2D text;

			void main()
			{
				outCol = fragCol * texture(text,fragUV.st);
			})";

		shader = std::make_unique<openGLBackend::Shader>(vertSource, fragSource);

		ImGuiIO& io = ImGui::GetIO();
		const float ortho[4][4] =
		{
			{2.0f / io.DisplaySize.x, 0.0f,                   0.0f, 0.0f},
			{0.0f,                  2.0f / -io.DisplaySize.y, 0.0f, 0.0f},
			{0.0f,                  0.0f,                  -1.0f, 0.0f},
			{-1.0f,                  1.0f,                   0.0f, 1.0f},
		};
		shader->SetUniform4Mat("orthoProjMat", ortho);
		shader->SetUniform1i("text", 0);


	}

	void IMGuiBackend::CreateBuffers()
	{
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &IBO);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
		glEnableVertexAttribArray(0);//Position
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
		glEnableVertexAttribArray(1);//Color
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
		glEnableVertexAttribArray(2);//UV
#undef OFFSETOF
		glBindVertexArray(0);
	}

	void IMGuiBackend::LoadFontTexture()
	{
		// Build texture atlas
		ImGuiIO& io = ImGui::GetIO();
		unsigned char* pixels;
		int width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

		//Create OpenGL texture
		glGenTextures(1, &textID);
		glBindTexture(GL_TEXTURE_2D, textID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		io.Fonts->ClearTexData();
	}

	void IMGuiBackend::SetCallbacks()
	{
		glfwSetCharCallback(win, IMGuiBackend::CharacterCallback);
		glfwSetKeyCallback(win, IMGuiBackend::KeyCallback);
		glfwSetCursorPosCallback(win, IMGuiBackend::MousePositionCallback);
		glfwSetMouseButtonCallback(win, IMGuiBackend::MouseButtonCallback);
		glfwSetScrollCallback(win, IMGuiBackend::MouseScrollCallback);
		glfwSetCursorEnterCallback(win, IMGuiBackend::MouseEnterCallback);
	}

	void IMGuiBackend::RenderFnc(ImDrawData * draw_data)
	{
		// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_SCISSOR_TEST);
		glActiveTexture(GL_TEXTURE0);

		ImGuiIO& io = ImGui::GetIO();

		shader->Bind();
		glBindVertexArray(VAO);

		for (int n = 0; n < draw_data->CmdListsCount; n++)
		{
			const ImDrawList* cmd_list = draw_data->CmdLists[n];
			const ImDrawIdx* idx_buffer_offset = 0;

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

			for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
			{
				const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
				if (pcmd->UserCallback)
				{
					pcmd->UserCallback(cmd_list, pcmd);
				}
				else
				{
					glBindTexture(GL_TEXTURE_2D, textID);
					glScissor((int)pcmd->ClipRect.x, (int)(io.DisplaySize.y - pcmd->ClipRect.w),
						(int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
					glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount,
								   sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset);
				}
				idx_buffer_offset += pcmd->ElemCount;
			}
		}
		glDisable(GL_SCISSOR_TEST);
	}

	void IMGuiBackend::KeyCallback(GLFWwindow* , int key, int, int action, int)
	{
		ImGuiIO& io = ImGui::GetIO();

		if (action == GLFW_PRESS)
			io.KeysDown[key] = true;
		if (action == GLFW_RELEASE)
			io.KeysDown[key] = false;

		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];

		//Windows key??
	}

	void IMGuiBackend::MouseScrollCallback(GLFWwindow* , double , double dy)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel += static_cast<float>(dy);
	}

	void IMGuiBackend::MouseButtonCallback(GLFWwindow* , int button, int action, int )
	{
		ImGuiIO& io = ImGui::GetIO();

		if (0 <= button && button < 5)
		{
			if (action != GLFW_RELEASE)
				io.MouseDown[button] = true;
			else
				io.MouseDown[button] = false;

		}
	}

	void IMGuiBackend::MousePositionCallback(GLFWwindow* , double xpos, double ypos)
	{
		ImGuiIO& io = ImGui::GetIO();

		io.MousePos = {static_cast<float>(xpos),static_cast<float>(ypos)};

	}

	void IMGuiBackend::CharacterCallback(GLFWwindow*, unsigned int ch)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (ch < std::numeric_limits<ImWchar>::max())
			io.AddInputCharacter(static_cast<ImWchar>(ch));
	}

	void IMGuiBackend::MouseEnterCallback(GLFWwindow*, int enter)
	{
		if (!enter)//Cursor left the screen
		{
			ImGuiIO& io = ImGui::GetIO();
			io.MousePos = {-1,-1};
		}
		//Enter should be captured by mousePosCallback
	}

}
