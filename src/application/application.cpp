#include "opengl/glcall.hpp"
#include "application.hpp"
#include "views/app_ui.hpp"
#include "views/app_menu_bar_view.hpp"

#include <stdexcept>

#include "logic/models/opened_ports_model.hpp"
#include "logic/viewmodels/app_menu_bar_viewmodel.hpp"
#include "logic/viewmodels/dock_area_viewmodel.hpp"
#include "logic/viewmodels/port_configuration_viewmodel.hpp"
#include "logic/viewmodels/ble_configuration_viewmodel.hpp"
#include "models/data_source_model.hpp"

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
		logic::DataSourceModel dataSourceModel;
		logic::OpenedPortsModel portsModel{ dataSourceModel };
		logic::PortConfigurationViewModel portConfigVM{ portsModel };
		logic::BleConfigurationViewModel bleConfigVM{};
		logic::DockAreaViewModel dockVM{ portConfigVM, bleConfigVM, portsModel, dataSourceModel };
		logic::AppMenuBarViewModel appMenuBarVM{ dockVM };
		views::AppUI ui{ mMainWindow, appMenuBarVM, dockVM };

		GLCall(glEnable(GL_MULTISAMPLE));
		GLCall(glClearColor(0.3f, 0.3f, 0.3f, 1.0f));
		
		while (!mMainWindow.shouldClose()) {
			glfw::Window::processEvents();
			dataSourceModel.update();
			dockVM.update();
			bleConfigVM.update();
			GLCall(glClear(GL_COLOR_BUFFER_BIT));
			ui.render();
			mMainWindow.swapBuffers();
		}
	}
}
