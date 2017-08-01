#include "TestDrawer.h"
#include "Source/Viewers/IMGuiViewer/OpenGL/Shader.h"
#include "Source/Viewers/IMGuiViewer/Camera.h"

#include <gl/glew.h>
solar::drawers::TestDrawer::TestDrawer(const Camera & cam)
{
	CreateShader(cam);

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

solar::drawers::TestDrawer::~TestDrawer()
{

}
void solar::drawers::TestDrawer::Draw()
{
	shader->Bind();
	glBindVertexArray(VAO);
	//Close
	shader->SetUniform3f("scale", Vec3f(1.0f, 1.0f, 1.0f));
	shader->SetUniform3f("offset", Vec3f(1.0f+1e-2, -1.0f, -1.0f));
	shader->SetUniform4f("col", Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	shader->SetUniform3f("scale", Vec3f(1.0f, 1.0f, 1.0f));
	shader->SetUniform3f("offset", Vec3f(10.0f,-1.1f, -1.0f - 1e-4));
	shader->SetUniform4f("col", Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	shader->SetUniform3f("scale", Vec3f(1.0f, 1.0f, 1.0f));
	shader->SetUniform3f("offset", Vec3f(1, -1.0f, -10));
	shader->SetUniform4f("col", Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	shader->SetUniform3f("scale", Vec3f(1.0f, 1.0f, 1.0f));
	shader->SetUniform3f("offset", Vec3f(0.0f, -1.1f, -1.00001f*10e12));
	shader->SetUniform4f("col", Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
	shader->UnBind();
}

void solar::drawers::TestDrawer::CreateShader(const Camera & cam)
{
	const std::string vSource = R"(
			#version 140
			#extension GL_ARB_explicit_attrib_location : require
			//Vertices of a cube
			in vec3 position;

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
