#include "os_config.h"

#include "c_def.h"
#include "debug.h"
#include "oem.h"

#include "hw_timer.h"

#include "mem_reloc.h"
#include "fifo.h"
#include "audio_com.h"

#include "codec.h"
#include <string.h>

#ifdef MP3_ENCODE_ENABLE
#include "app_mp3_encode.h"
#endif

#include "ide_file.h"
#include "ide_flash.h"

#include "usbd.h"
#include "usbmsc.h"
#include "app_media_com.h"
#include "app_media_config.h"

#ifdef PTP_MTP_CLASS
#include "mtpmain.h"
#include "mtphost.h"
#include "usbmtpif.h"
#include "mtphostFS.h"
#endif

#ifdef SD_ENABLE
#include "sd_switch.h"
#endif

#include "app_media.h"

#ifdef WAV_DEC
#include "wave_decode.h"
#endif

#include "app_sdram.h"
#include "hal_key_adc.h"

#include "Audio_TAS6582.h"
#include "sona_dsp.h"
#include "audio_device.h"
#include "oem.h"

#undef STREAM_MUTEX_LOCK
#define STREAM_MUTEX_LOCK

#undef STREAM_MUTEX_UNLOCK
#define STREAM_MUTEX_UNLOCK

U8 PHASE_KEY_STATUS = TRUE;
U8 HIGH_PASS_MODE_KEY_STATUS = TRUE;
U8 FULL_RANGE_OUTPUT_KEY_STATUS = TRUE;
U8 F_LED_STATUS = TRUE;
//#define TASK_HDD_PRIORITY_CHANGE

#if (defined SD_ENABLE || defined USB_HOST_ENABLE)

#ifdef SYSTEM_CRASH_CHECK_ENABLE
volatile U8 task_hdd_check_cnt;
#endif


#define ukMsgSend    func_wo_param_msgSend


extern int osentry_HddTaskPriority( void );
extern int osentry_restoreHdd_TaskPriority( void );

//#pragma section ide_data " " ".fat_file_data1"
//#pragma use_section ide_data  ide_buf

#define	MAX_PACKET_SIZE		64

U8 ide_buf[IDE_BLOCK_SIZE] __USB_FS_BSS;
U32 wFsReadErrPos;
U8 wFsReadErr;
U8 wFsReadTries;
volatile U8 wFileSysPresent;

#if (defined RECORD_ENABLE)
volatile U8 gwReqDelCpyFile;
U8 usb_lunOpenOk;
#endif

U8 usb_lunNum[USB_LUN_OPEN_MAX];
volatile U8 usb_lunCount;
volatile U8 usb_lunIndex;
U8 wMediaMute;


#ifdef PTP_MTP_CLASS
//use sdram bss
U8 tempWriteBuffer[MAX_PACKET_SIZE]	__MTPHOST_BSS;

static U32 mtpReadObjSize			__MTPHOST_BSS;
static U32 mtpParOffset				__MTPHOST_BSS;

extern SObjectInfo mtpobjInfo;
extern U32 curObjHandle;
#endif


#ifdef SD_ENABLE
volatile U8 sd1_FileSysPresent;
#endif

extern U16 gwEntryNum;
extern U16 gwFolderNum;

volatile U8 gwUsbDeviceReady;
volatile MYFILE *pMediaRead;

U8 media_write_err_cnt;

#define TASK_USBHDD_STACK_SIZE         (2048)


void *qHDD;

void task_hdd(void *pvParameters);
void task_hdd_init( void )
{
	mutex_stream = xSemaphoreCreateMutex();
	mutex_fs = xSemaphoreCreateMutex();

	qHDD = xQueueGenericCreate( DEPTH_OF_HDD_QUEUE, (WIDTH_OF_HDD_QUEUE*4), queueQUEUE_TYPE_SET );

    if (xTaskCreate( task_hdd, " task_hdd task", TASK_USBHDD_STACK_SIZE, 0, TASK_HDD_PRIORITY, NULL) != pdTRUE )
	{
		DBG_Assert(FALSE);
    }
}


void app_media_mute_enable (void)
{
	DBG_Printf("%s\n\r", __func__);
	wMediaMute = TRUE;
}

void app_media_mute_disable (void)
{
	DBG_Printf("%s\n\r", __func__);
	wMediaMute = FALSE;
}

U8 app_media_mute_status (void)
{
	return wMediaMute;
}

U32 app_oem_get_ext_mask (void);


