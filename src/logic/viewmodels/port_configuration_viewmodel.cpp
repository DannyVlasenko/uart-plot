#include "port_configuration_viewmodel.hpp"

#include <ranges>

#include "models/opened_ports_model.hpp"

namespace logic
{
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
		mPortsModel.openPort(mAvailablePortsNames.at(mSelectedAvailablePort));
		updateAvailablePortList();
		updateOpenedPortList();
	}

	bool PortConfigurationViewModel::portControlsEnabled() noexcept
	{
		return !mOpenedPortsNames.empty() && mSelectedOpenedPort < mOpenedPortsNames.size();
	}

	void PortConfigurationViewModel::updateAvailablePortList()
	{
		mAvailablePortsDescriptions.clear();
		mAvailablePortsNames.clear();
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

	void PortConfigurationViewModel::updateOpenedPortList()
	{
		mOpenedPortsNames.clear();
		for (const auto& port : mPortsModel.ports() | std::views::keys)
		{
			mOpenedPortsNames.push_back(port);
		}
	}

	void PortConfigurationViewModel::onPortCloseButtonClicked()
	{
		mPortsModel.closePort(mOpenedPortsNames.at(mSelectedOpenedPort));
		updateOpenedPortList();
		updateAvailablePortList();
	}
}
