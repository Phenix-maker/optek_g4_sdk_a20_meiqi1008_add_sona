#include "os_config.h"

#include "regmap.h"

#include "mem_reloc.h"

#include "app_main.h"


#define MULTI_PROJECTS


#ifdef MULTI_PROJECTS

#define FLASH_LOADER_CONFIG_USED;

//#define UPGRADE_BOOT_SPACE_FILL



#ifdef UPGRADE_BOOT_SPACE_FILL 
const U8 upgrade_boot_dummy[16] __attribute__((section(".rom_upgrade_boot"))) = 
{
	0x06,0x01,0x00,0x00,0x00,0x08,0x00,0x6A,0x01,0xFF,0xFF,0xA0,0x00,0x00,0x00,0x00
};
#endif


#ifdef UPGRADE
//const U8 flash_boot_config[16] __attribute__((section(".boot_rodata"))) = {"UPGUSBEFA"};

//const U8 flash_boot_config[16] __attribute__((section(".boot_rodata"))) = {"UPGUSBEFP"};
//const U8 flash_boot_config[16] __attribute__((section(".boot_rodata"))) = {"UPG BTEFP"};

const U8 flash_boot_config[16] __attribute__((section(".boot_rodata"))) = {"UPGUSBEFA 2M"};
//const U8 flash_boot_config[16] __attribute__((section(".boot_rodata"))) = {"UPGUSBEFP 2M"};
//const U8 flash_boot_config[16] __attribute__((section(".boot_rodata"))) = {"UPG BTEFP 2M"};
#endif //UPGRADE


//#define UPGRADE_CHECK_TEST

#ifdef UPGRADE_CHECK_TEST
const U8 flash_boot_config[16] __attribute__((section(".boot_rodata"))) = {"UPGUSBEFP 2M"};
//const U8 flash_boot_config[16] __attribute__((section(".boot_rodata"))) = {"UPG BTEFP 2M"};
#else
//const U8 flash_boot_config[16] = {"BT APP"};
const U8 flash_boot_config[16] __attribute__((section(".boot_rodata"))) = {"DD APP    2M"};
#endif



#if 0//def BT_BLE_UPGRADE_ENABLE
#ifdef BOOT_APP_RUNNING_FLAG
//const U8 flash_boot_app_run_config[16] __attribute__((section(".boot_app_run_flag_rodata"))) = {"UPPER CODE:NEW"};
//const U8 flash_boot_app_run_config[16] __attribute__((section(".boot_app_run_flag_rodata"))) = {"LOWER CODE:NEW"};
const U8 flash_boot_app_run_config[16] __attribute__((section(".boot_app_run_flag_rodata"))) = {"HCIAPP"};
#endif

const U8 flash_boot_firmware_size[16] __attribute__((section(".boot_firmware_size_rodata"))) = {"0"};
#endif


#ifdef MULTI_PROJECTS
//U8 sram_boot_config[16];
U8 sram_boot_config[16]	__attribute__ ((section (".internal_ram_boot_bss"))) = {"OTKAPP    2M"};
#endif


#ifdef MULTI_PROJECTS
//const U8 flash_dummy_data[16] __attribute__((section(".rom_dummy_rodata"))) = {"DUMMY"};
#endif


#ifdef FLASH_LOADER_CONFIG_USED

#if 0//def BOOT_FROM_FLASH
U32 SFLASH_BOOT_LOADER_STORE_ADDR[];

BOOT_LOADER_STRU *pBootLoader = (BOOT_LOADER_STRU *)SFLASH_BOOT_LOADER_STORE_ADDR;

#else
const BOOT_LOADER_STRU boot_loader_init __attribute__((section(".boot_copy_loader_rodata"))) = 
{
	#if 1
	0x400000,
	0x6A080000,
	0x70000000,
	0x01,
	0xFF,
	0xFF,
	0xFF,
	#else
	0x400000,
	SFLASH_BOOT_LOADER_STORE_ADDR,
	0x70000000,
	0x01,
	0xFF,
	0xFF,
	0xFF,
	#endif
};

#endif

#endif //FLASH_LOADER_CONFIG_USED



#if 0
enum {
	enUSB_DEV_UPGRADE,
	enBT_DEV_UPGRADE,
//	enSD_DEV_UPGRADE,
//	enUART_DEV_UPGRADE
};

//#define SFLASH_2M_BYTE
#define SFLASH_4M_BYTE

#if defined SFLASH_1M_BYTE
#define SFLASH_BIT_SIZE     (1*8)
#elif defined SFLASH_2M_BYTE
#define SFLASH_BIT_SIZE    (2*8)
#elif defined SFLASH_4M_BYTE
#define SFLASH_BIT_SIZE    (4*8)
#else
#define SFLASH_BIT_SIZE    (8*8)
#endif
#endif



//extern UPGRADE_HW_SET_DATA upgrade_hw_set_data;
extern const UPGRADE_HW_SET_DATA upgrade_hw_set_data;

//UPGRADE_HW_SET_DATA upgrade_hw_set_data = 
//UPGRADE_HW_SET_DATA upgrade_hw_set_data __UPGRADE_HW_SET_RODATA = 
//const UPGRADE_HW_SET_DATA upgrade_hw_set_data = 
const UPGRADE_HW_SET_DATA upgrade_hw_set_data __UPGRADE_HW_SET_RODATA = 
{
#if 1
	//sFlash size
	SFLASH_BIT_SIZE, //8M Bits, 16M Bits, 32 Mits or 64M Bits sFlash.

	//GPIO 0 Group
	0x00000000, //gpio in read
	0x00000000, //'0': Output low level, '1' Outpu high level
	0xFFFFFFFF, //'0': Input enable,     '1': Output enable
	0x00000000, //'0': GPIO fucntion,    '1': no GPIO function
	0x00000000, //'0': Input read disable, '1': Input read enable

	//GPIO 3 Group
	0x00000000, //'0':,    '1':

	#if defined UPGRADE_BY_USB
	enUSB_DEV_UPGRADE, //upgrade by USB
	#elif defined UPGRADE_BY_BT
	enBT_DEV_UPGRADE,  //upgrade by BT
	#else
	#error Must Define upgrading media.
	#endif
	11, //SD Detect Pin Index:11

	//"BT Ugrade\0\0",        //bt name
	//"BT Ugrade Init\0\0",   //bt init name
	//"Optek_BT_Ugrade\0\0", //bt_upgrade_file_name
#endif
	0x00000000, //'0':,    '1':
};

#endif //MULTI_PROJECTS