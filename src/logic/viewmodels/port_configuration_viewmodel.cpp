#include "port_configuration_viewmodel.hpp"

#include <ranges>

#include "models/opened_ports_model.hpp"

using namespace std::string_literals;

namespace
{
	constexpr glm::vec4 ColorRed{ 1.0f, 0.0f, 0.0f, 0.7f };
	constexpr glm::vec4 ColorGreen{ 0.0f, 1.0f, 0.0f, 0.7f };

	const std::unordered_map<std::string, uart::BaudRate> NameToBaudRate{
		{"110",uart::BaudRate::BR110},
		{"300",uart::BaudRate::BR300},
		{"600",uart::BaudRate::BR600},
		{"1200",uart::BaudRate::BR1200},
		{"2400",uart::BaudRate::BR2400},
		{"4800",uart::BaudRate::BR4800},
		{"9600",uart::BaudRate::BR9600},
		{"14400",uart::BaudRate::BR14400},
		{"19200",uart::BaudRate::BR19200},
		{"38400",uart::BaudRate::BR38400},
		{"57600",uart::BaudRate::BR57600},
		{"115200",uart::BaudRate::BR115200},
		{"128000",uart::BaudRate::BR128000},
		{"256000",uart::BaudRate::BR256000}
	};

	const std::unordered_map<uart::BaudRate, std::string> BaudRateToName{
		{uart::BaudRate::BR110, "110"},
		{uart::BaudRate::BR300, "300"},
		{uart::BaudRate::BR600, "600"},
		{uart::BaudRate::BR1200, "1200"},
		{uart::BaudRate::BR2400, "2400"},
		{uart::BaudRate::BR4800, "4800"},
		{uart::BaudRate::BR9600, "9600"},
		{uart::BaudRate::BR14400, "14400"},
		{uart::BaudRate::BR19200, "19200"},
		{uart::BaudRate::BR38400, "38400"},
		{uart::BaudRate::BR57600, "57600"},
		{uart::BaudRate::BR115200, "115200"},
		{uart::BaudRate::BR128000, "128000"},
		{uart::BaudRate::BR256000, "256000"}
	};

	const std::unordered_map<std::string, uart::Parity> NameToParity{
		{"No",uart::Parity::NO},
		{"Odd",uart::Parity::ODD},
		{"Even",uart::Parity::EVEN},
		{"Mark",uart::Parity::MARK},
		{"Space",uart::Parity::SPACE}
	};

	const std::unordered_map<uart::Parity, std::string> ParityToName{
		{uart::Parity::NO, "No"},
		{uart::Parity::ODD, "Odd"},
		{uart::Parity::EVEN, "Even"},
		{uart::Parity::MARK, "Mark"},
		{uart::Parity::SPACE, "Space"}
	};

	const std::unordered_map<std::string, uart::StopBits> NameToStopBits{
		{"1",uart::StopBits::ONE},
		{"1.5",uart::StopBits::ONE5},
		{"2",uart::StopBits::TWO}
	};

	const std::unordered_map<uart::StopBits, std::string> StopBitsToName{
		{uart::StopBits::ONE, "1"},
		{uart::StopBits::ONE5, "1.5"},
		{uart::StopBits::TWO, "2"}
	};

	std::vector<std::string> getBaudRates()
	{
		return {
			BaudRateToName.at(uart::BaudRate::BR110),
			BaudRateToName.at(uart::BaudRate::BR300),
			BaudRateToName.at(uart::BaudRate::BR600),
			BaudRateToName.at(uart::BaudRate::BR1200),
			BaudRateToName.at(uart::BaudRate::BR2400),
			BaudRateToName.at(uart::BaudRate::BR4800),
			BaudRateToName.at(uart::BaudRate::BR9600),
			BaudRateToName.at(uart::BaudRate::BR14400),
			BaudRateToName.at(uart::BaudRate::BR19200),
			BaudRateToName.at(uart::BaudRate::BR38400),
			BaudRateToName.at(uart::BaudRate::BR57600),
			BaudRateToName.at(uart::BaudRate::BR115200),
			BaudRateToName.at(uart::BaudRate::BR128000),
			BaudRateToName.at(uart::BaudRate::BR256000)
		};
	}

	std::vector<std::string> getParities()
	{
		return {
			ParityToName.at(uart::Parity::NO),
			ParityToName.at(uart::Parity::ODD),
			ParityToName.at(uart::Parity::EVEN),
			ParityToName.at(uart::Parity::MARK),
			ParityToName.at(uart::Parity::SPACE)
		};
	}

	std::vector<std::string> getStopBits()
	{
		return {
			StopBitsToName.at(uart::StopBits::ONE),
			StopBitsToName.at(uart::StopBits::ONE5),
			StopBitsToName.at(uart::StopBits::TWO)
		};
	}
}

