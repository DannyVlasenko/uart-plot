#ifndef UART_PLOT_APPLICATION_HPP
#define UART_PLOT_APPLICATION_HPP

#include "glfwrap/library.hpp"
#include "glfwrap/window.hpp"

namespace uart_plot
{
	class Application : glfw::EnableLibrary
	{
	public:
		Application();

		[[nodiscard]]
		const char* glVersion() const noexcept;

		void run() const;

	private:
		const glfw::Window mMainWindow{ 1920, 1080, "UART Samples Plotter" };
	};

}
#endif // UART_PLOT_APPLICATION_HPP
