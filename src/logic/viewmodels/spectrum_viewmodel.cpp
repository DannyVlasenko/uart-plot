#include "spectrum_viewmodel.hpp"

#include "models/data_source_model.hpp"
#include "kissfft.hh"

namespace logic
{
	void SpectrumViewModel::update()
	{
		if (mDataSourceModel.dataSources().size() != mLastSourceCount) {
			mAvailableDataSources.clear();
			mAvailableDataSources.reserve(mDataSourceModel.dataSources().size());
			for (const auto& name : mDataSourceModel.dataSources() | std::views::keys) {
				mAvailableDataSources.push_back(name);
			}
			mLastSourceCount = mDataSourceModel.dataSources().size();
		}
		const auto& dataSource = mDataSourceModel.dataSources().at(mAvailableDataSources.at(mSelectedDataSource));
		//const auto samplesRequired = static_cast<int>(mDuration * mSamplingFrequency);
		const auto offset = dataSource->size() > mSamplesNumber ? dataSource->size() - mSamplesNumber : 0;
		const auto data = dataSource->getData(offset, mSamplesNumber);
		const auto spectrumStartIndex = mSamplesNumber > data.size() ? mSamplesNumber - data.size() : 0;
		std::vector<std::complex<double>> complexInput(mSamplesNumber);
		std::ranges::copy(data | std::views::transform([](double real) {return std::complex<double>(real); }), complexInput.begin() + spectrumStartIndex);
		kissfft<double> fftTransform(mSamplesNumber, false);
		std::vector<std::complex<double>> complexSpectrum(mSamplesNumber);
		fftTransform.transform(complexInput.data(), complexSpectrum.data());
		mSpectrum.clear();
		mSpectrum.resize(mSamplesNumber / 2, 0.0);
		std::ranges::copy(complexSpectrum | std::views::take(mSamplesNumber / 2) | std::views::transform([](const auto& complex) { return std::abs(complex); }), mSpectrum.begin());
	}

	std::span<const double> SpectrumViewModel::data() const noexcept
	{
		return mSpectrum;
	}
}
