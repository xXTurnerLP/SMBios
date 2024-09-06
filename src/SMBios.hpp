#pragma once

#include <cstdint>
#include <vector>
#include <Windows.h>

struct SMBIOSVersion
{
	byte major;
	byte minor;

	inline bool IsLessThan(byte major, byte minor) const
	{
		uint16_t this_num = (uint16_t)this->major << 8 | this->minor;
		uint16_t other_num = (uint16_t)major << 8 | minor;

		return this_num < other_num;
	}

	inline bool IsEqualTo(byte major, byte minor) const
	{
		uint16_t this_num = (uint16_t)this->major << 8 | this->minor;
		uint16_t other_num = (uint16_t)major << 8 | minor;

		return this_num == other_num;
	}

	inline bool IsGreaterThanOrEqualTo(byte major, byte minor) const
	{
		uint16_t this_num = (uint16_t)this->major << 8 | this->minor;
		uint16_t other_num = (uint16_t)major << 8 | minor;

		return this_num >= other_num;
	}

	inline bool IsGreaterThan(byte major, byte minor) const
	{
		uint16_t this_num = (uint16_t)this->major << 8 | this->minor;
		uint16_t other_num = (uint16_t)major << 8 | minor;

		return this_num > other_num;
	}
};

struct SMBIOSBiosInfo
{
	const char* vendor_name;
	const char* bios_version;
	uint16_t bios_start_address;
	const char* bios_release_date;
	uint8_t bios_rom_size; // 64K * (n + 1) where n is the bios_rom_size number
	uint64_t bios_characteristics; // bitfield; Defines which functions the BIOS supports: PCI, PCMCIA, Flash, and so on. (see 7.1.1).
	uint16_t bios_characteristics_ext;
	uint8_t bios_major_version;
	uint8_t bios_minor_version;
	uint8_t embedded_controller_firmware_major_version;
	uint8_t embedded_controller_firmware_minor_version;
	uint16_t extended_bios_rom_size; // 3.1+
};

struct UUID_RFC4122
{
	uint32_t time_low;
	uint16_t time_mid;
	uint16_t time_high_and_version;
	uint8_t clock_sequence_high_and_reserved;
	uint8_t clock_sequence_low;
	uint8_t node[6];
};

struct SMBIOSSystemInfo
{
	const char* manufacturer;
	const char* product_name;
	const char* version;
	const char* serial_number;
	UUID_RFC4122 uuid;
	uint8_t wake_up_type;
	const char* sku_number;
	const char* family;
};

struct SMBIOSMotherboardInfo
{
	const char* manufacturer;
	const char* product;
	const char* version;
	const char* serial_number;
	const char* asset_tag;
	uint8_t feature_flags;
	const char* location_in_chassis; // location is case/chassis/pc casing
	uint16_t chassis_handle;
	uint8_t board_type;
	uint8_t number_of_contained_object_handles;
	uint16_t* contained_object_handles;
};

struct SMBIOSProcessorInfo
{
	const char* socket_designation; // eg. LGA1151
	uint8_t processor_type;
	uint8_t processor_family;
	const char* manufacturer;
	uint64_t processor_id; // processor features
	const char* version;
	uint8_t voltage; // bitfield; what voltage the processor socket supports
	uint16_t external_clock; // MHz
	uint16_t max_speed; // MHz; max speed for the processor socket
	uint16_t current_speed; // MHz; current speed for the processor socket
	uint8_t status;
	uint8_t upgrade; // type of socket
	uint16_t l1_cache_handle;
	uint16_t l2_cache_handle;
	uint16_t l3_cache_handle;
	const char* serial_number;
	const char* asset_tag;
	const char* part_number;
	uint8_t core_count; // requires 2.5+; number of cores per processor socket
	uint8_t cores_enabled; // requires 2.5+; enabled cores per processor socket
	uint8_t thread_count; // requires 2.5+; threads per processor socket
	uint16_t processor_characteristics; // requires 2.5+; bitfield
	uint16_t processor_family2; // requires 2.6+;
	uint16_t core_count2; // requires 3.0+;
	uint16_t core_enabled2; // requires 3.0+;
	uint16_t thread_count2; // requires 3.0+;
	uint16_t threads_enabled; // requires 3.6+;
};

