#include "ble.hpp"

#include <map>
#include <ranges>
#include <future>


#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Devices.Bluetooth.Advertisement.h>
#include "winrt/Windows.Devices.Bluetooth.GenericAttributeProfile.h"

using winrt::Windows::Foundation::Collections::IVector;
using winrt::Windows::Foundation::Collections::IVectorView;
using winrt::Windows::Foundation::TimeSpan;
using winrt::Windows::Foundation::IAsyncOperation;
using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher;
using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementReceivedEventArgs;
using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEScanningMode;
using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcherStatus;
using winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCommunicationStatus;
using winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattDeviceService;
using winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattDeviceServicesResult;
using winrt::Windows::Devices::Bluetooth::BluetoothSignalStrengthFilter;
using winrt::Windows::Devices::Bluetooth::BluetoothConnectionStatus;
using winrt::Windows::Devices::Bluetooth::BluetoothLEDevice;

namespace
{
	std::string parseUuid(const winrt::guid& uuid) 
	{
		std::stringstream resultStream;
		resultStream << std::hex << std::uppercase;
		resultStream << std::setfill('0') << std::setw(2);
		resultStream << uuid.Data1 << "-";
		resultStream << uuid.Data2 << "-";
		resultStream << uuid.Data3 << "-";
		for (auto b : uuid.Data4) resultStream << b;
		return resultStream.str();
	}

	std::unordered_set<std::string> parseUuids(const IVector<winrt::guid>& uuids)
	{
		std::unordered_set<std::string> result;
		for (const auto& uuid : uuids)
		{
			result.insert(parseUuid(uuid));
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
					if (!args.IsConnectable()) {
						return;
					}
					const auto name = to_string(args.Advertisement().LocalName());
					auto uuids = parseUuids(args.Advertisement().ServiceUuids());
					std::lock_guard dataLock{ mDataMapMutex };
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
							.Address = args.BluetoothAddress(),
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
		std::map<uint64_t, AdvertisementData> mAddressToDataMap;
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

	struct GattService::Impl {
		GattDeviceService mService;
	};

	GattService::GattService(Impl* impl): mImpl(impl){}

	GattService::~GattService() = default;

	std::string GattService::uuid() const
	{
		return parseUuid(mImpl->mService.Uuid());
	}

	struct GattDevice::Impl {
		BluetoothLEDevice mDevice;
	};

	static BluetoothLEDevice deviceFromAddress(uint64_t address)
	{
		const auto deviceAsyncOperation = BluetoothLEDevice::FromBluetoothAddressAsync(address);
		if (deviceAsyncOperation == nullptr) {
			throw std::runtime_error("Failed to connect to device. Async operation is null.");
		}
		auto device = deviceAsyncOperation.get();
		if (device == nullptr) {
			throw std::runtime_error("Failed to connect to device. Device ptr is null.");
		}

		if (device.BluetoothAddress() != address) {
			throw std::runtime_error("Failed to connect to device. Wrong BLE device address.");
		}

		return device;
	}


	GattDevice::GattDevice(uint64_t address):
		mImpl(new Impl{ .mDevice = deviceFromAddress(address) })
	{}

	GattDevice::~GattDevice() = default;

	bool GattDevice::isConnected() const
	{
		return mImpl->mDevice.ConnectionStatus() == BluetoothConnectionStatus::Connected;
	}

	std::vector<GattService> GattDevice::discoverServices() const
	{
		std::promise<std::vector<GattService>> servicesPromise;
		auto futureResult = servicesPromise.get_future();
		mImpl->mDevice
			.GetGattServicesAsync()
			.Completed
			([servicesResult = std::move(servicesPromise), this]
			(IAsyncOperation<GattDeviceServicesResult> async_operation, auto) mutable
				{
					const auto asyncResult = async_operation.GetResults();
					if (asyncResult.Status() != GattCommunicationStatus::Success)
					{
						throw std::runtime_error("Cannot get all services of device.");
					}

					try
					{						
						std::vector<GattService> result;
						for (const auto& service : asyncResult.Services()) {
							result.emplace_back(new GattService::Impl{ .mService = service });
						}
						servicesResult.set_value(std::move(result));
					}
					catch (...)
					{
						try
						{
							servicesResult.set_exception(std::current_exception());
						}
						catch (...) {}
					}
				});
		return futureResult.get();

	}
}
