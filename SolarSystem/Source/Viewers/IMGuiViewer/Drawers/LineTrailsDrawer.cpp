#include "LineTrailsDrawer.h"
#include "Source/Viewers/IMGuiViewer/OpenGL/Shader.h"
#include "Source/Viewers/IMGuiViewer/OpenGL/UnitTrail.h"
#include "Source/Viewers/IMGuiViewer/OpenGL/Error.h"
#include "Source/Viewers/IMGuiViewer.h"
#include <algorithm>

namespace solar
{
	namespace drawers
	{
		LineTrailsDrawer::LineTrailsDrawer(size_t dataSize, double aspectRatio) :frameCounter(0)
		{
			CreateShader(aspectRatio);
			CreateTrails(dataSize);
		}

		LineTrailsDrawer::~LineTrailsDrawer()
		{
			//Because of shader pointer type's completness
		}

		void LineTrailsDrawer::Draw(const simData_t& data, double scaleFactor, const Vec2& offset)
		{
			UpdateTrails(data);
			assert(trails.size() == simData->size() && trails.size() == trailsControls.size());

			shader->Bind();
			auto trailIT = trails.begin();
			auto trailCtrlIT = trailsControls.begin();
			for (auto unitIT = data.begin(); unitIT != data.end(); ++unitIT, ++trailIT, ++trailCtrlIT)
			{
				shader->SetUniform4f("col", unitIT->color);
				if (*trailCtrlIT)
				{
					float scale = static_cast<float>(scaleFactor);
					shader->SetUniform2f("scale", scale, scale);
					shader->SetUniform2f("offset", offset);
					trailIT->Draw();
				}
			}
			shader->UnBind();
		}

		void LineTrailsDrawer::SwitchTrail(size_t index, bool enable)
		{
			assert(index < trailsControls.size());

			trailsControls[index] = enable;
			if (!enable)//Disabling trail also clears it
				trails[index].Clear();

		}

		void LineTrailsDrawer::SwitchAll(bool enable)
		{
			std::fill(trailsControls.begin(), trailsControls.end(), enable);
			if (!enable)//Disabling
				ClearAll();
		}

		void LineTrailsDrawer::ClearAll()
		{
			std::for_each(trails.begin(), trails.end(), [](openGL::UnitTrail& trail) {trail.Clear(); });
		}

		bool LineTrailsDrawer::IsTrailEnabled(size_t index)
		{
			assert(index < trailsControls.size());
			return trailsControls[index];
		}

		void LineTrailsDrawer::CreateShader(double aspectRatio)
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
			shader->SetUniform2f("AR", 1.0f, static_cast<float>(aspectRatio));

		}

		void LineTrailsDrawer::CreateTrails(size_t dataSize)
		{
			try
			{
				trails.resize(dataSize);
				trailsControls.resize(dataSize, settings::lineTrail::enabledByDefault);

			}
			catch (openGL::GLError& e)
			{
				if (e.GetErrType() == openGL::errors::outOfMemory)
					throw Exception("Failed to create lineTrails' buffers because there is not enough GPU memory."
									"Lowering settings::lineTrail::maxLength value might help with that.");
				else
					throw Exception("Failed to create lineTrails, reason:" + std::string(e.what()));
			}
		}

		void LineTrailsDrawer::UpdateTrails(const simData_t& data)
		{
			assert(simData->size() == trails.size() && trails.size() == trailsControls.size());

			++frameCounter %= settings::lineTrail::resolution;
			if (!frameCounter)//Only update trails every trailRes frames
			{
				auto trailIT = trails.begin();
				auto trailsCtrlIT = trailsControls.cbegin();
				for (auto unitIT = data.cbegin(); unitIT != data.cend(); ++unitIT, ++trailIT, ++trailsCtrlIT)
				{
					if (*trailsCtrlIT)
						trailIT->Push(unitIT->pos);
				}

			}
		}

	}
}
