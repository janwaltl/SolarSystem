#include "TestDrawer.h"
#include "Source/Viewers/IMGuiViewer/OpenGL/Shader.h"
#include "Camera.h"

#include <gl/glew.h>

solar::drawers::TestDrawer::TestDrawer(const Camera & cam)
{
	CreateShader(cam);

	constexpr size_t resolution = 32;
	std::vector<Vec3f> vertices;
	std::vector<uint32_t> indices;
	MakeSphere(resolution, vertices, indices);
	numIndices = indices.size();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(size_t)*indices.size(), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3f)*vertices.size(), vertices.data(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


}

void solar::drawers::TestDrawer::MakeSphere(const size_t &resolution, std::vector<solar::Vec3f> &vertices, std::vector<uint32_t> &indices)
{
	assert(resolution >= 4);
	//Rings + poles
	vertices.reserve(resolution*(resolution - 2) + 2);

	//Adds top and bottom pole
	vertices.emplace_back(0.0f, 0.0f, 1.0f);
	vertices.emplace_back(0.0f, 0.0f, -1.0f);

	const size_t verticalSteps = resolution - 2;
	float vStep = 1.0f / (resolution - 1);
	const size_t horSteps = resolution;
	float hStep = 1.0f / horSteps;
	for (size_t v = 1; v <= verticalSteps; ++v)
	{
		float theta = v*vStep * PI<float>;
		float z = cos(theta);
		float r = sin(theta);
		for (size_t h = 0; h < horSteps; ++h)
		{
			float phi = h * hStep * 2 * PI<float>;
			float x = r*sin(phi);
			float y = r*cos(phi);
			vertices.emplace_back(x, y, z);
			if (v == 1)//Top ring
			{
				indices.push_back(0);
				indices.push_back(2 + h);
				indices.push_back(2 + ((h + 1) % horSteps));
			}
			else
			{
				indices.push_back(resolution*(v - 2) + 2 + h);
				indices.push_back(resolution*(v - 1) + 2 + (h + 1) % horSteps);
				indices.push_back(resolution*(v - 1) + 2 + h);

				indices.push_back(resolution*(v - 2) + 2 + h);
				indices.push_back(resolution*(v - 2) + 2 + (h + 1) % horSteps);
				indices.push_back(resolution*(v - 1) + 2 + (h + 1) % horSteps);
			}
			if (v == verticalSteps)//Bottom ring
			{
				indices.push_back(1);
				indices.push_back(resolution*(resolution - 3) + 2 + h);
				indices.push_back(resolution*(resolution - 3) + 2 + (h + 1) % horSteps);

			}
		}
	}
}

solar::drawers::TestDrawer::~TestDrawer()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
	glDeleteVertexArrays(1, &VAO);
}
void solar::drawers::TestDrawer::Draw(const SimData& data)
{
	shader->Bind();
	glBindVertexArray(VAO);

	float offset = 0.0f;
	std::vector<Vec3f> colors {
		Vec3f(1.0f,0.0f,0.0f),Vec3f(0.0f,1.0f,0.0f),Vec3f(0.0f,0.0f,1.0f),
		Vec3f(1.0f,1.0f,0.0f),Vec3f(0.0f,1.0f,1.0f),Vec3f(1.0f,0.0f,1.0f)};
	size_t i = 0;
	for (float size = 1.0/data.RatioOfDistTo(PhysUnits::meter); size <10.0/data.RatioOfDistTo(PhysUnits::lightYear); size *= 10)
	{
		shader->SetUniform3f("scale", size*0.5f*Vec3f(1.0f, 1.0f, 1.0f));
		shader->SetUniform3f("offset", Vec3f(0.0f, offset + size*0.5f, 0.0));
		shader->SetUniform4f("col", Vec4f(colors[i++ %colors.size()], 1.0f));
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
		offset += size*0.5f;
	}
	glBindVertexArray(0);
	shader->UnBind();
}

void solar::drawers::TestDrawer::CreateShader(const Camera & cam)
{
	const std::string vSource = R"(
			#version 140
			#extension GL_ARB_explicit_attrib_location : require
			//Vertices of a cube
			layout(location=0) in vec3 position;

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
			uniform vec3 scale=vec3(1.0f);

			void main()
			{
				//Aspect ratio of screen(Valid both for ortho and perspective matrices)
				vec2 AR = vec2(1.0, cam.projection[1][1]/cam.projection[0][0]);
				gl_Position=  cam.projection  *cam.view*vec4(scale*position+offset, 1.0);
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
