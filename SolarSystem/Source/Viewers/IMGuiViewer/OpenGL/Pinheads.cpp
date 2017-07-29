#include "Pinheads.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include "Shader.h"
#include "Source/Viewers/IMGuiViewer/Camera.h"
#include "Error.h"
namespace solar
{
	namespace openGL
	{
		Pinheads::Pinheads(const Camera& cam, size_t dataSize) :
			dataSize(dataSize)
		{
			assert(dataSize > 0);

			glGenBuffers(1, &VBO);
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, dataSize * 3 * sizeof(float), nullptr, GL_STREAM_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

			glBindVertexArray(0);

			shaders[XY] = CreateShader(cam, XY);
			shaders[XZ] = CreateShader(cam, XZ);
			shaders[YZ] = CreateShader(cam, YZ);
		}
		std::unique_ptr<Shader> Pinheads::CreateShader(const solar::Camera & cam, plane p)
		{
			std::string projPos, baseOffset[2];
			switch (p)
			{
			case XY:
				projPos = "inPos.xy, planeOffset, inPos.w";
				baseOffset[0] = "1.0f, 0.0f, 0.0f, 0.0f";
				baseOffset[1] = "0.0f, 1.0f, 0.0f, 0.0f";
				break;
			case XZ:
				projPos = "inPos.x, planeOffset, inPos.zw";
				baseOffset[0] = "1.0f, 0.0f, 0.0f, 0.0f";
				baseOffset[1] = "0.0f, 0.0f, 1.0f, 0.0f";
				break;
			case YZ:
				projPos = "planeOffset, inPos.yzw";
				baseOffset[0] = "0.0f, 1.0f, 0.0f, 0.0f";
				baseOffset[1] = "0.0f, 0.0f, 1.0f, 0.0f";
				break;
			default:
				assert(0);
				break;
			}

			const std::string vSource = R"(
			#version 330
			layout(location=0) in vec3 position;

			void main()
			{
				gl_Position = vec4(position,1.0f);
			}
			)";
			const std::string gSource = R"(
			#version 330
			
			layout(points) in;
			layout(line_strip, max_vertices=6) out;

			std140 uniform CameraMatrices
			{
					mat4 projection;
					mat4 view;
					mat4 projView;
					mat4 invProj;
					mat4 invView;
					mat4 invProjView;
			};
			void MakeLine(vec4 projPos,vec4 color);
			void MakeBase(vec4 porjPos,vec4 color);

			uniform vec2 baseSize;
			uniform float planeOffset;
			uniform vec4 upCol;
			uniform vec4 downCol;

			out vec4 col;

			void main()
			{
				vec4 inPos = gl_in[0].gl_Position;
				vec4 projPos = vec4()" + projPos + R"();
				col = sign(inPos.y - projPos.y)>0.0f? upCol:downCol;
				MakeLine(projPos,vec4(0.0f));
				MakeBase(projPos,vec4(0.0f));
			}
			void MakeLine(vec4 projPos,vec4 color)
			{
				gl_Position = projection*view*gl_in[0].gl_Position;
				EmitVertex();
				gl_Position = projection*view*projPos;
				EmitVertex();
				EndPrimitive();
			}
			void MakeBase(vec4 projPos,vec4 color)
			{
				vec4 firstOffset =  baseSize.x*vec4()" + baseOffset[0] + R"();
				vec4 secondOffset = baseSize.y*vec4()" + baseOffset[1] + R"();

				gl_Position = projection*view*(projPos + firstOffset);
				EmitVertex();
				gl_Position = projection*view*(projPos - firstOffset);
				EmitVertex();
				EndPrimitive();
				gl_Position = projection*view*(projPos + secondOffset);
				EmitVertex();
				gl_Position = projection*view*(projPos - secondOffset);
				EmitVertex();
				EndPrimitive();
			}
			)";
			const std::string fSource = R"(
			#version 330
			out vec4 color;

			in vec4 col;

			void main()
			{
				color = col;
			})";

			auto shader = std::make_unique<Shader>(vSource, fSource, gSource);
			shader->Bind();
			shader->SetUniform4f("upCol", Vec4f(0.90f, 0.78f, 0.00f, 1.00f));
			shader->SetUniform4f("downCol", Vec4f(0.90f, 0.28f, 0.00f, 1.00f));
			cam.Subscribe(*shader);
			shader->UnBind();
			return shader;
		}
		Pinheads::~Pinheads()
		{}
		void Pinheads::Draw(const SimData& data, plane p, float planeOffset, const Vec2f& baseSize)
		{
			assert(data->size() == dataSize);

			shaders[p]->Bind();
			shaders[p]->SetUniform2f("baseSize", baseSize);
			shaders[p]->SetUniform1f("planeOffset", planeOffset);
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			float* buffer = reinterpret_cast<float*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
			assert(buffer);
			for (const auto& unit : data.Get())
			{
				*buffer = static_cast<float>(unit.pos.x); buffer++;
				*buffer = static_cast<float>(unit.pos.y); buffer++;
				*buffer = static_cast<float>(unit.pos.z); buffer++;
			}
			auto result = glUnmapBuffer(GL_ARRAY_BUFFER);
			assert(result);
			glDrawArrays(GL_POINTS, 0, dataSize);
			glBindVertexArray(0);
			shaders[p]->UnBind();
		}
	}
}

