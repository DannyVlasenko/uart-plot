#include "ble_configuration_viewmodel.hpp"
#include "logic/models/connected_ble_devices_model.hpp"

#include <ranges>

namespace logic
{
	BleConfigurationViewModel::BleConfigurationViewModel(ConnectedBleDevicesModel& devicesModel):
		mDevicesModel(devicesModel)
	{}

	void BleConfigurationViewModel::update()
	{
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

		mAdvertisements.clear();
		for (const auto& adv : mScanner.activeAdvertisements()) {
			mAdvertisements.push_back(views::AdvertisementData{.Address = adv.Address, .Name = adv.Name, .RSSI = adv.RSSI});
		}

		mConnectedDevices.clear();
		for (const auto& device : mDevicesModel.devices() | std::views::values) {
			mConnectedDevices.push_back(views::ConnectedDevice{ .Address = device.address(), .Name = device.name(), .InRange = device.isConnected() });
			const auto services = device.discoverServices();
			mConnectedDevices.back().Services.clear();
			for (const auto& service : services) {
				mConnectedDevices.back().Services.push_back(views::ConnectedDevice::Service{ service.uuid() });
			}
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

	const char* BleConfigurationViewModel::scanButtonText() const noexcept
	{
		return mScanner.isScanning() ? "Stop" : "Scan";
	}

	void BleConfigurationViewModel::onConnectButtonClicked()
	{
		mDevicesModel.connectDevice(mSelectedAdvertisement->Address);
	}

	bool BleConfigurationViewModel::isConnectButtonEnabled() const noexcept
	{
		return mSelectedAdvertisement.has_value() && std::ranges::find(mAdvertisements, mSelectedAdvertisement.value()) != mAdvertisements.end();
	}

	void BleConfigurationViewModel::onDisconnectButtonClicked()
	{
		mDevicesModel.disconnectDevice(mSelectedConnected->Address);
	}

	bool BleConfigurationViewModel::isDisconnectButtonEnabled() const noexcept
	{
		return mSelectedConnected.has_value() && std::ranges::find(mConnectedDevices, mSelectedConnected.value()) != mConnectedDevices.end();
	}
}
