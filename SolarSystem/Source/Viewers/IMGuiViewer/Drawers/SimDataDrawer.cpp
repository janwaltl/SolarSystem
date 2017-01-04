#include "SimDataDrawer.h"

#include "Source/Viewers/IMGuiViewer/OpenGL/Shader.h"
#include "Source/Viewers/IMGuiViewer/OpenGL/CircleBuffer.h"
#include "Source/Viewers/ImGUIViewer.h"

namespace solar
{
	namespace drawers
	{
		SimDataDrawer::SimDataDrawer(double aspectRatio)
		{
			circle = std::make_unique<openGL::CircleBuffer>(settings::circleBuffer::resolution,
															settings::circleBuffer::radius);
			CreateShader(aspectRatio);
		}

		SimDataDrawer::~SimDataDrawer()
		{
			//For unique ptrs' destructors
		}

		void SimDataDrawer::Draw(const simData_t& data, double scaleFactor,const Vec2& offset)
		{
			shader->Bind();
			for (const auto& unit : data)
			{
				shader->SetUniform4f("col", unit.color);
				shader->SetUniform2f("unitPos", scaleFactor *unit.pos + offset);
				circle->Draw();
			}
			shader->UnBind();
		}

		void SimDataDrawer::CreateShader(double aspectRatio)
		{
			const std::string vSource = R"(
			#version 330 core
			layout(location = 0) in vec2 position;

			uniform vec2 unitPos;
			uniform vec2 AR;//Aspect ratio
			void main()
			{
				vec2 pos = position + unitPos;
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
			shader->SetUniform2f("AR", 1.0f, static_cast<float>(aspectRatio));
		}
	}
}
