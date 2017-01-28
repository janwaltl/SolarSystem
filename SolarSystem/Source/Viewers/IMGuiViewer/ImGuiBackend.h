#ifndef IMGUI_BACKEND_HEADER
#define IMGUI_BACKEND_HEADER

#include <memory>

struct GLFWwindow;
struct ImDrawData;

namespace solar
{
	namespace openGL
	{
		class Shader;
	}
	//Integrates ImGui library into program
	//Handles its initialization and also sets callbacks for mouse&keyboard
	class IMGuiBackend
	{
	public:
		//Needs valid GLFWindow to register callbacks
		IMGuiBackend(GLFWwindow* win);
		~IMGuiBackend();

		void NewFrame(double lastFrameTime);
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
		//Shader to use for rendering of ImGui
		static std::unique_ptr<openGL::Shader> shader;
		//Font texture and buffer for vertex data
		static unsigned int textID, VAO, IBO, VBO;
	};
}


#endif