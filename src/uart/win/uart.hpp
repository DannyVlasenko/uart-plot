#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace uart
{
	enum class BaudRate
	{
		BR110 = 110,
		BR300 = 300,
		BR600 = 600,
		BR1200 = 1200,
		BR2400 = 2400,
		BR4800 = 4800,
		BR9600 = 9600,
		BR14400 = 14400,
		BR19200 = 19200,
		BR38400 = 38400,
		BR57600 = 57600,
		BR115200 = 115200,
		BR128000 = 128000,
		BR256000 = 256000
	};

	enum class Parity
	{
		NO,
		ODD,
		EVEN,
		MARK,
		SPACE
	};

	enum class StopBits
	{
		ONE,
		ONE5,
		TWO
	};

	struct PortState
	{
		BaudRate BaudRate;
		uint8_t DataBits;
		Parity Parity;
		StopBits StopBits;
	};

	class PortException : public std::runtime_error
	{
	public:
		using std::runtime_error::runtime_error;
	};

	class Port
	{
	public:
		explicit Port(const std::string& name);
		[[nodiscard]] PortState getState() const;
		void setState(const PortState& state) const;
	private:
		std::string mName;
		std::unique_ptr<void, void(*)(void*)> mPortHandle;
	};

	struct PortDescription
	{
		std::string PortName;
		std::string Description;
	};

	class PortEnumerator
	{
	public:
		std::vector<PortDescription> enumeratePorts();
	};
}
