#pragma once

#include "views/app_ui.hpp"
#include "views/port_configuration_view.hpp"
#include "views/signal_view.hpp"
#include "signal_viewmodel.hpp"

#include <vector>
#include <optional>

#include "spectrum_viewmodel.hpp"
#include "models/data_source_model.hpp"
#include "views/spectrum_view.hpp"

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
			for (auto& signalViewModel : mSpectrumViewModels) {
				signalViewModel.update();
			}
		}

		[[nodiscard]]
		bool portConfigViewVisible() const noexcept;

		void switchPortConfigViewVisibility();

		void addSignalView();

		void addSpectrumView();

	private:
		OpenedPortsModel& mPortsModel;
		DataSourceModel& mDataSourceModel;
		views::IPortConfigurationViewModel& mPortConfigViewModel;
		std::vector<SignalViewModel> mSignalViewModels;
		std::vector<SpectrumViewModel> mSpectrumViewModels;
		std::optional<views::PortConfigurationView> mPortConfigView;
		std::vector<views::SignalView> mSignalViews;
		std::vector<views::SpectrumView> mSpectrumViews;
		std::vector<const views::IView*> mViews;

		void updateViewList();
	};
}
