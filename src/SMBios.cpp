#include "SMBios.hpp"

SMBios::SMBios()
{
	raw_data_len = GetSystemFirmwareTable('RSMB', 0, NULL, 0);
	if (raw_data_len == 0)
		return;

	raw_data = new byte[raw_data_len];
	if (raw_data_len != GetSystemFirmwareTable('RSMB', 0, raw_data, raw_data_len))
	{
		return;
	}

	smbios_raw_data = (RawSMBIOSData*)raw_data;
}

SMBios::~SMBios()
{
	delete[] raw_data;
}

SMBIOSVersion SMBios::GetVersion()
{
	return SMBIOSVersion
	{
		.major = smbios_raw_data->SMBIOSMajorVersion,
		.minor = smbios_raw_data->SMBIOSMinorVersion
	};
}

void SMBios::ParseTables()
{
	auto length = smbios_raw_data->Length;
	auto table_data = &smbios_raw_data->SMBIOSTableDataStart;
	auto version = GetVersion();
	size_t offset = 0;

	while (offset != length)
	{
		#define CURRENT_TABLE (table_data + offset)

		Table table = {};
		Header header = *(Header*)(CURRENT_TABLE);
		table.type = header.type;
		table.length = header.length;
		table.handle = header.handle;

		//printf("Table: %d\n", header.type);
		table.data = CURRENT_TABLE;
		offset += header.length;

		#define CURRENT_STRING ((char*)(CURRENT_TABLE))
		#define CURRENT_STRING_LENGTH (strlen(CURRENT_STRING))

		bool no_strings = CURRENT_STRING_LENGTH == 0;
		while (CURRENT_STRING_LENGTH != 0)
		{
			//printf("\t%s\n", CURRENT_STRING);
			table.strings.push_back(CURRENT_STRING);
			offset += CURRENT_STRING_LENGTH + 1;
		}
		offset += no_strings ? 2 : 1;

		#define GET_PTR(offset) (table.data + offset)
		#define GET_BYTE(offset) (table.data[(offset)])
		#define GET_WORD(offset) (*(uint16_t*)(GET_PTR(offset)))
		#define GET_DWORD(offset) (*(uint32_t*)(GET_PTR(offset)))
		#define GET_QWORD(offset) (*(uint64_t*)(GET_PTR(offset)))
		#define GET_UUID(offset) (*(UUID_RFC4122*)(GET_PTR(offset)))
		#define GET_STRING(offset) (GET_BYTE(offset) == 0 ? "" : table.strings[GET_BYTE(offset) - 1]) // this can crash if someone spoofs wrong data

		// Table decoding
		switch (table.type)
		{
		case TableType::BiosInfo:
		{
			SMBIOSBiosInfo info = {};
			info.vendor_name = GET_STRING(0x4);
			info.bios_version = GET_STRING(0x5);
			info.bios_start_address = GET_WORD(0x6);
			info.bios_release_date = GET_STRING(0x8);
			info.bios_rom_size = GET_BYTE(0x9);
			info.bios_characteristics = GET_QWORD(0xA);
			info.bios_characteristics_ext = GET_WORD(0x12);
			info.bios_major_version = GET_BYTE(0x14);
			info.bios_minor_version = GET_BYTE(0x15);
			info.embedded_controller_firmware_major_version = GET_BYTE(0x16);
			info.embedded_controller_firmware_minor_version = GET_BYTE(0x17);
			if (version.IsGreaterThanOrEqualTo(3, 1) && table.length > 0x18)
			{
				info.extended_bios_rom_size = GET_WORD(0x18);
			}

			bios_infos.push_back(info);

			break;
		}

		case TableType::SystemInfo:
		{
			SMBIOSSystemInfo info = {};
			info.manufacturer = GET_STRING(0x4);
			info.product_name = GET_STRING(0x5);
			info.version = GET_STRING(0x6);
			info.serial_number = GET_STRING(0x7);
			info.uuid = GET_UUID(0x8);
			info.wake_up_type = GET_BYTE(0x18);
			info.sku_number = GET_STRING(0x19);
			info.family = GET_STRING(0x1A);

			system_infos.push_back(info);

			break;
		}

		case TableType::MotherboardInfo:
		{
			SMBIOSMotherboardInfo info = {};
			info.manufacturer = GET_STRING(0x4);
			info.product = GET_STRING(0x5);
			info.version = GET_STRING(0x6);
			info.serial_number = GET_STRING(0x7);
			info.asset_tag = GET_STRING(0x8);
			info.feature_flags = GET_BYTE(0x9);
			info.location_in_chassis = GET_STRING(0xA);
			info.chassis_handle = GET_WORD(0xB);
			info.board_type = GET_BYTE(0xD);
			info.number_of_contained_object_handles = GET_BYTE(0xE);
			info.contained_object_handles = info.number_of_contained_object_handles == 0 ? nullptr : (uint16_t*)GET_PTR(0xF);

			motherboard_infos.push_back(info);

			break;
		}

		case TableType::ProcessorInfo:
		{
			SMBIOSProcessorInfo info = {};
			info.socket_designation = GET_STRING(0x4);
			info.processor_type = GET_BYTE(0x5);
			info.processor_family = GET_BYTE(0x6);
			info.manufacturer = GET_STRING(0x7);
			info.processor_id = GET_QWORD(0x8);
			info.version = GET_STRING(0x10);
			info.voltage = GET_BYTE(0x11);
			info.external_clock = GET_WORD(0x12);
			info.max_speed = GET_WORD(0x14);
			info.current_speed = GET_WORD(0x16);
			info.status = GET_BYTE(0x18);
			info.upgrade = GET_BYTE(0x19);
			info.l1_cache_handle = GET_WORD(0x1A);
			info.l2_cache_handle = GET_WORD(0x1C);
			info.l3_cache_handle = GET_WORD(0x1E);
			info.serial_number = GET_STRING(0x20);
			info.asset_tag = GET_STRING(0x21);
			info.part_number = GET_STRING(0x22);
			if (version.IsGreaterThanOrEqualTo(2, 5) && table.length > 0x23)
			{
				info.core_count = GET_BYTE(0x23);
				info.cores_enabled = GET_BYTE(0x24);
				info.thread_count = GET_BYTE(0x25);
				info.processor_characteristics = GET_WORD(0x26);
			}
			if (version.IsGreaterThanOrEqualTo(2, 6) && table.length > 0x28)
			{
				info.processor_family2 = GET_WORD(0x28);
			}
			if (version.IsGreaterThanOrEqualTo(3, 0) && table.length > 0x2A)
			{
				info.core_count2 = GET_WORD(0x2A);
				info.core_enabled2 = GET_WORD(0x2C);
				info.thread_count2 = GET_WORD(0x2E);
			}
			if (version.IsGreaterThanOrEqualTo(3, 6) && table.length > 0x30)
			{
				info.threads_enabled = GET_WORD(0x30);
			}

			processor_infos.push_back(info);

			break;
		}

		case TableType::MemoryDevice:
		{
			SMBIOSMemoryDevice info = {};
			info.physical_memory_array_handle = GET_WORD(0x4);
			info.memory_error_information_handle = GET_WORD(0x6);
			info.total_width = GET_WORD(0x8);
			info.data_width = GET_WORD(0xA);
			info.size = GET_WORD(0xC);
			info.form_factor = GET_BYTE(0xE);
			info.device_set = GET_BYTE(0xF);
			info.device_locator = GET_STRING(0x10);
			info.bank_locator = GET_STRING(0x11);
			info.memory_type = GET_BYTE(0x12);
			info.type_detail = GET_WORD(0x13);
			info.speed = GET_WORD(0x15);
			info.manufacturer = GET_STRING(0x17);
			info.serial_number = GET_STRING(0x18);
			info.asset_tag = GET_STRING(0x19);
			info.part_number = GET_STRING(0x1A);
			if (version.IsGreaterThanOrEqualTo(2, 6) && table.length > 0x1B)
			{
				info.attributes = GET_BYTE(0x1B);
			}
			if (version.IsGreaterThanOrEqualTo(2, 7) && table.length > 0x1C)
			{
				info.extended_size = GET_DWORD(0x1C);
				info.configured_memory_speed = GET_WORD(0x20);
			}
			if (version.IsGreaterThanOrEqualTo(2, 8) && table.length > 0x22)
			{
				info.minimum_voltage = GET_WORD(0x22);
				info.maximum_voltage = GET_WORD(0x24);
				info.configured_voltage = GET_WORD(0x26);
			}
			if (version.IsGreaterThanOrEqualTo(3, 2) && table.length > 0x28)
			{
				info.memory_technology = GET_BYTE(0x28);
				info.memory_operating_mode_capability = GET_WORD(0x29);
				info.firmware_version = GET_STRING(0x2B);
				info.module_manifacturer_id = GET_WORD(0x2C);
				info.module_product_id = GET_WORD(0x2E);
				info.module_subsystem_controller_manufacturer_id = GET_WORD(0x30);
				info.module_subsystem_controller_product_id = GET_WORD(0x32);
				info.non_volatile_size = GET_QWORD(0x34);
				info.volatile_size = GET_QWORD(0x3C);
				info.cache_size = GET_QWORD(0x44);
				info.logical_size = GET_QWORD(0x4C);
			}
			if (version.IsGreaterThanOrEqualTo(3, 3) && table.length > 0x54)
			{
				info.extended_speed = GET_DWORD(0x54);
				info.extended_configured_memory_speed = GET_DWORD(0x58);
			}
			if (version.IsGreaterThanOrEqualTo(3, 7) && table.length >= 0x5C)
			{
				info.pmic0_manifacturer_id = GET_WORD(0x5C);
				info.pmic0_revision_number = GET_WORD(0x5E);
				info.rcd_manifacturer_id = GET_WORD(0x60);
				info.rcd_revision_number = GET_WORD(0x62);
			}

			memory_devices.push_back(info);

			break;
		}
		}
	}
}

std::vector<SMBIOSBiosInfo> SMBios::GetBiosInfos()
{
	return bios_infos;
}

std::vector<SMBIOSSystemInfo> SMBios::GetSystemInfos()
{
	return system_infos;
}

std::vector<SMBIOSMotherboardInfo> SMBios::GetMotherboardInfos()
{
	return motherboard_infos;
}

std::vector<SMBIOSProcessorInfo> SMBios::GetProcessorInfos()
{
	return processor_infos;
}

std::vector<SMBIOSMemoryDevice> SMBios::GetMemoryDevices()
{
	return memory_devices;
}
