#include "port_configuration_view.hpp"

#include "imgui.h"

#include <functional>

namespace 
{

	void ComboBox(const char *text, const std::span<const std::string>& elements, size_t& selected, std::function<void(size_t)> onSelected)
	{
		const char* preview = elements.empty() ? "" : elements[selected].c_str();
		if (ImGui::BeginCombo(text, preview))
		{
			for (size_t n = 0; n < elements.size(); n++)
			{
				const bool is_selected = selected == n;
				if (ImGui::Selectable(elements[n].c_str(), is_selected)) {
					if (selected != n) {
						selected = n;
						onSelected(n);
					}
				}

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}

	void ListBox(const char *text, const std::span<const std::string>& elements, std::optional<size_t>& selected, std::function<void(size_t)> onSelected)
	{
		if (ImGui::BeginListBox(text))
		{
			for (size_t n = 0; n < elements.size(); n++)
			{
				const bool is_selected = selected == n;
				if (ImGui::Selectable((elements[n]+"##"+text).c_str(), is_selected)) {
					if (selected != n) {
						selected = n;
						onSelected(n);
					}
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
		if (ImGui::Begin("Port Configuration")) {
			if (ImGui::Button("Refresh")) {
				mViewModel.onPortRefreshButtonClicked();
			}
			ListBox("Available ports", mViewModel.availablePorts(), mViewModel.selectedAvailablePort(),[](size_t){});
			if (!mViewModel.portOpenEnabled())
			{
				ImGui::BeginDisabled();
			}
			if (ImGui::Button(mViewModel.portOpenButtonText().c_str())) {
				mViewModel.onPortOpenButtonClicked();
			}
			if (!mViewModel.portOpenEnabled())
			{
				ImGui::EndDisabled();
			}
			const auto portControlsEnabled = mViewModel.portControlsEnabled();
			if (!portControlsEnabled)
			{
				ImGui::BeginDisabled();
			}
			ImGui::SameLine();
			if (ImGui::Button(mViewModel.portCloseButtonText().c_str())) {
				mViewModel.onPortCloseButtonClicked();
			}
			if (!portControlsEnabled)
			{
				ImGui::EndDisabled();
			}
			ListBox("Opened ports", mViewModel.openedPorts(), mViewModel.selectedOpenedPort(), [this](size_t) {
				mViewModel.onReadParamsClicked();
			});
			if (!portControlsEnabled)
			{
				ImGui::BeginDisabled();
			}
			ComboBox("Bits per second", mViewModel.baudRatesList(), mViewModel.selectedBaudRate(), [this](size_t) {
				mViewModel.onWriteParamsClicked();
			});
			if (ImGui::DragInt("Data bits", &mViewModel.dataBits(), 1, 6, 9)) {
				mViewModel.onWriteParamsClicked();
			}
			ComboBox("Parity", mViewModel.paritiesList(), mViewModel.selectedParity(), [this](size_t) {
				mViewModel.onWriteParamsClicked();
			});
			ComboBox("Stop bits", mViewModel.stopBitsList(), mViewModel.selectedStopBits(), [this](size_t) {
				mViewModel.onWriteParamsClicked();
			});
			if (!portControlsEnabled)
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
		}
		ImGui::End();
	}
}
