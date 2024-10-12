#include "os_config.h"



#ifdef USB_HOST_ENABLE
#include "mem_reloc.h"

#include "usbd.h"
#include "IRP.h"
#include "hstack.h"
#include "mscClass.h"
#include "usbmsc.h"

#include "app_media_config.h"


static void usbmsc_irpCompleteFunc( U8 iprResult, U32 iprActualLen ) __INTERNAL_RAM_TEXT;
U16	usbmsc_sectorWrite(U8 * buf, U32 lba, U16 sectorCount) __INTERNAL_RAM_TEXT;


//make sure all calls are issued from one task
DEVICE_INFO	deviceInfo	__USBHOST_BSS;

static U8 mscError;
U32	transLength;
U8 wrUSBDevIndex;
U8 usbmsc_USBhostOpened = FALSE;



int usbtest_forceDevType( void )
{
	return	enUSB_DEV_TEST_BYPASS;
}

static void usbmsc_irpCompleteFunc( U8 iprResult, U32 iprActualLen )
{
	mscError	=	iprResult;
	transLength	=	iprActualLen;

	usbmsg_IRPcompleteFunc();
}

static void getDeviceInfo( void )
{
	IRP	irp;

	if( deviceInfo.valid ) {
		return;
	}

	FILL_NORMAL_IRP( &irp, enIRPT_MSC_CLASS, enIRP_GET_DEVINFO,	0,
		0,	&deviceInfo,	sizeof(deviceInfo),	enIRPR_SUBMIT, usbmsc_irpCompleteFunc );

	usbmsg_sendIRP( &irp );
	usbmsg_waitIRPcomplete();
	DBG_assert( mscError == enIRPR_SUCCESS );

	deviceInfo.valid	=	TRUE;
}

BOOL usbmsc_init( void )
{
	memset( &deviceInfo, 0, sizeof(deviceInfo) );
	
	deviceInfo.valid	=	FALSE;
	return TRUE;
}

BOOL usbmsc_open( void )
{
	IRP	irp;

	USB_MUTEX_LOCK;
	DBG_assert( !deviceInfo.valid );

	FILL_NORMAL_IRP( &irp, enIRPT_MSC_CLASS, enIRP_OPEN,	0,
		0,	0,	0,	enIRPR_SUBMIT, usbmsc_irpCompleteFunc );

	usbmsg_sendIRP( &irp );
	usbmsg_waitIRPcomplete();
	DBG_assert( mscError == enIRPR_SUCCESS );

	if( mscError == enIRPR_SUCCESS ) {
		getDeviceInfo();

		USB_MUTEX_UNLOCK;
		return TRUE;
	}

	USB_MUTEX_UNLOCK;
	return FALSE;
}

BOOL usbmsc_close( void )
{
	IRP	irp;

	USB_MUTEX_LOCK;
	DBG_assert( deviceInfo.valid );

	FILL_NORMAL_IRP( &irp, enIRPT_MSC_CLASS, enIRP_CLOSE,	0,
		0,	0,	0,	enIRPR_SUBMIT, usbmsc_irpCompleteFunc );

	usbmsg_sendIRP( &irp );
	usbmsg_waitIRPcomplete();
	DBG_assert( mscError == enIRPR_SUCCESS );

	memset( &deviceInfo, 0, sizeof(deviceInfo) );	
	deviceInfo.valid	=	FALSE;
	memset( deviceInfo.lunOpen, FALSE, sizeof(deviceInfo.lunOpen) );

	USB_MUTEX_UNLOCK;
	return 0;
}

BOOL usbmsc_fini( void )
{
	return 0;
}

U8 usbmsc_stopUSBhost( void )
{
	IRP	irp;

	if (!usbmsc_USBhostOpened)
	{
		DBG_TPrintf ("usbHost wasn't opened\n\r");
		return TRUE;
	}

	DBG_TPrintf ("Stop USBhost0\n\r");

	USB_MUTEX_LOCK;
	DBG_TPrintf ("Stop USBhost1\n\r");
	FILL_NORMAL_IRP( &irp, enIRPT_USBD, enIRP_USBHOST_STOP,	0,
		0,	0,	0,	enIRPR_SUBMIT, usbmsc_irpCompleteFunc );

	usbmsg_sendIRP( &irp );
	usbmsg_waitIRPcomplete();
	DBG_assert( mscError == enIRPR_SUCCESS );

	memset( &deviceInfo, 0, sizeof(deviceInfo) );	
	deviceInfo.valid	=	FALSE;
	memset( deviceInfo.lunOpen, FALSE, sizeof(deviceInfo.lunOpen) );

	DBG_TPrintf ("Stop USBhost2\n\r");

	USB_MUTEX_UNLOCK;
	return TRUE;
}

