#include "os_config.h"

#ifdef FILE_SYSTEM_ENABLE

#include "usbmsc.h"
#include "ide_flash.h"

#ifdef SD_ENABLE
#include "sdif.h"
#endif



#ifdef FS_TWO_SUPPORT
MEDIA_DEVICE mDev [2]= {
	{ USB_MASTER},
	{ SD_DEVICE},
};
#else
MEDIA_DEVICE mDev [1]= {
	{ USB_MASTER},
};
#endif

void app_media_device_set (U16 media_index)
{
	int dev_index;
	
	if (media_index == SOURCE_USB_MSC)
	{
		#ifdef USB_HOST_ENABLE		
		dev_index = 0;

		mDev[dev_index].init = usbmsc_init;
		mDev[dev_index].fini = usbmsc_fini;
		mDev[dev_index].open = usbmsc_open;
		mDev[dev_index].close = usbmsc_close;
		mDev[dev_index].sector_write = usbmsc_sectorWrite;
		mDev[dev_index].sector_read= usbmsc_sectorRead;
		mDev[dev_index].get_capacity = usbmsc_getCapacity;
		mDev[dev_index].get_media = usbmsc_Get_Media;
		mDev[dev_index].erase = usbmsc_Erase;
		mDev[dev_index].format_unit = usbmsc_Format_Unit;
		mDev[dev_index].get_format_progress = usbmsc_Get_Format_Propgress;
		mDev[dev_index].get_sector_size = usbmsc_getSectorSize;
		mDev[dev_index].media_active = usbmsc_Media_Active;
		#endif

		DBG_Puts("Media USB\n\r");
	}
	#ifdef SD_ENABLE	
	else if (media_index == MEDIA_SDMMC)
	{
		#ifdef SD_ENABLE

		#ifdef FS_TWO_SUPPORT
		dev_index = 1;
		#else
		dev_index = 0;
		#endif

		mDev[dev_index].init = sdif_init;
		mDev[dev_index].fini = sdif_fini;
		mDev[dev_index].open = sdif_open;
		mDev[dev_index].close = sdif_close;
		mDev[dev_index].sector_write = sdif_writeBlocks;
		mDev[dev_index].sector_read= sdif_readBlocks;
		mDev[dev_index].get_capacity = sdif_getCapacity;
		mDev[dev_index].get_media = sdif_Get_Media;
		mDev[dev_index].erase = sdif_Erase;
		mDev[dev_index].format_unit = sdif_Format_Unit;
		mDev[dev_index].get_format_progress = sdif_Get_Format_Propgress;
		mDev[dev_index].get_sector_size = sdif_getSectorSize;
		mDev[dev_index].media_active = sdif_Media_Active;

		DBG_Puts("Media SD\n\r");
		#endif
	}
	#endif
	else
	{
		DBG_Printf("Media Unknown\n\r");
		DBG_Assert(FALSE);
	}
}

void app_media_fs_number_set (U16 media_index)
{
	if (media_index == SOURCE_USB_MSC)
	{
		Fs_number_select(enMEDIA_USB_FS_INDEX);
	}
	#ifdef SD_ENABLE
	else if (media_index == MEDIA_SDMMC)
	{
		Fs_number_select(enMEDIA_SD_FS_INDEX);
	}
	#endif
	else
	{
		DBG_Printf("Media Unknown\n\r");
		DBG_Assert(FALSE);
	}
}

#endif //FILE_SYSTEM_ENABLE

