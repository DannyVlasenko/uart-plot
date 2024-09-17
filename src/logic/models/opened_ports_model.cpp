#include "opened_ports_model.hpp"

namespace logic
{
	void OpenedPortsModel::openPort(const std::string& name)
	{
		if (mPorts.contains(name)) {
			throw std::runtime_error("Port " + name + " is already opened.");
		}
		mPorts.emplace(name, uart::Port{name});
	}

	void OpenedPortsModel::closePort(const std::string& name)
	{
		if (!mPorts.contains(name)) {
			throw std::runtime_error("There is no opened port " + name +".");
		}
		mPorts.erase(name);
	}
}
