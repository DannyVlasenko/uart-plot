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
		explicit PortConfigurationViewModel(OpenedPortsModel& portsModel);

		void onPortRefreshButtonClicked() override;

		std::span<const std::string> availablePorts() const noexcept override
		{
			return mAvailablePortsDescriptions;
		}

		std::optional<size_t>& selectedAvailablePort() noexcept override
		{
			return mSelectedAvailablePort;
		}

		bool portOpenEnabled() const noexcept override;

		void onPortOpenButtonClicked() override;

		std::span<const std::string> openedPorts() const noexcept override
		{
			return mOpenedPortsNames;
		}

		std::optional<size_t>& selectedOpenedPort() noexcept override
		{
			return mSelectedOpenedPort;
		}

		bool portControlsEnabled() const noexcept override;

		std::span<const std::string> baudRatesList() const noexcept override
		{
			return mBaudRates;
		}

		size_t& selectedBaudRate() noexcept override
		{
			return mSelectedBaudRate;
		}

		int& dataBits() noexcept override
		{
			return mDataBits;
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

		void onPortCloseButtonClicked() override;

		void onClearLogClicked() override;

		[[nodiscard]] std::span<const views::PortLogEntry> portLogEntries() override
		{
			return mPortLogEntries;
		}

		void onReadParamsClicked() override;

		void onWriteParamsClicked() override;

		[[nodiscard]] std::string portOpenButtonText() const override;

		[[nodiscard]] std::string portCloseButtonText() const override;

	private:
		OpenedPortsModel& mPortsModel;
		std::vector<std::string> mAvailablePortsDescriptions;
		std::vector<std::string> mAvailablePortsNames;
		std::optional<size_t> mSelectedAvailablePort{ std::nullopt };
		std::vector<std::string> mOpenedPortsNames;
		std::optional<size_t> mSelectedOpenedPort{ std::nullopt };
		const std::vector<std::string> mBaudRates;
		size_t mSelectedBaudRate{ 0 };
		int mDataBits{ 0 };
		const std::vector<std::string> mParities;
		size_t mSelectedParity{ 0 };
		const std::vector<std::string> mStopBits;
		size_t mSelectedStopBits{ 0 };
		std::vector<views::PortLogEntry> mPortLogEntries;

		void updateAvailablePortList();
		void updateOpenedPortList();
		void readPortParams();
		void writePortParams();
	};
}