void task_hdd(void *pvParameters)
{
	U32 data[WIDTH_OF_HDD_QUEUE];
	HDD_MSG *msg = (HDD_MSG *)data;
	
	MYFILE *pEncFile;
	STREAM_ID s;
	SDRAM_STREAM_CONFIG stream_config;
	I32 i, pos;
	volatile U32 len, space, wlen;

#ifdef TASK_HDD_PRIORITY_CHANGE
	U8 usb_pri = FALSE;
#endif

	wMediaMute = TRUE;
	wFileSysPresent = FALSE;
	wFsReadErr = FALSE;

#if (defined RECORD_ENABLE)
	usb_lunOpenOk = FALSE;
#endif

#ifdef BT_HCI_ENABLE
	DBG_USBHPrintf ("app bt init\n\r");	
#endif
#ifdef SD_ENABLE
	sd1_FileSysPresent = FALSE;
#endif

	while (1)
	{
		QUEUE_HDD_RECEIVE;
		asm("nop");

		switch ( msg->msg )
		{
		case USB_DEV_HUB_PLUG_IN:
			func_wo_param_msgSend(app_nav_usb_hub_plug_in);
			break;

		#ifdef USB_HOST_ENABLE
		case USB_MSC_INIT:
			usb_lunCount = 0;
			memset(&usb_lunNum, 0, sizeof(usb_lunNum));

			if (msg->parm1 == TRUE)
			{
				usbDevType = enUSBH_DEV_MSC;

				usbmsc_init();

				if( usbmsc_open() )
				{
					DBG_USBHPrintf ("USBdev init success\n\r");
				}
				else
				{
					DBG_USBHPrintf ("USBdev init fail\n\r");
					USB_PowerOff(TRUE);
					USB_PowerOn();
				}
			}
			else
			{
				DBG_Assert (FALSE);
				DBG_USBHPrintf ("USBdev error\n\r");
				if (global_vari.audio_source == SOURCE_USB_MSC)
				{
					app_nav_sleep(2000);
				}
			}
			break;

		#ifdef PTP_MTP_CLASS
		case USBH_MTP_INIT:
			if (msg->parm1 == TRUE)
			{
				if (usbmtpif_init())
				{
					gwUsbDeviceReady = TRUE;
					usbDevType = enUSBH_DEV_MTP;
					if (usbmtpif_open())
					{
						I32 ret;

						ret = usbmtpif_getDeviceInfo( &mtpdevInfo );
						if( !ret )
						{
							DBG_Assert(FALSE);
							DBG_USBHPrintf ("USBdev MTP Get DevInfo err\n\r");
						}

						ret = usbmtpif_openSession();
						if( !ret )
						{
							DBG_Assert(FALSE);
							DBG_USBHPrintf ("USBdev MTP Open Session err\n\r");
						}

						wFileSysPresent = TRUE;
						DBG_USBHPrintf ("USBdev MTP Open succ\n\r");
						app_media_new_album ();
						if (global_vari.audio_source == SOURCE_USB_MSC)
						{
							func_wo_param_msgSend (app_nav_usb_device_connected);
						}
					}
					else
					{
						DBG_USBHPrintf ("USBdev MTP init fail\n\r");
					}
				}
			}
			break;
		#endif

		case USB_MSC_CLOSE:
			wFileSysPresent = FALSE;
			wFsReadErr = FALSE;
			usb_lunCount = 0;

		#if (defined RECORD_ENABLE)
			usb_lunOpenOk = FALSE;
		#endif

		#ifdef PTP_MTP_CLASS
			if (usbDevType == enUSBH_DEV_MTP)
			{
				usbmtpif_close();
				usbmtpif_fini();
			}
			else 
		#endif
			{
				usbmsc_close ();
				usbmsc_fini ();
				DBG_USBHPrintf ("usb msc close\n\r" );
			}

			//app_media_mute_disable();
			usbDevType = enUSBH_DEV_UNKOWN;
			ukMsgSend(app_nav_usb_device_disconnected);

			break;

		#ifdef FILE_SYSTEM_ENABLE
		case USB_LUN_OPEN:
			timer_usbLUNdetTimerStop();
			DBG_USBHPrintf ("ENTER USB Lun open %d\n\r", msg->parm1);

			if( !gwUsbDeviceReady )
			{
				DBG_USBHPrintf ("host stoped,so exit\n\r");
				break;
			}

			DBG_Assert(usb_lunCount < USB_LUN_OPEN_MAX);
			usb_lunNum[usb_lunCount] = msg->parm1;
			usb_lunCount++;
			app_usb_vlome_lable_name_init();

			if ( (global_vari.audio_source == SOURCE_USB_MSC) && (gwUsbDeviceReady) )
			{

			}
			else
			{
				if (wFileSysPresent)
				{
					break;
				}
			}

			if( usbmsc_LUNopen (msg->parm1) )
			{
			#if (defined RECORD_ENABLE)
				usb_lunOpenOk = TRUE;
			#endif
				DBG_USBHPrintf ("USB Lun open %d\n\r", msg->parm1);
			}
			else
			{
			#if (defined RECORD_ENABLE)
				usb_lunOpenOk = FALSE;
			#endif
				DBG_USBHPrintf ("USB Lun open failed %d\n\r", msg->parm1);
				USB_PowerOff(TRUE);
				USB_PowerOn();
				break;
			}

			if (wFsReadErr == TRUE)
			{
				STREAM_MUTEX_LOCK;

				Fs_cash_init ();
				if (wFsReadTries > 5)
				{
					Fs_init (enMEDIA_USB_FS_INDEX);
					if ( (Fs_open (enMEDIA_USB_FS_INDEX)) == 0 )
					{
						//no valid file system has been detected
						DBG_Assert (FALSE);
						func_wo_param_msgSend (app_nav_usb_device_disconnected);
					}
					else
					{
						//uMsgSend (UI_CD_INTERFACE, UI_DECODE_STREAM_EOF, 0);
						DBG_Printf ("USB Fatal Err Skipf\n\r");
					}
				}
				else
				{
					//read again
					DBG_Printf ("Re read again\n\r");
					if (Fs_file_seek (pMediaRead, wFsReadErrPos))
					{
						wFsReadErr = FALSE;
						uHddMsgSend (HDD_READ_FILE, 0, 0);
						DBG_Printf ("Re read seek end\n\r");
					}
					else
					{
						func_wo_param_msgSend (app_nav_media_fata_error);
						DBG_Printf ("Fs Seek Error\n\r");
					}

					//app_media_data.ui_background = app_nav_mp3_process;
				}

				STREAM_MUTEX_UNLOCK;
			}
			else if (global_vari.audio_source == SOURCE_USB_MSC)
			{
			#ifdef ADC_RECORD
				app_media_new_album ();
			#endif

				if (usb_lunCount == 1)
				{
					func_wo_param_msgSend (app_nav_usb_device_connected);
				}
			}
			else
			{
			#ifdef ADC_RECORD
				app_media_new_album ();
			#endif
			}
			break;
		#endif //FILE_SYSTEM_ENABLE


		case USB_LUN_CLOSE:
			DBG_USBHPrintf ("USB Lun remove %d\n\r", msg->parm1);

			if (msg->parm1 == wrUSBDevIndex)
			{
				wFileSysPresent = FALSE;
				wFsReadErr = FALSE;
				gwUsbDeviceReady = FALSE;

				usbmsc_LUNclose (msg->parm1);
			
				if (global_vari.audio_source == SOURCE_USB_MSC)
				{
					func_wo_param_msgSend (app_nav_usb_device_disconnected);
				}
			}
			break;
		#endif //USB_HOST_ENABLE


		#ifdef SD_ENABLE
		case SDMMC_INIT:
			wFsReadErr = FALSE;
			sdif_init ();
			if (sdif_open ()) 
			{
				DBG_Printf ("SD Open Succ\n\r");
			}
			else
			{
				DBG_Printf ("SD Open Fail\n\r");
				break;
			}

			if (global_vari.audio_source == MEDIA_SDMMC) 
			{
				#ifdef RECORD_ENABLE
				app_media_new_album ();
				#endif

				func_wo_param_msgSend (app_nav_usb_device_connected);
			}
			else
			{
				#ifdef RECORD_ENABLE
				app_media_new_album ();
				#endif

				app_media_device_set (MEDIA_SDMMC);
			}
			break;

		case SDMMC_CLOSE:
			sdif_close();

			sd1_FileSysPresent = FALSE;
			if (global_vari.audio_source == MEDIA_SDMMC) 
			{
				func_wo_param_msgSend (app_nav_usb_device_disconnected);
			}
			DBG_Printf ("SD Close\n\r");
			break;
		#endif //SD_ENABLE


		#ifndef UPGRADE_PROJECT_ENABLE
		case SFLASH_UPGRADE_BY_USB:
			//DBG_Printf ("SFLASH_UPGRADE_BY_USB\n\r");
			DBG_Printf ("sFlash Proram start\n\r");
			U8 *upgrade_buf = ide_buf;
			U32 offset;
			U32 len;
			U32 byte_size;
			U32 firmware_total_size;
			U32 remainder;
			U32 sector, sector_addr;
			U32 *chkSumBuf;
			U32 temp;
	
			offset = UPGRADE_FILE_UART_BAUDRATE_DET_SIZE;
			byte_size = sizeof(UPGRADE_FILE_HEADER);
			
			Fs_file_seek(upgrade_fp, offset);
			len = Fs_read_file((void *)upgrade_buf, byte_size, upgrade_fp);
			
			if (len == byte_size)
			{
				memcpy(&file_header, upgrade_buf, sizeof(UPGRADE_FILE_HEADER));

				if (file_header.rx_header_start == UPGRADE_FILE_HEADER_FLAG && 
					file_header.rx_copy_addr == UPGRADE_FILE_COPY_ADDR && 
					file_header.rx_run_addr == UPGRADE_FILE_RUN_ADDR)
				{
					offset = UPGRADE_FILE_UART_BAUDRATE_DET_SIZE + sizeof(UPGRADE_FILE_HEADER) +
						(ROM_APP_READ_START_ADDR - SFLASH_START_ADDR);
					
					Fs_file_seek(upgrade_fp, offset);
					
					firmware_total_size = file_header.extend_header.rx_firmware_size - 
										(ROM_APP_READ_START_ADDR - SFLASH_START_ADDR);

					sector = (firmware_total_size/FLASH_SECTOR_SIZE);
					remainder = firmware_total_size - sector * FLASH_SECTOR_SIZE;
					sector_addr = ROM_APP_COPY_START_ADDR;

					for (i=0; i<sector; i++)
					{
						len = Fs_read_file((void *)upgrade_buf, FLASH_SECTOR_SIZE, upgrade_fp);
						if (len == FLASH_SECTOR_SIZE)
						{
							app_flash_sector_program(sector_addr, upgrade_buf, FLASH_SECTOR_SIZE);
							sector_addr += FLASH_SECTOR_SIZE;
							flash_progrm_chksum += app_flash_program_checksum(upgrade_buf, FLASH_SECTOR_SIZE);
						}
						else
						{
							//error
							DBG_Printf ("USB Read error1\n\r");
						}

						if ( (i % 16) == 0)
						{
							DBG_printf("sector_addr:0x%x\r\n", sector_addr);
						}
					}
					
					if (remainder)
					{
						len = Fs_read_file((void *)upgrade_buf, remainder, upgrade_fp);
						if (len == remainder)
						{
							app_flash_sector_program(sector_addr, upgrade_buf, remainder);
							flash_progrm_chksum += app_flash_program_checksum(upgrade_buf, remainder);
						}
						else
						{
							//error
							DBG_Printf ("USB Read error2\n\r");
						}
					}

					if (remainder)
					{
						chkSumBuf =  (U32 *)&upgrade_buf[remainder-4];
					}
					else
					{
						chkSumBuf =  (U32 *)&upgrade_buf[FLASH_SECTOR_SIZE-4];
					}
					
					rx_firmware_chksum = *chkSumBuf;
					flash_progrm_chksum -= rx_firmware_chksum;

					DBG_printf("rx chksum:0x%x\r\n", rx_firmware_chksum);
					DBG_printf("flash_progrm_chksum:0x%x\r\n", flash_progrm_chksum);

					if (rx_firmware_chksum == flash_progrm_chksum)
					{
						ukParmSend( app_flash_copy_program, firmware_total_size);
					}
					else
					{
						CPU_SoftReset_Flag_Setting((U8 *)STRING_UPGRADE_ERROR, TRUE);
					}
				}
			}
			break;
		#endif //UPGRADE_PROJECT_ENABLE
		

		#ifdef FILE_SYSTEM_ENABLE
		case HDD_FILE_REINIT:
			STREAM_MUTEX_LOCK;
			wFsReadErr = FALSE;
			DBG_Printf ("HDD_FILE_REINIT\n\r");
			
			gwFolderNum = 0;
			gwEntryNum = 0;

			if ( (global_vari.audio_source == SOURCE_USB_MSC) )
			{
				if ( gwUsbDeviceReady && (Fs_open (enMEDIA_USB_FS_INDEX) == FALSE) )
				{
					wFileSysPresent = FALSE;
					DBG_Assert (FALSE);
				}
				else
				{
					wFileSysPresent = TRUE;
				}
			}
			#ifdef SD_ENABLE 
			else if (global_vari.audio_source == MEDIA_SDMMC)
			{
				if ( sdmmcReady && (Fs_open () == FALSE) )
				{
					sd1_FileSysPresent = TRUE;
					DBG_Assert (FALSE);
				}
				else
				{
					sd1_FileSysPresent = TRUE;
				}
			}
			#endif
			else
			{
				DBG_Assert (FALSE);
			}

			STREAM_MUTEX_UNLOCK;
			break;
		#endif //FILE_SYSTEM_ENABLE


		#ifdef FILE_DELETE_ENABLE
		case HDD_DDELETE_ENTRY:
			DBG_Printf ("HDD Delete entry\n\r");

			{ 
				U8 ret = FALSE;

				if (msg->parm1 >= app_media_data.total_tracks)
				{
					DBG_Assert (FALSE);
					break;
				}
				
				STREAM_MUTEX_LOCK;

			#ifdef SD_ENABLE
				if ( (global_vari.audio_source == MEDIA_SDMMC) && (SD_WR_Protect_Get() == enSD_WR_PROTECT))
				{
					ret = FALSE;
				}
			#else
				if (0)
				{
				}
			#endif

			#ifdef PTP_MTP_CLASS
				else if (usbDevType == enUSBH_DEV_MTP)
				{
					U32	objHandle;

					objHandle = mtpObjHandleBuffer[msg->parm1];
					ret = usbmtpif_deleteObject( objHandle, 0 );
					mtp_getDeviceFiles();
					app_media_data.ui_background = app_nav_usb_read_fs_process;
					uMsgSend (UI_USB, UI_FS_SEARCH_END, 0);
				}
			#endif
				else
				{
					U32 track_index = msg->parm1;
					U32 start_addr;

					app_media_data.fold = app_file_get_folder_of_entry(track_index);
					app_media_data.entry = app_file_get_entry_in_folder(track_index);

					app_file_search_folder (folder_info[app_media_data.fold].parent_cluster,
						folder_info[app_media_data.fold].index, NULL, 0, &start_addr);

					ret = app_file_delete_file (start_addr, (app_media_data.entry+1));
				}

				if (ret)
				{
					window_disp_state = enDELETE_END;
				}
				else
				{
				#ifdef SD_ENABLE
					if ( (global_vari.audio_source == MEDIA_SDMMC) && (SD_WR_Protect_Get()==enSD_WR_PROTECT ))
					{
						window_disp_state = enDELETE_WR_PROTECT;
					}
					else
				#endif
					{
						window_disp_state = enDELETE_ERROR;
					}
				}
				app_window_timer_count_init();
			}

			STREAM_MUTEX_UNLOCK;
			break;

		case HDD_DELTE_FOLDER:
			{
				U8 ret = FALSE;
				int entries;
				
				STREAM_MUTEX_LOCK;

			#ifdef SD_ENABLE
				if ( (global_vari.audio_source == MEDIA_SDMMC) && (SD_WR_Protect_Get() == enSD_WR_PROTECT ))
				{
					ret = FALSE;
				}
			#else
				if (0)
				{
				}
			#endif
			#ifdef PTP_MTP_CLASS
				else if (usbDevType == enUSBH_DEV_MTP)
				{
					U32	objHandle;

					objHandle = mtpObjHandleBuffer[msg->parm1];
					ret = usbmtpif_deleteObject( objHandle, 0 );
					mtp_getDeviceFiles();
					app_media_data.ui_background = app_nav_usb_read_fs_process;
					uMsgSend (UI_USB, UI_FS_SEARCH_END, 0);
				}
			#endif
				else
				{
					U32 folder_index = msg->parm1;
					U32 start_addr;
					U16 temp;
					
					temp = app_file_get_first_entrie_of_folder(folder_index);
					entries = app_file_get_folder_entries(folder_index);

					//delete from the end track of the folder to the top track of the folder
					for (i=(temp + entries - 1); i>=temp; i--)
					{
						app_media_data.fold = app_file_get_folder_of_entry(i);
						app_media_data.entry = app_file_get_entry_in_folder(i);
						
						app_file_search_folder (folder_info[app_media_data.fold].parent_cluster, 
							folder_info[app_media_data.fold].index, NULL, 0, &start_addr);
						
						ret = app_file_delete_file (start_addr, (app_media_data.entry+1));
						asm("nop");
					}
				}
				
				if (ret)
				{
					window_disp_state = enDELETE_END;
				}
				else
				{
				#ifdef SD_ENABLE
					if ( (global_vari.audio_source == MEDIA_SDMMC) && (SD_WR_Protect_Get() == enSD_WR_PROTECT ))
					{
						window_disp_state = enDELETE_WR_PROTECT;
					}
					else
				#endif
					{
						window_disp_state = enDELETE_ERROR;
					}
				}
				app_window_timer_count_init();
			}
			
			STREAM_MUTEX_UNLOCK;
			break;

		case HDD_DELETE_ALL:
			{
				U8 ret = FALSE;
				
				if (msg->parm1 >= app_media_data.total_tracks)
				{
					DBG_Assert (FALSE);
					break;
				}
				
				STREAM_MUTEX_LOCK;

			#ifdef SD_ENABLE
				if ( (global_vari.audio_source == MEDIA_SDMMC) && (SD_WR_Protect_Get() == enSD_WR_PROTECT ))
				{
					ret = FALSE;
				}
			#else
				if (0)
				{
				}
			#endif //SD_ENABLE
			#ifdef PTP_MTP_CLASS
				else if (usbDevType == enUSBH_DEV_MTP)
				{
					U32	objHandle;
					
					objHandle = mtpObjHandleBuffer[msg->parm1];

					ret = usbmtpif_deleteObject( objHandle, 0 );
					mtp_getDeviceFiles();
					app_media_data.ui_background = app_nav_usb_read_fs_process;
					uMsgSend (UI_USB, UI_FS_SEARCH_END, 0);
				}
			#endif //PTP_MTP_CLASS
				else
				{
					//fomat USB/SD fastly
					ret = Fs_delete_all();
				}

				
				if (ret)
				{
					window_disp_state = enDELETE_END;
				}
				else
				{
				#ifdef SD_ENABLE
					if ( (global_vari.audio_source == MEDIA_SDMMC) && (SD_WR_Protect_Get()==enSD_WR_PROTECT ))
					{
						window_disp_state = enDELETE_WR_PROTECT;
					}
					else
				#endif
					{
						window_disp_state = enDELETE_ERROR;
					}
				}
				app_window_timer_count_init();
			}
			STREAM_MUTEX_UNLOCK;
			break;

		#endif //FILE_DELETE_ENABLE


		#ifdef FILE_SYSTEM_ENABLE
		#if (defined MP3_ENCODE_ENABLE &&defined RECORD_ENABLE)
		#ifdef FILE_DELETE_ENABLE
		case HDD_SEARCH_FOLDER_FILE:
			STREAM_MUTEX_LOCK;
			int ret;
			ret = app_nav_search_mp3_folder_file();
			if (ret = FALSE)
			{
				DBG_Printf ("media erase error\n\r");
			}

			STREAM_MUTEX_UNLOCK;
			break;
		#endif //FILE_DELETE_ENABLE
		#endif //FILE_SYSTEM_ENABLE
		
		case HDD_SEARCH_AUDIO_ENTRY:
			STREAM_MUTEX_LOCK;

		#ifdef NO_CD_APPLICATION
			int size = sizeof(SEARCH_FOLDER_INFO) * SEARCH_FODLER_MAX_NUMBER;

			codec_malloc_init(codec_fast_mem, CODEC_FAST_MEM_SIZE, codec_slow_mem, CODEC_SLOW_MEM_SIZE);
			gpSearchFielInfo = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_4);
			gpSearchFolderNode = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_4);
 
			#ifdef FOLDER_TRACK_SORTING
			track_name_pos = (U16 *) codec_malloc ( sizeof(U16) * MAX_TRACK_NUM );
			gpFolderTrackName = (U16 *) codec_malloc ( MAX_FLODER_TRACK_NAME_LEN );
			#endif //#ifdef FOLDER_TRACK_SORTING

			#ifdef FOLDER_TRACK_SORTING
			if (gpSearchFielInfo == NULL || gpSearchFolderNode == NULL || track_name_pos == NULL || gpFolderTrackName == NULL)
			#else
			if (gpSearchFielInfo == NULL || gpSearchFolderNode == NULL)
			#endif
			{
				STREAM_MUTEX_UNLOCK;
				DBG_Printf ("Err: mem is not enough for search\n\r");
				DBG_Assert (FALSE);
				break;
			}
		#endif

			if (gwUsbDeviceReady && (global_vari.audio_source == SOURCE_USB_MSC) )
			{
				for (i=0; i<usb_lunCount; i++)
				{
					wFileSysPresent = FALSE;
					usbmsc_LUNopen (usb_lunNum[i]);
					SetWRDeviceIndex(usb_lunNum[i]);

					app_media_device_set (SOURCE_USB_MSC);

					gwFolderNum = 0;
					gwEntryNum = 0;
					Fs_init (enMEDIA_USB_FS_INDEX);

					int temp_mask = SEARCH_MASK;
					#if (defined WAV_DEC)
					temp_mask |= SEARCH_WAV_MASK;
					#endif
					#ifdef DTSDMP_DEC_TEST
					temp_mask |= SEARCH_DTSHD_MASK; 
					#endif
					#ifdef DOLBY_DDPLUS71_DEC_TEST
					temp_mask |= SEARCH_EC3_MASK; 
					#endif
					app_file_serach_set (temp_mask);

					if (Fs_open (enMEDIA_USB_FS_INDEX))
					{
						wFileSysPresent = TRUE;
						usb_lunIndex = i;
						gwEntryNum = 0;
						gwFolderNum = 0;

						#ifdef RECORD_ENABLE
						if (global_vari.audio_source == SOURCE_USB_MSC)
						#endif
						{
							while (1)
							{
								if (!app_media_status_get()) break;

								if ( (app_file_system_process ()) == PARSE_FILE_END)
								{
									if ( (gwEntryNum > 0) || (i >= (usb_lunCount-1)))
									{
										//uMsgSend (UI_USB, UI_FS_SEARCH_END, 0);
										system_msgSend(UI_FS_SEARCH_END);
										break;
									}
								}

								delayus(1);
							}
						}
					}
					else
					{
						DBG_USBHPrintf ("usb Fs_open err.\n");
					}
					
					if ( (gwEntryNum > 0) || (i >= (usb_lunCount-1)))
					{
						break;
					}
				}
			}

		#ifdef SD_ENABLE
			else if	(sdmmcReady && (global_vari.audio_source == MEDIA_SDMMC) )
			{
				sd1_FileSysPresent = FALSE;
				wFsReadErr = FALSE;
				app_media_device_set (MEDIA_SDMMC);

				gwFolderNum = 0;
				gwEntryNum = 0;
				Fs_init (enMEDIA_SD_FS_INDEX);

				int temp_mask = SEARCH_MASK;
				#ifdef DOLBY_AC3_DEC_TEST
				temp_mask |= SEARCH_AC3_MASK; 
				#endif
				#if (defined WAV_DEC)
				temp_mask |= SEARCH_WAV_MASK;
				#endif
				#ifdef DTSDMP_DEC_TEST
				temp_mask |= SEARCH_DTSHD_MASK; 
				#endif
				app_file_serach_set (temp_mask);
				
				/*			
				 *app_file_record_floder_as_root_set(FALSE);
				 *app_file_search_floder_set(SEARCH_ALL_FOLDER); //seek all folders 
				 *app_file_search_floder_set(SEARCH_EXCEPT_REC_FOLDER); //seek all folders except the recording folder
				 * app_file_record_floder_as_root_set(TRUE);
				*/


				if (Fs_open (enMEDIA_SD_FS_INDEX))
				{
					sd1_FileSysPresent = TRUE;

					#ifdef RECORD_ENABLE
					if (global_vari.audio_source == MEDIA_SDMMC)
					#endif
					{
						while (1)
						{
							if (!app_media_status_get()) break;

							if ( (app_file_system_process ()) == PARSE_FILE_END)
							{
								uMsgSend (UI_USB, UI_FS_SEARCH_END, 0);
								break;
							}
						}
					}
					#ifdef RECORD_ENABLE
					else
					{
						rec_files_num = Search_REC_folder_allfiles();
						DBG_Printf ("gwEntryNum3:%d\n",rec_files_num);
					}
					#endif
				}
				else
				{
					uMsgSend (UI_USB, UI_FS_SEARCH_END, 0);
					DBG_USBHPrintf ("sd Fs_open err.\n");
				}
			}
		#endif
			else
			{
				DBG_USBHPrintf ("Media isn't exist.\n\r");
			}

			DBG_USBHPrintf ("HDD Search Exit\n\r");

			STREAM_MUTEX_UNLOCK;
			break;

		case USB_LUN_AUDIO_ENTRY:
			STREAM_MUTEX_LOCK;
			if (gwUsbDeviceReady && (global_vari.audio_source == SOURCE_USB_MSC))
			{
				i = msg->parm1;

				//for (i=0; i<usb_lunCount; i++)
				{
					wFileSysPresent = FALSE;
					SetWRDeviceIndex(usb_lunNum[i]);

					app_media_device_set (SOURCE_USB_MSC);
					
					Fs_init (enMEDIA_USB_FS_INDEX);	
					if (Fs_open (enMEDIA_USB_FS_INDEX))
					{
						wFileSysPresent = TRUE;
						//usb_lunIndex = i;
						//break;

						while (1)
						{
							if (!app_media_status_get()) break;

							if ( (app_file_system_process ()) == PARSE_FILE_END)
							{
								//uMsgSend (UI_USB, UI_FS_SEARCH_END, 0);
								system_msgSend(UI_FS_SEARCH_END);
								break;
							}
						}
					}
					else
					{
						DBG_USBHPrintf ("usb Fs_open err.\n");
					}
				}
			}
			else
			{
				//uMsgSend (UI_USB, UI_FS_SEARCH_END, 0);
				system_msgSend(UI_FS_SEARCH_END);
				DBG_USBHPrintf ("Media isn't exist.\n");
			}

			DBG_USBHPrintf ("HDD Search Exit\n\r");
			STREAM_MUTEX_UNLOCK;
			break;
		#endif //FILE_SYSTEM_ENABLE


		#ifdef PTP_MTP_CLASS
		case USBH_MTP_SEARCH_AUDIO_ENTRY:
			DBG_USBHPrintf ("MTP Search Enter\n\r");
			STREAM_MUTEX_LOCK;
			mtp_getDeviceFiles();
			uMsgSend (UI_USB, UI_FS_SEARCH_END, 0);
			DBG_USBHPrintf ("MTP Search Exit\n\r");
			STREAM_MUTEX_UNLOCK;
			break;
		#endif


		#ifdef FILE_SYSTEM_ENABLE
		case HDD_FILE_SEEK:
		#ifdef SD_ENABLE 
			if ((global_vari.audio_source != SOURCE_USB_MSC) && (global_vari.audio_source != MEDIA_SDMMC)) break;
		#else
			if (global_vari.audio_source != SOURCE_USB_MSC) break;
		#endif				

			STREAM_MUTEX_LOCK;
			DBG_Printf ("Fs seek start\n\r");

		#ifdef PTP_MTP_CLASS
			if (usbDevType == enUSBH_DEV_MTP)
			{
				U32 size, tsize;
				size = mtpobjInfo.ObjectCompressedSize;

				mtpParOffset = msg->parm1;
				mtpReadObjSize = mtpParOffset;
			}
			else
		#endif
			{
				if (pMediaRead == NULL)
				{
					DBG_Assert (FALSE);
				}
				else
				{
					Fs_file_seek (pMediaRead, msg->parm1);
				}
			}

			DBG_Printf ("Fs seek end\n\r");
			STREAM_MUTEX_UNLOCK;
			break;

		case HDD_READ_FILE:
			DBG_Printf("HDD_READ_FILE Enter\n\r");
			if (pMediaRead == NULL)
			{
				DBG_Assert (FALSE);
				break;
			}

			STREAM_MUTEX_LOCK;

			Fs_file_seek (pMediaRead, msg->parm1);

			HDD_READ_FILE_EVENT_SET;

			#ifdef TASK_HDD_PRIORITY_CHANGE
			usb_pri = FALSE;
			#endif
			
			while (1) 
			{
				if (!app_media_status_get())
				{
					break;
				}

				watchdog_time_reset();

				//get sdram space
				space = AUDIO_FIFO_STREAM_FREE_LEN();
				if (space >= IDE_BLOCK_SIZE)
				{
					#ifdef TASK_HDD_PRIORITY_CHANGE
					if (!usb_pri)
					{
						//set high proity
						usb_pri = TRUE;
						osentry_HddTaskPriority();
					}
					#endif

					#ifdef SYSTEM_CRASH_CHECK_ENABLE
					task_hdd_check_cnt = 1;
					#endif
					
					len = Fs_read_file((BYTE *) ide_buf, IDE_BLOCK_SIZE, pMediaRead);
					pos = Fs_file_tell (pMediaRead);
					
					#ifdef SYSTEM_CRASH_CHECK_ENABLE
					task_hdd_check_cnt = 2;
					#endif

					if (!app_media_status_get())
					{
						break;
					}

					if ( (len != IDE_BLOCK_SIZE) && Fs_file_EOF (pMediaRead) != TRUE)
					{
						/*Fs Read Fatal Err, need to power off usb device, reinit*/
						wFileSysPresent = FALSE;
						wFsReadErr = TRUE;
						wFsReadTries++;
						wFsReadErrPos = pos;

						func_wo_param_msgSend (app_nav_media_fata_error);
						DBG_Printf ("Fs Read Fatal Error\n\r");
						break;
					}
					else
					{
						wFsReadErr = FALSE;
						wFsReadTries = 0;

						#ifdef WAV_DEC
						U32 adjust_wav_header;
						if (wav_jump_adjust == TRUE)
						{
							adjust_wav_header = app_wav_get_data_pos ();
							
							if (adjust_wav_header > 1020)
							{
								//for 24 bit
								adjust_wav_header = adjust_wav_header % 1152;
							}
							
							wlen = fifo_put_data (&audio_env.stream_fifo,  (U8 *)&ide_buf[adjust_wav_header], (IDE_BLOCK_SIZE - adjust_wav_header));
							wlen += adjust_wav_header;

							wav_jump_adjust = FALSE;
						}
						else
						#endif
						{
							wlen = (U32) AUDIO_FIFO_STREAM_PUT_DATA(ide_buf, IDE_BLOCK_SIZE);
						}

						HDD_READ_FILE_EVENT_SET;

						if ( wlen != IDE_BLOCK_SIZE )
						{    	
							if ( ((AUDIO_FIFO_STREAM_STREAM_LEN()) == (AUDIO_FIFO_STREAM_FIFO_WLEN())) )
							{
								DBG_Printf ("WStream End\n\r");
								break;
							}
							else
							{
								//fatal error
								DBG_Assert (FALSE);
								DBG_Printf ("WStream Fatal Error\n\r");
								break;
							}
						}
					}
				}
				#ifdef TASK_HDD_PRIORITY_CHANGE
				else
				{
					if (usb_pri)
					{
						//set low
						usb_pri = FALSE;
						osentry_restoreHdd_TaskPriority();
					}
				}
				#endif
			}

			#ifdef TASK_HDD_PRIORITY_CHANGE
			if (usb_pri)
			{
				//set low
				usb_pri = FALSE;
				osentry_restoreHdd_TaskPriority();
			}
			#endif

			/***********************************************/
			HDD_READ_FILE_EVENT_SET;
			//HDD_READ_FILE_EVENT_SET;

			DBG_Printf("HDD_READ_FILE Exit\n\r");

			STREAM_MUTEX_UNLOCK;
			break;
		#endif //FILE_SYSTEM_ENABLE


		#ifdef PTP_MTP_CLASS
		case USBH_MTP_READ_FILE:
			{
				U8 ret;
				U32	size, tsize, tmp;
				U32 rlen;

				if( app_media_data.media != MEDIA_USB_DEVICE ) break;
				DBG_Printf("MTP read entered\n\r");
	
				app_media_data.wMediaStatus = WSTREAM_BUSY;
				curObjHandle = msg->parm1;

				STREAM_MUTEX_LOCK;
				
				s = app_sdram_stream_get_wstream();	

				DBG_Printf("rd obj handle: %d\n\r", curObjHandle );

				ret	= usbmtpif_getObjectInfo( curObjHandle, &mtpobjInfo );
				if (ret)
				{
					size = mtpobjInfo.ObjectCompressedSize;
					if (size > IDE_BLOCK_SIZE)
					{
						tsize = IDE_BLOCK_SIZE;
					}
					else
					{
						tsize = size;
					}

					ret = usbmtpif_getObjectOpen( curObjHandle, size, ide_buf, tsize, &rlen );
					DBG_Printf("open rd tsize:%d  rlen:%d\n\r", tsize, rlen);

					if (!ret) {
						DBG_Printf("MTP read err1\n\r");
						DBG_Assert(FALSE);
					}
				}
				else
				{
					DBG_Assert(FALSE);
					DBG_Printf("MTP read err2\n\r");
				}

				//trans
				tmp = rlen;
				DBG_Printf ("MTP file len:%d\n\r", size);
			
				while (1)
				{
					if ( (wMediaMute == TRUE) || (gwUsbDeviceReady == FALSE) )
					{
						break;
					}

					watchdog_time_reset();

					//get sdram space
					space = app_sdram_stream_get_free_space();
					if (space >= IDE_BLOCK_SIZE)
					{
						wlen = (I16) app_sdram_stream_write (s, (U8 *) ide_buf, rlen);
						break;
					}
				}

				while (1) 
				{
					if ((wMediaMute == TRUE) || (gwUsbDeviceReady == FALSE) )
					{
						break;
					}

					watchdog_time_reset();

					//get sdram space
					space = app_sdram_stream_get_free_space();
					if (space >= IDE_BLOCK_SIZE)
					{						
						if (tmp >= size)
						{
							app_sdram_stream_close_wstream (s);
							uMsgSend (UI_CD_RECEIVER,UI_STREAM_EOF,0);
							DBG_Printf ("WStream End\n\r");
							break;
						}

						tsize	=	size - tmp;
						if (tsize > IDE_BLOCK_SIZE) {
							tsize	=	IDE_BLOCK_SIZE;
						}

						ret = usbmtpif_getObject( curObjHandle, ide_buf, tsize, &rlen );
						if (!ret)
						{
							DBG_Assert(FALSE);
							DBG_Printf("MTP read err3\n\r");
							break;
						}
						if (rlen != tsize) {
							DBG_Assert(FALSE);
							DBG_Printf("MTP read err4\n\r");
							break;
						}

						tmp += rlen;

						if ((wMediaMute == TRUE) || (gwUsbDeviceReady == FALSE) )
						{
							break;
						}

						wlen = (I16) app_sdram_stream_write (s, (U8 *) ide_buf, rlen);
						HDD_READ_FILE_EVENT_SET;
					}
				}

				/***********************************************/
				app_media_data.wMediaStatus = WSTREAM_IDLE;
				if (usbmtpif_getObjectClose( curObjHandle ))
				{
					DBG_Printf("MTP Close suc\n\r");
				}
				else
				{
					DBG_Printf("MTP Close err\n\r");
				}
				
				/***********************************************/
				HDD_READ_FILE_EVENT_SET;
				HDD_READ_FILE_EVENT_SET;

				DBG_Printf ("Read Stream End2\n\r");
				STREAM_MUTEX_UNLOCK;
			}
			break;

		case USBH_MTP_READ_PARTIAL_FILE:
			{
				U8 ret;
				U32	size, tsize;
				U32 rlen;

				if( global_vari.audio_source != SOURCE_USB_MSC ) break;
				DBG_Printf("MTP read partial entered");

				app_media_data.wMediaStatus = WSTREAM_BUSY;
				curObjHandle = msg->parm1;

				STREAM_MUTEX_LOCK;
				
				s = app_sdram_stream_get_wstream();	

				ret	= usbmtpif_getObjectInfo( curObjHandle, &mtpobjInfo );
				if (ret)
				{
					size = mtpobjInfo.ObjectCompressedSize;
				}
				else
				{
					DBG_Assert(FALSE);
				}

				//trans
				if (msg->parm2 == 0)
				{
					//don't initialize the following variables,  when seeking a file. 
					mtpReadObjSize = 0;
					mtpParOffset = 0;
				}

				DBG_Printf ("MTP file len:%d\n\r", size);

				while (1) 
				{
					if ( (wMediaMute == TRUE) || (gwUsbDeviceReady == FALSE) ) {
						break;
					}

					watchdog_time_reset();

					//get sdram space
					space = app_sdram_stream_get_free_space();
					if (space >= IDE_BLOCK_SIZE) {
						
						if (mtpReadObjSize >= size) {
							app_sdram_stream_close_wstream (s);
							uMsgSend (UI_CD_RECEIVER,UI_STREAM_EOF,0);
							DBG_Printf ("WStream End\n\r");
							break;
						}

						tsize	=	size - mtpReadObjSize;
						if (tsize > IDE_BLOCK_SIZE) {
							tsize	=	IDE_BLOCK_SIZE;
						}

						ret = usbmtpif_getPartialObject( curObjHandle, mtpParOffset, tsize, ide_buf, &rlen );
						if ( (!ret) || (rlen != tsize) ) {
							DBG_Assert(FALSE);
							DBG_Printf("MTP partial read err\n\r");
							break;
						}
						
						mtpReadObjSize += rlen;
						mtpParOffset += rlen;
						if (gwUsbDeviceReady == FALSE) {
							break;
						}

						wlen = (I16) app_sdram_stream_write (s, (U8 *) ide_buf, rlen);
						HDD_READ_FILE_EVENT_SET;
					}
				}

				/***********************************************/
				app_media_data.wMediaStatus = WSTREAM_IDLE;

				/***********************************************/
				HDD_READ_FILE_EVENT_SET;
				HDD_READ_FILE_EVENT_SET;

				DBG_Printf ("Read Stream End3\n\r");
				STREAM_MUTEX_UNLOCK;
			}
			break;
		#endif


		#if (defined RECORD_ENABLE || defined ADC_RECORD)
		case HDD_WRITE_ENC_FILE:
			DBG_Puts("Lock Enter\n\r");
			STREAM_MUTEX_LOCK;

			Ripping_Message = RIPPING_NO_ERR;

			pEncFile = app_media_create_rec_file ();
			if (pEncFile == NULL)
			{
				/*failed to write file*/
				DBG_Assert (FALSE);
				Ripping_Message = RIPPING_MEDIA_FULL;

				app_media_data.record_status = FALSE;
				func_wo_param_msgSend(app_nav_record_stop);

				STREAM_MUTEX_UNLOCK;
				break;
			}

			#ifdef TASK_HDD_PRIORITY_CHANGE
			osentry_HddTaskPriority();
			#endif

			while (1)
			{
				watchdog_time_reset();

				len = (U32) fifo_get_data_len (&record_fifo);
				if (len >= IDE_BLOCK_SIZE)
				{
					len = fifo_get_data(&record_fifo, (U8 *) ide_buf, IDE_BLOCK_SIZE);
					wlen = Fs_file_write ((U8 *) ide_buf, len, pEncFile);

					app_timer_idle_time_reset ();

				#ifdef SD_ENABLE
					if ( ((!gwUsbDeviceReady) && (app_media_data.record_media == REC_TO_USB)) ||
						 ((!gwUsbDeviceReady) && (app_media_data.record_media == REC_TO_USB1)) ||
						 ((!sdmmcReady) && (app_media_data.record_media == REC_TO_SDMMC)) )
				#else
					if (gwUsbDeviceReady == FALSE)
				#endif
					{
						Ripping_Message = RIPPING_USB_REMOVED;

						app_media_data.record_status = FALSE;
						func_wo_param_msgSend(app_nav_record_stop);
						break;
					}

					if (wlen != len)
					{
						DBG_Printf ("HDD write error1\n\r");
						
						if ( Fs_get_last_err (pEncFile) == FAT_NO_CLUSTER )
						{
							DBG_Puts ("(1)Media is full\n\r");
							Ripping_Message = RIPPING_MEDIA_FULL;

							/*del file*/
							if (pEncFile->lenth < 0x4000)
							{
								Fs_del_close_file (pEncFile);
							}

						}
						else
						{
							Ripping_Message = RIPPING_WRITE_ERR;
						}

						app_media_data.record_status = FALSE;
						func_wo_param_msgSend(app_nav_record_stop);
						break;
					}
					else
					{
						/*checking amount of data write to this file*/
						pos = Fs_file_tell (pEncFile);

						/* pos
						 * 0x014000 -> about 5 sec 192 kbps
						 * 0x028000 -> about 10 sec 192 kbps
						 * 0x090000 -> about 30 sec 192 kbps
						 * 0x180000 -> about 65 sec 192 kbps
						 */
						if ( (pos % 0x028000) == 0)   //about 10 sec 192 kbps
						{
							Fs_flush_file_buf (pEncFile);
						}

					#ifdef RECORD_ENABLE
z
						//create new file every 4 hours. 
						if (app_media_data.record_time > 4*60*60)
						{
							creatFileNum++;
							if (creatFileNum >= 3)
							{
								DBG_Puts ("Rec 12 hours\n\r");
								func_wo_param_msgSend(app_nav_record_stop);
							}
							else
							{
								func_wo_param_msgSend(app_nav_adc_record_create_new_file);
								break;
							}
						}
						else
						{
							if (record_new_file)
							{
								record_new_file = FALSE;
								func_wo_param_msgSend(app_nav_adc_record_create_new_file);
								break;
							}
						}

					#endif //ADC_RECORD
					}
				}
				else
				{
					if (wMediaMute == TRUE)
					{
						//read all data in the fifo
						len = fifo_get_data(&record_fifo, (U8 *) ide_buf, len);
						if (len >= IDE_BLOCK_SIZE)
						{
							len = IDE_BLOCK_SIZE;
						}
						wlen = Fs_file_write ((U8 *) ide_buf, len, pEncFile);
					
					#ifdef SD_ENABLE
						if ( ((!gwUsbDeviceReady) && (app_media_data.record_media == REC_TO_USB)) ||
							((!gwUsbDeviceReady) && (app_media_data.record_media == REC_TO_USB1)) ||
							((!sdmmcReady) && (app_media_data.record_media == REC_TO_SDMMC)) )
					#else
						if (gwUsbDeviceReady == FALSE)
					#endif
						{
							Ripping_Message = RIPPING_USB_REMOVED;

							app_media_data.record_status = FALSE;
							func_wo_param_msgSend(app_nav_record_stop);
							break;
						}

						if (wlen != len)
						{
							DBG_Assert (FALSE);
							DBG_Puts ("writing error\n\r");
							
							if ( Fs_get_last_err (pEncFile) == FAT_NO_CLUSTER )
							{
								DBG_Puts ("Media is full\n\r");
								Ripping_Message = RIPPING_MEDIA_FULL;

								//delete file
								if (pEncFile->lenth < 0x4000)
								{
									Fs_del_close_file (pEncFile);
								}
							}
							else
							{
								Ripping_Message = RIPPING_WRITE_ERR;
								Fs_del_close_file (pEncFile);
							}

							app_media_data.record_status = FALSE;
							func_wo_param_msgSend(app_nav_record_stop);
						}
						break;
					}
				}
			}

			#ifdef TASK_HDD_PRIORITY_CHANGE
				osentry_restoreHdd_TaskPriority();
			#endif
	
			DBG_Puts ("REC 1\n\r");
			if (pEncFile->lenth == 0)
			{
				Fs_del_close_file (pEncFile);
				DBG_Puts ("del REC file:\n\r");
			}
			else if (Ripping_Message != RIPPING_WRITE_ERR)
			{
				#ifdef WAV_ENCODE_ENABLE
				if (app_media_data.record_format == RECORD_FORMAT_WAV)
				{
					//if wave file, need to update lnegh to the file
					if (Fs_file_seek (pEncFile, 0)) 
					{
						U32 file_len;

						file_len = pEncFile->lenth - 8;
						memcpy (ide_buf, wave_header, sizeof(wave_header));
						ide_buf[4] = (file_len & 0x000000FF);
						ide_buf[5] = (file_len & 0x0000FF00) >> 8;
						ide_buf[6] = (file_len & 0x00FF0000) >> 16;
						ide_buf[7] = (file_len & 0xFF000000) >> 24;

						file_len = pEncFile->lenth - 44;

						ide_buf[40] = (file_len & 0x000000FF);
						ide_buf[41] = (file_len & 0x0000FF00) >> 8;
						ide_buf[42] = (file_len & 0x00FF0000) >> 16;
						ide_buf[43] = (file_len & 0xFF000000) >> 24;
						
						Fs_file_write ((U8 *) ide_buf, sizeof(wave_header), pEncFile);
					}
				}
				#endif

				Fs_close_file (pEncFile);
			}
			DBG_Puts ("REC 2\n\r");

			STREAM_MUTEX_UNLOCK;
			DBG_Puts("Lock Exit\n\r");
			break;
		#endif //(defined RECORD_ENABLE || defined ADC_RECORD)


		#ifdef MP3_FILE_COPY
		case HDD_WRITE_CPY_FILE:
			STREAM_MUTEX_LOCK;

			gwReqDelCpyFile = FALSE;
			Ripping_Message = RIPPING_NO_ERR;

			#ifndef WMA_FILE_NO_PLAYING_COPY
			if (app_media_data.playing_stream_type == STREAM_WMA)
			{
				app_nav_record_stop();
				lcdDispNoSupportType = enLCD_DISP_WMA_NO_SUPPORT;
				func_wo_param_msgSend (app_window_no_support_window_init_for_no_rolling);
				STREAM_MUTEX_UNLOCK;
				break;
			}
			#endif

			pEncFile = app_media_create_cpy_file ((U16 *) msg->parm1, app_media_data.playing_stream_type);
			if (pEncFile == NULL)
			{
				/*failed to write file*/
				DBG_Assert (FALSE);
				Ripping_Message = RIPPING_FILE_EXIST;
				app_media_data.record_status = FALSE;

				app_nav_record_stop();
				STREAM_MUTEX_UNLOCK;
				break;
			}

			DBG_Puts ("Create copy file\n\r");

			while (1)
			{
				watchdog_time_reset();

				len = (U32) fifo_get_fifo_data_len (&record_fifo);
				if (len >= IDE_BLOCK_SIZE)
				{
					len = fifo_get_data(&record_fifo, (U8 *) ide_buf, IDE_BLOCK_SIZE);
					wlen = Fs_file_write ((U8 *) ide_buf, len, pEncFile);

				#ifdef SD_ENABLE
					if ( ((!gwUsbDeviceReady) && (app_media_data.record_media == REC_TO_USB)) ||
						((!gwUsbDeviceReady) && (app_media_data.record_media == REC_TO_USB1)) ||
						((!sdmmcReady) && (app_media_data.record_media == REC_TO_SDMMC)) )
				#else
					if (gwUsbDeviceReady == FALSE)
				#endif
					{
						Ripping_Message = RIPPING_USB_REMOVED;

						app_media_data.record_status = FALSE;
						func_wo_param_msgSend(app_nav_record_stop);
						break;
					}

					if (wlen != len)
					{
						DBG_Assert (FALSE);
						DBG_Puts ("write error\n\r");

						if ( Fs_get_last_err (pEncFile) == FAT_NO_CLUSTER )
						{
							DBG_Puts ("(3)Media is full\n\r");
							Ripping_Message = RIPPING_MEDIA_FULL;

							/*delete file*/
							Fs_del_close_file (pEncFile);
						}
						else
						{
							Ripping_Message = RIPPING_WRITE_ERR;

							/*delete file*/
							Fs_del_close_file (pEncFile);
						}

						app_media_data.record_status = FALSE;
						func_wo_param_msgSend(app_nav_record_stop);
						break;
					}
				}
				else
				{
					if (wMediaMute == TRUE)
					{
						DBG_Assert (FALSE);
						while (1)
						{
							//read all data in the fifo
							len = (U32) fifo_get_fifo_data_len (&record_fifo);
							if (len == 0)
							{
								break;
							}
							else if (len >= IDE_BLOCK_SIZE)
							{
								len = IDE_BLOCK_SIZE;
							}
							len = fifo_get_data(&record_fifo, (U8 *) ide_buf, len);
							wlen = Fs_file_write ((U8 *) ide_buf, len, pEncFile);

						#ifdef SD_ENABLE
							if ( ((!gwUsbDeviceReady) && (app_media_data.record_media == REC_TO_USB)) ||
								((!gwUsbDeviceReady) && (app_media_data.record_media == REC_TO_USB1)) ||
								((!sdmmcReady) && (app_media_data.record_media == REC_TO_SDMMC)) )
						#else
							if (gwUsbDeviceReady == FALSE)
						#endif
							{
								Ripping_Message = RIPPING_USB_REMOVED;

								app_media_data.record_status = FALSE;
								func_wo_param_msgSend(app_nav_record_stop);
								break;
							}
							
							if (wlen != len)
							{
								DBG_Assert (FALSE);
								DBG_Puts ("write error\n\r");

								if ( Fs_get_last_err (pEncFile) == FAT_NO_CLUSTER )
								{
									DBG_Puts ("(4)Media is full\n\r");
									Ripping_Message = RIPPING_MEDIA_FULL;

									/*delete file*/
									Fs_del_close_file (pEncFile);
								}
								else
								{
									Ripping_Message = RIPPING_WRITE_ERR;

									/*delete file*/
									Fs_del_close_file (pEncFile);
								}

								app_media_data.record_status = FALSE;
								func_wo_param_msgSend(app_nav_record_stop);
								break;
							}
						}

						break;
					}
					else
					{
						asm("nop");
					}
				}
			}

			DBG_Puts ("COPY 1\n\r");
			if (pEncFile->lenth == 0)
			{
				Fs_del_close_file (pEncFile);
				DBG_Puts ("del COPY file 1:\n\r");
			}
			else if (gwReqDelCpyFile == TRUE)
			{
				Fs_del_close_file (pEncFile);
				DBG_Puts ("del COPY file 2:\n\r");
			}
			else if (Ripping_Message != RIPPING_WRITE_ERR)
			{
				Fs_close_file (pEncFile);
			}
			DBG_Puts ("COPY 2\n\r");
			STREAM_MUTEX_UNLOCK;
			break;

		#endif //MP3_FILE_COPY


		default:
			break;
		}
	}
}



