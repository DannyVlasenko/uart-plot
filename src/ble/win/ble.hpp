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
		std::string Address;
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
}