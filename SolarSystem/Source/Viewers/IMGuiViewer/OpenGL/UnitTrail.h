#ifndef OPENGLBACKEND_UNIT_TRAIL_HEADER
#define OPENGLBACKEND_UNIT_TRAIL_HEADER

#include "Source/Math/Math.h"
#include <algorithm>
namespace solar
{
	namespace openGL
	{
		//Line trail behind unit
		//Stores and renders maxLength amount of points as line strip behind the unit
		//Each call to Push add new line point, when maxLength is reached, new points overwrite the oldest ones
		// = like a queue
		class UnitTrail
		{
		public:
			//Construct empty line trail, creates VBO, VAO
			UnitTrail();
			UnitTrail(const UnitTrail&) = delete;
			UnitTrail(UnitTrail&& other);
			UnitTrail& operator=(const UnitTrail&) = delete;
			UnitTrail& operator=(UnitTrail&& other);
			//Frees VBO, VAO and IBO if this was last instance of class
			~UnitTrail();
			//Adds new line point to trail
			void Push(const Vec2d& newPos);
			//Draws UnitTrail, appropriate shader MUST be bind before call
			void Draw() const;
			//Removes all points from trail
			void Clear();
			friend void swap(UnitTrail& left, UnitTrail& right) noexcept;
		private:
			void DeleteBuffers();
			// #instances of this class, for managing lifetime of IBO
			static size_t refCount;
			//Indices buffer for the trail
			static unsigned int IBO;

			unsigned int VAO, VBO;
			//Number of indices of the trail
			size_t length;
			// Offset to indices array denoting which index is first to be rendered.
			size_t curIndex;
		};

		inline void swap(UnitTrail& left, UnitTrail& right) noexcept
		{
			using std::swap;
			swap(left.VAO, right.VAO);
			swap(left.VBO, right.VBO);
			swap(left.length, right.length);
			swap(left.curIndex, right.curIndex);
		}
	}
}


#endif