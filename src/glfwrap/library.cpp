#include "library.hpp"

#include "GLFW/glfw3.h"
#include "errors.hpp"

namespace glfw
{
	Library::Library()
	{
		if (!glfwInit())
		{
			LibraryError::checkLastError();
		}
		glfwWindowHint(GLFW_SAMPLES, 8);
	}

	Library::~Library()
	{
		glfwTerminate();
	}
}
