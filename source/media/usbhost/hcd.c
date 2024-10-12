#include "os_config.h"


#ifdef USB_HOST_ENABLE
#include "mem_reloc.h"

#include "hw_timer.h"

#include "IRP.h"
#include "usbd.h"
#include "otg.h"
#include "hstack.h"
#include "hcd.h"


#ifndef	HCD_V2

#define PORT_RESET_DELAY    40

#define LOAD_DATA_NOT_ALIGNED_4


typedef	struct {
	U8	opened;
	U8	forceSpeed;
	U8	devSpeed;
	
	U8	transMethod;
	HTD	*htd;
}	HCD;
HCD	hcd;



static void errorProcess( U8 endp, U8 dirin  );
static void htdProcess( U8 endp, U8 dirin );


static void virtualScheduler( void );

void hcd_intProcess(void)	__USBHOST_TEXT;
static void htdProcess( U8 endp, U8 dirin  ) __USBHOST_TEXT;


void hcd_intProcess(void)
{
	U16 reg;
	
	if( !hcd.opened )	return;

	//high speed int
	if( *usbhostReg->irq->usbirq & 0x20 )	{
		hcd.devSpeed		=	enSPEED_HIGH;
		*usbhostReg->irq->usbirq =	0x20;
	}

	reg = *usbhostReg->irq->hcouterrIrq;
	DBG_assert( reg == 0 );
	if( reg ) {
		*usbhostReg->irq->hcouterrIrq	=	reg;

		if( reg & 0x01 ) {	//out 0
			errorProcess( 0, FALSE );
		}
		if( reg & 0x02 ) {	//out 1
			errorProcess( 1, FALSE );
		}
		if( reg & 0x04 ) {	//out 2
			DBG_assert(0);
		}
	}

	reg = *usbhostReg->irq->hcinerrIrq;
	DBG_assert( reg == 0 );
	if( reg ) {
		*usbhostReg->irq->hcinerrIrq	=	reg;

		if( reg & 0x01 ) {	//in 0
			errorProcess( 0, TRUE );
		}
		if( reg & 0x02 ) {	//in 1
			errorProcess( 1, TRUE );
		}
		if( reg & 0x04 ) {	//in 2
			DBG_assert(0);
		}
	}

	reg = *usbhostReg->irq->txirq;
	if( reg ) {
		*usbhostReg->irq->txirq	=	reg;
		
		if( reg & 0x01 ) {
			//out 0
			htdProcess( 0, FALSE );
		}
		if( reg & 0x02 ) {
			//out 1
			htdProcess( 1, FALSE );
		}
	}

	reg = *usbhostReg->irq->rxirq;
	if( reg ) {
		*usbhostReg->irq->rxirq	=	reg;
		
		if( reg & 0x01 ) {
			//in 0
			htdProcess( 0, TRUE );
		}
		if( reg & 0x02 ) {
			//in 1
			htdProcess( 1, TRUE );
		}
	}
}



