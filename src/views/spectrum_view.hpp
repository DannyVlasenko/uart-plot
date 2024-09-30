#pragma once

#include <span>
#include <string>
#include <vector>

#include "view.hpp"

namespace views
{
	class ISpectrumViewModel
	{
	public:
		virtual ~ISpectrumViewModel() = default;
		virtual [[nodiscard]] const std::string& plotName() const noexcept = 0;
		virtual [[nodiscard]] std::span<const std::string> availableDataSources() const noexcept = 0;
		virtual [[nodiscard]] size_t& selectedDataSource() noexcept = 0;
		//virtual [[nodiscard]] float& spectrumDuration() noexcept = 0;
		virtual [[nodiscard]] int& samplingFrequency() noexcept = 0;
		virtual [[nodiscard]] int& samplesNumber() noexcept = 0;
		virtual [[nodiscard]] std::span<const double> data() const noexcept = 0;
	};

	class SpectrumView : public IView
	{
	public:
		explicit SpectrumView(ISpectrumViewModel& viewModel)
			: mViewModel(viewModel)
		{}

		void render() const override;

	private:
		ISpectrumViewModel& mViewModel;
		mutable std::vector<double> mFrequencyData;
	};
}