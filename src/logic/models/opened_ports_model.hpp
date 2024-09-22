#pragma once

#include <unordered_map>

#include "uart.hpp"

namespace logic 
{
	class DataSourceModel;
	class OpenedPortsModel
	{
	public:
		explicit OpenedPortsModel(DataSourceModel& dataSourceModel)
			: mDataSourceModel(dataSourceModel)
		{}

		void openPort(const std::string& name);

		void closePort(const std::string& name);

		const std::unordered_map<std::string, uart::Port>& ports() const noexcept
		{
			return mPorts;
		}

	private:
		std::unordered_map<std::string, uart::Port> mPorts;
		DataSourceModel& mDataSourceModel;
	};
}