#define	SETUP_PKT_LEN	8
static void loadData( HTD *htd, U8 first ) __USBHOST_TEXT;
static void loadData( HTD *htd, U8 first )
{
	PIPE	*pipe	=	htd->pipe;
	int i, size, tmp, loop;

	volatile U8 *ptr8;
	volatile U32 *ptr32;
	volatile U16 *ptr16;

	switch( pipe->type ) {
	case enPIPE_CTRL:
		switch( htd->ctlTransStat ) {
		case enCSTAGE_SETUP:
			*usbhostReg->endp0->cs		=	0x10;
			for( i = 0; i < SETUP_PKT_LEN; i ++ ) 
				usbhostReg->endp0->txdata[i]	=	htd->setupPkt[i];
			*usbhostReg->endp0->txbc	=	SETUP_PKT_LEN;
			break;
		
		case enCSTAGE_DATA:
			if( htd->dataIn ) {
				if( first ) {
					*usbhostReg->endp0->rxbc	=	0;
				}
				else {
					size = *usbhostReg->endp0->rxbc;

					DBG_assert( size + htd->transLen <= htd->len );
					for( i = 0; i < size; i ++ ) 
						htd->buffer[i]	=	usbhostReg->endp0->rxdata[i];	
					
					htd->buffer 	+=	size;
					htd->transLen	+=	size;

					if( size < pipe->maxpkt )	htd->shortPkt = TRUE;

					if( !htd->shortPkt && (htd->transLen < htd->len) ) 
						*usbhostReg->endp0->rxbc	=	0;
				}
			}
			else {
				size	=	htd->len - htd->transLen;
				if( size > pipe->maxpkt )	size = pipe->maxpkt;

				if( size > 0 ) {
					for( i = 0; i < size;  i ++ ) 
						usbhostReg->endp0->txdata[i]	=	htd->buffer[i];

					*usbhostReg->endp0->txbc	=	size;
					htd->buffer 	+=	size;
					htd->transLen	+=	size;
				}
			}
			break;

		case enCSTAGE_STATUS:
			*usbhostReg->endp0->cs	=	0x40;		//toggle 1

			if( (htd->len == 0) || !htd->dataIn )
				*usbhostReg->endp0->rxbc	=	0;
			else
				*usbhostReg->endp0->txbc	=	0;
			break;

		case enCSTAGE_COMPLETE:
			break;
		default:
			DBG_assert(0);
		}
		break;

	case enPIPE_BULK:
	case enPIPE_INT:
	case enPIPE_ISO:
		
		if( pipe->dirin ) {
			if( first ) {
				DBG_assert( htd->transLen < htd->len );

				*usbhostReg->endp1In->con	|=	0x80;
				*usbhostReg->endp1In->cs	=	0x01;
			}
			else {
				size	=	*usbhostReg->endp1In->bc;
				DBG_assert( size <= pipe->maxpkt );

				switch( hcd.transMethod ) {
				case enTRANS_PIO32:
					//changed that the htd->buffer isn't aligned 4.
					#ifdef LOAD_DATA_NOT_ALIGNED_4
					if ((U32)htd->buffer%4 == 0)
					{
						tmp = size/4;
						ptr32	=	(U32*)htd->buffer;
						for( i = 0; i < tmp; i ++ )
						{
							*ptr32 ++	=	*usbhostReg->endp1In->fifoDword;
						}

						tmp = size % 4;
						ptr8 = (U8*)ptr32;
					}
					else if ((U32)htd->buffer%4 == 2)
					{
						tmp = size/2;
						ptr16	=	(U16*)htd->buffer;
						for( i = 0; i < tmp; i ++ )
						{
							*ptr16 ++	=	*usbhostReg->endp1In->fifoWord;
						}

						tmp = size % 2;
						ptr8 = (U8*)ptr16;
					}
					else
					{
						tmp = size;
						ptr8 = (U8*)htd->buffer;
					}

					for( i = 0; i < tmp; i ++ )
					{
						*ptr8 ++	=	*usbhostReg->endp1In->fifoByte;
					}

					#else
					
					tmp = size/4;
					ptr32	=	(U32*)htd->buffer;
					DBG_assert( ( (U32)ptr32 & 0x03 ) == 0 );
					for( i = 0; i < tmp; i ++ )
					{
						*ptr32 ++	=	*usbhostReg->endp1In->fifoDword;
					}

					tmp = size % 4;
					if( tmp > 0 )
					{
						ptr8 = (U8*)ptr32;
						for( i = 0; i < tmp; i ++ ) 
						{
							*ptr8 ++	=	*usbhostReg->endp1In->fifoByte;
						}
					}
					#endif //LOAD_DATA_NOT_ALIGNED_4
					

					htd->buffer +=	size;
					htd->transLen +=	size;
					
					if( pipe->type == enPIPE_ISO )	
						htd->shortPkt = FALSE;
					else  if( size < pipe->maxpkt )	
						htd->shortPkt = TRUE;

					if( (htd->len > htd->transLen) && !htd->shortPkt )
						*usbhostReg->endp1In->cs	=	0x01;						
					break;

				case enTRANS_DMA32:
					break;

				case enTRANS_PIO64:
				case enTRANS_DMA64:
				default:
					DBG_assert(0);
				}
			}
		}
		else {
			//dir out
			size	=	htd->len - htd->transLen;
			if( size > pipe->maxpkt )	size = pipe->maxpkt;

			if( size > 0 ) {
				switch( hcd.transMethod ) {
				case enTRANS_PIO32:
					//changed that the htd->buffer isnot aligned 4.
					#ifdef LOAD_DATA_NOT_ALIGNED_4
					if ((U32)htd->buffer%4 == 0)
					{
						tmp = size/4;
						ptr32 = (U32*)htd->buffer;
						for( i = 0; i < tmp; i ++ )
						{
							*usbhostReg->endp1Out->fifoDword	=	*ptr32 ++;
						}

						tmp = size % 4;
						ptr8 = (U8*)ptr32;
					}
					else if ((U32)htd->buffer%4 == 2)
					{
						tmp = size/2;
						ptr16	=	(U16*)htd->buffer;
						for( i = 0; i < tmp; i ++ )
						{
							*usbhostReg->endp1Out->fifoWord	=	*ptr16 ++;
						}

						tmp = size % 2;
						ptr8 = (U8*)ptr16;
					}
					else
					{
						tmp = size;
						ptr8 = (U8*)htd->buffer;
					}

					for( i = 0; i < tmp; i ++ )
					{
						*usbhostReg->endp1Out->fifoByte	=	*ptr8 ++;
					}
						
					#else
					
					tmp = size/4;
					ptr32	=	(U32*)htd->buffer;
					DBG_assert( ( (U32)ptr32 & 0x03 ) == 0 );
					for( i = 0; i < tmp; i++ )
					{
						*usbhostReg->endp1Out->fifoDword	=	*ptr32 ++;
					}
					
					tmp = size % 4;
					if( tmp > 0 )
					{
						ptr8	=	(U8*)ptr32;
						for( i = 0; i < tmp; i++ )
						{
							*usbhostReg->endp1Out->fifoByte	=	*ptr8 ++;
						}
					}
					#endif //LOAD_DATA_NOT_ALIGNED_4
					
					htd->buffer +=	size;
					htd->transLen +=	size;
					*usbhostReg->endp1Out->bc	=	size;
					*usbhostReg->endp1Out->cs	=	0x01;
					break;

				case enTRANS_DMA32:
					break;

				case enTRANS_PIO64:
				case enTRANS_DMA64:
				default:
					DBG_assert(0);
				}
			}
		}

		break;

	default:
		DBG_assert(0);

	}
}


