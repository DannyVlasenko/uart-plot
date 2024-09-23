#pragma once

#include <vector>

#include "views/signal_view.hpp"

namespace logic
{
	class DataSourceModel;
	class SignalViewModel : public views::ISignalViewModel
	{
	public:
		explicit SignalViewModel(DataSourceModel& dataSourceModel):
			mDataSourceModel(dataSourceModel)
		{}

		void update();

		[[nodiscard]] const std::string& plotName() const noexcept override
		{
			return mName;
		}

		[[nodiscard]] std::span<const std::string> availableDataSources() const noexcept override
		{
			return mAvailableDataSources;
		}

		[[nodiscard]] size_t& selectedDataSource() noexcept override
		{
			return mSelectedDataSource;
		}

		[[nodiscard]] std::span<const double> data() const noexcept override;

	private:
		std::string mName{"Signal1"};
		size_t mLastSourceCount{ 0 };
		DataSourceModel& mDataSourceModel;
		std::vector<std::string> mAvailableDataSources;
		size_t mSelectedDataSource{0};
	};
}
