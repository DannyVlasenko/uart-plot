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
		virtual [[nodiscard]] std::span<const std::string> advertisingDevices() const noexcept = 0;
		virtual [[nodiscard]] std::optional<size_t>& selectedAdvertisingDevice() noexcept = 0;
		virtual [[nodiscard]] const char* scanButtonText() const noexcept = 0;
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