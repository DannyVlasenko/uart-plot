#pragma once

#include "views/app_ui.hpp"
#include "views/port_configuration_view.hpp"
#include "views/signal_view.hpp"
#include "signal_viewmodel.hpp"

#include <vector>
#include <optional>

#include "models/data_source_model.hpp"

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
		DockAreaViewModel(views::IPortConfigurationViewModel& portConfigViewModel, OpenedPortsModel& portsModel, DataSourceModel& dataSourceModel):
			mPortsModel(portsModel),
			mDataSourceModel(dataSourceModel),
			mPortConfigViewModel(portConfigViewModel)
		{}

		std::span<const views::IView* const> views() const override {
			return mViews;
		}

		void update()
		{
			for (auto& signalViewModel : mSignalViewModels) {
				signalViewModel.update();
			}
		}

		[[nodiscard]]
		bool portConfigViewVisible() const noexcept;

		void switchPortConfigViewVisibility();

		void addSignalView();

	private:
		OpenedPortsModel& mPortsModel;
		DataSourceModel& mDataSourceModel;
		views::IPortConfigurationViewModel& mPortConfigViewModel;
		std::vector<SignalViewModel> mSignalViewModels;
		std::optional<views::PortConfigurationView> mPortConfigView;
		std::vector<views::SignalView> mSignalViews;
		std::vector<const views::IView*> mViews;

		void updateViewList();
	};
}
