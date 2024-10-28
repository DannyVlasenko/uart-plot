#pragma once

#include "view.hpp"

#include <span>
#include <string>
#include <optional>

namespace views
{
	struct AdvertisementData 
	{
		uint64_t Address;
		std::string Name;
		int RSSI;
	};

	[[nodiscard]] std::string to_string(const AdvertisementData& data);

	inline [[nodiscard]] bool operator==(const AdvertisementData& lhs, const AdvertisementData& rhs) noexcept 
	{
		return lhs.Address == rhs.Address;
	}

	class IBleConfigurationViewModel
	{
	public:
		virtual ~IBleConfigurationViewModel() = default;
		virtual void onScanButtonClicked() = 0;
		virtual [[nodiscard]] bool rssiFilterControlsEnabled() const noexcept = 0;
		virtual [[nodiscard]] int& inRangeThreshold() noexcept = 0;
		virtual [[nodiscard]] int& outOfRangeThreshold() noexcept = 0;
		virtual [[nodiscard]] int& outOfRangeTimeout() noexcept = 0;
		virtual [[nodiscard]] std::span<const AdvertisementData> advertisingDevices() const noexcept = 0;
		virtual [[nodiscard]] std::optional<AdvertisementData>& selectedAdvertisingDevice() noexcept = 0;
		virtual [[nodiscard]] const char* scanButtonText() const noexcept = 0;
		virtual void onConnectButtonClicked() = 0;
		virtual [[nodiscard]] bool isConnectButtonEnabled() const noexcept = 0;
		virtual void onDisconnectButtonClicked() = 0;
		virtual [[nodiscard]] bool isDisconnectButtonEnabled() const noexcept = 0;
		virtual [[nodiscard]] std::span<const std::string> connectedDevices() const noexcept = 0;
		virtual [[nodiscard]] std::optional<size_t>& selectedConnectedDevice() noexcept = 0;
	};

	class BleConfigurationView final : public IView 
	{
	public:
		BleConfigurationView(IBleConfigurationViewModel& view_model) :
			mViewModel(view_model)
		{}
		void render() const override;

	private:
		IBleConfigurationViewModel& mViewModel;
	};
}