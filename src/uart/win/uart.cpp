#include "uart.hpp"

#include <cassert>
#include <stdexcept>
#include <Windows.h>

namespace
{
	std::string getErrorAsString(const DWORD errorMessageID)
	{
		if (errorMessageID == 0) {
			return {};
		}

		LPSTR messageBuffer = nullptr;
		const size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, errorMessageID, 0, reinterpret_cast<LPSTR>(&messageBuffer), 0, nullptr);

		std::string message(messageBuffer, size);
		LocalFree(messageBuffer);
		return message;
	}

	HANDLE openComPort(const std::string& name)
	{
		const auto handle = CreateFile(name.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
		if (handle == INVALID_HANDLE_VALUE)
		{
			const auto lastErrorCode = GetLastError();
			throw uart::PortException("Failed to open port " + name + ": Code (" + std::to_string(lastErrorCode) + ") " + getErrorAsString(lastErrorCode));
		}
		return handle;
	}

	void closeComPort(HANDLE portHandle)
	{
		[[maybe_unused]] const auto result = CloseHandle(portHandle);
		assert(result != FALSE);
	}

	HKEY openRegistryKey(HKEY baseKey, const char* name)
	{
		HKEY key;
		const auto status = RegOpenKeyEx(baseKey, name, 0, KEY_QUERY_VALUE, &key);
		if (status != ERROR_SUCCESS) {
			const auto lastErrorCode = GetLastError();
			throw uart::PortException("Failed to open registry key " + std::string(name) + ": Code (" + std::to_string(lastErrorCode) + ") " + getErrorAsString(lastErrorCode));
		}
		return key;
	}

	void closeRegistryKey(HKEY key)
	{
		[[maybe_unused]] const auto status = RegCloseKey(key);
		assert(status == ERROR_SUCCESS);
	}

	class RegKey
	{
	public:
		struct StringSubkey
		{
			std::string Name;
			std::string Value;
		};

		RegKey(HKEY baseKey, const char* name) :
			mKey(openRegistryKey(baseKey, name), &closeRegistryKey)
		{}

		std::vector<StringSubkey> enumerateStringSubKeys() const
		{
			DWORD maxValueNameLen{ 0 };
			DWORD maxValueLen{ 0 };
			auto status = RegQueryInfoKey(mKey.get(), nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &maxValueNameLen, &maxValueLen, nullptr, nullptr);
			if (status != ERROR_SUCCESS)
			{
				const auto lastErrorCode = GetLastError();
				throw uart::PortException("Failed to query key info: Code (" + std::to_string(lastErrorCode) + ") " + getErrorAsString(lastErrorCode));
			}
			DWORD dwIndex{ 0 };
			std::vector<StringSubkey> subKeys;
			while (true)
			{
				StringSubkey subKey;
				auto nameSize = maxValueNameLen + 1;
				subKey.Name.resize(nameSize, '\0');
				status = RegEnumValue(mKey.get(), dwIndex, subKey.Name.data(), &nameSize, nullptr, nullptr, nullptr, nullptr);
				if (status == ERROR_NO_MORE_ITEMS) {
					break;
				}
				if (status != ERROR_SUCCESS) {
					const auto lastErrorCode = GetLastError();
					throw uart::PortException("Failed to enumerate string key values: Code (" + std::to_string(lastErrorCode) + ") " + getErrorAsString(lastErrorCode));
				}

				auto valueSize = maxValueLen + 1;
				subKey.Value.resize(valueSize, '\0');
				DWORD dwType;
				status = RegQueryValueEx(mKey.get(), subKey.Name.c_str(), nullptr, &dwType, reinterpret_cast<LPBYTE>(subKey.Value.data()), &valueSize);
				if (status != ERROR_SUCCESS)
				{
					const auto lastErrorCode = GetLastError();
					throw uart::PortException("Failed to query value: Code (" + std::to_string(lastErrorCode) + ") " + getErrorAsString(lastErrorCode));
				}
				if ((dwType == REG_SZ) || (dwType == REG_EXPAND_SZ))
				{
					subKey.Name.erase(std::ranges::find(subKey.Name, '\0'), subKey.Name.end());
					subKey.Value.erase(std::ranges::find(subKey.Value, '\0'), subKey.Value.end());
					subKeys.push_back(subKey);
				}
				++dwIndex;
			}
			return subKeys;
		}

	private:
		std::unique_ptr<HKEY__, void(*)(HKEY)> mKey;
	};
}

