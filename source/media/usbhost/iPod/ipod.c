#include "os_config.h"


#ifdef IPOD_USB_AUDIO


#include "mem_reloc.h"
#include "tools.h"
#include "IRP.h"
#include "usbd.h"
#include "message.h"

#include "isrc.h"
#include "audio.h"
#include "ipoddev.h"
#include "lingo.h"
#include "iap.h"
#include "ipod.h"

#include "app_timer.h"
#include "app_cmd.h"
#include "app_main.h"
#include "src.h"
#include <string.h>

#include "app_sdram.h"

#ifdef	APPS_HARBOUR_LIGHT
#include "apps_hl.h"
#endif




IPOD_DEV_INFO	ipodInfo __IPHONE_BSS;
int iPod_track_sample_rate;


U8 *U32ToPtrb( U32 d, U8 *ptr )
{
	ptr[0]	=	d >> 24;
	ptr[1]	=	d >> 16;
	ptr[2]	=	d >> 8;
	ptr[3]	=	d;

	return ptr + 4;
}

U16 byteToU16b( U8 *ptr )
{
	U16 d
		=	(ptr[0] << 8) | ptr[1];
	return d;
}

U32	byteToU32b( U8 *ptr )
{
	//big endian
	U32	l
		=	(ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];

	return l;
}

U32	endianConvertU32 ( U32 b )
{
	U32	l
		=	(b >> 24) | ( (b >> 8) & 0xFF00 ) | (b << 24) | ((b << 8) & 0xFF0000);

	return l;
}

U16	endianConvertU16 ( U16 b )
{
	U16	l
		=	(b >> 8) | (b << 8);
	return l;
}

//***************************************************************************************
//	i2c/achip


//FIXME:	need modify to use hardware i2c
#include "app_i2c.h"		//use soft i2c temporarily, share with audio codec will cause problem
#include "hw_i2c.h"
#include "hw_i2cPoll.h"


#ifdef ACHIP_V20C
#define	ACHIP_I2C_ADDR		0x22		//mode1/mode0 = 11
#else
#define	ACHIP_I2C_ADDR		0x20		//mode1/mode0 = 10
#endif

//#define	USE_HW_I2C
#define	USE_SOFT_I2C
//#define	USE_HW_I2C_POLL

#ifdef ACHIP_V20C
#define	I2C_CLOCK_HZ		300*1000	// 200K, max 400K
#else
#define	I2C_CLOCK_HZ		30*1000		// 30K, max 50K
#endif


#define	MAX_PAGE_DATA_LEN		128
#define	MAX_CHALLEN_LEN			20
#define	MAX_DATA_LEN_SIZE		2

#define	BLOCK0_REG_START_ADDR	0x00
#define	BLOCK0_REG_SIZE			0x09

#define	REG_ADDR_ERR_CODE		0x05
#define	REG_ADDR_CTL_STATUS		0x10

#define	REG_ADDR_SIGN_LEN		0x11
#define	REG_ADDR_SIGN_DATA		0x12
#define	REG_ADDR_CHALLEN_LEN	0x20
#define	REG_ADDR_CHALLEN_DATA	0x21

#define	REG_ADDR_ACC_CERTI_LEN		0x30
#define	REG_ADDR_ACC_CERTI_DATA_START	0x31
#define	REG_ADDR_ACC_CERTI_DATA_END		0x3F
#define	REG_ACC_CERTI_PAGES		15

#define	REG_ADDR_SELF_TEST		0x40


typedef	struct {
	U8	devVer;
	U8	firmwareVer;
	U8	authenProtMajVer;
	U8	authenProtMinVer;
	U32	devId;

	int	certlen;
	U8	*challenge;
	int	chalen;
	int	siglen;

	U8	*buf;
	int	maxbufSize;
}	AUTHEN_CHIP_INFO;

AUTHEN_CHIP_INFO	authenChip	__IPHONE_BSS;

#define	MAX_ACHIP_BUF_SIZE		2048
U8	achipBuffer[MAX_ACHIP_BUF_SIZE]	__IPHONE_BSS;


#ifdef	USE_HW_I2C
int achip_i2c_write(U8 regAddr, U8 *buf, int len)
{
	return( hwi2c_writeSub( ACHIP_I2C_ADDR, regAddr, buf, len ) );
}
int achip_i2c_read(U8 regAddr, U8 *buf, int len )
{
	return ( hwi2c_readSub( ACHIP_I2C_ADDR, regAddr, buf, len ) );
}
#endif

#ifdef	USE_SOFT_I2C

int achip_i2c_write(U8 regAddr, U8 *buf, int len)
{
	int ret;


	ret	=	I2C0_achip_write_sub( ACHIP_I2C_ADDR, regAddr, buf, len );

	DBG_assert( ret == I2C_SUCCESS );

	return (ret == I2C_SUCCESS );
}


int achip_i2c_read(U8 regAddr, U8 *buf, int len )
{
	int ret;

	ret	=	I2C0_achip_read_sub( ACHIP_I2C_ADDR, regAddr, buf, len );

	DBG_assert( ret == I2C_SUCCESS );

	return (ret == I2C_SUCCESS );
}
#endif

#ifdef	USE_HW_I2C_POLL
int achip_i2c_write(U8 regAddr, U8 *buf, int len)
{
	int r;

	r = hwi2cPoll_write( ACHIP_I2C_ADDR, regAddr, buf, len );

	return (r == 0 );
}

int achip_i2c_read(U8 regAddr, U8 *buf, int len )
{
	int r;

	r = hwi2cPoll_read( ACHIP_I2C_ADDR, regAddr, buf, len );

	return (r == 0 );
}
#endif

int achip_readAccCertificate( U8 *buf, int *len )
{
	AUTHEN_CHIP_INFO	*achip	=	&authenChip;
	U16	rlen;
	*len	=	0;

	int ret	 =	achip_i2c_read( REG_ADDR_ACC_CERTI_LEN, (U8*)&rlen, 2 );
	DBG_assert( ret );

	if( !ret ) return FALSE;

	rlen	=	endianConvertU16( rlen );
	*len	=	rlen;
	DBG_assert( achip->maxbufSize >= rlen );

	ret	=	achip_i2c_read( REG_ADDR_ACC_CERTI_DATA_START, buf, rlen );
	DBG_assert( ret );

	return ret;
}

int achip_writeChallenge( U8 *chall, U16 len )
{
	int ret;
	U16	len2;

	DBG_assert( len <= MAX_CHALLEN_LEN  );

	len2	=	endianConvertU16(len);
	ret	=	achip_i2c_write( REG_ADDR_CHALLEN_LEN, (U8*)&len2, 2 );
	DBG_assert( ret );

	if( ret ) {
		ret	=	achip_i2c_write( REG_ADDR_CHALLEN_DATA, chall, len );
		DBG_assert( ret );
	}

	return ret;
}

