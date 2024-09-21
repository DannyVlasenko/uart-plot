#pragma once

#include "views/app_ui.hpp"
#include "views/port_configuration_view.hpp"
#include "views/signal_view.hpp"
#include <vector>
#include <optional>

namespace views
{
	class PortConfigurationView;
	class IPortConfigurationViewModel;
}

namespace logic
{
	class OpenedPortsModel;

	class DockAreaViewModel : public views::IDockAreaViewModel
	{
	public:
		DockAreaViewModel(views::IPortConfigurationViewModel& portConfigViewModel, OpenedPortsModel& portsModel):
			mPortConfigViewModel(portConfigViewModel),
			mPortsModel(portsModel)
		{}

		std::span<const views::IView* const> views() const override {
			return mViews;
		}

		[[nodiscard]]
		bool portConfigViewVisible() const noexcept;

		void switchPortConfigViewVisibility();

		void addSignalView();

		void addSpectrumView();

	private:
		views::IPortConfigurationViewModel& mPortConfigViewModel;
		OpenedPortsModel& mPortsModel;
		std::optional<views::PortConfigurationView> mPortConfigView;
		std::vector<views::SignalView> mSignalViews;
		std::vector<const views::IView*> mViews;

		void updateViewList();
	};
}
