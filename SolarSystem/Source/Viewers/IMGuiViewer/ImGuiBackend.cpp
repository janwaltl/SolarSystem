#include "ImGuiBackend.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "IMGuiLibrary/imgui.h"
#include "OpenGL/Shader.h"
#include "OpenGL/Error.h"
namespace solar
{
	unsigned int IMGuiBackend::textID, IMGuiBackend::VAO, IMGuiBackend::IBO, IMGuiBackend::VBO;
	std::unique_ptr<openGL::Shader> IMGuiBackend::shader;

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

		auto err = openGL::CheckForError();
		if (err != openGL::errors::noError)
			throw Exception("Failed to initialized IMGUI, because of following GL error: " + openGL::TranslateError(err));
	}
	void IMGuiBackend::NewFrame()
	{
		//Fill mouse position
		ImGuiIO& io = ImGui::GetIO();
		double x, y;
		glfwGetCursorPos(win, &x, &y);
		io.MousePos = {static_cast<float>(x),static_cast<float>(y)};
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

		//Register Draw function
		io.RenderDrawListsFn = IMGuiBackend::RenderFnc;

		SetStyle();
	}

	void IMGuiBackend::SetStyle()
	{
		ImGuiStyle * style = &ImGui::GetStyle();

		style->WindowPadding = ImVec2(15, 15);
		style->WindowRounding = 5.0f;
		style->FramePadding = ImVec2(5, 5);
		style->FrameRounding = 4.0f;
		style->ItemSpacing = ImVec2(12, 8);
		style->ItemInnerSpacing = ImVec2(8, 6);
		style->IndentSpacing = 25.0f;
		style->ScrollbarSize = 15.0f;
		style->ScrollbarRounding = 9.0f;
		style->GrabMinSize = 5.0f;
		style->GrabRounding = 3.0f;

		style->Colors[ImGuiCol_Text] = ImVec4(1.00f, 0.73f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_Border] = ImVec4(1.00f, 0.73f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.75f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.25f, 1.00f, 0.00f, 0.50f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.73f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.00f, 0.52f, 0.03f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.03f, 0.21f, 0.52f, 1.00f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.18f, 0.03f, 1.00f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 0.23f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_Column] = ImVec4(1.00f, 0.73f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_ColumnHovered] = ImVec4(1.00f, 0.73f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_ColumnActive] = ImVec4(1.00f, 0.73f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
		style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(1.00f, 0.93f, 1.00f, 1.00f);
		style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
		style->Colors[ImGuiCol_PlotLines] = ImVec4(0.03f, 0.31f, 1.00f, 1.00f);
		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.03f, 0.31f, 1.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.43f);
		style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
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

		shader = std::make_unique<openGL::Shader>(vertSource, fragSource);

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
		//glfwSetCursorPosCallback(win, IMGuiBackend::MousePositionCallback);
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

		auto err = openGL::CheckErrorDBG();
		if (err != openGL::errors::noError)
			throw Exception("IMGUI::Render failed, because of following GL error: " + openGL::TranslateError(err));
	}

	void IMGuiBackend::KeyCallback(GLFWwindow*, int key, int, int action, int)
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

	void IMGuiBackend::MouseScrollCallback(GLFWwindow*, double, double dy)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel += static_cast<float>(dy);
	}

	void IMGuiBackend::MouseButtonCallback(GLFWwindow*, int button, int action, int)
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
