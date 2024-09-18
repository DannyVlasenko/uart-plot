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
		if (!mViewModel.portControlsEnabled())
		{
			ImGui::BeginDisabled();
		}
		ImGui::SameLine();
		if (ImGui::Button("Close")) {
			mViewModel.onPortCloseButtonClicked();
		}
		if (!mViewModel.portControlsEnabled())
		{
			ImGui::EndDisabled();
		}
		ListBox("Opened ports", mViewModel.openedPorts(), mViewModel.selectedOpenedPort());
		if (!mViewModel.portControlsEnabled())
		{
			ImGui::BeginDisabled();
		}
		if (ImGui::Button("Read Params")) {
			mViewModel.onReadParamsClicked();
		}
		ImGui::SameLine();
		if (ImGui::Button("Write Params")) {
			mViewModel.onWriteParamsClicked();
		}
		ComboBox("Bits per second", mViewModel.baudRatesList(), mViewModel.selectedBaudRate());
		ImGui::DragInt("Data bits", &mViewModel.dataBits(), 1, 6, 9);
		ComboBox("Parity", mViewModel.paritiesList(), mViewModel.selectedParity());
		ComboBox("Stop bits", mViewModel.stopBitsList(), mViewModel.selectedStopBits());		
		if (!mViewModel.portControlsEnabled())
		{
			ImGui::EndDisabled();
		}
		if (ImGui::Button("Clear Log"))
		{
			mViewModel.onClearLogClicked();
		}
		if (ImGui::BeginChild("Port Log", ImVec2(0, 0), ImGuiChildFlags_Borders, ImGuiWindowFlags_HorizontalScrollbar))
		{
			for (const auto& logEntry : mViewModel.portLogEntries()) {
				ImGui::TextColored({ logEntry.Color.x, logEntry.Color.y,logEntry.Color.z,logEntry.Color.w }, logEntry.Text.c_str());
			}
		}
		ImGui::EndChild();
		ImGui::End();
	}
}
