#include "app_menu_bar_viewmodel.hpp"

namespace logic
{
	bool AppMenuBarViewModel::portConfigurationChecked()
	{
		return mDockViewModel.portConfigViewVisible();
	}

	void AppMenuBarViewModel::onPortConfigurationClicked()
	{
		mDockViewModel.switchPortConfigViewVisibility();
	}

	void AppMenuBarViewModel::onNewSignalViewClicked()
	{
		mDockViewModel.addSignalView();
	}

	void AppMenuBarViewModel::onNewSpectrumViewClicked()
	{
		mDockViewModel.addSpectrumView();
	}
}