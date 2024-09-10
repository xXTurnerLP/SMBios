using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.Marshalling;
using System.Text;
using System.Threading.Tasks;

namespace SMBiosNS
{
	internal static unsafe class SMBios
	{
		const string dll_name = "SMBios.dll";

		[DllImport(dll_name)]
		static extern bool SMBIOS_Init();

		[DllImport(dll_name)]
		static extern IntPtr SMBIOS_GetVersion();

		[DllImport(dll_name)]
		static extern int SMBIOS_GetBiosInfosCount();

		[DllImport(dll_name)]
		static extern int SMBIOS_GetSystemInfosCount();

		[DllImport(dll_name)]
		static extern int SMBIOS_GetMotherboardInfosCount();

		[DllImport(dll_name)]
		static extern int SMBIOS_GetProcessorInfosCount();

		[DllImport(dll_name)]
		static extern int SMBIOS_GetMemoryDevicesCount();

		[DllImport(dll_name)]
		static extern _SMBIOSBiosInfo SMBIOS_GetBiosInfoAtIndex(int index);

		[DllImport(dll_name)]
		static extern _SMBIOSSystemInfo SMBIOS_GetSystemInfoAtIndex(int index);

		[DllImport(dll_name)]
		static extern _SMBIOSMotherboardInfo SMBIOS_GetMotherboardInfoAtIndex(int index);

		[DllImport(dll_name)]
		static extern _SMBIOSProcessorInfo SMBIOS_GetProcessorInfoAtIndex(int index);

		[DllImport(dll_name)]
		static extern _SMBIOSMemoryDevice SMBIOS_GetMemoryDeviceAtIndex(int index);

		[StructLayout(LayoutKind.Sequential)]
		struct _SMBIOSBiosInfo
		{
			public IntPtr vendor_name;
			public IntPtr bios_version;
			public ushort bios_start_address;
			public IntPtr bios_release_date;
			public byte bios_rom_size; // 64K * (n + 1) where n is the bios_rom_size number
			public ulong bios_characteristics; // bitfield; Defines which functions the BIOS supports: PCI, PCMCIA, Flash, and so on. (see 7.1.1).
			public ushort bios_characteristics_ext;
			public byte bios_major_version;
			public byte bios_minor_version;
			public byte embedded_controller_firmware_major_version;
			public byte embedded_controller_firmware_minor_version;
			public ushort extended_bios_rom_size; // 3.1+
		}

		[StructLayout(LayoutKind.Sequential)]
		struct _UUID_RFC4122
		{
			public uint time_low;
			public ushort time_mid;
			public ushort time_high_and_version;
			public byte clock_sequence_high_and_reserved;
			public byte clock_sequence_low;
			public fixed byte node[6];
		};

		[StructLayout(LayoutKind.Sequential)]
		struct _SMBIOSSystemInfo
		{
			public IntPtr manufacturer;
			public IntPtr product_name;
			public IntPtr version;
			public IntPtr serial_number;
			public _UUID_RFC4122 uuid;
			public byte wake_up_type;
			public IntPtr sku_number;
			public IntPtr family;
		};

		[StructLayout(LayoutKind.Sequential)]
		struct _SMBIOSMotherboardInfo
		{
			public IntPtr manufacturer;
			public IntPtr product;
			public IntPtr version;
			public IntPtr serial_number;
			public IntPtr asset_tag;
			public byte feature_flags;
			public IntPtr location_in_chassis; // location is case/chassis/pc casing
			public ushort chassis_handle;
			public byte board_type;
			public byte number_of_contained_object_handles;
			public ushort* contained_object_handles;
		};

