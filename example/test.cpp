#include <iostream>
#include "SMBios.hpp"

int main()
{
	SMBios smbios;

	if (!smbios.IsValid())
	{
		std::cout << "Your PC has been tampered with, possible HWID changer detected, cannot continue, aborted!";
		Sleep(-1);
	}

	std::cout << "This PC's smbios specificaiton version: " << (int)smbios.GetVersion().major << "." << (int)smbios.GetVersion().minor << std::endl;

	smbios.ParseTables(); // this needs to be called first before accessing any tables

	// NOTE: the spec can be found here: https://www.dmtf.org/standards/smbios

	std::vector<SMBIOSBiosInfo> bios_infos = smbios.GetBiosInfos();
	for (auto& bios_info : bios_infos)
	{
		std::cout << "BiosInfo table:" << std::endl;
		std::cout << "  Vendor name: " << bios_info.vendor_name << std::endl;
		std::cout << "  BIOS version: " << bios_info.bios_version << std::endl;
		std::cout << "  BIOS starting address: " << bios_info.bios_start_address << std::endl;
		std::cout << "  BIOS release date: " << bios_info.bios_release_date << std::endl;
		std::cout << "  BIOS ROM size: " << (int)bios_info.bios_rom_size << std::endl;
		std::cout << "  BIOS characteristics (bitfield, see the spec for more details): " << bios_info.bios_characteristics << std::endl;
		std::cout << "  BIOS characteristics extended: " << bios_info.bios_characteristics_ext << std::endl;
		std::cout << "  BIOS major version: " << (int)bios_info.bios_major_version << std::endl;
		std::cout << "  BIOS minor version: " << (int)bios_info.bios_minor_version << std::endl;
		std::cout << "  Firmware major version: " << (int)bios_info.embedded_controller_firmware_major_version << std::endl;
		std::cout << "  Firmware minor version: " << (int)bios_info.embedded_controller_firmware_minor_version << std::endl;
		if (smbios.GetVersion().IsGreaterThanOrEqualTo(3, 1))
		{
			std::cout << "  Extended BIOS ROM size: " << bios_info.extended_bios_rom_size << std::endl;
		}
	}

	// other tables are read the exact same way
	// ensure to check version for fields that are only present on a newer SMBios spec

	Sleep(-1); // prevents the process from ending and closing the conhost (cmd) window
}
