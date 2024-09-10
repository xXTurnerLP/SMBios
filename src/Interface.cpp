// This interface is meant to be C style so it can be used from C# (mainly) or any other langauge

#include <format>
#include "SMBios.hpp"

#define API extern "C" __declspec(dllexport)

static SMBios* smbios;

static std::vector<SMBIOSBiosInfo> bios_infos;
static std::vector<SMBIOSSystemInfo> system_infos;
static std::vector<SMBIOSMotherboardInfo> motherboard_infos;
static std::vector<SMBIOSProcessorInfo> processor_infos;
static std::vector<SMBIOSMemoryDevice> memory_devices;

bool Init()
{
	smbios = new SMBios();

	if (!smbios->IsValid())
		return false;

	smbios->ParseTables();

	bios_infos = smbios->GetBiosInfos();
	system_infos = smbios->GetSystemInfos();
	motherboard_infos = smbios->GetMotherboardInfos();
	processor_infos = smbios->GetProcessorInfos();
	memory_devices = smbios->GetMemoryDevices();

	return true;
}

// this should be called first from external code
// returns false if the smbios fetch failed, if it failed its most likely due to a HWID spoofer/changer
API bool SMBIOS_Init()
{
	return Init();
}

// this should be called last to free up the memory used
API void SMBIOS_Cleanup()
{
	delete smbios;
}

API const char* SMBIOS_GetVersion()
{
	auto str = std::format("{}.{}", (int)smbios->GetVersion().major, (int)smbios->GetVersion().minor);
	char* ptr = new char[str.size() + 1];
	memcpy(ptr, str.data(), str.size());
	ptr[str.size()] = '\0';
	return ptr;
}

API int SMBIOS_GetBiosInfosCount()
{
	return (int)bios_infos.size();
}

API SMBIOSBiosInfo SMBIOS_GetBiosInfoAtIndex(int index)
{
	return bios_infos[index];
}

API int SMBIOS_GetSystemInfosCount()
{
	return (int)system_infos.size();
}

API SMBIOSSystemInfo SMBIOS_GetSystemInfoAtIndex(int index)
{
	return system_infos[index];
}

API int SMBIOS_GetMotherboardInfosCount()
{
	return (int)motherboard_infos.size();
}

API SMBIOSMotherboardInfo SMBIOS_GetMotherboardInfoAtIndex(int index)
{
	return motherboard_infos[index];
}

API int SMBIOS_GetProcessorInfosCount()
{
	return (int)processor_infos.size();
}

API SMBIOSProcessorInfo SMBIOS_GetProcessorInfoAtIndex(int index)
{
	return processor_infos[index];
}

API int SMBIOS_GetMemoryDevicesCount()
{
	return (int)memory_devices.size();
}

API SMBIOSMemoryDevice SMBIOS_GetMemoryDeviceAtIndex(int index)
{
	return memory_devices[index];
}
