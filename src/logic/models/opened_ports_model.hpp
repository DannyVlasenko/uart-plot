#pragma once

#include "uart.hpp"

namespace models 
{
	class OpenedPortsModel
	{
	public:

	private:
		std::vector<uart::Port> mPorts;
	};
}