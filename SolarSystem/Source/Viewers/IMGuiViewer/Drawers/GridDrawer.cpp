#include "GridDrawer.h"
#include "Source/Viewers/IMGuiViewer/OpenGL/Grid.h"
#include "Source/Viewers/IMGuiViewer/OpenGL/Shader.h"
#include "Source/Viewers/IMGuiViewer/Camera.h"

namespace solar
{

	drawers::GridDrawer::GridDrawer(const Camera& cam, size_t gridRes) :
		grid(std::make_unique<openGL::Grid>(gridRes))
	{

		shaders[XY] = CreateShader(cam, XY, gridRes);
		shaders[XZ] = CreateShader(cam, XZ, gridRes);
		shaders[YZ] = CreateShader(cam, YZ, gridRes);
	}

	drawers::GridDrawer::~GridDrawer()
	{
		//For unique ptrs
	}

	void drawers::GridDrawer::Draw(plane p, const Vec2f& scale, const Vec4f& col,float fade)
	{
		shaders[p]->Bind();
		shaders[p]->SetUniform2f("scale", scale);
		shaders[p]->SetUniform3f("offset", Vec3f());
		shaders[p]->SetUniform4f("col", col.x,col.y,col.z,col.w);
		shaders[p]->SetUniform1f("fade", fade);
		grid->Draw();
		shaders[p]->UnBind();
	}

	std::unique_ptr<openGL::Shader> drawers::GridDrawer::CreateShader(const Camera& cam, plane p, size_t gridRes)
	{
		std::string pos;
		switch (p)
		{
		case drawers::GridDrawer::XY:
			pos = "scaledPos.x,scaledPos.y,0.0f";
			break;
		case drawers::GridDrawer::XZ:
			pos = "scaledPos.x,0.0f,scaledPos.y";
			break;
		case drawers::GridDrawer::YZ:
			pos = "0.0f,scaledPos.x,scaledPos.y";
			break;
		default:
			assert(0);
			break;
		}
		const std::string vSource = R"(
			#version 330
			layout(location=0) in vec2 position;
			uniform vec3 offset;
			uniform vec2 scale;
			
			std140 uniform CameraMatrices
			{
					mat4 projection;
					mat4 view;
					mat4 projView;
					mat4 invProj;
					mat4 invView;
					mat4 invProjView;
			} cam;
			
			uniform int gridRes;

			out vec2 normPos;

			void main()
			{
				normPos = position/float(gridRes)*2.0f;
				
				vec2 scaledPos = position*scale;
				vec3 pos = vec3()" + pos + R"() + offset;
				gl_Position = cam.projection*cam.view*vec4(pos,1.0f);
			}
			)";
		const std::string fSource = R"(
			#version 140
			out vec4 color;

			uniform vec4 col;
			uniform float fade;
			in vec2 normPos;
			void main()
			{
				float maxDist =  max(abs(normPos.x),abs(normPos.y));
				float fadeFactor =maxDist*fade*0.0001+1.0f;
				//float fadeFactor =(maxDist-0.1<fade*1.1)? 1.0f: 0.0f;
				color =col*fadeFactor;
			})";
		// 0.01 -> true for normPos<0.1
		// 0.5 -> true for normPos<0.5
		// 0.99 -> true for normPos<0.99
		auto shader = std::make_unique<openGL::Shader>(vSource, fSource);
		shader->Bind();
		shader->SetUniform1i("gridRes", gridRes);
		cam.Subscribe(*shader);
		return std::move(shader);
	}
}
