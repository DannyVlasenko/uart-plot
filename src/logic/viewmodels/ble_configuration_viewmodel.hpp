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
		[[nodiscard]] bool rssiFilterControlsEnabled() const noexcept override;
		[[nodiscard]] int& inRangeThreshold() noexcept override
		{
			return mInRangeRssi;
		}
		[[nodiscard]] int& outOfRangeThreshold() noexcept override
		{
			return mOutOfRangeRssi;
		}
		[[nodiscard]] int& outOfRangeTimeout() noexcept override
		{
			return mOutOfRangeTimeout;
		}
		[[nodiscard]] std::span<const std::string> advertisingDevices() const noexcept override;
		[[nodiscard]] std::optional<size_t>& selectedAdvertisingDevice() noexcept override;
		[[nodiscard]] const char* scanButtonText() const noexcept override;

	private:
		ble::AdvertisementScanner mScanner;
		int mInRangeRssi{ -80 };
		int mOutOfRangeRssi{ -90 };
		int mOutOfRangeTimeout{ 5 };
		std::vector<std::string> mAdvertisements;
		std::optional<size_t> mSelectedAdvertisement;
	};
}