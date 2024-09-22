#include "views/signal_view.hpp"

#include "imgui.h"
#include "implot.h"

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
	void SignalView::render() const
	{
		if (ImGui::Begin("Signal1")) {
			ComboBox("Data source", mViewModel.availableDataSources(), mViewModel.selectedDataSource());
			ImPlot::BeginPlot(("Samples" + mViewModel.plotName()).c_str());
			ImPlot::SetupAxes("x", "y");
			ImPlot::PlotLine("A", mViewModel.data().data(), static_cast<int>(mViewModel.data().size()));
			ImPlot::EndPlot();
			ImGui::End();
		}
	}
}
