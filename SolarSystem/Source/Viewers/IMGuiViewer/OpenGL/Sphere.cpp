#include "Sphere.h"
#include "Source/Math/Math.h"
#include <vector>
#include <glad/glad.h>

namespace solar
{

	openGL::Sphere::Sphere(size_t resolution)
	{
		std::vector<solar::Vec3f> vertices;
		std::vector<uint32_t> indices;
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

	openGL::Sphere::~Sphere()
	{
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &IBO);
		glDeleteVertexArrays(1, &VAO);
	}

	void openGL::Sphere::Draw()
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

}
