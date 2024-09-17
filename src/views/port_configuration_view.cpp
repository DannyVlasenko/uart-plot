#include "port_configuration_view.hpp"

#include "imgui.h"

namespace 
{

	void ComboBox(const char *text, const std::span<const std::string>& elements, size_t& selected)
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

	void ListBox(const char *text, const std::span<const std::string>& elements, size_t& selected)
	{
		if (ImGui::BeginListBox(text))
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
			ImGui::EndListBox();
		}
	}
}

namespace views
{
	void PortConfigurationView::render() const
	{
		ImGui::Begin("Port Configuration");
		if (ImGui::Button("Refresh")) {
			mViewModel.onPortRefreshButtonClicked();
		}
		ListBox("Available ports", mViewModel.availablePorts(), mViewModel.selectedAvailablePort());
		if (!mViewModel.portOpenEnabled())
		{
			ImGui::BeginDisabled();
		}
		if (ImGui::Button("Open")) {
			mViewModel.onPortOpenButtonClicked();
		}
		if (!mViewModel.portOpenEnabled())
		{
			ImGui::EndDisabled();
		}

		ListBox("Opened ports", mViewModel.openedPorts(), mViewModel.selectedOpenedPort());
		if (!mViewModel.portControlsEnabled())
		{
			ImGui::BeginDisabled();
		}
		ComboBox("Bits per second", mViewModel.baudRatesList(), mViewModel.selectedBaudRate());
		ComboBox("Data bits", mViewModel.dataBitsList(), mViewModel.selectedDataBits());
		ComboBox("Parity", mViewModel.paritiesList(), mViewModel.selectedParity());
		ComboBox("Stop bits", mViewModel.stopBitsList(), mViewModel.selectedStopBits());
		ComboBox("Flow control", mViewModel.flowControlsList(), mViewModel.selectedFlowControl());
		if (ImGui::Button("Close")) {
			mViewModel.onPortCloseButtonClicked();
		}
		if (!mViewModel.portControlsEnabled())
		{
			ImGui::EndDisabled();
		}
		ImGui::End();
	}
}
