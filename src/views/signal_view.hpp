#pragma once

#include <span>
#include <string>
#include <vector>

#include "view.hpp"

namespace views
{
	class ISignalViewModel
	{
	public:
		virtual ~ISignalViewModel() = default;
		virtual [[nodiscard]] const std::string& plotName() const noexcept = 0;
		virtual [[nodiscard]] std::span<const std::string> availableDataSources() const noexcept = 0;
		virtual [[nodiscard]] size_t& selectedDataSource() noexcept = 0;
		virtual [[nodiscard]] std::span<const double> data() const noexcept = 0;
	};

	class SignalView : public IView
	{
	public:
		explicit SignalView(ISignalViewModel& viewModel)
			: mViewModel(viewModel)
		{}

		void render() const override;

	private:
		ISignalViewModel& mViewModel;
		mutable float mSamplingFrequency{ 500 };
		mutable bool mStickToLatest{ true };
		mutable float mStickDuration{ 30.0 };
		mutable std::vector<double> mTimeData;
	};
}
