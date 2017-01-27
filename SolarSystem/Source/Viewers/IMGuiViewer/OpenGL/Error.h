#ifndef OPENGL_ERROR_532523569248_HEADER
#define OPENGL_ERROR_532523569248_HEADER

#include <string>
#include <GL/glew.h>
#include <map>
#include "Source/Common/Exception.h"

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

		//Translates enum to string
		std::string TranslateError(errors err) noexcept;

		//Exception class for signaling OpenGL errors
		class GLError : public Exception
		{
		public:
			///Dont use string ctors for now
			//using Exception::Exception;
			GLError::GLError(errors err) :Exception(TranslateError(err)),error(err) {}
			errors GetErrType() { return error; }
		private:
			errors error;
		};

		errors CheckForError();
		//Throws GLError
		template<typename... CleanUp> void ThrowOnError(CleanUp&&... callBeforeThrow)
		{
			auto err = CheckForError();
			if (err != errors::noError)
			{
				using expand = bool[];
				//Calls all passed functions
				expand{true,((void)callBeforeThrow(), true) ...};
				throw GLError(err);
			}
		}

		//DEBUG ONLY
		inline errors CheckErrorDBG()
		{
#ifdef _DEBUG
			return CheckForError();
#else
			return errors::noError;
#endif
		}

		//DEBUG ONLY, throws GLError
		template<typename... CleanUp>
		inline void ThrowOnErrorDBG(CleanUp&&... callBeforeThrow)
		{
#ifdef _DEBUG
			ThrowOnError(std::forward<CleanUp...>(callBeforeThrow)...);
#endif
		}
	}
}


#endif