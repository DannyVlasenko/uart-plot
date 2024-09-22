#pragma once

#include <string>

#include "data_source_model.hpp"

namespace uart
{
	class Port;
}

namespace logic
{
	class UartSignalSource : public IDataSource
	{
	public:
		explicit UartSignalSource(const std::string& name, const uart::Port& port):
			mName(name),
			mPort(port)
		{}

		void update() override;

		[[nodiscard]] const std::string_view name() const noexcept override
		{
			return mName;
		}

		[[nodiscard]] size_t size() const noexcept override
		{
			return mBuffer.size();
		}

		[[nodiscard]] std::span<const double> getData(size_t offset, size_t length) const noexcept override
		{
			if (offset >= mBuffer.size()) {
				return {};
			}
			if (offset + length > mBuffer.size()) {
				return std::span{ &mBuffer[offset], &mBuffer.back() };
			}
			return std::span{ &mBuffer[offset], length };
		}

	private:
		const std::string mName;
		const uart::Port& mPort;
		bool mDataStarted{ false };
		std::vector<char> mRawDataToConvert;
		std::vector<double> mBuffer;

		void readNewData();
		void convertRawToDouble();
	};
}
