#pragma once

#include "view.hpp"

#include <span>
#include <string>
#include <optional>

namespace views
{
	class IBleConfigurationViewModel
	{
	public:
		virtual ~IBleConfigurationViewModel() = default;
		virtual void onScanButtonClicked() = 0;
		virtual [[nodiscard]] bool rssiFilterControlsEnabled() const noexcept = 0;
		virtual [[nodiscard]] int& inRangeThreshold() noexcept = 0;
		virtual [[nodiscard]] int& outOfRangeThreshold() noexcept = 0;
		virtual [[nodiscard]] int& outOfRangeTimeout() noexcept = 0;
		virtual [[nodiscard]] std::span<const std::string> advertisingDevices() const noexcept = 0;
		virtual [[nodiscard]] std::optional<size_t>& selectedAdvertisingDevice() noexcept = 0;
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