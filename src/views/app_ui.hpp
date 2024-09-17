#ifndef VIEWS_APP_UI_HPP
#define VIEWS_APP_UI_HPP

#include <span>

#include "view.hpp"
#include "app_menu_bar_view.hpp"

namespace glfw
{
	class Window;
}

namespace views
{
	class IDockAreaViewModel 
	{
	public:

		virtual ~IDockAreaViewModel() = default;
		virtual std::span<const IView* const> views() const = 0;
	};

	class AppUI
	{
	public:
		AppUI(const glfw::Window& window, IAppMenuBarViewModel& menuBarModel, const IDockAreaViewModel& dockAreaModel);
		AppUI(const AppUI& other) = delete;
		AppUI(AppUI&& other) noexcept = delete;
		AppUI& operator=(const AppUI& other) = delete;
		AppUI& operator=(AppUI&& other) noexcept = delete;
		~AppUI();

		void render();

	private:
		AppMenuBarView mMenuBarView;
		const IDockAreaViewModel& mDockViewModel;
	};
}
#endif // VIEWS_APP_UI_HPP
