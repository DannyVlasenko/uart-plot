#include "connected_ble_devices_model.hpp"

namespace logic
{
	void ConnectedBleDevicesModel::connectDevice(uint64_t address)
	{
		mDevices.emplace(address, ble::GattDevice{ address });
		mDevices.at(address).discoverServices();
	}

	void ConnectedBleDevicesModel::disconnectDevice(uint64_t address)
	{
		mDevices.erase(address);
	}
}