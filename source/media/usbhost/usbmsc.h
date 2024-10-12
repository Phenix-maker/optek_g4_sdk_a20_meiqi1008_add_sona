#ifndef	_USB_MSC_H_
#define	_USB_MSC_H_


#ifdef IPOD_USB_AUDIO
#include "ipod.h"
#endif


#define	DEVINFO_MAX_STRING_LEN	20
#define	MSC_VID_LEN		8
#define	MSC_PDI_LEN		16

#ifdef	IPOD_MEM_SIZE_DOWN
#define	USB_HOST_MAX_SUPPORT_LUNS	4
#else
#define	USB_HOST_MAX_SUPPORT_LUNS	8
#endif

typedef struct {
	U8	valid;

	U16	usbVid;
	U16 usbPid;
	U16	usbProduct[DEVINFO_MAX_STRING_LEN];
	U16	usbManufacture[DEVINFO_MAX_STRING_LEN];

	U8	mscVid[USB_HOST_MAX_SUPPORT_LUNS][MSC_VID_LEN];
	U8	mscPid[USB_HOST_MAX_SUPPORT_LUNS][MSC_PDI_LEN];

	U8	totLUNs;
	U8	validLuns;
	U32	capacity[USB_HOST_MAX_SUPPORT_LUNS];
	U32	sectorSize[USB_HOST_MAX_SUPPORT_LUNS];

	U8	lunOpen[USB_HOST_MAX_SUPPORT_LUNS];
	U8	lunWriteProtect[USB_HOST_MAX_SUPPORT_LUNS];
}	DEVICE_INFO;


typedef enum {
	enUSB_FSMSG_CONNECT,
	enUSB_FSMSG_DISCONNECT,
	enUSB_FSMSG_MEDIUM_INSERT,
	enUSB_FSMSG_MEDIUM_REMOVE,
	enUSB_FSMSG_OVER_CURRENT,
	enUSB_FSMSG_FATAL_ERR,

	enFSMSG_MTP_FILE_SEARCH,
	enFSMSG_MTP_FILE_READ,
	enFSMSG_MTP_FILE_WRITE
}	enum_USB_MSG;

typedef enum {
	enMSC_UNKOWN,
	enMSC_SUCCESS,
	enMSC_FAIL,
	enMSC_CANCLE,
	enMSC_DEV_DISCONNECT
}	enum_MSC_RESULT;

BOOL usbmsc_init( void );
BOOL usbmsc_open( void );
BOOL usbmsc_close( void );
BOOL usbmsc_fini( void );

typedef enum {
	enUSB_HSTK_ATTR_MSC			=	(1 << 0),
	enUSB_HSTK_ATTR_IAP1_DEV	=	(1 << 1),

	enUSB_HSTK_ATTR_AOA_AUDIO	=	(1 << 2),
	enUSB_HSTK_ATTR_AOA_APPS	=	(1 << 3),
	
	enUSB_HSTK_ATTR_MTP 		=	(1 << 4),
	enUSB_HSTK_ATTR_IAP1_DEV_CHARGE_ONLY 	=	(1 << 5),
	
	//for USB_HOST_AUDIO_ENABLE
	enUSB_HSTK_ATTR_HOST_AUDIO	=	(1 << 6)
}	enum_USB_HOST_STACK_ATTR;

#if defined AOA_USB_AUDIO
#define USB_HSTK_ATTR_DEFAULT       ( enUSB_HSTK_ATTR_MSC | enUSB_HSTK_ATTR_IAP1_DEV | enUSB_HSTK_ATTR_AOA_AUDIO)
#else
#define	USB_HSTK_ATTR_DEFAULT       ( enUSB_HSTK_ATTR_MSC )
#endif


#if defined USB_HOST_AUDIO_ENABLE
#define	USB_HSTK_ATTR_HOST_ADDED    ( USB_HSTK_ATTR_DEFAULT | enUSB_HSTK_ATTR_HOST_AUDIO )
#else
#define USB_HSTK_ATTR_HOST_ADDED    ( USB_HSTK_ATTR_DEFAULT )
#endif


U8 usbmsc_startUSBhostEx( enum_USB_HOST_STACK_ATTR attr );
U8 usbmsc_USBhostOpenEx( enum_USB_HOST_STACK_ATTR attr );

U8 usbmsc_startUSBhost( void );
U8 usbmsc_stopUSBhost( void );
U8 usbmsc_USBhostOpen( void );
U8 usbmsc_USBhostClose( void );


U16 usbmsc_sectorRead(U8 * buf, U32 lba, U16 sectorCount);
U16 usbmsc_sectorWrite(U8 * buf, U32 lba, U16 sectorCount);
U32 usbmsc_getCapacity(void);
U16 usbmsc_getSectorSize (void);

U8 usbmsc_LUNopen( U8 lun );
U8 usbmsc_LUNclose( U8 lun );

//return enum_MSC_RESULT
U8	usbmsc_getLastErr(void);
U8 usbmsc_getDevInfo( DEVICE_INFO **devInfo );

U8 usbmsc_isLunWriteProtect( U8 lun );
U8 usbmsc_setLunWriteProtect( U8 lun, U8 wp );

WORD usbmsc_Get_Media (void);
BOOL usbmsc_Erase (DWORD sector_addr, DWORD len);
BOOL usbmsc_Media_Active (void);
BOOL usbmsc_Format_Unit (void);
BOOL usbmsc_Get_Format_Propgress (BYTE *format_progress);
WORD _ATA_Read (BYTE *buf, DWORD lba, WORD sector_count);

void SetWRDeviceIndex(U8 index);
void SetWRDeviceIndexInit(void);
U16 getDeviceVID (void);

#ifdef USB_HOST_ENABLE
#define USB_MASTER	usbmsc_init,				\
					usbmsc_fini,				\
					usbmsc_open,				\
					usbmsc_close,				\
					usbmsc_sectorWrite,			\
					usbmsc_sectorRead,			\
					usbmsc_getCapacity,			\
					usbmsc_Get_Media,			\
					usbmsc_Erase,				\
					usbmsc_Format_Unit,			\
					usbmsc_Get_Format_Propgress,\
					usbmsc_getSectorSize,		\
					usbmsc_Media_Active
#endif


#define USB_DEVICE_NO_LUN_OPEN		0xFF

extern U8 wrUSBDevIndex;

#endif //_USB_MSC_H_
