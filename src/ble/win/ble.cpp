#include "ble.hpp"

#include <unordered_map>
#include <ranges>
#include <future>


#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Devices.Bluetooth.Advertisement.h>
#include <winrt/Windows.Foundation.Collections.h>

using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher;
using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementReceivedEventArgs;
using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEScanningMode;
using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcherStatus;
using winrt::Windows::Devices::Bluetooth::BluetoothSignalStrengthFilter;
using winrt::Windows::Foundation::Collections::IVector;
using winrt::Windows::Foundation::TimeSpan;

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

	std::unordered_set<std::string> parseUuids(const IVector<winrt::guid>& uuids)
	{
		std::unordered_set<std::string> result;
		for (const auto& uuid : uuids)
		{
			std::stringstream resultStream;
			resultStream << std::hex << std::uppercase;
			resultStream << std::setfill('0') << std::setw(2);
			resultStream << uuid.Data1 << "-";
			resultStream << uuid.Data2 << "-";
			resultStream << uuid.Data3 << "-";
			for (auto b : uuid.Data4) resultStream << b;
			result.insert(resultStream.str());
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
					std::lock_guard dataLock{ mDataMapMutex };
					const auto name = to_string(args.Advertisement().LocalName());
					auto uuids = parseUuids(args.Advertisement().ServiceUuids());
					if (mAddressToDataMap.contains(args.BluetoothAddress())) {
						if (args.RawSignalStrengthInDBm() == -127) {
							mAddressToDataMap.erase(args.BluetoothAddress());
						}
						else {
							auto& entry = mAddressToDataMap.at(args.BluetoothAddress());
							entry.RSSI = args.RawSignalStrengthInDBm();
							if (entry.Name.empty() && !name.empty()) {
								entry.Name = name;
							}
							entry.ServicesUuids.merge(uuids);
						}
					}
					else {
						mAddressToDataMap.insert({ args.BluetoothAddress(), AdvertisementData
						{
							.Name = name,
							.Address = formatAddress(args.BluetoothAddress()),
							.RSSI = args.RawSignalStrengthInDBm(),
							.ServicesUuids = uuids
						}
						});
					}
				})
			)
		{
			mWatcher.AllowExtendedAdvertisements(true);
			mWatcher.ScanningMode(BluetoothLEScanningMode::Active);
		}

		std::mutex mDataMapMutex;
		BluetoothLEAdvertisementWatcher mWatcher;
		winrt::event_token mReceivedEventToken;
		std::unordered_map<uint64_t, AdvertisementData> mAddressToDataMap;
	};

	AdvertisementScanner::AdvertisementScanner()
	{		
		winrt::init_apartment();		
		mImpl = std::make_unique<Impl>();
		setInRangeThreshold(-127);
		setOutOfRangeThreshold(-127);
		setOutOfRangeTimeoutSeconds(60);
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
	int AdvertisementScanner::inRangeThreshold() const
	{
		return mImpl->mWatcher.SignalStrengthFilter().InRangeThresholdInDBm().Value();
	}
	void AdvertisementScanner::setInRangeThreshold(int threshold)
	{
		mImpl->mWatcher.SignalStrengthFilter().InRangeThresholdInDBm(threshold);
	}
	int AdvertisementScanner::outOfRangeThreshold() const
	{
		return mImpl->mWatcher.SignalStrengthFilter().OutOfRangeThresholdInDBm().Value();
	}
	void AdvertisementScanner::setOutOfRangeThreshold(int threshold)
	{
		mImpl->mWatcher.SignalStrengthFilter().OutOfRangeThresholdInDBm(threshold);
	}
	int AdvertisementScanner::outOfRangeTimeoutSeconds() const
	{
		return std::chrono::duration_cast<std::chrono::seconds>(mImpl->mWatcher.SignalStrengthFilter().OutOfRangeTimeout().Value()).count();
	}
	void AdvertisementScanner::setOutOfRangeTimeoutSeconds(int timeout)
	{
		mImpl->mWatcher.SignalStrengthFilter().OutOfRangeTimeout(std::chrono::duration_cast<TimeSpan>(std::chrono::seconds(timeout)));
	}
	std::vector<AdvertisementData> AdvertisementScanner::activeAdvertisements() const
	{
		std::lock_guard dataLock{ mImpl->mDataMapMutex };
		auto dataRng = mImpl->mAddressToDataMap | std::views::values;
		return {dataRng.begin(), dataRng.end()};
	}
}
