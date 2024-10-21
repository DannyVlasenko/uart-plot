#include "ble_configuration_viewmodel.hpp"

namespace logic
{
	void BleConfigurationViewModel::update()
	{
		mAdvertisements.clear();
		for (const auto& adv : mScanner.activeAdvertisements()) {
			mAdvertisements.push_back(adv.Name + " [" + adv.Address + "](" + std::to_string(adv.RSSI) + ")");
		}
	}

	void BleConfigurationViewModel::onScanButtonClicked()
	{
		if (mScanner.isScanning()) {
			mScanner.stop();
		}
		else {
			mScanner.start();
		}
	}

	std::span<const std::string> BleConfigurationViewModel::advertisingDevices() const noexcept
	{
		return mAdvertisements;
	}

	std::optional<size_t>& BleConfigurationViewModel::selectedAdvertisingDevice() noexcept
	{
		return mSelectedAdvertisement;
	}

	const char* BleConfigurationViewModel::scanButtonText() const noexcept
	{
		return mScanner.isScanning() ? "Stop" : "Scan";
	}
}