#if (defined RECORD_ENABLE || defined ADC_RECORD)


#define TOSHIBA_TY_CR80

#ifdef TOSHIBA_TY_CR80
U16 wRecFolderName[7];
#else
U16 wRecFolderName[5];
#endif

#ifdef TOSHIBA_TY_CR80
//MATXXX
#define REC_FOLDER_START_POS	3

//Trackxxx
#define REC_FILE_START_POS		5

#else

//FXXX
#define REC_FOLDER_START_POS	1

//F001Txxx
#define REC_FILE_START_POS		5
#endif


void app_media_new_album (void)
{
	wRecFolderName[0] = 0;
}

#define CREATE_RECORD_FOLDER
	
MYFILE * app_media_create_rec_file (void)
{
	U16 num;
	U16 rec_name[13];
	U8 rec_num[4];
	U8 pos, i, len;
	MYFILE *pFile;
	U16 wRecFolderNum = 0;

	Fs_change_dir ((U16 *) ROOT_DIR);

	#ifdef CREATE_RECORD_FOLDER
	//create a recording folder
	if (Fs_change_dir ((U16 *) RECORD_FOLDER) == FALSE)
	{
		if (Fs_create_file ((U16 *) RECORD_FOLDER, ATTR_DIRECTORY) == FALSE)
		{
			/*fail to create Record folder*/
			DBG_Printf ("Failed to create Enc Folder\n\r");
			DBG_Assert (FALSE);
			return NULL;
		}

		if (Fs_change_dir ((U16 *) RECORD_FOLDER) == FALSE)
		{
			/*fail to create Record folder*/
			DBG_Assert (FALSE);
			return NULL;
		}
	}

	if (wRecFolderName[0] != 0x0)
	{
		if (Fs_change_dir ((U16 *) wRecFolderName) == FALSE)
		{
			/*fail to create Record folder*/
			DBG_Assert (FALSE);
			wRecFolderName[0] = 0x0;	/*no current rec folder*/
		}
	}

	if (wRecFolderName[0] == 0x0)
	{
		/*need to create one more album*/
		wRecFolderNum = Fs_find_max_rec_folder (REC_FOLDER_START_POS);
		if (wRecFolderNum > (MAX_FOLDER_NUM-1) )
		{
			return NULL;
		}

		wRecFolderNum++;

	#ifdef TOSHIBA_TY_CR80
		wRecFolderName[0] = 'M';
		wRecFolderName[1] = 'A';
		wRecFolderName[2] = 'T';
		wRecFolderName[3] = '0';
		wRecFolderName[4] = '0';
		wRecFolderName[5] = '0';
		wRecFolderName[6] = 0x0;
	#else
		wRecFolderName[0] = 'F';
		wRecFolderName[1] = '0';
		wRecFolderName[2] = '0';
		wRecFolderName[3] = '0';
		wRecFolderName[4] = 0x0;
	#endif

		/*the num should be U16 range*/
		memset(rec_num, 0, sizeof(rec_num));
		itoa ( (U16) wRecFolderNum, rec_num);

		len = strlen ((const char *) rec_num);
		if (len > 3)
		{
			DBG_Assert (FALSE);
			len = 3;
		}

	#ifdef TOSHIBA_TY_CR80
		pos = 6 - len;
	#else
		pos = 4 - len;
	#endif

		for (i=0; i<len; i++)
		{
			wRecFolderName[pos+i] = rec_num[i];
		}

		if (Fs_create_file (wRecFolderName, ATTR_DIRECTORY) == FALSE)
		{
			DBG_Assert (FALSE);
			return NULL;
		}

		if (Fs_change_dir ((U16 *) wRecFolderName) == FALSE)
		{
			/*fail to create Record folder*/
			DBG_Assert (FALSE);
			return NULL;
		}
	}
	#endif //CREATE_RECORD_FOLDER


	#ifdef TOSHIBA_TY_CR80
	rec_name[0] = 'T';
	rec_name[1] = 'r';
	rec_name[2] = 'a';
	rec_name[3] = 'c';
	rec_name[4] = 'k';
	rec_name[5] = '0';
	rec_name[6] = '0';
	rec_name[7] = '0';
	rec_name[8] = '.';
	#ifdef WAV_ENCODE_ENABLE
	if (app_media_data.record_format == RECORD_FORMAT_WAV)
	{
		rec_name[9] = 'W';
		rec_name[10] = 'A';
		rec_name[11] = 'V';
	}
	else
	#endif
	{
		rec_name[9] = 'M';
		rec_name[10] = 'P';
		rec_name[11] = '3';
	}
	rec_name[12] = 0;
	#else
	wstrncpy (rec_name, wRecFolderName, 8);

	rec_name[4] = 'T';
	rec_name[5] = '0';
	rec_name[6] = '0';
	rec_name[7] = '0';
	rec_name[8] = '.';	
	#if def WAV_ENCODE_ENABLE
	if (app_media_data.record_format == RECORD_FORMAT_WAV)
	{
		rec_name[9] = 'W';
		rec_name[10] = 'A';
		rec_name[11] = 'V';
	}
	else
	#endif
	{
		rec_name[9] = 'M';
		rec_name[10] = 'P';
		rec_name[11] = '3';
	}
	rec_name[12] = 0;
	#endif


	num = Fs_find_max_rec_file (REC_FILE_START_POS);
	num++;

	if (num > (MAX_REC_NUM - 1) )
	{
		DBG_Assert (FALSE);
		return NULL;
	}

	/*the num should be U16 range*/
	memset(rec_num, 0, sizeof(rec_num));
	itoa ( (U16) num, rec_num);

	len = strlen ((const char *) rec_num);
	if (len > 3)
	{
		DBG_Assert (FALSE);
		len = 3;
	}

	#ifdef TOSHIBA_TY_CR80
	pos = 8 - len;
	#else
	pos = 8 - len;
	#endif

	for (i=0; i<len; i++)
	{
		rec_name[pos+i] = rec_num[i];
	}

	if (Fs_create_file (rec_name, ATTR_ARCHIVE) == FALSE)
	{
		DBG_Assert (FALSE);
		return NULL;
	}
		
	pFile = Fs_file_open (rec_name, WRITE_READ);
	if (pFile == NULL)
	{
		DBG_Assert (FALSE);
		return NULL;
	}

	return pFile;
}