int achip_readSignature( U8 *sig, int *len )
{
	U16	len2;
	int ret;
	*len	=	0;

	ret	=	achip_i2c_read( REG_ADDR_SIGN_LEN, (U8*)&len2, 2 );
	DBG_assert( ret );

	if( ret ) {
		len2	=	endianConvertU16(len2);
		*len	=	len2;

		ret	=	achip_i2c_read( REG_ADDR_SIGN_DATA, sig, len2 );
		DBG_assert( ret );
	}

	return ret;
}

#define	ACHIP_ERR_CODE_NO_ERR		0x00
int achip_readErrCode( U8 *code )
{
	int ret
		=	achip_i2c_read( REG_ADDR_ERR_CODE, code, 1 );
	DBG_assert( ret );
	DBG_assert( 0 == *code );
	
	return ret;
}

//write
#define	AUTHEN_PROC_NONE		0x00
#define	AUTHEN_PROC_SIG_GEN		0x01
#define	AUTHEN_PROC_CHALL_GEN	0x02
#define	AUTHEN_PROC_SIG_VERIFY	0x03
#define	AUTHEN_PROC_CERT_VALID	0x04
#define	AUTHEN_PROC_SLEEP		0x05


int achip_writeAuthenCtlStat( U8 ctl )
{
	int ret	=	achip_i2c_write( REG_ADDR_CTL_STATUS, &ctl, 1 );
	DBG_assert( ret );

	return ret;
}


//read, ctl/stat register
#define	AUTHEN_ERR_SET					0x80
#define	AUTHEN_STAT_MASK				(7 << 4)
#define	AUTHEN_PROC_RESLT_NOT_VALID		(0 << 4)
#define	AUTHEN_ACC_SIGNATURE_OK			(1 << 4)
#define	AUTHEN_CHALLENGE_OK				(2 << 4)
#define	AUTHEN_IPOD_SIGNATURE_OK		(3 << 4)
#define	AUTHEN_IPOD_CERTI_OK			(4 << 4)

int achip_readAuthenCtlStat( U8 *stat )
{
	int ret;
	U8	status;

	ret	=	achip_i2c_read( REG_ADDR_CTL_STATUS, &status, 1 );
	DBG_assert( ret );

	*stat	=	0;
	if( ret ) {
		*stat	=	status;
		if( status & AUTHEN_ERR_SET )
			achip_readErrCode( &status );
	}

	return ret;
}


int	achip_init( void )
{
	AUTHEN_CHIP_INFO	*achip	=	&authenChip;

	memset( achip, 0, sizeof(AUTHEN_CHIP_INFO) );

	achip->buf	=	achipBuffer;
	achip->maxbufSize	=	MAX_ACHIP_BUF_SIZE;

	return TRUE;
}


int achip_open(void)
{
	int ret;
	AUTHEN_CHIP_INFO	*achip	=	&authenChip;

	//FIXME:	reset achip
	DBG_Puts("achip_open 0\n\r");

#ifdef	USE_HW_I2C
	hwi2c_init();
	hwi2c_open( 40 );		//40khz
#endif

#ifdef USE_SOFT_I2C
	I2C0_achip_init();
#endif

#ifdef USE_HW_I2C_POLL
	hwI2cPoll_init();
	hwi2cPoll_openMaster(I2C_CLOCK_HZ );
#endif

#if (defined SHARP_8P && defined RELEASE_VERSION)

#ifndef ACHIP_V20C
	//reset achip
	gpio2_SetFunction(IPHONE_ACHIP_RESET_PIN, TRUE);
	gpio2_SetOutputEnable(IPHONE_ACHIP_RESET_PIN);
	gpio2_ClrOutput(IPHONE_ACHIP_RESET_PIN);
	timer_delayms(20);
	gpio2_SetOutput(IPHONE_ACHIP_RESET_PIN);
	timer_delayms(50);
#endif

#else

#ifndef ACHIP_V20C
	//reset achip
	hal_gpio_mux_func_select( IPHONE_ACHIP_RESET_PIN, MUX_SEL_GPIO_OUTPUT);
	hal_gpio_output_level_set( IPHONE_ACHIP_RESET_PIN, GPIO_LOW);
	timer_delayms(20);
	hal_gpio_output_level_set( IPHONE_ACHIP_RESET_PIN, GPIO_HIGH);
	timer_delayms(50);
#endif

#endif

	DBG_Puts("achip_open 1\n\r");

	//read block 0
	ret = achip_i2c_read( BLOCK0_REG_START_ADDR, achip->buf, BLOCK0_REG_SIZE );
	DBG_assert( ret );
	if( !ret )	return FALSE;

	DBG_Puts("achip_open 2\n\r");

	achip->devVer		=	achip->buf[0];
	achip->firmwareVer	=	achip->buf[1];
	achip->authenProtMajVer	=	achip->buf[2];
	achip->authenProtMinVer	=	achip->buf[3];
	achip->devId	=	*(U32*)&achip->buf[4];
	achip->devId	=	endianConvertU32( achip->devId );

#ifndef ACHIP_V20C
	// devVersion:  2.0B - 0x03.
	if( (0x03 != achip->devVer) || (0x200 != achip->devId) )
	{
		DBG_Printf("achip Ver:%d, ID:%d\n\r", achip->devVer, achip->devId);
		return FALSE;
	}
#else
	// devVersion:  2.0C - 0x05
	if( (0x05 != achip->devVer) || (0x200 != achip->devId) )
	{
		DBG_Printf("achip Ver:%d, ID:%d\n\r", achip->devVer, achip->devId);
		return FALSE;
	}
#endif

	if( achip->authenProtMajVer < 0x02 )
	{
		DBG_Printf("achipProMajVer::%d\n\r", achip->authenProtMajVer);
		return FALSE;
	}

	DBG_Puts("achip_open ok\n\r");

	return TRUE;
}

int achip_close(void)
{
#ifdef	USE_HW_I2C
	hwi2c_close();
#endif

	return TRUE;
}


U32 ipod_getDevid( void )
{
	AUTHEN_CHIP_INFO	*achip	=	&authenChip;

	return achip->devId;
}

//***************************************************************************************
//	ipod task

int ipod_init(void)
{
	IPOD_DEV_INFO	*iinfo	=	&ipodInfo;

	memset( iinfo, 0, sizeof(IPOD_DEV_INFO ) );
	iinfo->initOK	=	FALSE;

	achip_init();

#ifdef APPS_SUPPORT
#ifdef APPS_HARBOUR_LIGHT
	appsHl_init();
#endif
#endif

	DBG_Puts("ipod init\n\r");

	return TRUE;
}

int	ipod_open( int type )
{
	IPOD_DEV_INFO	*iinfo	=	&ipodInfo;


	memset( iinfo, 0, sizeof(IPOD_DEV_INFO ) );

#ifdef APPS_SUPPORT
#ifdef APPS_HARBOUR_LIGHT
	appsHl_open();
#endif
#endif

	iPod_track_sample_rate = TSAMPLE_RATE_44100;

	return TRUE;
}

