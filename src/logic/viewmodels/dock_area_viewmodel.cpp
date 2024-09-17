#include "dock_area_viewmodel.hpp"

namespace logic
{
	void DockAreaViewModel::switchPortConfigViewVisibility()
	{
		if (mPortConfigView.has_value()) {
			mPortConfigView.reset();
		}
		else {
			mPortConfigView.emplace(mPortConfigViewModel);
		}
		updateViewList();
	}

	void DockAreaViewModel::addSignalView()
	{
	}

	void DockAreaViewModel::addSpectrumView()
	{
	}

	void DockAreaViewModel::updateViewList()
	{
		mViews.clear();
		if (mPortConfigView.has_value()) {
			mViews.push_back(&mPortConfigView.value());
		}
	}
}