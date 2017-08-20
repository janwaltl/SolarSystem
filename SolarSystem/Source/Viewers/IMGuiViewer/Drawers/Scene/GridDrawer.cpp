#include "GridDrawer.h"
#include "Source/Viewers/IMGuiViewer/OpenGL/Grid.h"
#include "Source/Viewers/IMGuiViewer/OpenGL/Shader.h"
#include "Source/Viewers/IMGuiViewer/OpenGL/Pinheads.h"
#include "Source/Viewers/IMGuiViewer/Drawers/Scene/Camera.h"

#include <algorithm>
#include <cmath>
namespace solar
{

	drawers::GridDrawer::GridDrawer(const SimData& data, const Camera& cam, size_t gridRes, size_t smallToBig) :
		smallerGrid(std::make_unique<openGL::Grid>(gridRes*smallToBig)),
		biggerGrid(std::make_unique<openGL::Grid>(gridRes)),
		smallToBig(smallToBig),
		invSTB(1.0f / smallToBig),
		gridRes(gridRes)
	{
		shaders[XY] = CreateShader(cam, XY);
		shaders[XZ] = CreateShader(cam, XZ);
		shaders[YZ] = CreateShader(cam, YZ);

		pinheads = std::make_unique<openGL::Pinheads>(cam, data->size());
		pinHeadsEnabled = true;
	}

	drawers::GridDrawer::~GridDrawer()
	{
		//For unique ptrs
	}

	double drawers::GridDrawer::Draw(const SimData& data, const Camera& cam, plane p, const Vec2f& scale, float offset)
	{
		//Set zoom based on dist to target or dist to grid ( both are linear in the sense that 2xZoom=2xwider view, holds for both perspective and ortho cameras)
		auto zoomLevel = cam.DistToTarget();
		//auto zoomLevel = cam.CamPos().z - offset;
		//Keeps offset constant troughout all zoom levels
		float corrOffset = float(zoomLevel*offset);
		//Calculates scale of smaller and bigger grid
		//Smaller grid is always smallToBig times smaller, when zooming in the bigger one switches to smaller one's scale and smaller shrinks to preserve smallToBig ratio.
		//Thus only two grids are needed to create grid with infinite resolution.
		auto logScale = log(zoomLevel) / log(smallToBig);
		auto smallerScale = scale * invSTB  * float(pow(smallToBig, floor(logScale)));
		auto biggerScale = smallerScale*float(smallToBig);
		double x;
		float frac = float(abs(modf(logScale, &x)));
		if (logScale < 0.0f)//So frac is always increasing when zooming out
			frac = 1.0f - frac;

		//Always centers grid to screen's center, but snaps it to biggerScale.
		Vec3f smallGridOffset, bigGridOffset;
		switch (p)
		{
		case solar::drawers::GridDrawer::XY:
			smallGridOffset = Vec3f(floorToMult(float(cam.TargetPos().x), biggerScale.x),
									floorToMult(float(cam.TargetPos().y), biggerScale.y),
									corrOffset);
			bigGridOffset = smallGridOffset + Vec3f(0, 0, 0)*float(copysign(1, cam.CamPos().z));
			break;
		case solar::drawers::GridDrawer::XZ:
			smallGridOffset = Vec3f(floorToMult(float(cam.TargetPos().x), biggerScale.x),
									corrOffset,
									floorToMult(float(cam.TargetPos().z), biggerScale.y));
			break;
		case solar::drawers::GridDrawer::YZ:
			smallGridOffset = Vec3f(corrOffset,
									floorToMult(float(cam.TargetPos().y), biggerScale.x),
									floorToMult(float(cam.TargetPos().z), biggerScale.y));
			break;
		default:
			break;
		}
		//Map 0.0-0.5 to 0.0-1.0, clamp 0.5+ to 1.0
		//So biggerGrid dissappears when zoomed in
		float bigAlpha = std::min(frac*2.0f, 1.0f);
		//Map 0.5-1.0 to 1.0-0.0, clamp 0.5- to 1.0
		//So smallerGrid dissappears when zoomed out
		float smallAlpha = std::min((1.0f - frac)*2.0f, 1.0f);
		Vec3f smallCol {0.06f, 0.14f, 0.31f};
		Vec3f bigCol {0.06f, 0.24f, 0.71f};

		//Map 0.0-1.0 to invSTB-1.0
		//Scales rendering limit of the grid to preserve smooth transition when switching to next scales
		float fadeRange = frac*(1.0f - invSTB) + invSTB;
		Draw(smallerGrid, p, smallerScale, Vec4f(smallCol*bigAlpha + bigCol*(1.0f - bigAlpha), smallAlpha), smallGridOffset, fadeRange, gridRes*smallToBig);
		Draw(biggerGrid, p, biggerScale, Vec4f(bigCol, bigAlpha), bigGridOffset, fadeRange, gridRes);
		if (pinHeadsEnabled)
		{
			float baseSize = offset*0.2f*zoomLevel;
			pinheads->Draw(data, p, corrOffset, Vec2f(baseSize, baseSize));
		}



		return smallerScale.x;
	}

	size_t drawers::GridDrawer::SmallToBig()
	{
		return smallToBig;
	}

	void drawers::GridDrawer::Draw(const std::unique_ptr<openGL::Grid>& grid, plane p, const Vec2f & scale, const Vec4f & col, const Vec3f & offset, float fadeRange, size_t gridResolution)
	{
		shaders[p]->Bind();
		shaders[p]->SetUniform2f("scale", scale);
		shaders[p]->SetUniform3f("offset", offset);
		shaders[p]->SetUniform4f("col", col);
		shaders[p]->SetUniform1f("fade", fadeRange);
		shaders[p]->SetUniform1i("gridRes", gridResolution);
		grid->Draw();
		shaders[p]->UnBind();
	}

	std::unique_ptr<openGL::Shader> drawers::GridDrawer::CreateShader(const Camera& cam, plane p)
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

				//float fadeFactor =(maxDist<fade)? 1.0f: 0.0f;
				float fadeFactor =1.0f-smoothstep(fade,fade,maxDist);
				color =col*fadeFactor;
			})";

		auto shader = std::make_unique<openGL::Shader>(vSource, fSource);
		cam.Subscribe(*shader);
		return std::move(shader);
	}
}