int ipod_close(void)
{
	ipod_init();

#ifdef APPS_SUPPORT
#ifdef	APPS_HARBOUR_LIGHT
	appsHl_close();
#endif
#endif

	return TRUE;
}


#ifdef APPS_SUPPORT

#ifdef APPS_AUTO_LAUNCH
int ipod_isAutoLaunchSupport( void )
{
	IPOD_DEV_INFO	*iinfo	=	&ipodInfo;

	return iinfo->appsAutoLaunch;
}
#endif

int ipod_isApplsComm( void )
{
	IPOD_DEV_INFO	*iinfo	=	&ipodInfo;

	return iinfo->appsComm;
}
#endif

int ipod_isInitOk( void )
{
	IPOD_DEV_INFO	*iinfo	=	&ipodInfo;

	return iinfo->initOK;
}

int ipod_isTrackInfoGot( void )
{
	IPOD_DEV_INFO	*iinfo	=	&ipodInfo;
	return iinfo->trkInfoGot;
}


#define	MODEL_ID_IPHONE			0x0011
#define	MODEL_ID_IPHONE_3G		0x0018
#define	MODEL_ID_IPHONE_3GS		0x001B
#define	MODEL_ID_IPHONE_4		0x0020
#define	MODEL_ID_IPAD			0x001F

int ipod_getModelTypes( void )
{
	IPOD_DEV_INFO	*iinfo	=	&ipodInfo;
	U16	mid;

	if( !iinfo->modelOk )	return enIPOD_OTHERS;

	mid	=	iinfo->modelid >> 16;

	if( mid == MODEL_ID_IPAD )	return enIPOD_IPAD;

	if( (mid == MODEL_ID_IPHONE) || (mid == MODEL_ID_IPHONE_3G) || (mid == MODEL_ID_IPHONE_3GS )
		|| (mid == MODEL_ID_IPHONE_4) )
		return enIPOD_IPHONE;

	return enIPOD_OTHERS;
}

int ipod_lingoRxCtl( int ena )
{
	IRP	irp;

	FILL_IPHONE_IRP( &irp, enIRPT_IPOD, enIRP_TYPE_IPOD_LINGO_RX_REQ,  NULL, ena, NULL );
	usbmsg_sendIRP( &irp );

	return TRUE;
}

int ipod_audioRxCtl( int ena )
{
	IRP	irp;

	FILL_IPHONE_IRP( &irp, enIRPT_IPOD, enIRP_TYPE_IPOD_AUDIO_RX_REQ,  NULL, ena, NULL );
	usbmsg_sendIRP( &irp );

	return TRUE;
}

int ipod_audioSetSampleRate( int srate )
{
	IRP	irp;

	FILL_IPHONE_IRP( &irp, enIRPT_IPOD, enIRP_TYPE_IPOD_SET_SAMPLE_RATE,  NULL, srate, NULL );
	usbmsg_sendIRP( &irp );

	return TRUE;
}

int ipod_audioSetIntf( int zerobandwidth )
{
	IRP	irp;

	FILL_IPHONE_IRP( &irp, enIRPT_IPOD, enIRP_TYPE_IPOD_SET_INTF,  NULL, zerobandwidth, NULL );
	usbmsg_sendIRP( &irp );

	return TRUE;
}

static int getIpodInfor( void )
{
	return TRUE;
}


static int getDaudioLingoVer( void )
{
	IPOD_DEV_INFO	*iinfo	=	&ipodInfo;
	int ret	=	FALSE;

	iinfo->vlingoDaudio.support	=	FALSE;
	iinfo->vlingoDaudio.lingo	=	LINGO_ID_DIGI_AUDIO;

	ret = lingo_gen_reqLingoVer( iinfo->vlingoDaudio.lingo );
	DBG_assert( ret );

	ret	=	lingo_gen_retLingoVer( &iinfo->vlingoDaudio );
	DBG_assert( ret );
	if( !ret )	return FALSE;

	return TRUE;
}

static int isDaudioNeedExtendedUImode( void )
{
	IPOD_DEV_INFO	*iinfo	=	&ipodInfo;

	if( (iinfo->vlingoDaudio.major == 1) && (iinfo->vlingoDaudio.minor <= 1) ) 
		return TRUE;
	else
		return FALSE;
}


static int ipodInfoGet( void )
{
	IPOD_DEV_INFO	*iinfo	=	&ipodInfo;
	int ret	=	FALSE;
	int len;
	int i;
	U8	status;

	//for transid
	do {
		ret = lingo_gen_reqIpodName();
		DBG_assert( ret );
		ret = lingo_gen_retIpodName( iinfo->name, &len, MAX_IPOD_NAME, &status );
		DBG_assert( ret );
		if( !ret )	break;
	}	while( ret && (len == 0) );
	if( !ret )	return FALSE;


	ret = lingo_gen_reqSerialNum();
	DBG_assert( ret );
	ret = lingo_gen_retSerialNum( iinfo->serialnum, &len, MAX_IPOD_SERIAL_NUM );
	DBG_assert( ret );
	if( !ret )	return FALSE;

	return ret;
}

static int ipodNonIdpsInit( void )
{
	int ret;
	U8	tmp;
	U32	id;

#define	CANCEL_AUTHEN_LOOPS		3

	timer_delayms( 50 );

	//FIXME:	3G ipod
	//FIXME:	lingo conflict check
	ret = lingo_gen_identifyDevLingo( IDENTIFY_DEV_LINGO_SPOKEN_INIT, 0, 0 );
	DBG_assert( ret );
	if( !ret )	return FALSE;
	ret = lingo_gen_ack( &tmp );
	DBG_assert( ret );
	if( tmp != ACK_CODE_SUCCESS )	return FALSE;
	if( !ret )	return FALSE;

	id	=	ipod_getDevid();

	//TBD
	getDaudioLingoVer();

	if( isDaudioNeedExtendedUImode() )
	{
	#ifdef	IPOD_LINE_IN
		ret = lingo_gen_identifyDevLingo( IDENTIFY_DEV_LINGO_SPOKEN_EXT_REC, IDENTIFY_DEV_LINGO_OPTION, id );
	#else
		ret = lingo_gen_identifyDevLingo( IDENTIFY_DEV_LINGO_SPOKEN_EXT, IDENTIFY_DEV_LINGO_OPTION, id );
	#endif
	}
	else {
	#ifdef	IPOD_LINE_IN
		ret = lingo_gen_identifyDevLingo( IDENTIFY_DEV_LINGO_SPOKEN_RECORD, IDENTIFY_DEV_LINGO_OPTION, id );
	#else
		ret = lingo_gen_identifyDevLingo( IDENTIFY_DEV_LINGO_SPOKEN_BASIC, IDENTIFY_DEV_LINGO_OPTION, id );
	#endif
	}


	//ext no record
	DBG_assert( ret );
	if( !ret )	return FALSE;
	ret = lingo_gen_ack( &tmp );
	DBG_assert( ret );
	if( !ret )	return FALSE;
	if( tmp != ACK_CODE_SUCCESS )	return FALSE;

	return TRUE;
}


