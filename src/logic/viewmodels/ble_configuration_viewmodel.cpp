#include "ble_configuration_viewmodel.hpp"

namespace logic
{
	void BleConfigurationViewModel::update()
	{
		mAdvertisements.clear();
		for (const auto& adv : mScanner.activeAdvertisements()) {
			mAdvertisements.push_back((adv.Name.empty() ? "N/A" : adv.Name) + "\nAddress: " + adv.Address + "\nRSSI: " + std::to_string(adv.RSSI));
		}
		if (mInRangeRssi != mScanner.inRangeThreshold()) {
			mScanner.setInRangeThreshold(mInRangeRssi);
			mInRangeRssi = mScanner.inRangeThreshold();
		}
		if (mOutOfRangeRssi != mScanner.outOfRangeThreshold()) {
			mScanner.setOutOfRangeThreshold(mOutOfRangeRssi);
			mOutOfRangeRssi = mScanner.outOfRangeThreshold();
		}
		if (mOutOfRangeTimeout != mScanner.outOfRangeTimeoutSeconds()) {
			mScanner.setOutOfRangeTimeoutSeconds(mOutOfRangeTimeout);
			mOutOfRangeTimeout = mScanner.outOfRangeTimeoutSeconds();
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

	bool BleConfigurationViewModel::rssiFilterControlsEnabled() const noexcept
	{
		return !mScanner.isScanning();
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
