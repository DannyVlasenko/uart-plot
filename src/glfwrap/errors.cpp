#include "errors.hpp"

#include "GLFW/glfw3.h"

namespace glfw
{
	void LibraryError::checkLastError()
	{
		const char* error;
		if (glfwGetError(&error) != GLFW_NO_ERROR)
		{
			throw LibraryError(error);
		}
	}
}