MYFILE * app_media_create_cpy_file (U16 *pBuf, U16 ext)
{
	U16 num;
	U16 rec_name[13];
	U8 rec_num[4];
	U8 pos, i, len;
	MYFILE *pFile;
	U16 wRecFolderNum = 0;

	if (*pBuf == 0)
	{
		return NULL;
	}
	
	Fs_change_dir ((U16 *) ROOT_DIR);
	
	if (Fs_change_dir ((U16 *) RECORD_FOLDER) == FALSE)
	{
		if (Fs_create_file ((U16 *) RECORD_FOLDER, ATTR_DIRECTORY) == FALSE)
		{
			/*fail to create Record folder*/
			DBG_Assert (FALSE);
			return NULL;
		}
			
		if (Fs_change_dir ((U16 *) RECORD_FOLDER) == FALSE)
		{
			/*fail to create Record folder*/
			DBG_Assert (FALSE);
			return NULL;
		}
	}

	if (wRecFolderName[0] != 0x0)
	{
		if (Fs_change_dir ((U16 *) wRecFolderName) == FALSE)
		{
			/*fail to create Record folder*/
			DBG_Assert (FALSE);
			wRecFolderName[0] = 0x0;	/*no current rec folder*/
		}
		else
		{
			if (Fs_create_file (pBuf, ATTR_ARCHIVE) == FALSE)
			{
				/*media full or file name exit*/
				wRecFolderName[0] = 0x0;	/*no current rec folder*/

				Fs_change_dir ((U16 *) ROOT_DIR);
	
				if (Fs_change_dir ((U16 *) RECORD_FOLDER) == FALSE)
				{
					/*fail to create Record folder*/
					DBG_Assert (FALSE);
					return NULL;
				}
			}
			else
			{
				pFile = Fs_file_open (pBuf, WRITE_READ);

				if (pFile == NULL) {
					DBG_Assert (FALSE);
					return NULL;
				}

				return pFile;
			}
		}
	}

	if (wRecFolderName[0] == 0x0)
	{
		/*need to create one more album*/
		wRecFolderNum = Fs_find_max_rec_folder (REC_FOLDER_START_POS);
		if (wRecFolderNum > (MAX_FOLDER_NUM-1))
		{
			return NULL;
		}

		wRecFolderNum++;

		#ifdef TOSHIBA_TY_CR80
		wRecFolderName[0] = 'M';
		wRecFolderName[1] = 'A';
		wRecFolderName[2] = 'T';
		wRecFolderName[3] = '0';
		wRecFolderName[4] = '0';
		wRecFolderName[5] = '0';
		wRecFolderName[6] = 0x0;
		#else
		wRecFolderName[0] = 'F';
		wRecFolderName[1] = '0';
		wRecFolderName[2] = '0';
		wRecFolderName[3] = '0';
		wRecFolderName[4] = 0;
		#endif

		/*the num should be U16 range*/
		memset(rec_num, 0, sizeof(rec_num));
		itoa ( (U16) wRecFolderNum, rec_num);

		len = strlen ((const char *) rec_num);
		if (len > 3)
		{
			DBG_Assert (FALSE);
			len = 3;
		}

		#ifdef TOSHIBA_TY_CR80
		pos = 6 - len;
		#else
		pos = 4 - len;
		#endif

		for (i=0; i<len; i++)
		{
			wRecFolderName[pos+i] = rec_num[i];
		}

		if (Fs_create_file (wRecFolderName, ATTR_DIRECTORY) == FALSE)
		{
			DBG_Assert (FALSE);
			return NULL;
		}
	}

	if (Fs_change_dir ((U16 *) wRecFolderName) == FALSE)
	{
		/*fail to create Record folder*/
		DBG_Assert (FALSE);
		return NULL;
	}

	if (Fs_create_file (pBuf, ATTR_ARCHIVE) == FALSE)
	{
		/*media full or file name exist*/
		wRecFolderName[0] = 0x0;	/*no current rec folder*/
		return NULL;
	}
	else
	{
		pFile = Fs_file_open (pBuf, WRITE_READ);
		if (pFile == NULL)
		{
			DBG_Assert (FALSE);
			return NULL;
		}

		return pFile;
	}
}
#endif //(defined CD_RIPPING || defined ADC_RECORD)