static int getIpodOptionForLingo( void )
{
	IPOD_DEV_INFO	*iinfo	=	&ipodInfo;
	int r;

	r = lingo_gen_func_getIpodOptionForLingo( LINGO_ID_GENERAL, iinfo->lgen_option );
	if( r ) 
		iinfo->appsAutoLaunch	=	lingo_gen_func_isAutoLaunchSupport( iinfo->lgen_option );
	else
		return r;	//ipod don't support getIpodOptionForLingo()

	r = lingo_gen_func_getIpodOptionForLingo( LINGO_ID_SIMPLE_REMOTE, iinfo->lsrmt_option );
	DBG_assert( r );

	r = lingo_gen_func_getIpodOptionForLingo( LINGO_ID_DISPLAY_REMOTE, iinfo->ldsp_option );
	DBG_assert( r );

	r = lingo_gen_func_getIpodOptionForLingo( LINGO_ID_DIGI_AUDIO, iinfo->lda_option );
	DBG_assert( r );

	return TRUE;
}

#define	IPOD_INIT_IDLE_TIME		(500 / OS_MSEC_PER_TICK + 1)	//0.5s

static int ipodIdpsInit( void )
{
	IPOD_DEV_INFO	*iinfo	=	&ipodInfo;
	int ret;
	U8	tmp;

	int loop	=	0;
	int init	=	TRUE;


	//FIXME:	first get ipod name / version ... 	

	iinfo->idps	=	TRUE;

#ifdef APPS_SUPPORT

#ifdef appsAutoLaunch
	iinfo->appsAutoLaunch	=	FALSE;
#endif

#endif


#ifdef	IAP_TIME_OUT

	do {
		ret = lingo_gen_startIdps();
		if( !ret ) goto WAIT_IPOD;

		if( !iinfo->rxInit ) {
			iinfo->rxInit	=	TRUE;
			ipod_lingoRxCtl( TRUE );
		}

		ret = lingo_gen_ack_ext( IDPS_TOUT_MS, &tmp );
		if( !ret ) goto WAIT_IPOD;
			
		break;

WAIT_IPOD:
		
		if( (tmp != ACK_CODE_BAD_PARAM) && (tmp != ACK_CODE_SUCCESS) ) {
			ukMsgSend(cmdIphone_restartEnum);
			return FALSE;
		}

		if( isIphoneDisconnected() ) return FALSE;
		if ( !usbd_isDevActive(NULL) )	return FALSE; 
		tx_thread_sleep( IPOD_INIT_IDLE_TIME );		
		if( isIphoneDisconnected() ) return FALSE;
		if ( !usbd_isDevActive(NULL) )	return FALSE; 

		DBG_Printf("idps loops:%d, stat:%d\n\r", loop, tmp );

	}	while( loop ++ < MAX_INIT_LOOPS );

	if( loop >= MAX_INIT_LOOPS )	{
		DBG_Printf("idps retry failed\n\r");
		return FALSE;
	}
	if( (tmp != ACK_CODE_BAD_PARAM) && (tmp != ACK_CODE_SUCCESS) ) {
		//FIXME: need disconnnect
		DBG_Printf("bad ipod status\n\r");
		return FALSE;
	}

#else //ndef IAP_TIME_OUT

#define	MAX_INIT_LOOPS	6

	do {
		ret = lingo_gen_startIdps();
		if( !ret ) goto WAIT_IPOD;
		
		if( !iinfo->rxInit ) {
			iinfo->rxInit	=	TRUE;
			ipod_lingoRxCtl( TRUE );
		}
		
		do {
			ret = lingo_gen_ack( &tmp );
			if( !ret ) goto WAIT_IPOD;
			
			if( tmp == ACK_CODE_PENDING )	timer_delayms(10);
		} while( tmp == ACK_CODE_PENDING );
		
		break;

WAIT_IPOD:
		if (!app_media_status_get())	return FALSE;
		if ( !usbd_isDevActive(NULL) )	return FALSE; 
		tx_thread_sleep( IPOD_INIT_IDLE_TIME );		
		if (!app_media_status_get())	return FALSE;
		if ( !usbd_isDevActive(NULL) )	return FALSE; 
	}	while( loop ++ < MAX_INIT_LOOPS );

	if( loop >= MAX_INIT_LOOPS )	return FALSE;

#endif //IAP_TIME_OUT

	DBG_Printf( "IDPS 1\n\r" );
	if( tmp == ACK_CODE_BAD_PARAM ) {
		DBG_Printf( "non-IDPS init start\n\r" );
		timer_delayms(50);

		iinfo->idps	=	FALSE;
		ret = ipodNonIdpsInit();
		DBG_assert(ret);
		return ret;
	}
	if( tmp != ACK_CODE_SUCCESS ) {
		//unsupported ipod
		DBG_assert(0);
		return FALSE;
	}

	iinfo->maxPayloadSize	=	0;
	iinfo->maxPlayLoadSupport	=	FALSE;			
	ret = lingo_gen_reqMaxPayloadSize();
	if( ret ) {
		ret = lingo_gen_retMaxPayloadSize( &iinfo->maxPayloadSize );		
		DBG_Printf("maxPayloadSize: %d\n\r", iinfo->maxPayloadSize );
		if( ret)
			iinfo->maxPlayLoadSupport	=	TRUE;			
	}

	if ( !usbd_isDevActive(NULL) )	return FALSE; 

	getIpodOptionForLingo();

	DBG_Printf( "IDPS 2\n\r" );
	ret = lingo_gen_setFidTokenValue();
	if( !ret ) {
		DBG_assert(0);
		return ret;
	}
	ret = lingo_gen_retFidTokenVAlueAck();
	if( !ret ) {
		DBG_assert(0);
		return ret;
	}
	DBG_Printf( "IDPS 3\n\r" );


	ret = lingo_gen_endIdps( ACC_ENDIDPS_STAT_OK );
	if( !ret ) {
		DBG_assert(0);
		return ret;
	}
	DBG_Printf( "IDPS 4\n\r" );
	ret = lingo_gen_idpsStatus_xxx(&tmp);
	if( !ret ) {
		DBG_assert(0);
		return ret;
	} 
	DBG_Printf( "IDPS 5\n\r" );
	DBG_assert( tmp == IDPS_STATUS_OK );
	if( tmp != IDPS_STATUS_OK )	return FALSE;
	DBG_Printf( "IDPS 6\n\r" );

	return TRUE;
}


static void ipodIrpComplete ( U8 irpResult, U32 iprTransLen)
{
	usbmsg_IRPcompleteFunc();
}

static int msg2Usbd( void )
{
	IRP	irp;

	FILL_IPHONE_IRP( &irp, enIRPT_USBD, enIRP_TYPE_IPOD_BACK2MSC, NULL,  0, ipodIrpComplete );
	usbmsg_sendIRP( &irp );
	usbmsg_waitIRPcomplete();

	DBG_assert( irp.result );

	return irp.result;
}


