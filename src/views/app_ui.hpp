#ifndef VIEWS_APP_UI_HPP
#define VIEWS_APP_UI_HPP

#include <vector>
#include <memory>

#include "view.hpp"
#include "glfwrap/window.hpp"

namespace views
{
	class IMainViewModel 
	{
	public:

		virtual ~IMainViewModel() = default;
		virtual IViewIterator begin() const = 0;
		virtual IViewIterator end() const = 0;
	};

	class AppUI
	{
	public:
		AppUI(const glfw::Window& window, const IMainViewModel& viewModel);
		AppUI(const AppUI& other) = delete;
		AppUI(AppUI&& other) noexcept = delete;
		AppUI& operator=(const AppUI& other) = delete;
		AppUI& operator=(AppUI&& other) noexcept = delete;
		~AppUI();

		void render() const;

	private:
		const IMainViewModel& mViewModel;
	};
}
#endif // VIEWS_APP_UI_HPP