U8 app_media_status_get(void)
{
	U8 ret = TRUE;

#ifdef SD_ENABLE
	if ( (wMediaMute == TRUE) || ((!gwUsbDeviceReady) && (global_vari.audio_source == SOURCE_USB_MSC)) )
	{
		ret = FALSE;
		//break;
	}
	else if ( (wMediaMute == TRUE) || ((!sdmmcReady) && (global_vari.audio_source == MEDIA_SDMMC)) )
	{
		ret = FALSE;
		//break;
	}
#else
	if ( (wMediaMute == TRUE) || (!gwUsbDeviceReady) )
	{
		ret = FALSE;
		//break;
	}
#endif

	return ret;
}


U32 app_media_file_create_data_set(void)
{
	//2012(Year) 01(Month) 01(Dad)
	U16 temp;

	temp = (FS_CREATE_FILE_YEAR - FS_CREATE_FILE_YEAR_OFFSET) * 512 +
		FS_CREATE_FILE_MONTH * 32 + FS_CREATE_FILE_DAY;

	return  temp;
}

U32 app_media_file_create_time_set(void)
{
	//12(Hour):10(Minute):20(Second)
	U16 temp;
	
	temp = FS_CREATE_FILE_HOUR * 2048 + FS_CREATE_FILE_MINUTE * 32 + FS_CREATE_FILE_SECOND * 2;

	return  temp;	
}