namespace logic
{
	PortConfigurationViewModel::PortConfigurationViewModel(OpenedPortsModel& portsModel) :
		mPortsModel(portsModel),
		mBaudRates(getBaudRates()),
		mParities(getParities()),
		mStopBits(getStopBits())
	{}

	void PortConfigurationViewModel::onPortRefreshButtonClicked()
	{
		updateAvailablePortList();
	}

	bool PortConfigurationViewModel::portOpenEnabled() noexcept
	{
		return !mAvailablePortsDescriptions.empty() && mSelectedAvailablePort < mAvailablePortsDescriptions.size();
	}

	void PortConfigurationViewModel::onPortOpenButtonClicked()
	{
		try {
			mPortsModel.openPort(mAvailablePortsNames.at(mSelectedAvailablePort));
			updateOpenedPortList();
			readPortParams();
		}
		catch (uart::PortException& e) {
			mPortLogEntries.emplace_back(ColorRed, e.what());
		}
		updateAvailablePortList();
	}

	bool PortConfigurationViewModel::portControlsEnabled() noexcept
	{
		return !mOpenedPortsNames.empty() && mSelectedOpenedPort < mOpenedPortsNames.size();
	}

	void PortConfigurationViewModel::updateAvailablePortList()
	{
		mAvailablePortsDescriptions.clear();
		mAvailablePortsNames.clear();
		try {
			const auto ports = uart::enumeratePorts();
			for (const auto& port : ports)
			{
				if (mPortsModel.ports().contains(port.PortName)) {
					continue;
				}
				mAvailablePortsDescriptions.push_back(port.Description + " (" + port.PortName + ")");
				mAvailablePortsNames.push_back(port.PortName);
			}
		}
		catch (uart::PortException& e) {
			mPortLogEntries.emplace_back(ColorRed, "Port enumeration error: "s + e.what());
		}
	}

	void PortConfigurationViewModel::updateOpenedPortList()
	{
		mOpenedPortsNames.clear();
		for (const auto& port : mPortsModel.ports() | std::views::keys)
		{
			mOpenedPortsNames.push_back(port);
		}
	}

	void PortConfigurationViewModel::readPortParams()
	{
		try {
			const auto portParams = mPortsModel.ports().at(mOpenedPortsNames.at(mSelectedOpenedPort)).getState();
			mSelectedBaudRate = std::ranges::find(mBaudRates, BaudRateToName.at(portParams.BaudRate)) - mBaudRates.begin();
			mSelectedParity = std::ranges::find(mParities, ParityToName.at(portParams.Parity)) - mParities.begin();
			mSelectedStopBits = std::ranges::find(mStopBits, StopBitsToName.at(portParams.StopBits)) - mStopBits.begin();
			mDataBits = portParams.DataBits;
		}
		catch (uart::PortException& e) {
			mPortLogEntries.emplace_back(ColorRed, "Port " + mOpenedPortsNames.at(mSelectedOpenedPort) + " params read error: " + e.what());
		}
	}

	void PortConfigurationViewModel::writePortParams()
	{
		try {
			uart::PortState portParams;
			portParams.BaudRate = NameToBaudRate.at(mBaudRates.at(mSelectedBaudRate));
			portParams.Parity = NameToParity.at(mParities.at(mSelectedParity));
			portParams.StopBits = NameToStopBits.at(mStopBits.at(mSelectedStopBits));
			portParams.DataBits = mDataBits;
			mPortsModel.ports().at(mOpenedPortsNames.at(mSelectedOpenedPort)).setState(portParams);
			mPortLogEntries.emplace_back(ColorGreen, "Port " + mOpenedPortsNames.at(mSelectedOpenedPort) + " params has been successfully written.");
		}
		catch (uart::PortException& e) {
			mPortLogEntries.emplace_back(ColorRed, "Port " + mOpenedPortsNames.at(mSelectedOpenedPort) + " params read error: " + e.what());
		}
	}

	void PortConfigurationViewModel::onPortCloseButtonClicked()
	{
		try {
			mPortsModel.closePort(mOpenedPortsNames.at(mSelectedOpenedPort));
		}
		catch (uart::PortException& e) {
			mPortLogEntries.emplace_back(ColorRed, e.what());
		}
		updateOpenedPortList();
		updateAvailablePortList();
	}

	void PortConfigurationViewModel::onClearLogClicked()
	{
		mPortLogEntries.clear();
	}

	void PortConfigurationViewModel::onReadParamsClicked()
	{
		readPortParams();
	}

	void PortConfigurationViewModel::onWriteParamsClicked()
	{
		writePortParams();
	}
}
