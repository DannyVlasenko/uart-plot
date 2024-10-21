#include "ble.hpp"

#include <unordered_map>
#include <ranges>
#include <future>

#include "winrt/Windows.Devices.Bluetooth.Advertisement.h"
#include <winrt/Windows.Foundation.Collections.h>

using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher;
using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementReceivedEventArgs;
using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEScanningMode;
using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcherStatus;
using winrt::Windows::Foundation::Collections::IVector;

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
        std::stringstream resultStream;
        resultStream << std::hex << std::uppercase;
        const auto last_element_index = addressArray.size() - 1;

        for (size_t i = 0; i < last_element_index; i++) {
            resultStream << std::setfill('0') << std::setw(2) << static_cast<unsigned short>(addressArray[i]) << ":";
        }
        resultStream << std::setfill('0') << std::setw(2)
            << static_cast<unsigned short>(addressArray[last_element_index]);
        return resultStream.str();
    }

	std::vector<std::string> parseUuids(const IVector<winrt::guid>& uuids)
	{
		std::vector<std::string> result;
		for (const auto& uuid : uuids)
		{
			std::stringstream resultStream;
			resultStream << std::hex << std::uppercase;
			resultStream << std::setfill('0') << std::setw(2);
			resultStream << uuid.Data1 << "-";
			resultStream << uuid.Data2 << "-";
			resultStream << uuid.Data3 << "-";
			for (auto b : uuid.Data4) resultStream << b;
			result.push_back(resultStream.str());
		}
		return result;
	}
}

namespace ble
{
	struct AdvertisementScanner::Impl
	{
		Impl():
			mReceivedEventToken
			(
				mWatcher.Received([this](const BluetoothLEAdvertisementWatcher& watcher, const BluetoothLEAdvertisementReceivedEventArgs& args)
				{
					mAddressToDataMap.insert_or_assign(args.BluetoothAddress(), AdvertisementData
					{
						.Name = to_string(args.Advertisement().LocalName()),
						.Address = formatAddress(args.BluetoothAddress()),
						.RSSI = args.RawSignalStrengthInDBm(),
						.ServicesUuids = parseUuids(args.Advertisement().ServiceUuids())
					});
				})
			)
		{}

		BluetoothLEAdvertisementWatcher mWatcher;
		winrt::event_token mReceivedEventToken;
		std::unordered_map<uint64_t, AdvertisementData> mAddressToDataMap;
	};

	AdvertisementScanner::AdvertisementScanner()
	{
		/*if (winrt::impl::is_sta())
		{
			std::async(std::launch::async, []()
				{
					winrt::init_apartment();
				}).get();
		}
		else*/
		{
			winrt::init_apartment();
		}
		mImpl = std::make_unique<Impl>();
	}
	AdvertisementScanner::~AdvertisementScanner() = default;

	void AdvertisementScanner::start()
	{
		mImpl->mAddressToDataMap.clear();
		mImpl->mWatcher.Start();
	}

	void AdvertisementScanner::stop()
	{
		mImpl->mWatcher.Stop();
	}
	bool AdvertisementScanner::isScanning() const
	{
		return mImpl->mWatcher.Status() == BluetoothLEAdvertisementWatcherStatus::Started;
	}
	std::vector<AdvertisementData> AdvertisementScanner::activeAdvertisements() const
	{
		auto dataRng = mImpl->mAddressToDataMap | std::views::values;
		return {dataRng.begin(), dataRng.end()};
	}
}
