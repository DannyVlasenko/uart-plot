#include "opened_ports_model.hpp"

#include "data_source_model.hpp"
#include "uart_signal_source.hpp"

namespace logic
{
	void OpenedPortsModel::openPort(const std::string& name)
	{
		if (mPorts.contains(name)) {
			throw std::runtime_error("Port " + name + " is already opened.");
		}
		const auto& port = mPorts.emplace(name, uart::Port{name}).first->second;
		mDataSourceModel.addDataSource(std::make_unique<UartSignalSource>(name, port));
	}

	void OpenedPortsModel::closePort(const std::string& name)
	{
		if (!mPorts.contains(name)) {
			throw std::runtime_error("There is no opened port " + name + ".");
		}
		mDataSourceModel.removeDataSource(name);
		mPorts.erase(name);
	}
}