		[StructLayout(LayoutKind.Sequential)]
		struct _SMBIOSProcessorInfo
		{
			public IntPtr socket_designation; // eg. LGA1151
			public byte processor_type;
			public byte processor_family;
			public IntPtr manufacturer;
			public ulong processor_id; // processor features
			public IntPtr version;
			public byte voltage; // bitfield; what voltage the processor socket supports
			public ushort external_clock; // MHz
			public ushort max_speed; // MHz; max speed for the processor socket
			public ushort current_speed; // MHz; current speed for the processor socket
			public byte status;
			public byte upgrade; // type of socket
			public ushort l1_cache_handle;
			public ushort l2_cache_handle;
			public ushort l3_cache_handle;
			public IntPtr serial_number;
			public IntPtr asset_tag;
			public IntPtr part_number;
			public byte core_count; // requires 2.5+; number of cores per processor socket
			public byte cores_enabled; // requires 2.5+; enabled cores per processor socket
			public byte thread_count; // requires 2.5+; threads per processor socket
			public ushort processor_characteristics; // requires 2.5+; bitfield
			public ushort processor_family2; // requires 2.6+;
			public ushort core_count2; // requires 3.0+;
			public ushort core_enabled2; // requires 3.0+;
			public ushort thread_count2; // requires 3.0+;
			public ushort threads_enabled; // requires 3.6+;
		};

		[StructLayout(LayoutKind.Sequential)]
		struct _SMBIOSMemoryDevice
		{
			public ushort physical_memory_array_handle;
			public ushort memory_error_information_handle;
			public ushort total_width;
			public ushort data_width;
			public ushort size;
			public byte form_factor;
			public byte device_set;
			public IntPtr device_locator;
			public IntPtr bank_locator;
			public byte memory_type;
			public ushort type_detail;
			public ushort speed; // unit: MT/s
			public IntPtr manufacturer;
			public IntPtr serial_number;
			public IntPtr asset_tag;
			public IntPtr part_number;
			public byte attributes; // requires: 2.6+
			public uint extended_size; // requires: 2.7+
			public ushort configured_memory_speed; // requires: 2.7+
			public ushort minimum_voltage; // requires: 2.8+; unit: mV (millivolt)
			public ushort maximum_voltage; // requires: 2.8+; unit: mV (millivolt)
			public ushort configured_voltage; // requires: 2.8+; unit: mV (millivolt)
			public byte memory_technology; // requires: 3.2+
			public ushort memory_operating_mode_capability; // requires: 3.2+
			public IntPtr firmware_version; // requires: 3.2+
			public ushort module_manifacturer_id; // requires: 3.2+
			public ushort module_product_id; // requires: 3.2+
			public ushort module_subsystem_controller_manufacturer_id; // requires: 3.2+
			public ushort module_subsystem_controller_product_id; // requires: 3.2+
			public ulong non_volatile_size; // requires: 3.2+
			public ulong volatile_size; // requires: 3.2+
			public ulong cache_size; // requires: 3.2+
			public ulong logical_size; // requires: 3.2+
			public uint extended_speed; // requires: 3.3+
			public uint extended_configured_memory_speed; // requires: 3.3+
			public ushort pmic0_manifacturer_id; // requires: 3.7+
			public ushort pmic0_revision_number; // requires: 3.7+
			public ushort rcd_manifacturer_id; // requires: 3.7+
			public ushort rcd_revision_number; // requires: 3.7+
		};

		public struct SMBIOSBiosInfo
		{
			public string vendor_name;
			public string bios_version;
			public ushort bios_start_address;
			public string bios_release_date;
			public byte bios_rom_size; // 64K * (n + 1) where n is the bios_rom_size number
			public ulong bios_characteristics; // bitfield; Defines which functions the BIOS supports: PCI, PCMCIA, Flash, and so on. (see 7.1.1).
			public ushort bios_characteristics_ext;
			public byte bios_major_version;
			public byte bios_minor_version;
			public byte embedded_controller_firmware_major_version;
			public byte embedded_controller_firmware_minor_version;
			public ushort extended_bios_rom_size; // 3.1+
		}

		public struct UUID_RFC4122
		{
			public UUID_RFC4122()
			{
			}

			public uint time_low;
			public ushort time_mid;
			public ushort time_high_and_version;
			public byte clock_sequence_high_and_reserved;
			public byte clock_sequence_low;
			public byte[] node = new byte[6];
		};

		public struct SMBIOSSystemInfo
		{
			public string manufacturer;
			public string product_name;
			public string version;
			public string serial_number;
			public UUID_RFC4122 uuid;
			public byte wake_up_type;
			public string sku_number;
			public string family;
		};

		public struct SMBIOSMotherboardInfo
		{
			public string manufacturer;
			public string product;
			public string version;
			public string serial_number;
			public string asset_tag;
			public byte feature_flags;
			public string location_in_chassis; // location is case/chassis/pc casing
			public ushort chassis_handle;
			public byte board_type;
			public ushort[] contained_object_handles;
		};

