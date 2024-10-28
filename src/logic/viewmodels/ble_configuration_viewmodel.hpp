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
		[[nodiscard]] std::span<const std::string> advertisingDevices() const noexcept override 
		{
			return mAdvertisements;
		}
		[[nodiscard]] std::optional<size_t>& selectedAdvertisingDevice() noexcept override
		{
			return mSelectedAdvertisement;
		}
		[[nodiscard]] const char* scanButtonText() const noexcept override;
		void onConnectButtonClicked() override;
		[[nodiscard]] bool isConnectButtonEnabled() const noexcept override;
		void onDisconnectButtonClicked() override;
		[[nodiscard]] bool isDisconnectButtonEnabled() const noexcept override;
		[[nodiscard]] std::span<const std::string> connectedDevices() const noexcept override
		{
			return mConnectedDevices;
		}
		[[nodiscard]] std::optional<size_t>& selectedConnectedDevice() noexcept override
		{
			return mSelectedConnected;
		}

	private:
		ble::AdvertisementScanner mScanner;
		int mInRangeRssi{ -80 };
		int mOutOfRangeRssi{ -90 };
		int mOutOfRangeTimeout{ 5 };
		std::vector<std::string> mAdvertisements;
		std::optional<size_t> mSelectedAdvertisement;
		std::vector<std::string> mConnectedDevices;
		std::optional<size_t> mSelectedConnected;
	};
}