#include "ble_configuration_viewmodel.hpp"

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
}

namespace logic
{
	void BleConfigurationViewModel::update()
	{
		mAdvertisements.clear();
		for (const auto& adv : mScanner.activeAdvertisements()) {
			mAdvertisements.push_back((adv.Name.empty() ? "N/A" : adv.Name) + "\nAddress: " + formatAddress(adv.Address) + "\nRSSI: " + std::to_string(adv.RSSI));
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

	const char* BleConfigurationViewModel::scanButtonText() const noexcept
	{
		return mScanner.isScanning() ? "Stop" : "Scan";
	}

	void BleConfigurationViewModel::onConnectButtonClicked()
	{

	}

	bool BleConfigurationViewModel::isConnectButtonEnabled() const noexcept
	{
		return false;
	}

	void BleConfigurationViewModel::onDisconnectButtonClicked()
	{

	}

	bool BleConfigurationViewModel::isDisconnectButtonEnabled() const noexcept
	{
		return false;
	}
}