		public struct SMBIOSProcessorInfo
		{
			public string socket_designation; // eg. LGA1151
			public byte processor_type;
			public byte processor_family;
			public string manufacturer;
			public ulong processor_id; // processor features
			public string version;
			public byte voltage; // bitfield; what voltage the processor socket supports
			public ushort external_clock; // MHz
			public ushort max_speed; // MHz; max speed for the processor socket
			public ushort current_speed; // MHz; current speed for the processor socket
			public byte status;
			public byte upgrade; // type of socket
			public ushort l1_cache_handle;
			public ushort l2_cache_handle;
			public ushort l3_cache_handle;
			public string serial_number;
			public string asset_tag;
			public string part_number;
			public byte core_count; // requires 2.5+; number of cores per processor socket
			public byte cores_enabled; // requires 2.5+; enabled cores per processor socket
			public byte thread_count; // requires 2.5+; threads per processor socket
			public ushort processor_characteristics; // requires 2.5+; bitfield
			public ushort processor_family2; // requires 2.6+;
			public ushort core_count2; // requires 3.0+;
			public ushort core_enabled2; // requires 3.0+;
			public ushort thread_count2; // requires 3.0+;
			public ushort threads_enabled; // requires 3.6+;
		};

		public struct SMBIOSMemoryDevice
		{
			public ushort physical_memory_array_handle;
			public ushort memory_error_information_handle;
			public ushort total_width;
			public ushort data_width;
			public ushort size;
			public byte form_factor;
			public byte device_set;
			public string device_locator;
			public string bank_locator;
			public byte memory_type;
			public ushort type_detail;
			public ushort speed; // unit: MT/s
			public string manufacturer;
			public string serial_number;
			public string asset_tag;
			public string part_number;
			public byte attributes; // requires: 2.6+
			public uint extended_size; // requires: 2.7+
			public ushort configured_memory_speed; // requires: 2.7+
			public ushort minimum_voltage; // requires: 2.8+; unit: mV (millivolt)
			public ushort maximum_voltage; // requires: 2.8+; unit: mV (millivolt)
			public ushort configured_voltage; // requires: 2.8+; unit: mV (millivolt)
			public byte memory_technology; // requires: 3.2+
			public ushort memory_operating_mode_capability; // requires: 3.2+
			public string firmware_version; // requires: 3.2+
			public ushort module_manifacturer_id; // requires: 3.2+
			public ushort module_product_id; // requires: 3.2+
			public ushort module_subsystem_controller_manufacturer_id; // requires: 3.2+
			public ushort module_subsystem_controller_product_id; // requires: 3.2+
			public ulong non_volatile_size; // requires: 3.2+
			public ulong volatile_size; // requires: 3.2+
			public ulong cache_size; // requires: 3.2+
			public ulong logical_size; // requires: 3.2+
			public uint extended_speed; // requires: 3.3+
			public uint extended_configured_memory_speed; // requires: 3.3+
			public ushort pmic0_manifacturer_id; // requires: 3.7+
			public ushort pmic0_revision_number; // requires: 3.7+
			public ushort rcd_manifacturer_id; // requires: 3.7+
			public ushort rcd_revision_number; // requires: 3.7+
		};

		static SMBios()
		{
			if (!SMBIOS_Init())
			{
				throw new InvalidOperationException("SMBIOS not supported");
			}
		}

		static string UnmanagedToManagedAndFree(this IntPtr ptr)
		{
			string str = UnmanagedToManaged(ptr);
			Marshal.FreeHGlobal(ptr);
			return str;
		}

		static string UnmanagedToManaged(this IntPtr ptr)
		{
			return Marshal.PtrToStringUTF8(ptr);
		}

		public static string GetVersion()
		{
			return SMBIOS_GetVersion().UnmanagedToManagedAndFree();
		}