void app_cmd_send_iphone2msc_msg( void );

extern int osentry_changeKeyFuncTaskPriority( void );
extern int osentry_restoreKeyFuncTaskPriority( void );

extern int osentry_HddTaskPriority( void );
extern int osentry_restoreHdd_TaskPriority( void );


#define	IPOD_CONNECT_IDLE_TIME		(2500 / OS_MSEC_PER_TICK + 1)	//2.5s


/*
 * nano 3(8GB):vid 0x5ac, pid 0x1262
 * nano 4(8GB):vid 0x5ac, pid 0x1263
 * nano 4(8GB):vid 0x5ac, pid 0x1263
 * classic(160GB):vid 0x5ac, pid 0x1261
 * classic(160GB):vid 0x5ac, pid 0x1261
 */
const USBDEV_ID ipodSyncDelayIDs[]	=	{
	{0x05AC,0x1262	},		//ipod 3G nano (8GB)
	{0x05AC,0x1263	},		//ipod 4G nano (8GB)
	{0x05AC,0x1261	}		//classic(160GB)
};



int ipod_isSync2dockDelayNeeded( void )
{
	//ugly but simple
	USB_DEVICE *dev = &singleUsbDevice;

	int i;
	int size;

	size = sizeof(ipodSyncDelayIDs)/sizeof(USBDEV_ID);
	for (i = 0; i < size; i ++)
	{
		if( (dev->devDescriptor.idVendor == ipodSyncDelayIDs[i].vid) &&(dev->devDescriptor.idProduct == ipodSyncDelayIDs[i].pid) )
		{
			return TRUE;
		}
	}

	return FALSE;
}

#define	IPOD_INIT_MAX_LOOP	8
static int ipodConnect( IPOD_MSG *imsg )
{
	int ret	=	FALSE;
	int i;
	TX_INTERRUPT_SAVE_AREA;

	#ifdef OS_TASK_PRIORITY_CHANGE
	osentry_changeKeyFuncTaskPriority();
	#endif

	if ( (app_main_data.media == MEDIA_IPHONE) || (app_main_data.media == SOURCE_USB_MSC) )
	{
	#ifdef	AUDIO_SAMPLE_RATE_SWITCH_TO_48K
		src_init( 44100, 2, TSAMPLE_RATE_48000 );
	#else
		src_init( 44100, 2, TSAMPLE_RATE_44100 );
	#endif
	}

	if(imsg->param1 == enIPOD_DIGITAL_AUDIO ) {

		DBG_Puts( "ipod digital audio\n\r" );

#ifdef	IPOD_ACC_POWER_WAIT
		waitIpodAccPower();
#endif

		lingo_init();
		iap_init();

		lingo_open();
		iap_open();

		ipod_open( enIPOD_DIGITAL_AUDIO );
		DBG_Printf( "ipod 1\n\r" );

		ret = achip_open();

		if( !ret )	{
			DBG_Printf( "achip open failed\n\r" );
			return FALSE;
		}

		DBG_Printf( "ipod 2\n\r" );

#ifdef	IPOD_ACC_POWER_WAIT
		waitIpodAccPower();
#endif

		timer_delayms(100);
		//sleep before init begin, iphone 3GS need this
		DBG_Printf( "ipod 3\n\r" );

		i = 0;
		while( i ++ < IPOD_INIT_MAX_LOOP ) {
			ret = ipodIdpsInit();
			if( ret ) {
				DBG_Printf( "ipodIdpsInit ok:%d\n\r", i );
				break;
			}

			if ( !usbd_isDevActive(NULL) )	break; 

			DBG_Printf( "ipodIdpsInit fail:%d\n\r", i );
			timer_delayms( 100 );
		}

		if( !ret )	{
			//unsupported ipod
			DBG_Printf( "unknown ipod 1\n\r" );
			//for display No support
			//modified for iphone power on on usb port
			ukMsgSend(app_cmd_iPhone_power_on);

			//FIXME: back to msc ?

			return FALSE;
		}
#ifdef IPOD_UART_SUPPORT
		else
		{
			if (ipodDigital_converted_to_ipodUart())
			{
				DBG_Puts( "ipod digital to uart\n\r" );
				ukMsgSend(app_cmd_iPhone_no_support);

				return FALSE;
			}
		}
#endif

		DBG_Printf( "ipod init succeeded\n\r" );
	}
	else {
		DBG_Printf( "ipod don't support digital audio\n\r" );
		ukMsgSend(app_cmd_iPhone_no_support);

		return FALSE;
	}

	//FIXME:	send msg out to notify ipod dev connection

	return TRUE;
}


static int ipodDisconnect( IPOD_MSG *imsg )
{
	DBG_Printf( "ipod disconnected 3\n\r" );

	ipod_close();

#ifdef OS_TASK_PRIORITY_CHANGE
	osentry_restoreKeyFuncTaskPriority();
#endif

	//FIXME: temporarily add this
	app_cmd_send_iphone2msc_msg();

	return TRUE;
}



int iap_checkIpodLingo( IPOD_PKT *ipkt );

