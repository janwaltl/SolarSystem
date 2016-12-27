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
		class SimDataDrawer :public Drawer
		{
		public:
			SimDataDrawer(IMGuiViewer* parent, simData_t* data);
			~SimDataDrawer();//For unique ptrs
			void Draw() override final;
		private:
			//Settings
			static float cSize;
			static size_t cRes;

			void CreateShader();
			std::unique_ptr<openGL::Shader> shader;
			std::unique_ptr<openGL::CircleBuffer> circle;

			simData_t* simData;
		};
	}
}
#endif