		public static SMBIOSBiosInfo[] GetBiosInfos()
		{
			int count = SMBIOS_GetBiosInfosCount();
			var arr = new SMBIOSBiosInfo[count];
			for (int i = 0; i < count; i++)
			{
				var data = SMBIOS_GetBiosInfoAtIndex(i);
				arr[i].vendor_name = data.vendor_name.UnmanagedToManaged();
				arr[i].bios_version = data.bios_version.UnmanagedToManaged();
				arr[i].bios_start_address = data.bios_start_address;
				arr[i].bios_release_date = data.bios_release_date.UnmanagedToManaged();
				arr[i].bios_rom_size = data.bios_rom_size;
				arr[i].bios_characteristics = data.bios_characteristics;
				arr[i].bios_characteristics_ext = data.bios_characteristics_ext;
				arr[i].bios_major_version = data.bios_major_version;
				arr[i].bios_minor_version = data.bios_minor_version;
				arr[i].embedded_controller_firmware_major_version = data.embedded_controller_firmware_major_version;
				arr[i].embedded_controller_firmware_minor_version = data.embedded_controller_firmware_minor_version;
				arr[i].extended_bios_rom_size = data.extended_bios_rom_size;
			}

			return arr;
		}

		public static SMBIOSSystemInfo[] GetSystemInfos()
		{
			int count = SMBIOS_GetSystemInfosCount();
			var arr = new SMBIOSSystemInfo[count];
			for (int i = 0; i < count; i++)
			{
				var data = SMBIOS_GetSystemInfoAtIndex(i);
				arr[i].manufacturer = data.manufacturer.UnmanagedToManaged();
				arr[i].product_name = data.product_name.UnmanagedToManaged();
				arr[i].version = data.version.UnmanagedToManaged();
				arr[i].serial_number = data.serial_number.UnmanagedToManaged();
				arr[i].uuid = new();
				arr[i].uuid.time_low = data.uuid.time_low;
				arr[i].uuid.time_mid = data.uuid.time_mid;
				arr[i].uuid.time_high_and_version = data.uuid.time_high_and_version;
				arr[i].uuid.clock_sequence_high_and_reserved = data.uuid.clock_sequence_high_and_reserved;
				arr[i].uuid.clock_sequence_low = data.uuid.clock_sequence_low;
				arr[i].uuid.node[0] = data.uuid.node[0];
				arr[i].uuid.node[1] = data.uuid.node[1];
				arr[i].uuid.node[2] = data.uuid.node[2];
				arr[i].uuid.node[3] = data.uuid.node[3];
				arr[i].uuid.node[4] = data.uuid.node[4];
				arr[i].uuid.node[5] = data.uuid.node[5];
				arr[i].wake_up_type = data.wake_up_type;
				arr[i].sku_number = data.sku_number.UnmanagedToManaged();
				arr[i].family = data.family.UnmanagedToManaged();
			}

			return arr;
		}

		public static SMBIOSMotherboardInfo[] GetMotherboardInfos()
		{
			int count = SMBIOS_GetMotherboardInfosCount();
			var arr = new SMBIOSMotherboardInfo[count];
			for (int i = 0; i < count; i++)
			{
				var data = SMBIOS_GetMotherboardInfoAtIndex(i);
				arr[i].manufacturer = data.manufacturer.UnmanagedToManaged();
				arr[i].product = data.product.UnmanagedToManaged();
				arr[i].version = data.version.UnmanagedToManaged();
				arr[i].serial_number = data.serial_number.UnmanagedToManaged();
				arr[i].asset_tag = data.asset_tag.UnmanagedToManaged();
				arr[i].feature_flags = data.feature_flags;
				arr[i].location_in_chassis = data.location_in_chassis.UnmanagedToManaged();
				arr[i].chassis_handle = data.chassis_handle;
				arr[i].board_type = data.board_type;
				arr[i].contained_object_handles = new ushort[data.number_of_contained_object_handles];
				for (int j = 0; j < data.number_of_contained_object_handles; j++)
				{
					arr[i].contained_object_handles[j] = data.contained_object_handles[j];
				}
			}

			return arr;
		}

