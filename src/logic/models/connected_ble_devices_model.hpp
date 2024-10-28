#pragma once

#include "ble/win/ble.hpp"

#include <unordered_map>

namespace logic
{
	class ConnectedBleDevicesModel
	{
	public:
		void connectDevice(uint64_t address);

		void disconnectDevice(uint64_t address);

		[[nodiscard]] const std::unordered_map<uint64_t, ble::GattDevice>& devices() const noexcept 
		{
			return mDevices;
		}
	private:
		std::unordered_map<uint64_t, ble::GattDevice> mDevices;
	};
}