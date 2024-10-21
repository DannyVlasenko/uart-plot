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

	bool DockAreaViewModel::bleConfigViewVisible() const noexcept
	{
		return mBleConfigView.has_value();
	}

	void DockAreaViewModel::switchBleConfigViewVisibility()
	{
		if (mBleConfigView.has_value()) {
			mBleConfigView.reset();
		}
		else {
			mBleConfigView.emplace(mBleConfigViewModel);
		}
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
		if (mBleConfigView.has_value()) {
			mViews.push_back(&mBleConfigView.value());
		}
		for (const auto& signalView : mSignalViews) {
			mViews.push_back(&signalView);
		}
		for (const auto& spectrumView : mSpectrumViews) {
			mViews.push_back(&spectrumView);
		}
	}
}