U8 usbmsc_startUSBhostEx( enum_USB_HOST_STACK_ATTR attr )
{
	IRP	irp;

	USB_MUTEX_LOCK;
	FILL_NORMAL_IRP( &irp, enIRPT_USBD, enIRP_USBHOST_START,	0,
		attr,	0,	0,	enIRPR_SUBMIT, usbmsc_irpCompleteFunc );

	usbmsg_sendIRP( &irp );
	usbmsg_waitIRPcomplete();
	DBG_assert( mscError == enIRPR_SUCCESS );

	USB_MUTEX_UNLOCK;
	return TRUE;
}

U8 usbmsc_startUSBhost( void )
{
	int r;
	
	r = usbmsc_startUSBhostEx( USB_HSTK_ATTR_HOST_ADDED );
	return r;
}

U8 usbmsc_USBhostOpenEx( enum_USB_HOST_STACK_ATTR attr )
{
#ifdef DVD_MODULE_ENABLE
	if (app_media_data.media == SOURCE_USB_MSC)
	{
		if (usbVideo)
		{
			return FALSE;
		}
	}

	if (!usbmsc_USBhostOpened)
	{
		usbmsc_startUSBhostEx( attr );
		usbmsc_USBhostOpened = TRUE;
	}

	return TRUE;
#else
	if (!usbmsc_USBhostOpened)
	{
		DBG_Printf ("%s\n\r", __func__);

		usbmsc_startUSBhostEx( attr );
		usbmsc_USBhostOpened = TRUE;

#ifdef IPOD_USB_AUDIO
#if ( (!defined IPOD_DISABLE_USB_PORT) || defined IPOD_DOCKING_ENABLE)
		app_timer_ipod_authen_timeout_timer_set(2200);
#endif
#endif
	}
	else
	{
		DBG_Printf ("usbHost opened\n\r");
	}

	return TRUE;
#endif
}

U8 usbmsc_USBhostOpen( void )
{
	if (!usbmsc_USBhostOpened)
	{
		DBG_Printf ("USB open\n\r");
		
		usbmsc_startUSBhostEx( USB_HSTK_ATTR_HOST_ADDED );
		usbmsc_USBhostOpened = TRUE;
	}
	else
	{
		DBG_Printf ("USB is open\n\r");
	}
	return TRUE;
}

U8 usbmsc_USBhostClose( void )
{
	if (usbmsc_USBhostOpened)
	{
		DBG_Printf ("USB close\n\r");
		usbmsc_stopUSBhost();
	}
	else
	{
		DBG_Printf("usb not open\n\r");
	}

	usbmsc_USBhostOpened = FALSE;
	
	return TRUE;
}

U8 usbmsc_LUNopen( U8 lun )
{
	IRP	irp;
	U8	ret;

	USB_MUTEX_LOCK;
	DBG_assert( deviceInfo.valid );
	DBG_assert( lun < deviceInfo.validLuns );

	FILL_NORMAL_IRP( &irp, enIRPT_MSC_CLASS, enIRP_LUN_OPEN,	0,
		lun,	&deviceInfo,	0,	enIRPR_SUBMIT, usbmsc_irpCompleteFunc );

	usbmsg_sendIRP( &irp );
	usbmsg_waitIRPcomplete();
	ret = (mscError == enIRPR_SUCCESS );
	DBG_assert( ret );

	if( ret )	deviceInfo.lunOpen[lun]	=	TRUE;

	USB_MUTEX_UNLOCK;
	return ret;
}

U8 usbmsc_LUNclose( U8 lun )
{
	IRP	irp;

	USB_MUTEX_LOCK;
	DBG_assert( deviceInfo.valid );
	DBG_assert( lun < deviceInfo.validLuns );

	FILL_NORMAL_IRP( &irp, enIRPT_MSC_CLASS, enIRP_LUN_CLOSE,	0,
		lun,	0,	0,	enIRPR_SUBMIT, usbmsc_irpCompleteFunc );

	usbmsg_sendIRP( &irp );
	usbmsg_waitIRPcomplete();
	DBG_assert( mscError == enIRPR_SUCCESS );

	deviceInfo.capacity[lun] = 0;
	deviceInfo.sectorSize[lun] = 0;
	deviceInfo.lunOpen[lun]	=	FALSE;
	deviceInfo.lunWriteProtect[lun]	=	FALSE;

	DBG_Printf("usbmsc_LUNclose = %d\n\r",lun);

	USB_MUTEX_UNLOCK;
	return (mscError == enIRPR_SUCCESS);
}

