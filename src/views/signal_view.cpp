#include "views/signal_view.hpp"

#include "imgui.h"
#include "implot.h"

#include <ranges>

namespace 
{
	void ComboBox(const char* text, const std::span<const std::string>& elements, size_t& selected)
	{
		const char* preview = elements.empty() ? "" : elements[selected].c_str();
		if (ImGui::BeginCombo(text, preview))
		{
			for (size_t n = 0; n < elements.size(); n++)
			{
				const bool is_selected = selected == n;
				if (ImGui::Selectable(elements[n].c_str(), is_selected)) {
					selected = n;
				}

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}

	double SampleNumberToTime(double number, void* user_data)
	{
		const auto samplingFreq = *static_cast<float*>(user_data);
		return number / samplingFreq;
	}

	double TimeToSampleNumber(double time, void* user_data)
	{
		const auto samplingFreq = *static_cast<float*>(user_data);
		return time * samplingFreq;
	}
}

namespace views
{
	void SignalView::render() const
	{
		if (ImGui::Begin("Signal1")) {
			ComboBox("Data source", mViewModel.availableDataSources(), mViewModel.selectedDataSource());
			if (ImGui::DragFloat("Sampling frequency, Hz##Signal1FSampling", &mSamplingFrequency, 1, 1, 1024, "%.f", ImGuiSliderFlags_AlwaysClamp)) {
				mTimeData.clear();
			}
			ImGui::Checkbox("Stick to latest", &mStickToLatest);
			if (mStickToLatest) {
				ImGui::SameLine();
				ImGui::DragFloat("Screen duration", &mStickDuration, 1, 1, 100, "%.f", ImGuiSliderFlags_AlwaysClamp);
			}
			ImPlot::BeginPlot(("##Signal1Samples" + mViewModel.plotName()).c_str(), ImVec2(-1,-1));
			ImPlot::SetupAxis(ImAxis_X1, "t, sec");
			ImPlot::SetupAxis(ImAxis_Y1, "A");
			ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0.0, std::numeric_limits<double>::max());
			const auto data = mViewModel.data();
			auto newTimes = std::views::iota(mTimeData.size(), data.size())
				| std::views::transform([&](size_t index) { return static_cast<double>(index) / mSamplingFrequency; });
			std::ranges::copy(newTimes, std::back_inserter(mTimeData));
			const auto dataDuration = static_cast<float>(data.size()) / mSamplingFrequency;
			if (mStickToLatest) {
				ImPlot::SetupAxisLimits(ImAxis_X1, dataDuration - mStickDuration, dataDuration, ImGuiCond_Always);
			}
			ImPlot::PlotLine("##Signal1A", mTimeData.data(), data.data(), data.size());
			ImPlot::EndPlot();
			ImGui::End();
		}
	}
}
