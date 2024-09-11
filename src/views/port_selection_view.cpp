#include "port_selection_view.hpp"

#include "imgui.h"

namespace 
{
	void ComboBox(const char *text, const std::span<const std::string_view>& elements, size_t& selected)
	{
		if (ImGui::BeginCombo(text, elements[selected].data()))
		{
			for (size_t n = 0; n < elements.size(); n++)
			{
				const bool is_selected = selected == n;
				if (ImGui::Selectable(elements[n].data(), is_selected)) {
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
	void PortSelectionView::render()
	{
		ImGui::Begin("Port selection");
		ComboBox("Port", mViewModel.portsDescriptions(), mViewModel.selectedPort());
		if (!mViewModel.portControlsEnabled())
		{
			ImGui::BeginDisabled();
		}
		ComboBox("Bits per second", mViewModel.baudRatesList(), mViewModel.selectedBaudRate());
		ComboBox("Data bits", mViewModel.dataBitsList(), mViewModel.selectedDataBits());
		ComboBox("Parity", mViewModel.paritiesList(), mViewModel.selectedParity());
		ComboBox("Stop bits", mViewModel.stopBitsList(), mViewModel.selectedStopBits());
		ComboBox("Flow control", mViewModel.flowControlsList(), mViewModel.selectedFlowControl());
		if (ImGui::Button(mViewModel.portOperationButtonText())) {
			mViewModel.onPortOperationButtonClicked();
		}
		if (!mViewModel.portControlsEnabled())
		{
			ImGui::EndDisabled();
		}
		ImGui::End();
	}
}
