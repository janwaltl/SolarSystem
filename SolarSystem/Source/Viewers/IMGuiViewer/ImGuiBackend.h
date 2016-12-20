#ifndef IMGUI_BACKEND_HEADER
#define IMGUI_BACKEND_HEADER

#include <memory>

struct GLFWwindow;
struct ImDrawData;

namespace solar
{
	namespace openGLBackend
	{
		class Shader;
	}
	//Handles ImGui related stuff, also sets glfw callbacks
	class IMGuiBackend
	{
	public:
		IMGuiBackend(GLFWwindow* win);
		~IMGuiBackend();

		void NewFrame();
		void Render();
	private:
		void SetImGUISettings();
		void SetStyle();
		void LoadShader();
		void CreateBuffers();
		void LoadFontTexture();
		void SetCallbacks();
		void static RenderFnc(ImDrawData* draw_data);
		void static KeyCallback(GLFWwindow* win, int key, int, int action, int mods);
		void static MouseScrollCallback(GLFWwindow* win, double dx, double dy);
		void static MouseButtonCallback(GLFWwindow* win, int button, int action, int mods);
		void static CharacterCallback(GLFWwindow* win, unsigned int ch);
		void static MouseEnterCallback(GLFWwindow* win, int enter);
		GLFWwindow* win;
		static std::unique_ptr<openGLBackend::Shader> shader;
		static unsigned int textID, VAO, IBO, VBO;
	};
}


#endif