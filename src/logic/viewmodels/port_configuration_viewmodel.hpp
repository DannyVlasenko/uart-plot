#pragma once

#include "views/port_configuration_view.hpp"

#include <vector>
#include <string>

namespace logic
{
	class OpenedPortsModel;

	class PortConfigurationViewModel : public views::IPortConfigurationViewModel
	{
	public:
		explicit PortConfigurationViewModel(OpenedPortsModel& portsModel):
			mPortsModel(portsModel)
		{}

		void onPortRefreshButtonClicked() override;

		std::span<const std::string> availablePorts() const noexcept override
		{
			return mAvailablePortsDescriptions;
		}

		size_t& selectedAvailablePort() noexcept override
		{
			return mSelectedAvailablePort;
		}

		bool portOpenEnabled() noexcept override;

		void onPortOpenButtonClicked() override;

		std::span<const std::string> openedPorts() const noexcept override
		{
			return mOpenedPortsNames;
		}

		size_t& selectedOpenedPort() noexcept override
		{
			return mSelectedOpenedPort;
		}

		bool portControlsEnabled() noexcept override;

		std::span<const std::string> baudRatesList() const noexcept override
		{
			return mBaudRates;
		}

		size_t& selectedBaudRate() noexcept override
		{
			return mSelectedBaudRate;
		}

		std::span<const std::string> dataBitsList() const noexcept override
		{
			return mDataBits;
		}

		size_t& selectedDataBits() noexcept override
		{
			return mSelectedDataBits;
		}

		std::span<const std::string> paritiesList() const noexcept override
		{
			return mParities;
		}

		size_t& selectedParity() noexcept override
		{
			return mSelectedParity;
		}

		std::span<const std::string> stopBitsList() const noexcept override
		{
			return mStopBits;
		}

		size_t& selectedStopBits() noexcept override
		{
			return mSelectedStopBits;
		}

		std::span<const std::string> flowControlsList() const noexcept override
		{
			return mFlowControls;
		}

		size_t& selectedFlowControl() noexcept override
		{
			return mSelectedFlowControl;
		}

		void onPortCloseButtonClicked() override;

	private:
		OpenedPortsModel& mPortsModel;
		std::vector<std::string> mAvailablePortsDescriptions;
		std::vector<std::string> mAvailablePortsNames;
		size_t mSelectedAvailablePort{ 0 };
		std::vector<std::string> mOpenedPortsNames;
		size_t mSelectedOpenedPort{ 0 };
		std::vector<std::string> mBaudRates;
		size_t mSelectedBaudRate;
		std::vector<std::string> mDataBits;
		size_t mSelectedDataBits;
		std::vector<std::string> mParities;
		size_t mSelectedParity;
		std::vector<std::string> mStopBits;
		size_t mSelectedStopBits;
		std::vector<std::string> mFlowControls;
		size_t mSelectedFlowControl;
		void updateAvailablePortList();
		void updateOpenedPortList();
	};
}