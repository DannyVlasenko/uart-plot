#pragma once

#include "views/port_selection_view.hpp"

#include <vector>
#include <string>

namespace logic
{
	class PortSelectionViewModel : public views::IPortSelectionViewModel
	{
	public:

		std::span<const std::string> portsDescriptions() const noexcept override
		{
			return mPortsDescriptions;
		}

		size_t& selectedPort() noexcept
		{
			return mSelectedPort;
		}

		bool portControlsEnabled() noexcept
		{
			return false;
		}

		std::span<const std::string> baudRatesList() const noexcept
		{
			
		}

		size_t& selectedBaudRate() noexcept
		{
			
		}

		std::span<const std::string> dataBitsList() const noexcept
		{
			
		}

		size_t& selectedDataBits() noexcept
		{
			
		}

		std::span<const std::string> paritiesList() const noexcept
		{
			
		}

		size_t& selectedParity() noexcept
		{
			
		}

		std::span<const std::string> stopBitsList() const noexcept
		{
			
		}

		size_t& selectedStopBits() noexcept
		{
			
		}

		std::span<const std::string> flowControlsList() const noexcept
		{
			
		}

		size_t& selectedFlowControl() noexcept
		{
			
		}

		const char* portOperationButtonText() const noexcept
		{
			return nullptr;
		}

		void onPortOperationButtonClicked() noexcept
		{
		}

	private:
		std::vector<std::string> mPortsDescriptions;
		size_t mSelectedPort;
	};
}