//U16	usbmsc_sectorRead(U8 lun, U8 * buf, U32 lba, U16 sectorCount)
U16	usbmsc_sectorRead(U8 * buf, U32 lba, U16 sectorCount)
{
	IRP	irp;
	U8	lun;

	USB_MUTEX_LOCK;
	lun = wrUSBDevIndex;

	DBG_assert( deviceInfo.valid );
	DBG_assert( lun < deviceInfo.validLuns );

	if( !deviceInfo.lunOpen[lun] )	return 0;

	FILL_DATA_IRP( &irp, enIRPT_MSC_CLASS, enIRP_READ, 0, 
		lun,	buf,	lba,	sectorCount,	enIRPR_SUBMIT, usbmsc_irpCompleteFunc );

	usbmsg_sendIRP( &irp );
	usbmsg_waitIRPcomplete();
	DBG_assert( mscError == enIRPR_SUCCESS );

	DBG_assert( (transLength/deviceInfo.sectorSize[lun] == sectorCount)
		&& (0 == (transLength % deviceInfo.sectorSize[lun])) );

	USB_MUTEX_UNLOCK;
	return transLength/deviceInfo.sectorSize[lun];
}

//U16 usbmsc_sectorWrite(U8 lun, U8 * buf, U32 lba, U16 sectorCount)
U16	usbmsc_sectorWrite(U8 * buf, U32 lba, U16 sectorCount)
{
	IRP	irp;
	U8	lun;

	USB_MUTEX_LOCK;
	lun = wrUSBDevIndex;

	DBG_assert( deviceInfo.valid );
	DBG_assert( lun < deviceInfo.validLuns );

	if( !deviceInfo.lunOpen[lun] )	
	{
		USB_MUTEX_UNLOCK;
		return 0;
	}

	FILL_DATA_IRP( &irp, enIRPT_MSC_CLASS, enIRP_WRITE, 0,
		lun,	buf,	lba,	sectorCount,	enIRPR_SUBMIT, usbmsc_irpCompleteFunc );

	usbmsg_sendIRP( &irp );
	usbmsg_waitIRPcomplete();
	DBG_assert( mscError == enIRPR_SUCCESS );

	DBG_assert( (transLength/deviceInfo.sectorSize[lun] == sectorCount)
		&& (0 == (transLength % deviceInfo.sectorSize[lun])) );

	USB_MUTEX_UNLOCK;
	return transLength/deviceInfo.sectorSize[lun];
}


//U32 usbmsc_getCapacity (U8 lun)
U32 usbmsc_getCapacity (void)
{
	U8	lun;

	lun = wrUSBDevIndex;

	DBG_assert( deviceInfo.valid );
	DBG_assert( lun < deviceInfo.validLuns );

	if( !deviceInfo.lunOpen[lun] )	return 0;

	return deviceInfo.capacity[lun];
}

//U16 usbmsc_getSectorSize (U8 lun)
U16 usbmsc_getSectorSize (void)
{
	U8	lun;

	lun = wrUSBDevIndex;

	DBG_assert( deviceInfo.valid );
	DBG_assert( lun < deviceInfo.validLuns );

	if( !deviceInfo.lunOpen[lun] )	return 0;

	return deviceInfo.sectorSize[lun];
}

U8	usbmsc_getLastErr(void)
{
	return mscError;
}


U8 usbmsc_getDevInfo( DEVICE_INFO **devInfo )
{
	DBG_assert( deviceInfo.valid );

	*devInfo	=	&deviceInfo;

	return TRUE;
}

U8 usbmsc_isLunWriteProtect( U8 lun )
{
	DEVICE_INFO	*devinfo	=	&deviceInfo;

	DBG_assert( devinfo->valid );
	DBG_assert( lun < devinfo->validLuns );
	DBG_assert( devinfo->lunOpen[lun] );

	return devinfo->lunWriteProtect[ lun ];
}

U8 usbmsc_setLunWriteProtect( U8 lun, U8 wp )
{
	DEVICE_INFO	*devinfo	=	&deviceInfo;

	DBG_assert( devinfo->valid );
	DBG_assert( lun < devinfo->validLuns );
	DBG_assert( devinfo->lunOpen[lun] );

	devinfo->lunWriteProtect[ lun ]	=	wp;

	return TRUE;
}

WORD usbmsc_Get_Media (void)
{
	return TRUE;
}

BOOL usbmsc_Erase (DWORD sector_addr, DWORD len)
{
	return TRUE;
}

BOOL usbmsc_Media_Active (void)
{
	return TRUE;
}

BOOL usbmsc_Format_Unit (void)
{
	return TRUE;
}

BOOL usbmsc_Get_Format_Propgress (BYTE *format_progress)
{
	return TRUE;
}

WORD _ATA_Read (BYTE *buf, DWORD lba, WORD sector_count)
{
	return 0;
}

WORD _ATA_Write (BYTE *buf, DWORD lba, WORD sector_count)
{
	return 0;
}

void SetWRDeviceIndex(U8 index)
{
	wrUSBDevIndex = index;
}

void SetWRDeviceIndexInit(void)
{
	wrUSBDevIndex = USB_DEVICE_NO_LUN_OPEN;
}

U16 getDeviceVID (void)
{
	return deviceInfo.usbVid;
}

#endif //USB_HOST_ENABLE