int ipod_lingoProcess( IPOD_PKT *ipkt )
{
	IPOD_DEV_INFO	*iinfo	=	&ipodInfo;
	volatile int ret;
	AUTHEN_CHIP_INFO	*achip	=	&authenChip;
	U8	status, tmp;
	U8	*ptr;
	TX_INTERRUPT_SAVE_AREA;

	IPOD_TRACK_INFO	tinfo;
	int len;

	ret = iap_checkIpodLingo( ipkt );
	if( !ret )	{
		asm("nop");
		goto EXIT;
	}

	switch( ipkt->lingoID ) {
	case LINGO_ID_GENERAL:
		switch( ipkt->cmdID ) {

#ifdef	APPS_SUPPORT
		case LINGO_GEN_OPEN_DATA_SESSION:
			iinfo->appsComm	=	TRUE;

			DBG_Printf( "open DataSession\n\r" );
			ret  = lingo_gen_openDataSession( ipkt );
			timer_delayms(50);
			lingo_gen_devAck( ret,  LINGO_GEN_OPEN_DATA_SESSION );

#if (defined APPS_UPGRADE_ENABLE && defined APPS_HARBOUR_LIGHT)
			appsHl_process( LINGO_ID_GENERAL, LINGO_GEN_OPEN_DATA_SESSION, NULL, 0 );
#endif
			break;

		case LINGO_GEN_CLOSE_DATA_SESSION:
			DBG_Printf( "close DataSession\n\r" );
			ret = lingo_gen_closeDataSession( ipkt );
			lingo_gen_devAck( ret, LINGO_GEN_CLOSE_DATA_SESSION );

#if (defined APPS_UPGRADE_ENABLE && defined APPS_HARBOUR_LIGHT)
			appsHl_process( LINGO_ID_GENERAL, LINGO_GEN_CLOSE_DATA_SESSION, NULL, 0 );
#endif

			iinfo->appsComm	=	FALSE;
			break;

		case LINGO_GEN_IPOD_DATA_TRANSFER:

			len	=	0;
			ret = lingo_gen_ipodDataTransfer( ipkt, achip->buf, &len );
			lingo_gen_devAck( ret, LINGO_GEN_IPOD_DATA_TRANSFER );

#ifdef	APPS_HARBOUR_LIGHT
			appsHl_process( LINGO_ID_GENERAL, LINGO_GEN_IPOD_DATA_TRANSFER, achip->buf, len );
#endif
			break;
#endif

		case LINGO_GEN_GET_DEV_AUTHEN_INFO:
			lingo_gen_getDevAuthenInfo( ipkt );

			ret = achip_readAccCertificate( achip->buf, &achip->certlen );
			DBG_assert( ret );

			DBG_Printf("certlen: %d\n\r", achip->certlen );
			ret = lingo_gen_retDevAuthenInfo( achip->buf, achip->certlen );
			DBG_assert( ret );
			break;

		case LINGO_GEN_GET_DEV_AUTHEN_SIGNATURE:
			ret = lingo_gen_getDevAuthenSignature( ipkt, &achip->challenge, &achip->chalen );
			DBG_assert( ret );
			if( !ret )	break;

			DBG_Printf("challen: %d\n\r", achip->chalen );
#ifndef ACHIP_V20C
TX_DISABLE;
#endif
			//must disable it for sometimes ipod can not connect
			ret =  achip_writeChallenge( achip->challenge, achip->chalen );
			DBG_assert( ret );
			ret	=	achip_writeAuthenCtlStat( AUTHEN_PROC_SIG_GEN );
#ifndef ACHIP_V20C
TX_RESTORE;
#endif

			DBG_assert( ret );
			do
			{
				timer_delayms(10);
#ifndef ACHIP_V20C
TX_DISABLE;
#endif
				//must disable it for sometimes ipod can not connect
				ret =	achip_readAuthenCtlStat( &status );
#ifndef ACHIP_V20C
TX_RESTORE;
#endif
				DBG_assert( ret );
				DBG_assert( (status & AUTHEN_STAT_MASK) == AUTHEN_ACC_SIGNATURE_OK );
			} while( (status & AUTHEN_STAT_MASK) != AUTHEN_ACC_SIGNATURE_OK );

#ifndef ACHIP_V20C
TX_DISABLE;
#endif
			ret	=	achip_readSignature( achip->buf, &achip->siglen );
#ifndef ACHIP_V20C
TX_RESTORE;
#endif
			DBG_assert( ret );

			DBG_Printf("siglen: %d\n\r", achip->siglen );
			ret =	lingo_gen_retDevAuthenSignature( achip->buf, achip->siglen );
			DBG_assert( ret );
			asm("nop");
			if( !ret )	{
				DBG_Printf( "authen fail 1\n\r" );
				break;
			}

			ret	=	lingo_gen_ackDevAuthenStat( &status );
			DBG_assert( ret );
			DBG_assert( 0 == status );
			if( !ret || (0 != status ) ) {
				DBG_Printf( "authen fail 2\n\r" );
				asm( "nop" );
			}

			if( ret && !iinfo->initOK ) {

				DBG_Printf( "ipod authen done\n\r");

				if( !iinfo->idps )
					lingo_gen_setIpodPrefAll();

#ifdef IPOD_VOLUME_SET
				app_cmd_iphone_raw_vol_set( audio_vari.volume );
				timer_delayms(20);
#else
				timer_delayms(10);
#endif

				ipodInfoGet();
				asm( "nop" );

				lingo_disp_setRmtEventNotifyFunc( DEFAULT_RMT_EVENT_MASK );

				ret = ipod_audioSetIntf( FALSE );
				if( !ret )	break;

				if( !iinfo->idps && isDaudioNeedExtendedUImode() ) {
					DBG_Printf( "enter extended mode\n\r");
					ret = lingo_genFunc_enterRmtUI();
					if( !ret )	break;
				}

				DBG_Printf( "ipod begin playing\n\r");

				//send msg out
				app_cmd_send_switch2iphone_msg();
				iinfo->initOK	=	TRUE;
			}
			break;

		case LINGO_GEN_GET_ACC_INFOR:
			ret	=	lingo_gen_getAccInfo( ipkt, &status, &ptr );
			DBG_assert( ret );
			DBG_Printf( "accInfo: %d\n\r", status );
			ret	=	lingo_gen_retAccInfo( status, ptr );
			DBG_assert( ret );

			break;

		default:
			DBG_assert(0);
		}
		break;

	case LINGO_ID_DIGI_AUDIO:
		switch( ipkt->cmdID ) {
		case LINGO_DAUDIO_CMD_IPOD_ACK:
			//FIXME
			DBG_Printf("daudio ipod ack\n\r");
			asm( "nop" );
			DBG_assert(0);
			break;
		case LINGO_DAUDIO_CMD_GET_ACC_SRATE_CAP:
			if( !iinfo->initOK )	break;

			ret  = lingo_daudio_getAccSrateCaps( ipkt );
			DBG_assert( ret );

			ret = lingo_daudio_retAccSrateCaps();
			DBG_assert( ret );

			DBG_Printf( "ipod: get ACC \n\r");
			timer_delayms(100);
			break;
		case LINGO_DAUDIO_CMD_NEW_IPOD_TRACK_INFO:
			DBG_Printf( "ipod newTrack\n\r");

			ret = lingo_daudio_newIpodTrackInfo( ipkt, &tinfo );
			DBG_assert( ret );

			ret = lingo_daudio_accAck( LINGO_DAUDIO_ACK_STATUS_OK, ipkt->cmdID );
			DBG_assert( ret );
			if( !ret )	break;

			asm( "nop" );
			DBG_Printf("track sample rate: %d\n\r", tinfo.sampleRate );

#ifndef ACHIP_V20C
TX_DISABLE;
#endif
			iPod_track_sample_rate = tinfo.sampleRate;
			if ( (app_main_data.media == MEDIA_IPHONE) || (app_main_data.media == SOURCE_USB_MSC) )
			{
#ifdef	AUDIO_SAMPLE_RATE_SWITCH_TO_48K
				src_init( tinfo.sampleRate, 2, TSAMPLE_RATE_48000 );
#else
				src_init( tinfo.sampleRate, 2, TSAMPLE_RATE_44100 );
#endif
			}
#ifndef ACHIP_V20C
TX_RESTORE;
#endif
			ret = ipod_audioSetSampleRate( tinfo.sampleRate );
			DBG_assert( ret );
			if( !ret )	break;

			iinfo->trkInfoGot	=	TRUE;

			break;
		default:
			DBG_Printf( "ipkt_cmdID:\n\r", ipkt->cmdID);
			DBG_assert(0);
		}
		break;

	case LINGO_ID_MICROPHONE:

		DBG_Printf( "microphone lingo\n\r" );
		switch( ipkt->cmdID ) {
		case LINGO_MICROPHONE_GETDEV_ACK:
			ret = lingo_mic_getDevAck( ipkt );
			ret = lingo_mic_devAck( ACK_CODE_SUCCESS, ipkt->cmdID );
			break;

		case LINGO_MICROPHONE_IPOD_MODE_CHG:
			ret = lingo_mic_ipodModeChg( ipkt, &status );
			//FIXME:	send to UI for status:	begin/end record ...
			ret = lingo_mic_devAck( ACK_CODE_SUCCESS, ipkt->cmdID );
			break;
		case LINGO_MICROPHONE_GET_DEV_CAPS:
			ret = lingo_mic_getDevCaps( ipkt );
			ret = lingo_mic_retDevCaps( DEFAULT_MIC_DEV_CAPS );
			break;
		case LINGO_MICROPHONE_GET_DEV_CTL:
			ret = lingo_mic_getDevCtl( ipkt, &status );
			ret = lingo_mic_retDevCtl( status );
			break;
		case LINGO_MICROPHONE_SET_DEV_CTL:
			ret = lingo_mic_setDevCtl( ipkt, &status, &tmp );
			ret = lingo_mic_devAck( ACK_CODE_BAD_PARAM, ipkt->cmdID );
			break;

		default:
			DBG_assert(0);
		}
		break;

	case LINGO_ID_DISPLAY_REMOTE:
		switch( ipkt->cmdID ) {

		case LINGO_DSPRMT_RMT_EVT_NOTIFY:
			app_main_data.disp_info = DISP_INFO_WAITING;
			lingo_disp_rmtEventNotify( ipkt );
			break;

		default:
			break;
		}
		break;


	default:
		DBG_assert(0);
	}

EXIT:
	if( ipkt->callback != NULL )
		(*ipkt->callback) ( ipkt );

	return TRUE;
}