uart::Port::Port(const std::string& name) :
	mName(name),
	mPortHandle(openComPort(name), &closeComPort)
{}

uart::PortState uart::Port::getState() const
{
	DCB dcbSerialParams;
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (GetCommState(mPortHandle.get(), &dcbSerialParams) == FALSE)
	{
		const auto lastErrorCode = GetLastError();
		throw PortException("Failed to get port " + mName + " state: Code (" + std::to_string(lastErrorCode) + ") " + getErrorAsString(lastErrorCode));
	}
	return
	{
		.BaudRate = static_cast<BaudRate>(dcbSerialParams.BaudRate),
		.DataBits = dcbSerialParams.ByteSize,
		.Parity = static_cast<Parity>(dcbSerialParams.Parity),
		.StopBits = static_cast<StopBits>(dcbSerialParams.StopBits)
	};
}

void uart::Port::setState(const PortState& state) const
{
	DCB dcbSerialParams;
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (GetCommState(mPortHandle.get(), &dcbSerialParams) == FALSE)
	{
		const auto lastErrorCode = GetLastError();
		throw PortException("Failed to get port " + mName + " state: Code (" + std::to_string(lastErrorCode) + ") " + getErrorAsString(lastErrorCode));
	}
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	dcbSerialParams.BaudRate = static_cast<DWORD>(state.BaudRate);
	dcbSerialParams.ByteSize = state.DataBits;
	dcbSerialParams.Parity = static_cast<BYTE>(state.Parity);
	dcbSerialParams.StopBits = static_cast<BYTE>(state.StopBits);
	if (SetCommState(mPortHandle.get(), &dcbSerialParams) == FALSE)
	{
		const auto lastErrorCode = GetLastError();
		throw PortException("Failed to set port " + mName + " state: Code (" + std::to_string(lastErrorCode) + ") " + getErrorAsString(lastErrorCode));
	}
}

std::vector<char> uart::Port::readData() const
{
	COMMTIMEOUTS commTimeouts;
	if (!GetCommTimeouts(mPortHandle.get(), &commTimeouts)) {
		const auto lastErrorCode = GetLastError();
		throw PortException("Failed to get port " + mName + " timeouts: Code (" + std::to_string(lastErrorCode) + ") " + getErrorAsString(lastErrorCode));
	}
	commTimeouts.ReadIntervalTimeout = 0;
	commTimeouts.ReadTotalTimeoutMultiplier = 0;
	commTimeouts.ReadTotalTimeoutConstant = 1;
	if (!SetCommTimeouts(mPortHandle.get(), &commTimeouts)) {
		const auto lastErrorCode = GetLastError();
		throw PortException("Failed to set port " + mName + " timeouts: Code (" + std::to_string(lastErrorCode) + ") " + getErrorAsString(lastErrorCode));
	}
	COMMPROP commProps;
	if (!GetCommProperties(mPortHandle.get(), &commProps)) {
		const auto lastErrorCode = GetLastError();
		throw PortException("Failed to get port " + mName + " properties: Code (" + std::to_string(lastErrorCode) + ") " + getErrorAsString(lastErrorCode));
	}

	DWORD bytesRead = 0;
	std::vector<char> readBuffer(commProps.dwCurrentRxQueue);
	if (!ReadFile(mPortHandle.get(), readBuffer.data(), static_cast<DWORD>(readBuffer.size()), &bytesRead, nullptr)) {
		const auto lastErrorCode = GetLastError();
		throw PortException("Failed to read data from port " + mName + ": Code (" + std::to_string(lastErrorCode) + ") " + getErrorAsString(lastErrorCode));
	}
	readBuffer.resize(bytesRead);
	return readBuffer;
}

std::vector<uart::PortDescription> uart::enumeratePorts()
{
	RegKey serialCommKey{ HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\SERIALCOMM" };
	std::vector<PortDescription> ports;
	for (const auto& [keyName, keyValue] : serialCommKey.enumerateStringSubKeys()) {
		ports.push_back({ .PortName = keyValue, .Description = keyName });
	}
	return ports;
}
