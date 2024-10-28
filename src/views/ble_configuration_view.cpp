#include "ble_configuration_view.hpp"

#include "imgui.h"

#include <array>
#include <sstream>
#include <iomanip>

namespace
{
	std::string formatAddress(uint64_t uintAddress)
	{
		std::array<unsigned char, 6> addressArray
		{
			static_cast<unsigned char>((uintAddress & 0x0000FF0000000000) >> 40),
			static_cast<unsigned char>((uintAddress & 0x000000FF00000000) >> 32),
			static_cast<unsigned char>((uintAddress & 0x00000000FF000000) >> 24),
			static_cast<unsigned char>((uintAddress & 0x0000000000FF0000) >> 16),
			static_cast<unsigned char>((uintAddress & 0x000000000000FF00) >> 8),
			static_cast<unsigned char>((uintAddress & 0x00000000000000FF))
		};
		std::stringstream resultStream{};
		resultStream << std::hex << std::uppercase;
		const auto last_element_index = addressArray.size() - 1;

		for (size_t i = 0; i < last_element_index; i++) {
			resultStream << std::setfill('0') << std::setw(2) << static_cast<unsigned short>(addressArray[i]) << ":";
		}
		resultStream << std::setfill('0') << std::setw(2)
			<< static_cast<unsigned short>(addressArray[last_element_index]);
		return resultStream.str();
	}

	void ListBox(const char* text, const std::span<const views::AdvertisementData>& elements, std::optional<views::AdvertisementData>& selected)
	{
		if (ImGui::BeginListBox(text, ImVec2{0.0f, ((ImGui::GetTextLineHeight() + 2 ) * 3 + 1) * 4}))
		{
			for (const auto& element : elements)
			{
				const bool is_selected = selected == element;
				if (ImGui::Selectable((to_string(element) + "##" + text).c_str(), is_selected)) {
					selected = element;
				}

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndListBox();
		}
	}

	void ListBox(const char* text, const std::span<const std::string>& elements, std::optional<size_t>& selected)
	{
		if (ImGui::BeginListBox(text))
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
	std::string to_string(const AdvertisementData& data)
	{
		return (data.Name.empty() ? "N/A" : data.Name) + "\nAddress: " + formatAddress(data.Address) + "\nRSSI: " + std::to_string(data.RSSI);
	}

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
