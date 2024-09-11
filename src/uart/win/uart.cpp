#include "uart.hpp"

#include <cassert>
#include <stdexcept>
#include <Windows.h>

namespace
{
	std::string GetErrorAsString(const DWORD errorMessageID)
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

	HANDLE OpenComPort(const std::string& name)
	{
		const auto handle = CreateFile(name.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
		if (handle == INVALID_HANDLE_VALUE)
		{
			const auto lastErrorCode = GetLastError();
			throw uart::PortException("Failed to open port " + name + ": Code (" + std::to_string(lastErrorCode) + ") " + GetErrorAsString(lastErrorCode));
		}
		return handle;
	}

	void CloseComPort(HANDLE portHandle)
	{
		[[maybe_unused]] const auto result = CloseHandle(portHandle);
		assert(result != FALSE);
	}

	bool RegQueryValueString(_In_ ATL::CRegKey& key, _In_ LPCTSTR lpValueName, _Inout_ String& sValue)
	{
		//Reset the output parameter
		sValue.clear();

		//First query for the size of the registry value
		ULONG nChars{ 0 };
		LSTATUS nStatus{ key.QueryStringValue(lpValueName, nullptr, &nChars) };
		if (nStatus != ERROR_SUCCESS)
		{
			SetLastError(nStatus);
			return false;
		}

		//Allocate enough bytes for the return value
#pragma warning(suppress: 26472 26489)
		sValue.resize(static_cast<size_t>(nChars) + 1); //+1 is to allow us to null terminate the data if required
#pragma warning(suppress: 26472)
		const DWORD dwAllocatedSize{ static_cast<DWORD>((nChars + 1) * sizeof(TCHAR)) };

		//We will use RegQueryValueEx directly here because ATL::CRegKey::QueryStringValue does not handle non-null terminated data
		DWORD dwType{ 0 };
		DWORD dwBytes{ dwAllocatedSize };
#pragma warning(suppress: 26446 26489 26490)
		nStatus = RegQueryValueEx(key, lpValueName, nullptr, &dwType, reinterpret_cast<LPBYTE>(sValue.data()), &dwBytes);
		if (nStatus != ERROR_SUCCESS)
		{
			SetLastError(nStatus);
			return false;
		}
		if ((dwType != REG_SZ) && (dwType != REG_EXPAND_SZ))
		{
			SetLastError(ERROR_INVALID_DATA);
			return false;
		}
		if ((dwBytes % sizeof(TCHAR)) != 0)
		{
			SetLastError(ERROR_INVALID_DATA);
			return false;
		}
#pragma warning(suppress: 26446 26489)
		if (sValue[(dwBytes / sizeof(TCHAR)) - 1] != _T('\0'))
		{
			//Forcibly null terminate the data ourselves
#pragma warning(suppress: 26446 26489)
			sValue[(dwBytes / sizeof(TCHAR))] = _T('\0');
		}

		return true;
	}
}

uart::Port::Port(const std::string& name) :
	mPortHandle(OpenComPort(name), &CloseComPort)
{}

uart::PortState uart::Port::getState() const
{
	DCB dcbSerialParams;
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if(GetCommState(mPortHandle.get(), &dcbSerialParams) == FALSE)
	{
		const auto lastErrorCode = GetLastError();
		throw PortException("Failed to get port state: " + GetErrorAsString(lastErrorCode));
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
	dcbSerialParams.BaudRate = static_cast<DWORD>(state.BaudRate);
	dcbSerialParams.ByteSize = state.DataBits;
	dcbSerialParams.Parity = static_cast<BYTE>(state.Parity);
	dcbSerialParams.StopBits = static_cast<BYTE>(state.StopBits);
	if (SetCommState(mPortHandle.get(), &dcbSerialParams) == FALSE)
	{
		const auto lastErrorCode = GetLastError();
		throw PortException("Failed to get port state: " + GetErrorAsString(lastErrorCode));
	}
}

std::vector<uart::PortDescription> uart::PortEnumerator::enumeratePorts()
{
	//Reset the output parameter
	std::vector<uart::PortDescription> ports;

	ATL::CRegKey serialCommKey;
	LSTATUS nStatus{ serialCommKey.Open(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"), KEY_QUERY_VALUE) };
	if (nStatus != ERROR_SUCCESS)
	{
		SetLastError(nStatus);
		return false;
	}

	//Get the max value name and max value lengths
	DWORD dwMaxValueNameLen{ 0 };
	nStatus = RegQueryInfoKey(serialCommKey, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &dwMaxValueNameLen, nullptr, nullptr, nullptr);
	if (nStatus != ERROR_SUCCESS)
	{
		SetLastError(nStatus);
		return false;
	}

	const DWORD dwMaxValueNameSizeInChars{ dwMaxValueNameLen + 1 }; //Include space for the null terminator

	//Allocate some space for the value name
	std::vector<TCHAR> valueName{ dwMaxValueNameSizeInChars, std::allocator<TCHAR>{} };

	//Enumerate all the values underneath HKEY_LOCAL_MACHINE\HARDWARE\DEVICEMAP\SERIALCOMM
	bool bContinueEnumeration{ true };
	DWORD dwIndex{ 0 };
	while (bContinueEnumeration)
	{
		DWORD dwValueNameSize{ dwMaxValueNameSizeInChars };
#pragma warning(suppress: 26446)
		valueName[0] = _T('\0');
#pragma warning(suppress: 26446)
		bContinueEnumeration = (RegEnumValue(serialCommKey, dwIndex, valueName.data(), &dwValueNameSize, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS);
		if (bContinueEnumeration)
		{
			std::string sPortName;
#pragma warning(suppress: 26446 26486)
			if (RegQueryValueString(serialCommKey, valueName.data(), sPortName))
#pragma warning(suppress: 26489)
				ports.push_back(sPortName);

			//Prepare for the next loop
			++dwIndex;
		}
	}

	return true;
}
