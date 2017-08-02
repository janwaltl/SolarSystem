#include "SimDataDrawer.h"

#include "Source/Viewers/IMGuiViewer/OpenGL/Shader.h"
#include "Source/Viewers/IMGuiViewer/OpenGL/CircleBuffer.h"
#include "Camera.h"
namespace solar
{
	namespace drawers
	{
		namespace
		{
			// Number of vertices to aproximate circle with.
			constexpr size_t resolution = 12;
			// In normalized screen units = <0,1.0>, 1.0 creates circle that spans across whole horizontal axis
			constexpr float radius = 0.01f;
		}

		SimDataDrawer::SimDataDrawer(const Camera& cam)
		{
			circle = std::make_unique<openGL::CircleBuffer>(resolution, radius);
			CreateShader(cam);
		}

		SimDataDrawer::~SimDataDrawer()
		{
			//For unique ptrs' destructors
		}

		void SimDataDrawer::Draw(const SimData& data)
		{
			shader->Bind();
			for (const auto& unit : data.Get())
			{
				shader->SetUniform4f("col", unit.color);
				shader->SetUniform3f("unitPos", unit.pos);
				circle->Draw();
			}
			shader->UnBind();
		}

		void SimDataDrawer::CreateShader(const Camera& cam)
		{
			
			const std::string vSource = R"(
			#version 140
			#extension GL_ARB_explicit_attrib_location : require
			//Vertices of a circle
			layout(location = 0) in vec2 position;

			std140 uniform CameraMatrices
			{
					mat4 projection;
					mat4 view;
					mat4 projView;
					mat4 invProj;
					mat4 invView;
					mat4 invProjView;
			} cam;

			uniform vec3 unitPos;

			void main()
			{
				//Aspect ratio of screen(Valid both for ortho and perspective matrices)
				vec2 AR = vec2(1.0, cam.projection[1][1]/cam.projection[0][0]);
				vec4 centerEye = cam.projection * cam.view * vec4(unitPos, 1.0);
				gl_Position = vec4(position*AR, 0.0,0.0) + centerEye/centerEye.w;
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
			cam.Subscribe(*shader);
		}
	}
}