//	enURBRST_INT_NAK
static U8 endpErr2htdErr( U8 endperr )
{
	U8	result = enURBRST_FAIL;

	if( !hcd_isDevConnected() ) return enURBRST_DEV_DISCONNECT;

	switch( endperr ) {
	case HC_ERR_NO_ERR:
		result	=	enURBRST_SUCESS;
		break;
	case HC_ERR_TOGGLE:
	case HC_ERR_CRC:
		result	=	enURBRST_FAIL;
		break;
	case HC_ERR_STALL:
		result	=	enURBRST_STALL;
		break;
	case HC_ERR_TIMEOUT:
		result	=	enURBRST_TIMEOUT;
		break;
	case HC_ERR_PID:
	case HC_ERR_OVERRUN:
	case HC_ERR_UNDERRUN:
	default:
		result	=	enURBRST_FAIL;
		break;
	}
}

static void errorProcess( U8 endp, U8 dirin  )
{
	HTD	*htd	=	hcd.htd;
	PIPE *pipe	=	hcd.htd->pipe;
	volatile U8 reg8;

	if( hcd.htd == NULL )	return;


	if( pipe->type == enPIPE_CTRL ) {
		DBG_assert( endp == 0 );

		switch( htd->ctlTransStat ) {
		case enCSTAGE_SETUP:
		default:
			DBG_assert(0);
			break;

		case enCSTAGE_DATA:
			if( htd->dataIn ) {
				reg8	=	*usbhostReg->endp0->hcinerr;
				reg8	&=	HC_ERR_MASK;
			}
			else {
				reg8	=	*usbhostReg->endp0->hcouterr;
				reg8	&=	HC_ERR_MASK;
				DBG_assert( (reg8 == HC_ERR_STALL) || (reg8 == HC_ERR_TIMEOUT) || (reg8 == HC_ERR_PID) );
			}
			htd->result	=	endpErr2htdErr( reg8 );
			usbmsg_setHTDevent();
			hcd.htd = NULL;				
			break;

		case enCSTAGE_STATUS:
			if( htd->dataIn ) {
				reg8	=	*usbhostReg->endp0->hcouterr;
				reg8	&=	HC_ERR_MASK;
				DBG_assert( (reg8 == HC_ERR_STALL) || (reg8 == HC_ERR_TIMEOUT) || (reg8 == HC_ERR_PID) );
			}
			else {
				reg8	=	*usbhostReg->endp0->hcinerr;
				reg8	&=	HC_ERR_MASK;
			}
			htd->result	=	endpErr2htdErr( reg8 );
			usbmsg_setHTDevent();
			hcd.htd = NULL;				
			break;
		}
	}
	else {
		DBG_assert( dirin == htd->dataIn );
		DBG_assert( endp == 1 );

		if( pipe->dirin ) {
			reg8	=	*usbhostReg->endp1In->hcerr;
			reg8	&=	HC_ERR_MASK;
			
			*usbhostReg->endp1In->con	&= ~0x80;	//disable
		}
		else {
			reg8	=	*usbhostReg->endp1Out->hcerr;
			reg8	&=	HC_ERR_MASK;
			DBG_assert( (reg8 == HC_ERR_STALL) || (reg8 == HC_ERR_TIMEOUT) || (reg8 == HC_ERR_PID) );

			*usbhostReg->endp1Out->con	&= ~0x80;	//disable
		}

		htd->result	=	endpErr2htdErr( reg8 );
		usbmsg_setHTDevent();
		hcd.htd = NULL;				

	}
}

