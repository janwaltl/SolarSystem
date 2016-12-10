#include "CircleBuffer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
namespace solar
{
	namespace openGLBackend
	{
		CircleBuffer::CircleBuffer(size_t numVertices, float radius)
		{
			std::vector<GLfloat> vertices;
			std::vector<GLuint>  indices;
			
			const GLfloat step = 4 * atanf(1) / numVertices;// 2PI/numVertices

			//Push center vertex in
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			indices.push_back(0);

			for (size_t i = 0; i <= numVertices; ++i)
			{
				//Push x,y coordinates of point on circle
				vertices.push_back(radius *cosf(i*step));
				vertices.push_back(radius *sinf(i*step));
				
				indices.push_back(i+1);
			}
			numIndices = indices.size();

			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &IBO);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, vertices.size(), vertices.data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), indices.data(), GL_STATIC_DRAW);
			
			//Positions are Vec2 of floats at location=0
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			//glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

		}
		CircleBuffer::~CircleBuffer()
		{
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &IBO);
			glDeleteVertexArrays(1, &VAO);
		}
		void CircleBuffer::Draw()
		{
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLE_FAN, numIndices, GL_UNSIGNED_INT, NULL);
			glBindVertexArray(0);
		}
	}
}
