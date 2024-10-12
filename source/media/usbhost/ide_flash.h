#ifndef _APP_FLASH_H_
#define _APP_FLASH_H_


#include "ide_file.h"

enum {
	#ifdef FS_TWO_SUPPORT
	enMEDIA_USB_FS_INDEX = 0,
	enMEDIA_SD_FS_INDEX = 1,
	#else
	enMEDIA_USB_FS_INDEX = 0,
	enMEDIA_SD_FS_INDEX = 0,
	#endif
};


enum {
	IDE0 = 0,
	IDE1,
	MMC
};


typedef struct _APP_MEDIA_DEVICE {
	BOOL		(*init) (void);
	BOOL		(*fini) (void);
	BOOL		(*open) (void);
	BOOL		(*close) (void);
    WORD        (*sector_write) (BYTE *wBuf, DWORD sector_addr, WORD sector_count);
    WORD        (*sector_read) (BYTE *rBuf, DWORD sector_addr, WORD sector_count);
	DWORD		(*get_capacity) (void);
	WORD		(*get_media) (void);
	BOOL		(*erase) (DWORD sector_addr, DWORD len);
	BOOL		(*format_unit) (void);
	BOOL		(*get_format_progress) (BYTE *format_progress);
	WORD		(*get_sector_size) (void);
	BOOL		(*media_active) (void);
} MEDIA_DEVICE;



extern MEDIA_DEVICE mDev [];

void app_media_device_set (U16 media_index);
void app_media_fs_number_set (U16 media_index);

#endif //_APP_FLASH_H_
