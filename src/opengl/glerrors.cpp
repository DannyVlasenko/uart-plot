#include "glerrors.hpp"

#include <cassert>
#include <iostream>

namespace opengl
{
	const char* OpenGLError::error_msg_from_code(GLenum err) noexcept
	{
		switch (err)
		{
		case GL_NO_ERROR:
			return "(GL_NO_ERROR) No error has been recorded.";

		case GL_INVALID_ENUM:
			return "(GL_INVALID_ENUM) An unacceptable value is specified for an enumerated argument.";

		case GL_INVALID_VALUE:
			return "(GL_INVALID_VALUE) A numeric argument is out of range.";

		case GL_INVALID_OPERATION:
			return "(GL_INVALID_OPERATION) The specified operation is not allowed in the current state.";

		case GL_INVALID_FRAMEBUFFER_OPERATION:
			return "(GL_INVALID_FRAMEBUFFER_OPERATION) The framebuffer object is not complete.";

		case GL_OUT_OF_MEMORY:
			return "(GL_OUT_OF_MEMORY) There is not enough memory left to execute the command.The state of the GL is undefined, except for the state of the error flags.";
#ifdef _WIN32
		case GL_STACK_UNDERFLOW:
			return "(GL_STACK_UNDERFLOW) An attempt has been made to perform an operation that would cause an internal stack to underflow.";

		case GL_STACK_OVERFLOW:
			return "(GL_STACK_OVERFLOW) An attempt has been made to perform an operation that would cause an internal stack to overflow.";
#endif
		default:
			return "Unspecified error.";
		}
	}

	void OpenGLError::clear()
	{
		while (glGetError() != GL_NO_ERROR);
	}

	void ThrowErrorBehavior::report(const char* msg, const char* expr)
	{
		throw OpenGLError(msg);
	}

	void DebugBreakBehavior::report(const char* msg, const char* expr)
	{
		std::cout << "Graphics error in expression \'" << expr << "\': ";
		std::cout << msg << std::endl;
		assert(false);
	}

	DefaultErrorBehavior ErrorBehavior{};
}
