#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <TlHelp32.h>

#include <cstdint>
#include <string_view>

void tostringigfuckthis(WCHAR* in, std::string& alsoin) {
	char ch[260];
	char DefChar = ' ';
	WideCharToMultiByte(CP_ACP, 0, in, -1, ch, 260, &DefChar, NULL);

	//A std:string  using the char* constructor.
	alsoin = std::string(ch);
}

class Memory
{
private:
	std::uintptr_t processId = 0;
	void* processHandle = nullptr;

public:
	// Constructor that finds the process id
	// and opens a handle
	Memory(const wchar_t* processName) noexcept
	{
		HWND csgo_hwnd = FindWindowA("Valve001", 0);
		while (!csgo_hwnd)
			Sleep(100);

		DWORD csgo_pid = 0;
		GetWindowThreadProcessId(csgo_hwnd, &csgo_pid);
		while (!csgo_pid)
			Sleep(100);

		processId = csgo_pid;
		processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, csgo_pid);
	}

	// Destructor that frees the opened handle
	~Memory()
	{
		if (processHandle)
			::CloseHandle(processHandle);
	}

	// Returns the base address of a module by name
	const std::uintptr_t GetModuleAddress(const std::string_view moduleName) const noexcept
	{
		::MODULEENTRY32 entry = { };
		entry.dwSize = sizeof(::MODULEENTRY32);

		const auto snapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId);

		std::uintptr_t result = 0;

		while (::Module32Next(snapShot, &entry))
		{
			std::string notretardedmodule;
			tostringigfuckthis(entry.szModule, notretardedmodule);
			if (!moduleName.compare(notretardedmodule))
			{
				result = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
				break;
			}
		}

		if (snapShot)
			::CloseHandle(snapShot);

		return result;
	}

	// Read process memory
	template <typename T>
	constexpr const T Read(const std::uintptr_t& address) const noexcept
	{
		T value = { };
		::ReadProcessMemory(processHandle, reinterpret_cast<const void*>(address), &value, sizeof(T), NULL);
		return value;
	}

	// Write process memory
	template <typename T>
	constexpr void Write(const std::uintptr_t& address, const T& value) const noexcept
	{
		::WriteProcessMemory(processHandle, reinterpret_cast<void*>(address), &value, sizeof(T), NULL);
	}
};
