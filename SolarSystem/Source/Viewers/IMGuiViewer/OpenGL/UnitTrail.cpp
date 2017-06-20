#include "UnitTrail.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <numeric>
#include <cassert>
#include "Shader.h"
#include "Error.h"

namespace solar
{
	namespace openGL
	{
		namespace
		{
			// Maximum number of points in one lineTrail.
			// GPU Memory footprint:
			// - maxLength*4KB for indices
			// - numUnits* maxLength*8KB for vertices
			constexpr size_t maxLength = 40'000;
		}

		unsigned int UnitTrail::IBO = 0;
		size_t UnitTrail::refCount = 0;

		UnitTrail::UnitTrail() :length(0), curIndex(0)
		{
			if (refCount == 0)//Create IBO only for the first time
			{
				assert(IBO == 0);

				// Indices array is two times bigger than length, stores two copies of indices
				//	-e.g. 0,1,2,0,1,2 for maxLength of 3
				// Together with curIndex this creates sort of sliding window which renderes correct line trail
				// that pushes new indices at the end if there's space. If there's not, then it overwrites the oldest ones
				// on the beginning. These double indices ensure correct rendering order for the whole line strip(trail)
				//  -example: maxLength 3
				// Push({x1,y1}) -			VBO: x1,y1					rendered indices: 0
				// Push({x2,y2})			VBO: x1,y1,x2,y2			rendered indices: 0,1
				// Push({x3,y3})			VBO: x1,y1,x2,y2,x3,y3		rendered indices: 0,1,2
				// Push({x4,y4})			VBO: x4,y4,x2,y2,x3,y3		rendered indices: 1,2,0
				// - Fourth point ovewrites the first one and line is correctly rendered
				//	 in order of second, third and fourth point as it should. Thanks to that indices array's structure.
				// 
				// It wastes memory a little, but 40KB or 80KB for 10'000 indices is nothing for GPU memory
				// In dire need it can be done with just 0 to n indices and two draw calls
				//		- one for old line(after curIndex) and one for new overwritten line(before curIndex indices)

				std::vector<GLuint> indices;
				indices.resize(2 * maxLength);
				std::iota(indices.begin(), indices.begin() + maxLength, 0);
				std::iota(indices.begin() + maxLength, indices.end(), 0);

				glGenBuffers(1, &IBO);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * maxLength * sizeof(GLuint),
							 indices.data(), GL_STATIC_DRAW);

			}
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, 2 * maxLength * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
			//Positions are Vec2d of floats at location=0 in shader
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			//Throws if something failed, cleans up anything that might have been created.
			ThrowOnError([this]() {this->DeleteBuffers(); });
			++refCount;
		}

		UnitTrail::UnitTrail(UnitTrail && other) :VAO(0), VBO(0), length(0), curIndex(0)
		{
			swap(*this, other);
			++refCount;
		}

		UnitTrail & UnitTrail::operator=(UnitTrail && other)
		{
			swap(*this, other);
			return *this;
		}

		UnitTrail::~UnitTrail()
		{
			DeleteBuffers();
			--refCount;
		}

		void UnitTrail::Push(const Vec2d & newPos)
		{
			GLfloat data[2] = {static_cast<GLfloat>(newPos.x),static_cast<GLfloat>(newPos.y)};
			//Uploads newPos into VBO
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 2 * sizeof(GLfloat)*curIndex, 2 * sizeof(GLfloat), data);

			length = std::min(length + 1, maxLength);
			++curIndex;
			curIndex %= maxLength;
		}

		void UnitTrail::Draw() const
		{
			//If line's buffer is not full yet, render from beginning. Or the points are being overwritten, then use
			//curIndex to mark first index to be rendered(explained in ctor)
			void* firstIndexOffset = length != maxLength ?
				nullptr : reinterpret_cast<void*>(curIndex * sizeof(GLuint));

			glBindVertexArray(VAO);
			glDrawElements(GL_LINE_STRIP, length, GL_UNSIGNED_INT, firstIndexOffset);
			glBindVertexArray(0);
		}
		void UnitTrail::Clear()
		{
			//Just mark it as clear, no need to clear VBO, it will overwrite itself by new data
			length = curIndex = 0;
		}
		void UnitTrail::DeleteBuffers()
		{
			glDeleteBuffers(1, &VBO);
			glDeleteVertexArrays(1, &VAO);
			if (refCount == 1)//If this was last instance of this class, delete IBO buffer
				glDeleteBuffers(1, &IBO);
		}
	}
}
