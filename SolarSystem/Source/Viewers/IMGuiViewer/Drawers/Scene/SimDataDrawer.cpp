#include "SimDataDrawer.h"

#include "Source/Viewers/IMGuiViewer/OpenGL/Shader.h"
#include "Source/Viewers/IMGuiViewer/OpenGL/Sphere.h"
#include "Camera.h"

#include <algorithm>
namespace solar
{
	namespace drawers
	{
		namespace
		{
			// Number of rings to aproximate sphere with.
			constexpr size_t resolution = 32;
			constexpr Vec3f ambientCol {0.05f,0.05f,0.05f};
		}

		SimDataDrawer::SimDataDrawer(const Camera& cam)
		{
			sphere = std::make_unique<openGL::Sphere>(resolution);
			CreateShader(cam);
			realScale = false;
			minRadius = 0.03f;
		}

		SimDataDrawer::~SimDataDrawer()
		{
			//For unique ptrs' destructors
		}

		void SimDataDrawer::Draw(const SimData& data, const Camera & cam)
		{
			shader->Bind();
			for (size_t i = 0; i < data.Get().size(); ++i)
			{
				const auto& unit = data.Get()[i];
				shader->SetUniform4f("col", unit.color);
				//TEST
				//Trying new approach to render stuff far from origin
				// - Send only offset from camera position and ignore translation of camera in view matrix
				// - Because offset and camera's position are close values and translating from offset to origin and from origin to camera's position 
				//		looses precision quickly for distances far from origin
				// Basicaly make Camera class hold world position(as double) and send positions relative to camera (subtracted here, using double) to OpenGL
				//		and view matrix only stores rotation
				//IMPROVE Could go further if relative is still to much( example?)
				// Divide world into chunks, store all positions relative to chunk's origin(both camera's and object's), so in individual chunks it will be still precise and further chunks will be
				// translated by chunks' size which can be integer or another float
				if (!realScale)
				{
					//Ensure minimum screen radius of the sphere = make it bigger the farther it is
					auto screenPos = cam.ViewMatrix()*Vec4f(static_cast<Vec3f>(unit.pos), 1.0f);
					screenPos /= screenPos.w;
					double screenScale = -minRadius*screenPos.z;
					shader->SetUniform1f("scale", std::max(screenScale, unit.radius));
				}
				else
					shader->SetUniform1f("scale", unit.radius);

				Vec3f relPos = Vec3f(unit.pos - cam.CamPos());
				if (i == 0)
					shader->SetUniform3f("lightDir", Vec3f());
				else
				{
					auto lightDir = data.Get()[0].pos - unit.pos;//Points to light source
					lightDir.Normalize();
					shader->SetUniform3f("lightDir", lightDir);
				}
				shader->SetUniform3f("offset", relPos);
				sphere->Draw();
			}
			shader->UnBind();
		}

		Vec2f SimDataDrawer::GetScreenPos(const Unit & object, const Camera & cam) const
		{
			//IMPROVE make position relative to the camera and then project it with view matrix with translation removed
			auto screenPos = cam.ProjViewMatrix()*Vec4f(static_cast<Vec3f>(object.pos), 1.0f);
			screenPos /= screenPos.w;
			return screenPos.z > 0 ? Vec2f(screenPos.x, screenPos.y) : Vec2f(-1, -1);
		}

		float SimDataDrawer::GetScreenRadius(const Unit &object, const Camera &cam) const
		{
			//Radius is now fixed, in future it might be different when zooming in more
			//TEST Should it always return minimum or bigger raidus when zoomed in? Cause you really don't need to click on it when you are already looking at it
			return minRadius;
		}

		void SimDataDrawer::CreateShader(const Camera& cam)
		{

			const std::string vSource = R"(
			#version 140
			#extension GL_ARB_explicit_attrib_location : require
			//Vertices of a sphere
			layout(location = 0) in vec3 position;

			std140 uniform CameraMatrices
			{
					mat4 projection;
					mat4 view;
					mat4 projView;
					mat4 invProj;
					mat4 invView;
					mat4 invProjView;
			} cam;

			uniform vec3 offset=vec3(0.0f);
			uniform float scale;

			out vec3 normal;
			void main()
			{
				//Aspect ratio of screen(Valid both for ortho and perspective matrices)
				vec2 AR = vec2(1.0, cam.projection[1][1]/cam.projection[0][0]);
				mat4 camView = cam.view;
				camView[3] = vec4(0,0,0,1.0f);
				//gl_Position = cam.projection*cam.view*vec4(scale*position+offset, 1.0);
				gl_Position = cam.projection*(camView)*vec4(scale*position+offset, 1.0);
				normal=position;
			})";
			const std::string fSource = R"(
			#version 140
			out vec4 color;

			uniform vec4 col;
			//Points to light source, is zero if this object should not be affected by it(i.e Sun)
			uniform vec3 lightDir;
			uniform vec3 ambientCol;

			in vec3 normal;
			void main()
			{
				float diffIntensity = max(dot(normalize(normal),lightDir),0.0f);
				if(length(lightDir)<0.1f)//Not affected
					diffIntensity=1.0f;
				color = col*0.9f*diffIntensity + vec4(ambientCol,1.0f);
			})";

			shader = std::make_unique<openGL::Shader>(vSource, fSource);
			cam.Subscribe(*shader);
			shader->Bind();
			shader->SetUniform3f("ambientCol", Vec3f());
		}
	}
}
