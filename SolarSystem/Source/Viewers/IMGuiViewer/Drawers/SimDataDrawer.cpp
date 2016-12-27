#include "SimDataDrawer.h"

#include "Source/Viewers/IMGuiViewer/OpenGL/Shader.h"
#include "Source/Viewers/IMGuiViewer/OpenGL/CircleBuffer.h"
#include "Source/Viewers/ImGUIViewer.h"

namespace solar
{
	namespace drawers
	{
		float SimDataDrawer::cSize = 0.005f;
		size_t SimDataDrawer::cRes = 12;

		SimDataDrawer::SimDataDrawer(IMGuiViewer * parent, simData_t * data) :
			Drawer(parent), simData(data)
		{
			circle = std::make_unique<openGL::CircleBuffer>(cRes, cSize);
			CreateShader();
		}

		SimDataDrawer::~SimDataDrawer()
		{
		}

		void SimDataDrawer::Draw()
		{
			shader->Bind();
			for (const auto& unit : *simData)
			{
				shader->SetUniform4f("col", unit.color);
				shader->SetUniform2f("scale", 1.0f, 1.0f);
				shader->SetUniform2f("offset", viewer->ScaleFactor() *unit.pos + viewer->GetOffset());
				circle->Draw();
			}
			shader->UnBind();
		}

		void SimDataDrawer::CreateShader()
		{
			const std::string vSource = R"(
			#version 330 core
			layout(location = 0) in vec2 position;

			uniform vec2 offset;// offset pos by this amount, should be normalized
			uniform vec2 scale;// Scaling for position
			uniform vec2 AR;//Aspect ratio
			void main()
			{
				vec2 pos = scale * position + offset;
				gl_Position = vec4(AR * pos, 0.0, 1.0);
			})";
			const std::string fSource = R"(
			#version 330 core
			out vec4 color;

			uniform vec4 col;

			void main()
			{
				color = col;
			})";

			shader = std::make_unique<openGL::Shader>(vSource, fSource);
			//Set aspect ratio, main axis is X and scale the Y axis 
			shader->SetUniform2f("AR", 1.0f, static_cast<float>(viewer->GetAspectRatio()));
		}
	}
}
