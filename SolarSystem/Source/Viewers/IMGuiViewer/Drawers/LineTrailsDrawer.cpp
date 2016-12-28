#include "LineTrailsDrawer.h"
#include "Source/Viewers/IMGuiViewer/OpenGL/Shader.h"
#include "Source/Viewers/IMGuiViewer/OpenGL/UnitTrail.h"
#include "Source/Viewers/IMGuiViewer.h"

namespace solar
{
	namespace drawers
	{
		LineTrailsDrawer::LineTrailsDrawer(IMGuiViewer * parent, simData_t * data) :
			Drawer(parent), simData(data), frameCounter(0)
		{
			assert(data);

			CreateShader();
			CreateTrails();
		}

		LineTrailsDrawer::~LineTrailsDrawer()
		{
			//Because of shader pointer type's completness
		}

		void LineTrailsDrawer::Draw()
		{
			UpdateTrails();
			assert(trails.size() == simData->size());

			shader->Bind();
			auto trailIT = trails.begin();
			for (auto unitIT = simData->begin(); unitIT != simData->end(); ++unitIT, ++trailIT)
			{
				shader->SetUniform4f("col", unitIT->color);

				float scale = static_cast<float>(viewer->ScaleFactor());
				shader->SetUniform2f("scale", scale, scale);
				shader->SetUniform2f("offset", viewer->GetOffset());
				trailIT->Draw();
			}
			shader->UnBind();
		}

		void LineTrailsDrawer::CreateShader()
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

		void LineTrailsDrawer::CreateTrails()
		{
			trails.resize(simData->size());
		}

		void LineTrailsDrawer::UpdateTrails()
		{
			assert(simData->size() == trails.size());
			++frameCounter %= settings::lineTrail::resolution;
			if (!frameCounter)//Only update trails every trailRes frames
			{
				auto trailIT = trails.begin();
				for (auto unitIT = simData->begin(); unitIT != simData->end(); ++unitIT, ++trailIT)
				{
					trailIT->Push(unitIT->pos);
				}

			}
		}

	}
}
