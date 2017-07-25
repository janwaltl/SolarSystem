#include "Grid.h"

#include <GL/glew.h>
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

	for (int i = 0; i < gridRes; ++i)
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
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * gridRes * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

solar::openGL::Grid::~Grid()
{
	if (VBO != 0)
		glDeleteBuffers(1, &VBO);
}

void solar::openGL::Grid::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_LINES, 0, 4*gridRes);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}
