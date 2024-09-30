#include "spectrum_view.hpp"

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
}

namespace views
{
	void SpectrumView::render() const
	{
		if (ImGui::Begin(mViewModel.plotName().c_str())) {
			ComboBox("Data source", mViewModel.availableDataSources(), mViewModel.selectedDataSource());
			if (ImGui::DragInt(("Sampling frequency, Hz##FSampling" + mViewModel.plotName()).c_str(), &mViewModel.samplingFrequency(), 1, 1, 1024, "%d", ImGuiSliderFlags_AlwaysClamp)) {
				mFrequencyData.clear();
			}
			if (ImGui::DragInt(("Samples number##SampleNum" + mViewModel.plotName()).c_str(), &mViewModel.samplesNumber(), 1, mViewModel.samplingFrequency(), 4096, "%d", ImGuiSliderFlags_AlwaysClamp)) {
				mFrequencyData.clear();
			}
			//ImGui::DragFloat(("Spectrum duration, s##SpectrumDuration" + mViewModel.plotName()).c_str(), &mViewModel.spectrumDuration(), 1, 1, mViewModel.samplesNumber() / mViewModel.samplingFrequency(), "%.f", ImGuiSliderFlags_AlwaysClamp);
			if (mFrequencyData.empty()) {
				auto frequencies = std::views::iota(0, mViewModel.samplesNumber() / 2)
					| std::views::transform([&](int index) { return static_cast<double>(index) * mViewModel.samplingFrequency() / mViewModel.samplesNumber(); });
				std::ranges::copy(frequencies, std::back_inserter(mFrequencyData));
			}
			if (ImPlot::BeginPlot(("##SpectrumSamples" + mViewModel.plotName()).c_str(), ImVec2(-1, -1))) {
				ImPlot::SetupAxis(ImAxis_X1, "f, Hz");
				ImPlot::SetupAxis(ImAxis_Y1, "A");
				ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0.0, std::numeric_limits<double>::max());
				ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0.0, mViewModel.samplingFrequency() * 0.5);
				ImPlot::SetupAxisLimits(ImAxis_X1, 0.0, mViewModel.samplingFrequency() * 0.5, ImGuiCond_Always);
				const auto data = mViewModel.data();
				ImPlot::PlotShaded("##Spectrum1A", mFrequencyData.data(), data.data(), data.size());
				ImPlot::EndPlot();
			}
			ImGui::End();
		}
	}
}
