#ifndef VIEWS_APP_UI_HPP
#define VIEWS_APP_UI_HPP

#include <vector>
#include <memory>

#include "view.hpp"
#include "glfwrap/window.hpp"

namespace views
{
	class AppUI
	{
	public:
		AppUI(const glfw::Window& window);
		AppUI(const AppUI& other) = delete;
		AppUI(AppUI&& other) noexcept = delete;
		AppUI& operator=(const AppUI& other) = delete;
		AppUI& operator=(AppUI&& other) noexcept = delete;
		~AppUI();

		void add_view(std::unique_ptr<IView> view);

		void render() const;

	private:
		std::vector<std::unique_ptr<IView>> mViews{};
	};
}
#endif // VIEWS_APP_UI_HPP
