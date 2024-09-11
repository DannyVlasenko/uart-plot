#include "opengl/glcall.hpp"
#include "application.hpp"
#include "views/app_ui.hpp"
#include "views/port_selection_view.hpp"

#include <stdexcept>

class PortSelectionViewModel : public views::IPortSelectionViewModel
{
public:
	const std::span<const std::string_view>& portsDescriptions() const noexcept override
	{
		static std::string_view Str[] = { "<none>" };
		static std::span<const std::string_view> None = { Str };
		return None;
	}
	size_t& selectedPort() noexcept override
	{
		static size_t Selected = 0;
		return Selected;
	}
	bool portControlsEnabled() noexcept override
	{
		return false;
	}
	const std::span<const std::string_view>& baudRatesList() const noexcept override
	{
		static std::string_view Str[] = { "<none>" };
		static std::span<const std::string_view> None = { Str };
		return None;
	}
	size_t& selectedBaudRate() noexcept override
	{
		static size_t Selected = 0;
		return Selected;
	}
	const std::span<const std::string_view>& dataBitsList() const noexcept override
	{
		static std::string_view Str[] = { "<none>" };
		static std::span<const std::string_view> None = { Str };
		return None;
	}
	size_t& selectedDataBits() noexcept override
	{
		static size_t Selected = 0;
		return Selected;
	}
	const std::span<const std::string_view>& paritiesList() const noexcept override
	{
		static std::string_view Str[] = { "<none>" };
		static std::span<const std::string_view> None = { Str };
		return None;
	}
	size_t& selectedParity() noexcept override
	{
		static size_t Selected = 0;
		return Selected;
	}
	const std::span<const std::string_view>& stopBitsList() const noexcept override
	{
		static std::string_view Str[] = { "<none>" };
		static std::span<const std::string_view> None = { Str };
		return None;
	}
	size_t& selectedStopBits() noexcept override
	{
		static size_t Selected = 0;
		return Selected;
	}
	const std::span<const std::string_view>& flowControlsList() const noexcept override
	{
		static std::string_view Str[] = { "<none>" };
		static std::span<const std::string_view> None = { Str };
		return None;
	}
	size_t& selectedFlowControl() noexcept override
	{
		static size_t Selected = 0;
		return Selected;
	}
	const char* portOperationButtonText() noexcept override
	{
		return "Open";
	}
	void onPortOperationButtonClicked() noexcept override
	{
		
	}
};

namespace uart_plot
{
	Application::Application()
	{
		mMainWindow.makeContextCurrent();
#ifdef _WIN32
		if (glewInit() != GLEW_OK)
		{
			throw std::runtime_error("GLEW init error.");
		}
#endif
	}

	const char* Application::glVersion() const noexcept
	{
		return reinterpret_cast<const char*>(glGetString(GL_VERSION));
	}

	void Application::run() const
	{
		PortSelectionViewModel portSelectionViewModel;
		views::AppUI ui{mMainWindow};
		ui.add_view(std::make_unique<views::PortSelectionView>(portSelectionViewModel));

		GLCall(glEnable(GL_MULTISAMPLE));
		GLCall(glClearColor(0.3f, 0.3f, 0.3f, 1.0f));
		
		while (!mMainWindow.shouldClose()) {
			glfw::Window::processEvents();
			//portSelectionViewModel.update();
			GLCall(glClear(GL_COLOR_BUFFER_BIT));
			ui.render();
			mMainWindow.swapBuffers();
		}
	}
}