struct SMBIOSMemoryDevice
{
	uint16_t physical_memory_array_handle;
	uint16_t memory_error_information_handle;
	uint16_t total_width;
	uint16_t data_width;
	uint16_t size;
	uint8_t form_factor;
	uint8_t device_set;
	const char* device_locator;
	const char* bank_locator;
	uint8_t memory_type;
	uint16_t type_detail;
	uint16_t speed; // unit: MT/s
	const char* manufacturer;
	const char* serial_number;
	const char* asset_tag;
	const char* part_number;
	uint8_t attributes; // requires: 2.6+
	uint32_t extended_size; // requires: 2.7+
	uint16_t configured_memory_speed; // requires: 2.7+
	uint16_t minimum_voltage; // requires: 2.8+; unit: mV (millivolt)
	uint16_t maximum_voltage; // requires: 2.8+; unit: mV (millivolt)
	uint16_t configured_voltage; // requires: 2.8+; unit: mV (millivolt)
	uint8_t memory_technology; // requires: 3.2+
	uint16_t memory_operating_mode_capability; // requires: 3.2+
	const char* firmware_version; // requires: 3.2+
	uint16_t module_manifacturer_id; // requires: 3.2+
	uint16_t module_product_id; // requires: 3.2+
	uint16_t module_subsystem_controller_manufacturer_id; // requires: 3.2+
	uint16_t module_subsystem_controller_product_id; // requires: 3.2+
	uint64_t non_volatile_size; // requires: 3.2+
	uint64_t volatile_size; // requires: 3.2+
	uint64_t cache_size; // requires: 3.2+
	uint64_t logical_size; // requires: 3.2+
	uint32_t extended_speed; // requires: 3.3+
	uint32_t extended_configured_memory_speed; // requires: 3.3+
	uint16_t pmic0_manifacturer_id; // requires: 3.7+
	uint16_t pmic0_revision_number; // requires: 3.7+
	uint16_t rcd_manifacturer_id; // requires: 3.7+
	uint16_t rcd_revision_number; // requires: 3.7+
};

class SMBios // Supports only SMBIOS version >=2.4
{
public:
	SMBios();
	~SMBios();

public:
	SMBIOSVersion GetVersion();
	void ParseTables();
	std::vector<SMBIOSBiosInfo> GetBiosInfos();
	std::vector<SMBIOSSystemInfo> GetSystemInfos();
	std::vector<SMBIOSMotherboardInfo> GetMotherboardInfos();
	std::vector<SMBIOSProcessorInfo> GetProcessorInfos();
	std::vector<SMBIOSMemoryDevice> GetMemoryDevices();

private:
	struct RawSMBIOSData;
	enum class TableType : byte;

private:
	byte* raw_data;
	DWORD raw_data_len;
	RawSMBIOSData* smbios_raw_data;
	std::vector<SMBIOSBiosInfo> bios_infos;
	std::vector<SMBIOSSystemInfo> system_infos;
	std::vector<SMBIOSMotherboardInfo> motherboard_infos;
	std::vector<SMBIOSProcessorInfo> processor_infos;
	std::vector<SMBIOSMemoryDevice> memory_devices;

private:
	struct RawSMBIOSData
	{
		byte Used20CallingMethod;
		byte SMBIOSMajorVersion;
		byte SMBIOSMinorVersion;
		byte DmiRevision;
		DWORD Length;
		byte SMBIOSTableDataStart;
	};

	struct Header
	{
		TableType type;
		uint8_t length;
		uint16_t handle;
	};

	struct Table : Header
	{
		byte* data;
		std::vector<const char*> strings;
	};

	enum class TableType : byte
	{
		BiosInfo = 0,
		SystemInfo = 1,
		MotherboardInfo = 2,
		CaseInfo = 3, // system enclosure or chassis
		ProcessorInfo = 4,
		MemoryControllerInfo_OBSOLETE = 5,
		MemoryModuleInfo_OBSOLETE = 6,
		CacheInfo = 7,
		PortConnectorInfo = 8,
		SystemSlots = 9,
		OnBoardDevicesInfo_OBSOLETE = 10,
		OEMStrings = 11,
		SystemConfigurationOptions = 12, // info for motherboard jumpers and switches
		BIOSLanguageInfo = 13,
		GroupAssociations = 14,
		SystemEventLog = 15,
		PhysicalMemoryArray = 16,
		MemoryDevice = 17,
		MemoryErrorInfo_32bit = 18,
		MemoryArrayMappedAddress = 19,
		MemoryDeviceMappedAddress = 20,
		BuiltinPointingDevice = 21,
		PortableBattery = 22,
		SystemReset = 23,
		HardwareSecurity = 24,
		SystemPowerControls = 25,
		VoltageProbe = 26,
		CoolingDevice = 27,
		TemperatureProbe = 28,
		CurrentProbe = 29, // electrical current; amperage
		OOBRemoteAccess = 30, // out-of-bound; remote administration
		BISEntryPoint = 31, // boot integrity services
		SystemBootInfo = 32,
		MemoryErrorInfo_64bit = 33,
		ManagementDevice = 34,
		ManagementDeviceComponent = 35,
		ManagementDeviceThresholdData = 36,
		MemoryChannel = 37,
		IPMIDeviceInfo = 38,
		SystemPowerSupply = 39,
		AdditionalInfo = 40,
		OnboardDevicesExtendedInfo = 41,
		ManagementControllerHostInterface = 42,
		TPMDevice = 43,
		ProcessorAdditionalInfo = 44,
		FirmwareInventoryInfo = 45,
		StringProperty = 46,
		Inactive = 126,
		EndOfTable = 127,
	};
};
