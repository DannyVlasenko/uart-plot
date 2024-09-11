#ifndef GLFW_WINDOW_HPP
#define GLFW_WINDOW_HPP

#include <memory>
#include <vec2.hpp>

#include "GLFW/glfw3.h"


namespace glfw
{
	class Window
	{
	public:
		Window(int width, int height, const char* title, bool makeCurrent = true);

		static void processEvents();

		operator GLFWwindow* () const noexcept;

		void setSwapInterval(int screen_updates) const;

		void makeContextCurrent() const;

		[[nodiscard]]
		bool shouldClose() const;

		void swapBuffers() const;

		[[nodiscard]]
		glm::vec2 size() const;

		[[nodiscard]]
		glm::vec2 frameBufferSize() const;

		[[nodiscard]]
		bool isKeyPressed(int key_code) const;

		[[nodiscard]]
		bool mouseButtonPressed(int button) const;

		void enableRawCursor(bool enable) const;

		void disableCursor(bool disable) const;

		[[nodiscard]]
		glm::vec2 cursorPosition() const;

	private:
		std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> mWindow;
	};
}
#endif // GLFW_WINDOW_HPP
