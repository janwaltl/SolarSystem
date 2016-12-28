#ifndef OPENGL_ERROR_532523569248_HEADER
#define OPENGL_ERROR_532523569248_HEADER

#include <string>
#include <GL/glew.h>
#include <map>
#include "Source/Exception.h"

namespace solar
{
	namespace openGL
	{
		enum class errors
		{
			noError = GL_NO_ERROR,
			invalidEnum = GL_INVALID_ENUM,
			invalidValue = GL_INVALID_VALUE,
			invalidOperation = GL_INVALID_OPERATION,
			invalidFBOperation = GL_INVALID_FRAMEBUFFER_OPERATION,
			outOfMemory = GL_OUT_OF_MEMORY,
			stackUnderflow = GL_STACK_UNDERFLOW,
			stackOverflow = GL_STACK_OVERFLOW,
		};

		//Exception class for signaling OpenGL errors
		class GLError : public Exception
		{
		public:
			using Exception::Exception;
			GLError(errors err);
		};

		//Translates enum to string
		std::string Translate(errors err) noexcept;

		errors CheckForError();
		//Throws GLError
		void ThrowOnError();

		//DEBUG ONLY
		inline errors CheckErrorDBG()
		{
#ifdef _DEBUG
			return CheckForError();
#endif
		}

		//DEBUG ONLY, throws GLError
		inline void ThrowErrorDBG()
		{
#ifdef _DEBUG
			ThrowOnError();
#endif
		}
	}
}


#endif