static void htdProcess( U8 endp, U8 dirin  )
{
	HTD	*htd	=	hcd.htd;
	PIPE *pipe	=	hcd.htd->pipe;

	if( hcd.htd == NULL )	return;

	if( pipe->type == enPIPE_CTRL ) {

		DBG_assert( endp == 0 );

		switch( htd->ctlTransStat ) {
		case enCSTAGE_SETUP:
			if( htd->len == 0 ) {
				htd->ctlTransStat = enCSTAGE_STATUS;
			}
			else {
				htd->ctlTransStat = enCSTAGE_DATA;
			}

			loadData( htd, TRUE );
			break;

		case enCSTAGE_DATA:

			if( htd->dataIn ) {

				loadData( htd, FALSE );
	
				if( htd->shortPkt || (htd->transLen == htd->len) ) {
					htd->ctlTransStat = enCSTAGE_STATUS;
					loadData( htd, FALSE );
				}
			}
			else {
				if( htd->transLen < htd->len ) {
					loadData( htd, FALSE );
				}
				else {
					htd->ctlTransStat = enCSTAGE_STATUS;
					loadData( htd, FALSE );
				}
			}
			break;

		case enCSTAGE_STATUS:
			htd->ctlTransStat = enCSTAGE_COMPLETE;
			htd->result	=	enURBRST_SUCESS;
			usbmsg_setHTDevent();

			hcd.htd = NULL;			
			break;

		default:
			DBG_assert(0);
		}

	}
	else {

		DBG_assert( dirin == htd->dataIn );
		DBG_assert( endp == 1 );

		if( pipe->dirin ) {
			loadData( htd, FALSE );

			if( htd->shortPkt || (htd->transLen == htd->len) ) {
				//disable iso in ?
				htd->result	=	enURBRST_SUCESS;
				usbmsg_setHTDevent();

				hcd.htd = NULL;				
			}
		}
		else {
			if( htd->transLen < htd->len ) {
				loadData( htd, FALSE );
			}
			else {

				htd->result	=	enURBRST_SUCESS;
				usbmsg_setHTDevent();

				hcd.htd = NULL;				
			}
		}
	}
}


int hcd_addHtd( HTD *htd )
{
	PIPE	*pipe;
	U8	ptype;
	volatile U8 reg8;

	DBG_assert( hcd.htd == NULL );

	hcd.htd	=	htd;
	pipe	=	hcd.htd->pipe;


	*usbhostReg->ctrl->funcaddr	=	pipe->usbdev->addr;
	if( pipe->type == enPIPE_CTRL ) {
		*usbhostReg->endp0->hcctrl		=	pipe->endpnum;
		*usbhostReg->endp0->hcmaxpkt	=	pipe->maxpkt;
	}
	else {

		//dir & pipe type
		ptype	=	0;
		switch( pipe->type ) {
		case enPIPE_ISO:
			ptype	=	0x04;
			break;
		case enPIPE_BULK:
			ptype	=	0x08;
			break;
		case enPIPE_INT:
			ptype	=	0x0c;
			break;
		default:
			DBG_assert(0);
		}
		
		if( pipe->dirin )	{
			*usbhostReg->endp1In->hcctrl	=	pipe->endpnum;
			*usbhostReg->endp1In->maxpkt	=	pipe->maxpkt;
			
			reg8 = *usbhostReg->endp1In->con;
			reg8 &= ~0x0c;
			reg8 |= ptype;
			*usbhostReg->endp1In->con	=	reg8;	
		}
		else	{
			*usbhostReg->endp1Out->hcctrl	=	pipe->endpnum;	
			*usbhostReg->endp1Out->maxpkt	=	pipe->maxpkt;

			reg8 = *usbhostReg->endp1Out->con;
			reg8 &= ~0x0c;
			reg8 |= ptype;
			*usbhostReg->endp1Out->con	=	reg8;
		}		
	}

	loadData( htd, TRUE );

	return TRUE;
}

