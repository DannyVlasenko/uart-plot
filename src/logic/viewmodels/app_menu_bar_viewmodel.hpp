#pragma once

#include "views/app_menu_bar_view.hpp"
#include "viewmodels/dock_area_viewmodel.hpp"

namespace logic
{
	class AppMenuBarViewModel : public views::IAppMenuBarViewModel
	{
	public:
		explicit AppMenuBarViewModel(DockAreaViewModel& dockViewModel):
			mDockViewModel(dockViewModel)
		{}

		bool portConfigurationChecked() override;
		void onPortConfigurationClicked() override;
		bool bleConfigurationChecked() override;
		void onBleConfigurationClicked() override;
		void onNewSignalViewClicked() override;
		void onNewSpectrumViewClicked() override;

	private:
		DockAreaViewModel& mDockViewModel;
	};
}