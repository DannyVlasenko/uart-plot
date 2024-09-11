#ifndef OPENGL_CALL_HPP
#define OPENGL_CALL_HPP

#include "glerrors.hpp"

#define GLCall(x) \
	opengl::OpenGLError::clear(); \
	x; \
	opengl::OpenGLError::check(opengl::ErrorBehavior, #x)

#endif // OPENGL_CALL_HPP
