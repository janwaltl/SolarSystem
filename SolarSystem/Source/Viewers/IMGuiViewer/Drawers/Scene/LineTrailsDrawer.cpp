#include "LineTrailsDrawer.h"

#include <algorithm>
#include <cassert>
#include "Source/Viewers/IMGuiViewer/OpenGL/Shader.h"
#include "Source/Viewers/IMGuiViewer/OpenGL/Error.h"
#include "Camera.h"

namespace solar
{
	namespace drawers
	{
		namespace
		{
			// Whether are lineTrails rendered by default for all units
			constexpr bool enabledByDefault = true;
			// How often get trails updated, counted in viewer's frames
			// Higher values result in more frequent updates and more precise but shorter-lasting trails
			constexpr size_t resolution = 10;
		}
		LineTrailsDrawer::LineTrailsDrawer(size_t dataSize, const Camera& cam) :frameCounter(0)
		{
			CreateShader(cam);
			CreateTrails(dataSize);
		}

		LineTrailsDrawer::~LineTrailsDrawer()
		{
			//Because of shader pointer type's completness
		}

		void LineTrailsDrawer::Draw(const SimData& data)
		{
			UpdateTrails(data);
			assert(trails.size() == data->size() && trails.size() == trailsControls.size());

			shader->Bind();
			auto trailIT = trails.begin();
			auto trailCtrlIT = trailsControls.begin();
			for (auto unitIT = data->begin(); unitIT != data->end(); ++unitIT, ++trailIT, ++trailCtrlIT)
			{
				shader->SetUniform4f("col", unitIT->color);
				if (*trailCtrlIT)
					trailIT->Draw();
			}
			shader->UnBind();
		}

		void LineTrailsDrawer::SwitchTrail(size_t index, bool enable)
		{
			assert(index < trailsControls.size());

			trailsControls[index] = enable;
			if (!enable)//Disabling trail also clears it
				trails[index].Clear();
			//Enabling first one
			if (anyEnabledCache.valid && !anyEnabledCache.enabled && enable)
			{
				anyEnabledCache.valid = true;
				anyEnabledCache.enabled = true;
			}
			else if (anyEnabledCache.valid && anyEnabledCache.enabled && !enable)//Might disable last one
				anyEnabledCache.valid = false;
		}

		void LineTrailsDrawer::SwitchAll(bool enable)
		{
			std::fill(trailsControls.begin(), trailsControls.end(), enable);
			if (!enable)//Disabling
				ClearAll();
			anyEnabledCache.valid = true;
			anyEnabledCache.enabled = enable;
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

		bool LineTrailsDrawer::IsAnyEnabled()
		{
			if (anyEnabledCache.valid)
				return anyEnabledCache.enabled;
			else
			{
				anyEnabledCache.enabled = std::any_of(trailsControls.begin(), trailsControls.end(), [](auto x) { return x; });
				anyEnabledCache.valid = true;
				return anyEnabledCache.enabled;
			}
		}

		void LineTrailsDrawer::CreateShader(const Camera& cam)
		{
			const std::string vSource = R"(
			#version 330 core
			layout(location = 0) in vec3 position;

			layout (std140) uniform CameraMatrices
			{
					mat4 projection;
					mat4 view;
					mat4 projView;
					mat4 invProj;
					mat4 invView;
					mat4 invProjView;
			} cam;

			void main()
			{
				gl_Position = cam.projection* cam.view * vec4(position, 1.0);
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
			cam.Subscribe(*shader);
		}

		void LineTrailsDrawer::CreateTrails(size_t dataSize)
		{
			try
			{
				trails.resize(dataSize);
				trailsControls.resize(dataSize, enabledByDefault);
				anyEnabledCache.enabled = enabledByDefault;
				anyEnabledCache.valid = true;
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

		void LineTrailsDrawer::UpdateTrails(const SimData& data)
		{
			assert(data->size() == trails.size() && trails.size() == trailsControls.size());

			++frameCounter %= resolution;
			if (!frameCounter)//Only update trails every trailRes frames
			{
				auto trailIT = trails.begin();
				auto trailsCtrlIT = trailsControls.cbegin();
				for (auto unitIT = data->cbegin(); unitIT != data->cend(); ++unitIT, ++trailIT, ++trailsCtrlIT)
					if (*trailsCtrlIT)
						trailIT->Push(unitIT->pos);
			}
		}

	}
}
