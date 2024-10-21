#include "ble_configuration_view.hpp"

#include "imgui.h"

namespace
{
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
	void BleConfigurationView::render() const
	{
		if (ImGui::Begin("Port Configuration"))
		{
			if (ImGui::Button(mViewModel.scanButtonText())) {
				mViewModel.onScanButtonClicked();
			}
			ListBox("Advertising devices", mViewModel.advertisingDevices(), mViewModel.selectedAdvertisingDevice());
			ImGui::End();
		}
	}
}
