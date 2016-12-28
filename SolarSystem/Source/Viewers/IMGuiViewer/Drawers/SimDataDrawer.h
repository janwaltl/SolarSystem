#ifndef DRAWERS_SIMDATA_DRAWER_HEADER
#define DRAWERS_SIMDATA_DRAWER_HEADER

#include "Drawer.h"

namespace solar
{
	namespace openGL
	{
		class Shader;
		class CircleBuffer;
	}
	namespace drawers
	{
		//Draws simulated units on screen
		//Require GL context at the point of creation
		class SimDataDrawer :public Drawer
		{
		public:
			SimDataDrawer(IMGuiViewer* parent, simData_t* data);
			~SimDataDrawer();
			void Draw() override final;
		private:
			void CreateShader();
			std::unique_ptr<openGL::Shader> shader;
			std::unique_ptr<openGL::CircleBuffer> circle;

			simData_t* simData;
		};
	}
}
#endif