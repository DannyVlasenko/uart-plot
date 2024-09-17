#pragma once

#include "views/app_ui.hpp"
#include "views/port_configuration_view.hpp"

#include <vector>
#include <optional>

namespace logic
{
	class DockAreaViewModel : public views::IDockAreaViewModel
	{
	public:
		explicit DockAreaViewModel(views::IPortConfigurationViewModel& portConfigViewModel):
			mPortConfigViewModel(portConfigViewModel)
		{}

		std::span<const views::IView* const> views() const override {
			return mViews;
		}

		[[nodiscard]]
		bool portConfigViewVisible() const noexcept {
			return mPortConfigView.has_value();
		}

		void switchPortConfigViewVisibility();

		void addSignalView();

		void addSpectrumView();

	private:
		views::IPortConfigurationViewModel& mPortConfigViewModel;
		std::optional<views::PortConfigurationView> mPortConfigView;
		std::vector<const views::IView*> mViews;

		void updateViewList();
	};
}