#include "Error.h"
#include <cassert>

//Macro magic to get strings for each GLenum error
#define SOLAR_GL_ERRORS_STRINGIFY_HELPER(X) #X
#define SOLAR_GL_ERROR_STRINGIFY(X) SOLAR_GL_ERRORS_STRINGIFY_HELPER(X)

namespace solar
{
	namespace openGL
	{

		std::string TranslateError(errors err) noexcept
		{
			//Helper map to do the translation
			const static std::map<errors, std::string> enumToText = {
				{errors::noError			,SOLAR_GL_ERROR_STRINGIFY(GL_NO_ERROR)},
				{errors::invalidEnum		,SOLAR_GL_ERROR_STRINGIFY(GL_INVALID_ENUM)},
				{errors::invalidValue		,SOLAR_GL_ERROR_STRINGIFY(GL_INVALID_VALUE)},
				{errors::invalidOperation	,SOLAR_GL_ERROR_STRINGIFY(GL_INVALID_OPERATION)},
				{errors::invalidFBOperation ,SOLAR_GL_ERROR_STRINGIFY(GL_INVALID_FRAMEBUFFER_OPERATION)},
				{errors::outOfMemory		,SOLAR_GL_ERROR_STRINGIFY(GL_OUT_OF_MEMORY)},
				{errors::stackUnderflow		,SOLAR_GL_ERROR_STRINGIFY(GL_STACK_UNDERFLOW)},
				{errors::stackOverflow		,SOLAR_GL_ERROR_STRINGIFY(GL_STACK_OVERFLOW)},};

			try
			{
				return enumToText.at(err);
			}
			catch (...)
			{
				assert("Forgot to add solar::openGL::errors enum to map in solar::openGL::Error class");
				return "UNKNOWN ERROR";
			}
		}

		errors CheckForError()
		{
			auto err = glGetError();
			return static_cast<errors>(err);
		}


	}
}
