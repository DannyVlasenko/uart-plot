#include "ble_configuration_view.hpp"

#include "imgui.h"

namespace
{
	void ListBox(const char* text, const std::span<const std::string>& elements, std::optional<size_t>& selected)
	{
		if (ImGui::BeginListBox(text, ImVec2{0.0f, ((ImGui::GetTextLineHeight() + 2 ) * 3 + 1) * 4}))
		{
			for (size_t n = 0; n < elements.size(); n++)
			{
				const bool is_selected = selected == n;
				if (ImGui::Selectable((elements[n] + "##" + text).c_str(), is_selected)) {
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
	void BleConfigurationView::render() const
	{
		if (ImGui::Begin("BLE Devices"))
		{
			if (ImGui::Button(mViewModel.scanButtonText())) {
				mViewModel.onScanButtonClicked();
			}
			if (!mViewModel.rssiFilterControlsEnabled()) {
				ImGui::BeginDisabled();
			}
			ImGui::DragInt("In range RSSI", &mViewModel.inRangeThreshold(), 1, -127, 20);
			ImGui::DragInt("Out of range RSSI", &mViewModel.outOfRangeThreshold(), 1, -127, 20);
			ImGui::DragInt("Timeout, s", &mViewModel.outOfRangeTimeout(), 1, 1, 60);
			if (!mViewModel.rssiFilterControlsEnabled()) {
				ImGui::EndDisabled();
			}
			ListBox("Advertising devices", mViewModel.advertisingDevices(), mViewModel.selectedAdvertisingDevice());
			if (!mViewModel.isConnectButtonEnabled()) {
				ImGui::BeginDisabled();
			}
			if (ImGui::Button("Connect")) {
				mViewModel.onConnectButtonClicked();
			}
			if (!mViewModel.isConnectButtonEnabled()) {
				ImGui::EndDisabled();
			}
			ImGui::SameLine();
			if (!mViewModel.isDisconnectButtonEnabled()) {
				ImGui::BeginDisabled();
			}
			if (ImGui::Button("Disconnect")) {
				mViewModel.onDisconnectButtonClicked();
			}
			if (!mViewModel.isDisconnectButtonEnabled()) {
				ImGui::EndDisabled();
			}
			ListBox("Connected devices", mViewModel.connectedDevices(), mViewModel.selectedConnectedDevice());
		}
		ImGui::End();
	}
}
