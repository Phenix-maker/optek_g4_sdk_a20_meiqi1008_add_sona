#ifndef __APP_MAIN__
#define __APP_MAIN__


#if 1
enum {
	enUSB_DEV_UPGRADE,
	enBT_DEV_UPGRADE,
//	enSD_DEV_UPGRADE,
//	enUART_DEV_UPGRADE
};

#define SFLASH_2M_BYTE
//#define SFLASH_4M_BYTE

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


typedef struct {
	U32	byte_size;
	U32	start_address;
	U32	run_address;
	U8 enable;
	U8 reserved_1;
	U8 reserved_2;
	U8 reserved_3;
} __attribute__ ((packed)) BOOT_LOADER_STRU;

extern const BOOT_LOADER_STRU boot_loader_init;
extern U32 *pBootLoader;


#define BT_NAME_STRING_MAX_LEN    64

typedef struct 
{
	unsigned long sflash_size;

	unsigned long gpio0_din;
	unsigned long gpio0_dout;
	unsigned long gpio0_doe;
	unsigned long gpio0_func;
	unsigned long gpio0_ie;

	unsigned long gpio3_func;

	unsigned long upgrade_media;
	unsigned long sd_detect_pin_index;

	char bt_name[BT_NAME_STRING_MAX_LEN]; 
	char bt_init_name[BT_NAME_STRING_MAX_LEN];
	char bt_upgrade_file_name[BT_NAME_STRING_MAX_LEN];

	unsigned long gpio4_func;

//} UPGRADE_HW_SET_DATA;
} __attribute__ ((packed)) UPGRADE_HW_SET_DATA;


#endif //__APP_MAIN__