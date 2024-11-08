#include "dock_area_viewmodel.hpp"

#include "viewmodels/port_configuration_viewmodel.hpp"

namespace logic
{
	bool DockAreaViewModel::portConfigViewVisible() const noexcept
	{
		return mPortConfigView.has_value();
	}

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

	void DockAreaViewModel::switchBleConfigViewVisibility()
	{
		updateViewList();
	}

	void DockAreaViewModel::addSignalView()
	{
		mSignalViews.emplace_back(mSignalViewModels.emplace_back(mDataSourceModel));
		updateViewList();
	}

	void DockAreaViewModel::addSpectrumView()
	{
		mSpectrumViews.emplace_back(mSpectrumViewModels.emplace_back(mDataSourceModel));
		updateViewList();
	}

	void DockAreaViewModel::updateViewList()
	{
		mViews.clear();
		if (mPortConfigView.has_value()) {
			mViews.push_back(&mPortConfigView.value());
		}
		for (const auto& signalView : mSignalViews) {
			mViews.push_back(&signalView);
		}
		for (const auto& spectrumView : mSpectrumViews) {
			mViews.push_back(&spectrumView);
		}
	}
}