#ifdef	IPOD_TASK_MODIFY

void task_ipod(IPOD_MSG *msg)
{
	IPOD_PKT	*ipkt;

	switch( msg->msg ) {
	case enIPOD_CONNECT:

		if (app_main_data.media == MEDIA_AUX || app_main_data.media == MEDIA_TUNER)
		{
			if (need_to_free_space_status != UNKNOW_REC_STATUS)
			{
				DBG_Puts("anolog rec readry do not conn ipod\n\r");
				ipodDisconnect( msg );
				break;
			}			
		}
		else
		{
			if (app_main_data.media != SOURCE_USB_MSC)
			{
				DBG_Puts("rec readry do not conn ipod\n\r");
				ipodDisconnect( msg );
				// cdda rec and can not conn ipod
				break;
			}
		}
		
		ipod_init();
		ipodConnect( msg );
		break;
		
	case enIPOD_DISCONNECT:
		ipodDisconnect( msg );
		break;
		
	case enIPOD_IPHONE_SEND_PKT:
		ipkt	=	(IPOD_PKT*)msg->param1;
		ipod_lingoProcess( ipkt );
		break;
		
	default:
		break;
	}
	
}

#else

void task_ipod(void)
{
	U32 buf[WIDTH_OF_IPOD_QUEUE];
	IPOD_MSG	*msg;
	int ret;
	IPOD_PKT	*ipkt;

	DBG_assert( sizeof(IPOD_MSG) <= WIDTH_OF_IPOD_QUEUE*sizeof(U32) );
	DBG_assert( sizeof(buf) >= sizeof(IPOD_MSG) );


	while(1) {
		ret = tx_queue_receive( &gIpodQueue, (void *)buf, TX_WAIT_FOREVER );
		DBG_assert( TX_SUCCESS == ret );
		msg	=	(IPOD_MSG*)buf;

		switch( msg->msg ) {
		case enIPOD_CONNECT:
			ipod_init();
			ipodConnect( msg );
			break;

		case enIPOD_DISCONNECT:
			ipodDisconnect( msg );
			break;

		case enIPOD_IPHONE_SEND_PKT:
			ipkt	=	(IPOD_PKT*)msg->param1;
			ipod_lingoProcess( ipkt );
			break;

		default:
			break;
		}
	}
}
#endif //IPOD_TASK_MODIFY


extern U8 ex_audio_stream_zero;

U8 ipod_tx_data_state_init(void)
{
	ex_audio_stream_zero = FALSE;
}

U8 ipod_tx_data_state_get(void)
{
	return ex_audio_stream_zero;
}


#ifdef	IPOD_VOLUME_SYNC
extern void app_cmd_iphone_vol_notify( U8 mute, U8 vol );

void app_cmd_volume(void)
{
	DBG_Printf("Set Volume:%d\n\r", audio_vari.volume);

	if (app_main_data.standby_status)
	{
		return;
	}

	#ifdef AUDIO_CODEC_USED_VOL
	AUDIOdevice.Set_Volume(audio_vari.volume);
	#endif
	#ifdef AUDIO_AMP_USED_VOL
	for (i=0;i<AMP_DEV_COUNT_MAX;i++)
	{
		AudioAmpDev[i].Set_Volume(audio_vari.volume);
	}
	#endif

	if (app_main_data.window != (WINDOW *) &volume_window)
	{
		app_nav_window_set(&volume_window, app_window_data.window_index+1);
		general_ticks = 0;
	}
	else
	{
		//app_main_data.window->draw_title ();
		app_main_data.window->draw_region ();
	}

	if( audio_vari.volume == 0 )
	{
		//AMP_MUTE_ON;
	}
}

void ipod_rmtEventVolNotify( U8 mute, U8 vol )
{
	static U8 iphone_sync_cnt = 0;;

	if (app_main_data.standby_status)
	{
		return;
	}

	if ( !( (app_main_data.media == MEDIA_IPHONE) || 
		((app_main_data.media == SOURCE_USB_MSC) && (usbDevType == enUSBH_DEV_IPOD_AUDIO)) ) )
	{
		return;
	}


	if (iphone_sync_cnt <= 2)
	{
		iphone_sync_cnt++;
		return;
	}

	if( mute ) vol	=	0;

	if( vol == 255 )
	{
		vol = VOLUME_MAX;
	}
	else
	{
		vol	=	vol / (256/VOLUME_MAX);
	}

	//set to onboard DSP chip

	audio_vari.volume =	vol;

#ifdef APPS_SUPPORT
	ukMsgSend(app_nav_cmd_iphone_apps_vol);
#else
	ukMsgSend(app_cmd_volume);
#endif


	DBG_Printf("IPOD Vol: %d\n\r", audio_vari.volume);
}
#endif

#ifdef LCD_DISP_IPHONE_ID3_MESSAGE
extern U32 iPhone_playing_track;
extern ID3_TAG id3_tag;
#endif

