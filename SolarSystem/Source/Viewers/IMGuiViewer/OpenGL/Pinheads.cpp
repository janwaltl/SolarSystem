#include "Pinheads.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include "Shader.h"
#include "Source/Viewers/IMGuiViewer/Drawers/Scene/Camera.h"
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

			shaders[plane::XY] = CreateShader(cam, plane::XY);
			shaders[plane::XZ] = CreateShader(cam, plane::XZ);
			shaders[plane::YZ] = CreateShader(cam, plane::YZ);
		}
		std::unique_ptr<Shader> Pinheads::CreateShader(const solar::Camera & cam, drawers::GridDrawer::plane p)
		{
			std::string projPos, baseOffset[2];
			switch (p)
			{
			case plane::XY:
				projPos = "inPos.xy, planeOffset, inPos.w";
				baseOffset[0] = "1.0f, 0.0f, 0.0f, 0.0f";
				baseOffset[1] = "0.0f, 1.0f, 0.0f, 0.0f";
				break;
			case plane::XZ:
				projPos = "inPos.x, planeOffset, inPos.zw";
				baseOffset[0] = "1.0f, 0.0f, 0.0f, 0.0f";
				baseOffset[1] = "0.0f, 0.0f, 1.0f, 0.0f";
				break;
			case plane::YZ:
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
			void MakeLine(vec4 projPos, vec4 color, mat4 projView);
			void MakeBase(vec4 porjPos, vec4 color, mat4 projView);

			uniform float baseSize;
			uniform float planeOffset;
			uniform vec4 upCol;
			uniform vec4 downCol;

			out vec4 col;

			void main()
			{

				mat4 camView = view;
				camView[3] = vec4(0,0,0,1.0f);//No need to translate, position is already relative to the camera
				mat4 projView = projection*camView;

				vec4 inPos = gl_in[0].gl_Position;
				vec4 projPos = vec4()" + projPos + R"();


				col = sign(inPos.y - projPos.y)>0.0f? upCol:downCol;

				MakeLine(projPos,col,projView);
				MakeBase(projPos,col,projView);
			}
			void MakeLine(vec4 projPos,vec4 color, mat4 projView)
			{
				gl_Position = projView*gl_in[0].gl_Position;
				col = color;
				EmitVertex();
				gl_Position = projView*projPos;
				col = color;
				EmitVertex();
				EndPrimitive();
			}
			void MakeBase(vec4 projPos, vec4 color, mat4 projView)
			{
				vec4 firstOffset =  baseSize*vec4()" + baseOffset[0] + R"();
				vec4 secondOffset = baseSize*vec4()" + baseOffset[1] + R"();

				gl_Position = projView*(projPos + firstOffset);
				col = color;
				EmitVertex();
				gl_Position = projView*(projPos - firstOffset);
				col = color;
				EmitVertex();
				EndPrimitive();
				gl_Position = projView*(projPos + secondOffset);
				col = color;
				EmitVertex();
				gl_Position = projView*(projPos - secondOffset);
				col = color;
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
		float Pinheads::CalcRelPlaneOffset(const solar::Camera & cam, drawers::GridDrawer::plane p, double planeOffset)
		{
			switch (p)
			{
			case plane::XY: return static_cast<float>(planeOffset - cam.CamPos().z);
			case plane::YZ: return static_cast<float>(planeOffset - cam.CamPos().x);
			case plane::XZ: return static_cast<float>(planeOffset - cam.CamPos().y);
			default:		assert(0); return 0.0f;
			}
		}

		Pinheads::~Pinheads()
		{}

		void Pinheads::Draw(const SimData& data, plane p, double planeOffset, float baseSize, const Camera& cam)
		{
			assert(data->size() == dataSize);
			//All positions are transformed from world coordinates to coordinates relative to camera's position.
			// - Because float does not have enough precision for values far from zero. So if origin is placed at camera's position then 
			//		far from zero means far from camera which does not need much precision(it will occupy small portion of the screen).

			shaders[p]->Bind();
			shaders[p]->SetUniform1f("baseSize", baseSize);
			shaders[p]->SetUniform1f("planeOffset", CalcRelPlaneOffset(cam, p, planeOffset));
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			float* buffer = reinterpret_cast<float*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
			assert(buffer);
			for (const auto& unit : data.Get())
			{
				const Vec3f relPos = static_cast<Vec3f>(unit.pos - cam.CamPos());
				*buffer = relPos.x; buffer++;
				*buffer = relPos.y; buffer++;
				*buffer = relPos.z; buffer++;
			}
			auto result = glUnmapBuffer(GL_ARRAY_BUFFER);
			assert(result);
			glDrawArrays(GL_POINTS, 0, dataSize);
			glBindVertexArray(0);
			shaders[p]->UnBind();
		}
	}
}

