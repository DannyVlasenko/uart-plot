#include "signal_viewmodel.hpp"

#include "models/data_source_model.hpp"

namespace logic
{
	void SignalViewModel::update()
	{
		if (mDataSourceModel.dataSources().size() == mLastSourceCount) {
			return;
		}
		mAvailableDataSources.clear();
		mAvailableDataSources.reserve(mDataSourceModel.dataSources().size());
		for (const auto& name : mDataSourceModel.dataSources() | std::views::keys) {
			mAvailableDataSources.push_back(name);
		}
		mLastSourceCount = mDataSourceModel.dataSources().size();
	}

	std::span<const double> SignalViewModel::data() const noexcept
	{
		if (mAvailableDataSources.empty()) {
			return {};
		}
		return mDataSourceModel.dataSources().at(mAvailableDataSources.at(mSelectedDataSource))->getData();
	}
}
