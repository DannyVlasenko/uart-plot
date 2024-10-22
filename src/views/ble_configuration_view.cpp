#include "ble_configuration_view.hpp"

#include "imgui.h"

namespace
{
	void ListBox(const char* text, const std::span<const std::string>& elements, std::optional<size_t>& selected)
	{
		if (ImGui::BeginListBox(text, ImVec2{0.0f, ImGui::GetTextLineHeightWithSpacing() * 3 * 5 }))
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
		}
		ImGui::End();
	}
}