int hcd_devUnplug( void )
{
	if( hcd.htd != NULL ) {
		hcd.htd->result	=	enURBRST_DEV_DISCONNECT;
		usbmsg_setHTDevent();
				
		hcd.htd = NULL;		
	}
	
	return TRUE;
}

int hcd_isDevConnected( void )
{
	return ( (otgRegister->status & 0x02) && (otgRegister->state == 0x03) );
}

int hcd_getDevSpeed( void )
{

	if( *usbhostReg->ctrl->usbcs & 0x01 ) {
		hcd.devSpeed	=	enSPEED_LOW;
	}
	else {
		if( hcd.devSpeed != enSPEED_HIGH ) {
			hcd.devSpeed	=	enSPEED_FULL;
		}
	}
	
	DBG_assert( hcd.devSpeed	 != enSPEED_UNKNOWN );
	return hcd.devSpeed;
}

int hcd_resetPort( void )
{
	hcd.devSpeed		=	enSPEED_UNKNOWN;

	*usbhostReg->portctrl	|=	0x20;
	while( *usbhostReg->portctrl & 0x20 );
	timer_delayms(PORT_RESET_DELAY); //10ms

	return hcd_getDevSpeed();
}

int hcd_setTransMethod( int trans )
{
	DBG_assert( trans < enTRANS_MAX );

	hcd.transMethod	=	trans;
	return TRUE;
}

int hcd_init(void)
{
	memset( &hcd, 0, sizeof(hcd) );
	hcd.opened	=	FALSE;

	return TRUE;
}

int hcd_open(void)
{
	volatile U32 reg;

	*usbhostReg->portctrl	|=	0x20;
	while( *usbhostReg->portctrl & 0x20 );
	timer_delayms(PORT_RESET_DELAY); //10ms

	//toggle & fifo	
	*usbhostReg->ctrl->enprst	=	0x00;
	*usbhostReg->ctrl->enprst	=	0x60;
	*usbhostReg->ctrl->enprst	=	0x10;
	*usbhostReg->ctrl->enprst	=	0x70;

	//fifo ctrl
	*usbhostReg->ctrl->fifoctl		=	0x81;
	*usbhostReg->ctrl->fifoctl		=	0x91;


	//use endpoint 0 & 1 only
	*usbhostReg->endp0->hcmaxpkt	=	0;
	*usbhostReg->endp1In->maxpkt	=	0;
	*usbhostReg->endp1Out->maxpkt	=	0;

	*usbhostReg->endp1In->staddr	=	ENDPOINT0_BUF_SIZE;
	*usbhostReg->endp1In->con		=	0;

	*usbhostReg->endp1Out->staddr	=	ENDPOINT0_BUF_SIZE;
	*usbhostReg->endp1Out->con		=	0;

	*usbhostReg->irq->hcinerrIrq=	0xff;
	*usbhostReg->irq->hcouterrIrq	=	0xff;
	*usbhostReg->irq->txirq			=	0xff;
	*usbhostReg->irq->rxirq			=	0xff;

	*usbhostReg->irq->hcinerrIen	=	0x03;
	*usbhostReg->irq->hcouterrIen	=	0x03;
	*usbhostReg->irq->txien			=	0x03;
	*usbhostReg->irq->rxien			=	0x03;

	*usbhostReg->irq->usbirq		=	0xff;
	*usbhostReg->irq->usbien		=	0x20;


	hcd.transMethod	=	enTRANS_PIO32;
	hcd.devSpeed	=	enSPEED_UNKNOWN;
	hcd.opened		=	TRUE;

	return TRUE;
}

int hcd_close(void)
{
	*usbhostReg->irq->hcinerrIen		=	0x00;
	*usbhostReg->irq->hcouterrIen	=	0x00;
	*usbhostReg->irq->txien			=	0x00;
	*usbhostReg->irq->rxien			=	0x00;
	*usbhostReg->irq->usbien	=	0x00;

	hcd_init();

	hcd.opened	=	FALSE;

	return TRUE;
}

HTD *hcd_removeHtd( PIPE *pipe )
{
	DBG_Assert(FALSE);
}

int hcd_setScheduleMode( int sof )
{
	DBG_Assert(FALSE);
	
	return 0;
}
#endif //HCD_V2

#endif //USB_HOST_ENABLE
