#include "uart_signal_source.hpp"
#include "opened_ports_model.hpp"

#include <charconv>

namespace logic
{
	void UartSignalSource::update()
	{
		readNewData();
		if (mDataStarted) {
			convertRawToDouble();
		}
	}

	void UartSignalSource::readNewData()
	{
		auto newData = mPort.readData();
		auto newDataStartIt = newData.begin();
		if (!mDataStarted) {
			newDataStartIt = std::ranges::find(newData, ';');
			if (newDataStartIt == newData.end()) {
				return;
			}
			mDataStarted = true;
			++newDataStartIt;
		}
		mRawDataToConvert.insert(mRawDataToConvert.end(), newDataStartIt, newData.end());
	}

	void UartSignalSource::convertRawToDouble()
	{
		auto convertStart = mRawDataToConvert.begin();
		auto separatorIt = std::ranges::find(mRawDataToConvert, ';');
		while (separatorIt != mRawDataToConvert.end()) {
			double sample;
			auto result = std::from_chars(&convertStart[0], &separatorIt[0], sample);
			if (result.ec == std::errc{}) {
				mBuffer.push_back(sample);
			}
			else if (result.ec == std::errc::result_out_of_range) {
				mBuffer.push_back(std::numeric_limits<double>::infinity());
			}
			else {
				mBuffer.push_back(std::numeric_limits<double>::quiet_NaN());
			}
			convertStart = separatorIt + 1;
			separatorIt = std::find(convertStart, mRawDataToConvert.end(), ';');
		}
		mRawDataToConvert.erase(mRawDataToConvert.begin(), convertStart);
	}
}
