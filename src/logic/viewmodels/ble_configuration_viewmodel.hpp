#pragma once

#include "views/ble_configuration_view.hpp"
#include "ble/win/ble.hpp"

namespace logic
{
	class BleConfigurationViewModel final : public views::IBleConfigurationViewModel
	{
	public:
		void update();
		void onScanButtonClicked() override;
		[[nodiscard]] std::span<const std::string> advertisingDevices() const noexcept override;
		[[nodiscard]] std::optional<size_t>& selectedAdvertisingDevice() noexcept override;
		[[nodiscard]] const char* scanButtonText() const noexcept override;

	private:
		ble::AdvertisementScanner mScanner;
		std::vector<std::string> mAdvertisements;
		std::optional<size_t> mSelectedAdvertisement;
	};
}