#ifndef FILE_SYSTEM_ENABLE
U16 gwEntryNum;
U16 gwFolderNum;
U32 usb_media_contents;

U16 app_file_get_entry_in_folder (U16 track_index)
{
	return 0;
}

U16 app_file_get_folder_of_entry (U16 track_index)
{
	return 0;
}

U16 app_file_get_first_entrie_of_folder (U16 index)
{
	return 0;
}


U16 app_file_get_folder_entries (U16 index)
{
	return 0;
}
#endif //FILE_SYSTEM_ENABLE

#else

void app_media_mute_enable (void)
{
}

void app_media_mute_disable (void)
{
}

U8 app_media_mute_status (void)
{
	return 0;
}

U32 app_media_file_create_data_set(void)
{
	return 0;
}

U32 app_media_file_create_time_set(void)
{
	return 0;
}
U8 app_media_status_get(void)
{
	U8 ret = TRUE;
	return ret;
}
void app_sw_key_status_check(U32 tick)
{
	static U8 high_pass_status = TRUE,f_status_bak = TRUE;
	static U8 phase_key_status = TRUE,full_range_key_status =TRUE;

	static int timers = 0,count = 0;
	static U8 pre_state = 0,cur_state =0;
	
	PHASE_KEY_STATUS = hal_gpio_input_level_read(PHASE_SW);
 	FULL_RANGE_OUTPUT_KEY_STATUS = hal_gpio_input_level_read(FRO_SW);
	HIGH_PASS_MODE_KEY_STATUS = hal_gpio_input_level_read(HPM_SW);
	F_LED_STATUS = hal_gpio_input_level_read(F_SW);

	#if 0
	timers++;
	if(timers == 40)
	{
		printf_info("p = %d,f = %d,h = %d,f = %d\r\n",PHASE_KEY_STATUS,FULL_RANGE_OUTPUT_KEY_STATUS,HIGH_PASS_MODE_KEY_STATUS,F_LED_STATUS);
		timers = 0;
	}
	#endif
	if(high_pass_status != HIGH_PASS_MODE_KEY_STATUS && HIGH_PASS_MODE_KEY_STATUS == FALSE)
	{
		high_pass_status = HIGH_PASS_MODE_KEY_STATUS;
		if(audio_in_stream.sample_rate == 44100)
		{
			#ifdef DSP_EQ_SETTING_ENABLE
				lr_hp100hz_enable(TRUE,44100);
			#endif
			#ifdef DSP_SONA_MODULE_ENABLE
			audio_vari.high_pass_mode = TRUE;
			#endif
		}
		else
		{
			#ifdef DSP_EQ_SETTING_ENABLE
				lr_hp100hz_enable(TRUE,48000);
			#endif

			#ifdef DSP_SONA_MODULE_ENABLE
			audio_vari.high_pass_mode = TRUE;
			#endif
		}
		DBG_Printf("high_pass_key down\r\n");
	}
	else if(high_pass_status!= HIGH_PASS_MODE_KEY_STATUS && HIGH_PASS_MODE_KEY_STATUS == TRUE)
	{
		high_pass_status = HIGH_PASS_MODE_KEY_STATUS;
		#ifdef DSP_EQ_SETTING_ENABLE

		lr_hp100hz_enable(FALSE,44100);
		#endif
		#ifdef DSP_SONA_MODULE_ENABLE
		audio_vari.high_pass_mode = FALSE;
		#endif
		DBG_Printf("high_pass_key up\r\n");
	}

	if(phase_key_status != PHASE_KEY_STATUS && PHASE_KEY_STATUS == FALSE)
	{
		#ifdef DSP_SONA_MODULE_ENABLE
		audio_vari.phase = TRUE;
		#endif
		phase_key_status = PHASE_KEY_STATUS;
		DBG_Printf("pahse_key down\r\n");
	}
	else if(phase_key_status != PHASE_KEY_STATUS && PHASE_KEY_STATUS == TRUE)
	{
		#ifdef DSP_SONA_MODULE_ENABLE
		audio_vari.phase = FALSE;
		#endif
		phase_key_status = PHASE_KEY_STATUS;
		DBG_Printf("pahse_key up\r\n");
	}

	if(full_range_key_status != FULL_RANGE_OUTPUT_KEY_STATUS  && FULL_RANGE_OUTPUT_KEY_STATUS == FALSE)
	{
		#ifdef DSP_SONA_MODULE_ENABLE
		audio_vari.stereoOrMono = TRUE;
		#endif
		full_range_key_status = FULL_RANGE_OUTPUT_KEY_STATUS;
		DBG_Printf("full_range_key down\r\n");
	}
	else if(full_range_key_status != FULL_RANGE_OUTPUT_KEY_STATUS  && FULL_RANGE_OUTPUT_KEY_STATUS == TRUE)
	{
		#ifdef DSP_SONA_MODULE_ENABLE
		audio_vari.stereoOrMono = FALSE;
		#endif
		full_range_key_status = FULL_RANGE_OUTPUT_KEY_STATUS;
		DBG_Printf("full_range_key up\r\n");

	}

	if(f_status_bak != F_LED_STATUS && F_LED_STATUS == FALSE)
	{
		f_status_bak = F_LED_STATUS;
		hal_gpio_output_level_set(F_LED_CTL,GPIO_HIGH);
		DBG_Printf("front_led_key down\r\n");
	}
	else if(f_status_bak != F_LED_STATUS && F_LED_STATUS == TRUE)
	{
		f_status_bak = F_LED_STATUS;
		hal_gpio_output_level_set(F_LED_CTL,GPIO_LOW);
		DBG_Printf("front_led_key up\r\n");
	}

	if(pre_state != cur_state )
	{
		pre_state = cur_state;
		count = 0;
	}
	if(hal_gpio_input_level_read(AC_INT_DET) == GPIO_HIGH)
	{
		hal_gpio_output_level_set(BATT_EN,GPIO_HIGH);
		hal_gpio_output_level_set(PVDD_AC_EN,GPIO_LOW);
		cur_state = TRUE;
		// DBG_Printf("batt_en\r\n");
		// if(++count >10)
		// {
		// 	// func_w_one_param_msgSend(audio_amp_tas6582_class_H,TRUE);
		// 	count = 0;
		// }

	}
	else
	{
		cur_state = FALSE;
		hal_gpio_output_level_set(BATT_EN,GPIO_LOW);
		hal_gpio_output_level_set(PVDD_AC_EN,GPIO_HIGH);
		// DBG_Printf("AC_EN\r\n");
		// if(++count >10)
		// {
		// 	// func_w_one_param_msgSend(audio_amp_tas6582_class_H,FALSE);
		// 	count = 0;
		// }

	}

}

#if 0
//red board test
void app_phase_key_status_test(void)
{
	if(PHASE_KEY_STATUS)
	{
		PHASE_KEY_STATUS = FALSE;
	}
	else
	{
		PHASE_KEY_STATUS = TRUE;
	}
	
}
void app_full_range_output_key_status_test(void)
{
	if(FULL_RANGE_OUTPUT_KEY_STATUS)
	{
		FULL_RANGE_OUTPUT_KEY_STATUS = FALSE;
	}
	else
	{
		FULL_RANGE_OUTPUT_KEY_STATUS = TRUE;
	}
	
}
void app_high_pass_mode_key_test(void)
{
	if(HIGH_PASS_MODE_KEY_STATUS)
	{
		HIGH_PASS_MODE_KEY_STATUS = FALSE;
	#if  0//def DSP_EQ_SETTING_ENABLE

		lr_hp100hz_enable(TRUE,44100);
	}
	else
	{
		HIGH_PASS_MODE_KEY_STATUS = TRUE;
		lr_hp100hz_enable(FALSE,44100);
	#endif
	}
	
}

#endif
#endif
