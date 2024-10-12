#ifndef _FLASH_BOOT_H
#define _FLASH_BOOT_H


#undef BOOT_FROM_FLASH
#define BOOT_FROM_FLASH

#ifdef BOOT_FROM_FLASH
#define WATCH_DOG
#endif


#endif //_FLASH_BOOT_H
