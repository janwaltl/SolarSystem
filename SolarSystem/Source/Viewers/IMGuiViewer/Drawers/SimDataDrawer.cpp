#include "SimDataDrawer.h"

#include "Source/Viewers/IMGuiViewer/OpenGL/Shader.h"
#include "Source/Viewers/IMGuiViewer/OpenGL/CircleBuffer.h"

namespace solar
{
	namespace drawers
	{
		namespace
		{
			// Number of vertices to aproximate circle with.
			constexpr size_t resolution = 12;
			// In normalized screen units = <0,1.0>, 1.0 creates circle that spans across whole horizontal axis
			constexpr float radius = 0.005f;
		}

		SimDataDrawer::SimDataDrawer(double aspectRatio)
		{
			circle = std::make_unique<openGL::CircleBuffer>(resolution, radius);
			CreateShader(aspectRatio);
		}

		SimDataDrawer::~SimDataDrawer()
		{
			//For unique ptrs' destructors
		}

		void SimDataDrawer::Draw(const simData_t& data, double scaleFactor, const Vec2d& offset)
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
			#version 140
			#extension GL_ARB_explicit_attrib_location : require

			layout(location = 0) in vec2 position;

			uniform vec2 unitPos;
			uniform vec2 AR;//Aspect ratio
			void main()
			{
				vec2 pos = position + unitPos;
				gl_Position = vec4(AR * pos, 0.0, 1.0);
			})";
			const std::string fSource = R"(
			#version 140
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
