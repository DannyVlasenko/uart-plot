#pragma once

#include <vector>
#include <string>
#include <memory>

namespace ble
{
	struct AdvertisementData
	{
		std::string Name;
		std::string Address;
		int RSSI;
		std::vector<std::string> ServicesUuids;
	};

	class AdvertisementScanner
	{
	public:
		AdvertisementScanner();
		~AdvertisementScanner();

		void start();
		void stop();
		bool isScanning() const;
		std::vector<AdvertisementData> activeAdvertisements() const;

	private:
		struct Impl;
		std::unique_ptr<Impl> mImpl;
	};
}