		public static SMBIOSProcessorInfo[] GetProcessorInfos()
		{
			int count = SMBIOS_GetProcessorInfosCount();
			var arr = new SMBIOSProcessorInfo[count];
			for (int i = 0; i < count; i++)
			{
				var data = SMBIOS_GetProcessorInfoAtIndex(i);
				arr[i].socket_designation = data.socket_designation.UnmanagedToManaged();
				arr[i].processor_type = data.processor_type;
				arr[i].processor_family = data.processor_family;
				arr[i].manufacturer = data.manufacturer.UnmanagedToManaged();
				arr[i].processor_id = data.processor_id;
				arr[i].version = data.version.UnmanagedToManaged();
				arr[i].voltage = data.voltage;
				arr[i].external_clock = data.external_clock;
				arr[i].max_speed = data.max_speed;
				arr[i].current_speed = data.current_speed;
				arr[i].status = data.status;
				arr[i].upgrade = data.upgrade;
				arr[i].l1_cache_handle = data.l1_cache_handle;
				arr[i].l2_cache_handle = data.l2_cache_handle;
				arr[i].l3_cache_handle = data.l3_cache_handle;
				arr[i].serial_number = data.serial_number.UnmanagedToManaged();
				arr[i].asset_tag = data.asset_tag.UnmanagedToManaged();
				arr[i].part_number = data.part_number.UnmanagedToManaged();
				arr[i].core_count = data.core_count;
				arr[i].cores_enabled = data.cores_enabled;
				arr[i].thread_count = data.thread_count;
				arr[i].processor_characteristics = data.processor_characteristics;
				arr[i].processor_family2 = data.processor_family2;
				arr[i].core_count2 = data.core_count2;
				arr[i].core_enabled2 = data.core_enabled2;
				arr[i].thread_count2 = data.thread_count2;
				arr[i].threads_enabled = data.threads_enabled;
			}

			return arr;
		}

		public static SMBIOSMemoryDevice[] GetMemoryDevices()
		{
			int count = SMBIOS_GetMemoryDevicesCount();
			var arr = new SMBIOSMemoryDevice[count];
			for (int i = 0; i < count; i++)
			{
				var data = SMBIOS_GetMemoryDeviceAtIndex(i);
				arr[i].physical_memory_array_handle = data.physical_memory_array_handle;
				arr[i].memory_error_information_handle = data.memory_error_information_handle;
				arr[i].total_width = data.total_width;
				arr[i].data_width = data.data_width;
				arr[i].size = data.size;
				arr[i].form_factor = data.form_factor;
				arr[i].device_set = data.device_set;
				arr[i].device_locator = data.device_locator.UnmanagedToManaged();
				arr[i].bank_locator = data.bank_locator.UnmanagedToManaged();
				arr[i].memory_type = data.memory_type;
				arr[i].type_detail = data.type_detail;
				arr[i].speed = data.speed;
				arr[i].manufacturer = data.manufacturer.UnmanagedToManaged();
				arr[i].serial_number = data.serial_number.UnmanagedToManaged();
				arr[i].asset_tag = data.asset_tag.UnmanagedToManaged();
				arr[i].part_number = data.part_number.UnmanagedToManaged();
				arr[i].attributes = data.attributes;
				arr[i].extended_size = data.extended_size;
				arr[i].configured_memory_speed = data.configured_memory_speed;
				arr[i].minimum_voltage = data.minimum_voltage;
				arr[i].maximum_voltage = data.maximum_voltage;
				arr[i].configured_voltage = data.configured_voltage;
				arr[i].memory_technology = data.memory_technology;
				arr[i].memory_operating_mode_capability = data.memory_operating_mode_capability;
				arr[i].firmware_version = data.firmware_version.UnmanagedToManaged();
				arr[i].module_manifacturer_id = data.module_manifacturer_id;
				arr[i].module_product_id = data.module_product_id;
				arr[i].module_subsystem_controller_manufacturer_id = data.module_subsystem_controller_manufacturer_id;
				arr[i].module_subsystem_controller_product_id = data.module_subsystem_controller_product_id;
				arr[i].non_volatile_size = data.non_volatile_size;
				arr[i].volatile_size = data.volatile_size;
				arr[i].cache_size = data.cache_size;
				arr[i].logical_size = data.logical_size;
				arr[i].extended_speed = data.extended_speed;
				arr[i].extended_configured_memory_speed = data.extended_configured_memory_speed;
				arr[i].pmic0_manifacturer_id = data.pmic0_manifacturer_id;
				arr[i].pmic0_revision_number = data.pmic0_revision_number;
				arr[i].rcd_manifacturer_id = data.rcd_manifacturer_id;
				arr[i].rcd_revision_number = data.rcd_revision_number;
			}

			return arr;
		}
	}
}
