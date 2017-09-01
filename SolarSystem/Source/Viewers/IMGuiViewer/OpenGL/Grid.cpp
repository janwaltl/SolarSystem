#include "Grid.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <vector>
solar::openGL::Grid::Grid(size_t gridRes)
	:gridRes(gridRes)
{
	assert(gridRes > 0);
	float halfRes = gridRes / 2.0f;
	std::vector<float> vertices;
	vertices.reserve(8 * gridRes);

	for (size_t i = 0; i < gridRes; ++i)
	{
		float offset = (i - halfRes);
		//X line
		vertices.insert(vertices.end(), {-halfRes,offset});
		vertices.insert(vertices.end(), {+halfRes,offset});

		//Y line
		vertices.insert(vertices.end(), {offset,-halfRes});
		vertices.insert(vertices.end(), {offset,+halfRes});
	}
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * gridRes * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

solar::openGL::Grid::~Grid()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void solar::openGL::Grid::Draw() const
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 4 * gridRes);
	glBindVertexArray(0);

}