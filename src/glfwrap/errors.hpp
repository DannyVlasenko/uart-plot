#ifndef GLFW_ERRORS_HPP
#define GLFW_ERRORS_HPP

#include <stdexcept>

namespace glfw
{
	class LibraryError : public std::runtime_error
	{
	public:
		using std::runtime_error::runtime_error;

		static void checkLastError();
	};
}
#endif // GLFW_ERRORS_HPP
