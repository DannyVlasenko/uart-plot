#pragma once

#include "views/spectrum_view.hpp"
#include <span>
#include <string>
#include <vector>

namespace logic
{
	class DataSourceModel;
	class SpectrumViewModel : public views::ISpectrumViewModel
	{
	public:
		explicit SpectrumViewModel(DataSourceModel& dataSourceModel) :
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

		/*[[nodiscard]] float& spectrumDuration() noexcept override
		{
			return mDuration;
		}*/

		[[nodiscard]] int& samplingFrequency() noexcept override
		{
			return mSamplingFrequency;
		}

		[[nodiscard]] int& samplesNumber() noexcept override
		{
			return mSamplesNumber;
		}

	private:
		std::string mName{ "Spectrum1" };
		size_t mLastSourceCount{ 0 };
		DataSourceModel& mDataSourceModel;
		std::vector<std::string> mAvailableDataSources;
		size_t mSelectedDataSource{ 0 };
		std::vector<double> mSpectrum;
		//float mDuration{ 20.0 };
		int mSamplingFrequency{ 500 };
		int mSamplesNumber{ 2048 };
	};
}
