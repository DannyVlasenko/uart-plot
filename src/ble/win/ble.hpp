#pragma once

#include <vector>
#include <string>
#include <memory>
#include <unordered_set>

namespace ble
{
	struct AdvertisementData
	{
		std::string Name;
		uint64_t Address;
		int RSSI;
		std::unordered_set<std::string> ServicesUuids;
	};

	class AdvertisementScanner
	{
	public:
		AdvertisementScanner();
		~AdvertisementScanner();

		void start();
		void stop();
		bool isScanning() const;
		int inRangeThreshold() const;
		void setInRangeThreshold(int threshold);
		int outOfRangeThreshold() const;
		void setOutOfRangeThreshold(int threshold);
		int outOfRangeTimeoutSeconds() const;
		void setOutOfRangeTimeoutSeconds(int timeout);
		std::vector<AdvertisementData> activeAdvertisements() const;

	private:
		struct Impl;
		std::unique_ptr<Impl> mImpl;
	};

	class GattService {
	public:
		struct Impl;
		GattService(Impl*);
		GattService(GattService&&) noexcept = default;
		GattService& operator=(GattService&&) noexcept = default;
		~GattService();
		std::string uuid() const;
	private:
		std::unique_ptr<Impl> mImpl;
	};

	class GattDevice 
	{
	public:
		GattDevice(uint64_t address);
		~GattDevice();
		bool isConnected() const;
		std::vector<GattService> discoverServices() const;

	private:
		struct Impl;
		std::unique_ptr<Impl> mImpl;
	};
}