void ipod_rmtEventTrackIndexNotify( U32 tidx, U32 track_info_type)
{
#ifdef LCD_DISP_IPHONE_ID3_MESSAGE
	U8 utf8str[MAX_TITLE_LEN];
	int len16;
	int slen;
	int ret;

	memset( utf8str, 0, sizeof(utf8str) );

	iPhone_playing_track	=	tidx;

	ret = lingo_dispFunc_getIndexTrackInfo(tidx, track_info_type,
		utf8str, MAX_TITLE_LEN, &slen );

	DBG_Printf("id3:%s\n\r", utf8str);

	if ( ret && track_id3_ready )
	{
		if (track_info_type == TRACK_INFO_TYPE_TITLE)
		{
			memset( id3_tag.title, 0, (MAX_ID3_TEXT + 1)*2);
			len16 = utf8_toUcs16( utf8str, slen, id3_tag.title, MAX_TITLE_LEN );
			memset(file_name, 0, (MAX_TITLE_LEN+1)*2);
			CFasm_memcpy(file_name, id3_tag.title, len16*2);
		}
		else if (track_info_type == TRACK_INFO_TYPE_ARTIST)
		{
			memset( id3_tag.artist, 0, (MAX_ID3_TEXT + 1)*2 );
			len16 = utf8_toUcs16( utf8str, slen, id3_tag.artist, MAX_TITLE_LEN );
			memset(file_name, 0, (MAX_TITLE_LEN+1)*2);
			CFasm_memcpy(file_name, id3_tag.artist, len16*2);
		}
		else if (track_info_type == TRACK_INFO_TYPE_ALBUM)
		{
			memset( id3_tag.album, 0, (MAX_ID3_TEXT + 1)*2 );
			len16 = utf8_toUcs16( utf8str, slen, id3_tag.album, MAX_TITLE_LEN );
			memset(file_name, 0, (MAX_TITLE_LEN+1)*2);
			CFasm_memcpy(file_name, id3_tag.album, len16*2);
		}
	}
	else
	{

	}
#endif
}

U8 installIpodDeviceDriver_state_get(USB_DEVICE *usbdev)
{
	volatile U8 ret = TRUE;

#ifdef IPOD_DISABLE_USB_PORT
	if( app_cmd_isSourceUsbPort() )
	{
		usbdev->devType		=	enUSB_DEV_UNKOWN;
		usbdev->ipodType	=	enNON_IPOD;

		app_cmd_send_iphoneOnUsbPort_msg();
	}
	else
	{
		ret = installIpodDeviceDriver( usbdev );
	}
#else
	ret = installIpodDeviceDriver( usbdev );
#endif

	return ret;
}

void ipod_acc_power_set(USB_DEVICE * usbDev)
{
#ifdef	IPOD_NO_CHARGE
	setIpodAccPower( usbDev, EXTRA_CURRENT_SUSPEND0, EXTRA_CURRENT_NON_SUSPEND_MIN );
#else

#ifdef SHARP_KP82
	//2.1A for usb connector; 1A for lightning connector
	if( usbPortSwitch_isLightning() ) {
		setIpodAccPower( usbDev, EXTRA_CURRENT_SUSPEND0, EXTRA_CURRENT_NON_SUSPEND_MAX );	//1A
	}
	else {
		DBG_assert( usbPortSwitch_isUSB() );
		setIpodAccPower( usbDev, EXTRA_CURRENT_SUSPEND0, EXTRA_CURRENT_NON_SUSPEND_MAX_IPAD );	//2.1A
	}
#else
	setIpodAccPower( usbDev, EXTRA_CURRENT_SUSPEND0, EXTRA_CURRENT_NON_SUSPEND_MAX );
#endif

#endif
}

//ipod don't support sample rate conversion
const USBDEV_ID usb_ipod_devId[]	=	{
	{0x05AC,	0x120A	},		//ipod nano, 
	{0x05AC,	0x1209	},		//ipod 
};

U8 ipodDigital_converted_to_ipodUart(void)
{
#ifdef IPOD_UART_SUPPORT
	USB_DEVICE *dev = &singleUsbDevice;

	int i;
	int size;

	size = sizeof(usb_ipod_devId)/sizeof(USBDEV_ID);
	for (i = 0; i < size; i ++)
	{
		if( dev->devDescriptor.idVendor == (usb_ipod_devId[i].vid) &&
			(dev->devDescriptor.idProduct == usb_ipod_devId[i].pid) )
		{
			return TRUE;
		}
	}

	return FALSE;
#else
	return FALSE;
#endif
}


const USBDEV_ID ipod_id_noTrackIndex[]	=	{
	{0x05AC,	0x1262	},		//ipod nano 3G,
	{0x05AC,	0x1263	},		//ipod nano 3G,
	{0x05AC,	0x1264	},		//ipod nano 4G,
	{0x05AC,	0x1265	},		//ipod nano 5G,
	{0x05AC,	0x1266	},		//ipod nano 6G,
	{0x05AC,	0x1267	},		//ipod nano 7G,
};

int isIpodEventNotify_NoTrackIndex( void )
{
	USB_DEVICE *dev = &singleUsbDevice;
	int i;
	int size;

	size = sizeof(ipod_id_noTrackIndex)/sizeof(USBDEV_ID);
	for (i = 0; i < size; i ++)
	{
		if( dev->devDescriptor.idVendor == (ipod_id_noTrackIndex[i].vid) &&
			(dev->devDescriptor.idProduct == ipod_id_noTrackIndex[i].pid) )
		{
			return TRUE;
		}
	}

	return FALSE;
}

int ipod_isSourceDocking( void )
{
#ifdef	IPOD_VIDEO_OUT
	return TRUE;
#else
	return FALSE;
#endif
}

#else

#include "usbd.h"

#include "ipoddev.h"
#include "ipod.h"

U8 installIpodDeviceDriver_state_get(USB_DEVICE *usbdev)
{
#ifdef IPOD_USB_AUDIO_CHARGE_ONLY
	volatile U8 ret = TRUE;

	#ifdef IPOD_DISABLE_USB_PORT
	if( app_cmd_isSourceUsbPort() )
	{
		usbdev->devType		=	enUSB_DEV_UNKOWN;
		usbdev->ipodType	=	enNON_IPOD;

		app_cmd_send_iphoneOnUsbPort_msg();
	}
	else
	{
		ret = installIpodDeviceDriver( usbdev );
	}
	#else
	ret = installIpodDeviceDriver( usbdev );
	#endif

	return ret;

#else
	return FALSE;
#endif //IPOD_USB_AUDIO_CHARGE_ONLY
}

void ipod_acc_power_set(USB_DEVICE * usbDev)
{
#ifdef IPOD_USB_AUDIO_CHARGE_ONLY

#ifdef IPOD_NO_CHARGE
	setIpodAccPower( usbDev, EXTRA_CURRENT_SUSPEND0, EXTRA_CURRENT_NON_SUSPEND_MIN );
#else
	setIpodAccPower( usbDev, EXTRA_CURRENT_SUSPEND0, EXTRA_CURRENT_NON_SUSPEND_MAX );
#endif

#endif //IPOD_USB_AUDIO_CHARGE_ONLY
}

#endif //IPOD_USB_AUDIO

