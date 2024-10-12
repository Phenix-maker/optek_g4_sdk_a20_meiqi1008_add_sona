#include "os_config.h"

#ifdef USB_HOST_ENABLE

#include "mem_reloc.h"
#include "interrupt.h"
#include "hw_timer.h"

#ifdef IPOD_USB_AUDIO
#include "audio.h"
#endif

#include "usbd.h"
#include "otg.h"
#include "hstack.h"
#include "hcd.h"


#ifdef HCD_V2

#define PORT_RESET_DELAY    40

#define LOAD_DATA_NOT_ALIGNED_4

//enable upstream ISO transfer
#define IsoIN_Feature

#undef DBG_USBHPrintf
#define DBG_USBHPrintf

#undef DBG_Printf
#define DBG_Printf 


typedef	struct {
	U8	opened;
	U8	devSpeed;
	
	U8	transMethod;
	
	int	intModeSof;		//schedule
	U8	highSpeedTick;
	U8	timeTick;		//256ms

	int	maxCtlTransPerFrame;

	HTD	*htdCtl;
	HTD	*htdBulk;
	HTD	*htdInt;
	HTD	*htdIso;
    #ifdef IsoIN_Feature
	HTD	*htdIsoIN; //jj+ for dirin = TRUE
    #endif
}	HCD;

HCD	hcd	__USBHOST_BSS;


#define	MAX_CTL_INT_TRANS_PER_FRAME_FULL_SPEED	(8/2)
#define	MAX_CTL_INT_TRANS_PER_FRAME_HIGH_SPEED	(24/2)


static int schedulerRun0( void );
static int schedulerErr( int endp, int dirin );
static int schedulerRun( int endp, int dirin );


void hcd_intProcess(void)	__USBHOST_TEXT;
static int htdIssue( HTD *htd )	__USBHOST_TEXT;
static int htdComplete( HTD *htd) __USBHOST_TEXT;
static int schedulerRun( int endp, int dirin ) __USBHOST_TEXT;
static int schedulerRun0( void ) __USBHOST_TEXT;


//assuem:	frame bandwidth is far from enough
void hcd_intProcess(void)
{
	volatile U16 reg;
	HCD	*phcd	=	&hcd;

	if( !phcd->opened )	return;

	reg	 =	*usbhostReg->irq->usbirq;
	reg &=  *usbhostReg->irq->usbien;

	//high speed int
	if( reg & 0x20 )	{
		phcd->devSpeed		=	enSPEED_HIGH;
		*usbhostReg->irq->usbirq =	0x20;
		phcd->maxCtlTransPerFrame	=	MAX_CTL_INT_TRANS_PER_FRAME_HIGH_SPEED;
	}

	//sof
	if( reg & 0x02 ) {
		*usbhostReg->irq->usbirq =	0x02;

		DBG_assert( phcd->intModeSof );
		DBG_assert( *usbhostReg->irq->hcouterrIrq == 0 );
		DBG_assert( *usbhostReg->irq->hcinerrIrq == 0 );
		DBG_assert( *usbhostReg->irq->txirq == 0 );
		DBG_assert( *usbhostReg->irq->rxirq == 0 );

		//jj move from schedulerRun0()
		if( phcd->intModeSof ) {
			if( phcd->devSpeed == enSPEED_HIGH  ) {
				if( ++phcd->highSpeedTick >= 8 ) {
					phcd->highSpeedTick = 0;
					++phcd->timeTick; //1 FS tick/ per 8 HS ticks
				}
			}
			else {
				++phcd->timeTick;
				if (phcd->timeTick >= 0xff)	//jj+ U8 type
					phcd->timeTick = 0;
			}
		}
		
		schedulerRun0();
	}

	reg = *usbhostReg->irq->hcouterrIrq;
	DBG_assert( reg == 0 );
	if( reg ) {
		*usbhostReg->irq->hcouterrIrq	=	reg;

		if( reg & 0x01 ) {	//out 0
			schedulerErr( 0, FALSE );
		}
		if( reg & 0x02 ) {	//out 1
			schedulerErr( 1, FALSE );
		}
		if( reg & 0x04 ) {	//out 2
			schedulerErr( 2, FALSE );
		}
	}

	reg = *usbhostReg->irq->hcinerrIrq;
	DBG_assert( reg == 0 );
	if( reg ) {
		*usbhostReg->irq->hcinerrIrq	=	reg;

		if( reg & 0x01 ) {	//in 0
			schedulerErr( 0, TRUE );
		}
		if( reg & 0x02 ) {	//in 1
			schedulerErr( 1, TRUE );
		}
		if( reg & 0x04 ) {	//in 2
			schedulerErr( 2, TRUE );
		}
	}

	reg = *usbhostReg->irq->txirq;
	if( reg ) {
		*usbhostReg->irq->txirq	=	reg;
		
		if( reg & 0x01 ) {
			//out 0
			schedulerRun( 0, FALSE );
		}
		if( reg & 0x02 ) {
			//out 1
			schedulerRun( 1, FALSE );
		}
		if( reg & 0x04 ) {
			//out 2
			schedulerRun( 2, FALSE );
		}
	}

	reg = *usbhostReg->irq->rxirq;
	if( reg ) {
		*usbhostReg->irq->rxirq	=	reg;
		
		if( reg & 0x01 ) {
			//in 0
			schedulerRun( 0, TRUE );
		}
		if( reg & 0x02 ) {
			//in 1
			schedulerRun( 1, TRUE );
		}
		if( reg & 0x04 ) {
			//in 2
			schedulerRun( 2, TRUE );
		}
	}
}


#define	SETUP_PKT_LEN	8

static int htdIssue( HTD *htd )
{
	PIPE	*pipe	=	htd->pipe;
	int i, size, tmp, loop;

	volatile U8 *ptr8;
	volatile U32 *ptr32;
	ENDPOINT_REGS	*endp;

	if( htd->complete )	return FALSE;

	htd->active	=	TRUE;

	if( pipe->type == enHPIPE_CTRL ) {

		switch( htd->ctlTransStat ) {
		case enCSTAGE_SETUP:
			*usbhostReg->endp0->cs		=	0x10;
			for( i = 0; i < SETUP_PKT_LEN; i ++ ) 
				usbhostReg->endp0->txdata[i]	=	htd->setupPkt[i];
			*usbhostReg->endp0->txbc	=	SETUP_PKT_LEN;

			//FIXME: for mtp ?
			if( *usbhostReg->endp0->hcouterr & 0x03 )
				*usbhostReg->endp0->hcouterr |=	0x20;
			break;
		
		case enCSTAGE_DATA:
			if( htd->dataIn ) {
				*usbhostReg->endp0->rxbc	=	0;

				//FIXME
				if( *usbhostReg->endp0->hcinerr & 0x03 )
					*usbhostReg->endp0->hcinerr |=	0x20;
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
	}
	else {

		if( pipe->type == enHPIPE_INT ) {
			if( pipe->dirin )
				endp	=	usbhostReg->endp2In;
			else
				endp	=	usbhostReg->endp2Out;
		}
		else {
			//enPIPE_BULK, enPIPE_ISO
			if( pipe->dirin )
				endp	=	usbhostReg->endp1In;
			else
				endp	=	usbhostReg->endp1Out;
		}
		
		if( pipe->dirin ) {
			DBG_assert( htd->transLen < htd->len );

			size	=	htd->len - htd->transLen;
			if( size > pipe->maxpkt )	size = pipe->maxpkt;			
			DBG_Printf(" di %x %x\n", size, htd->transLen);
			DBG_assert( size <= pipe->maxpkt );	//jj+

			if( *endp->hcerr & 0x03 ) 
				*endp->hcerr	|=	0x20;	//resend
			else {
			*endp->con	|=	0x80;
			*endp->cs	=	0x02;
			*endp->bc	=	size;
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
							*endp->fifoDword	=	*ptr32 ++;
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
							*endp->fifoWord	=	*ptr16 ++;
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
						*endp->fifoByte	=	*ptr8 ++;
					}
						
					#else
					
					tmp = size/4;
					ptr32	=	(U32*)htd->buffer;
					DBG_assert( ( (U32)ptr32 & 0x03 ) == 0 );
					for( i = 0; i < tmp; i++ )
					{
						*endp->fifoDword	=	*ptr32 ++;
					}
					
					tmp = size % 4;
					if( tmp > 0 )
					{
						ptr8	=	(U8*)ptr32;
						for( i = 0; i < tmp; i++ )
						{
							*endp->fifoByte	=	*ptr8 ++;
						}
					}
					#endif //LOAD_DATA_NOT_ALIGNED_4

					htd->buffer +=	size;
					htd->transLen +=	size;
					*endp->bc	=	size;
					*endp->cs	=	0x01;
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
	}

}

static int htdComplete( HTD *htd)
{
	PIPE	*pipe	=	htd->pipe;
	int i, size, tmp, loop;

	volatile U8 *ptr8;
	volatile U32 *ptr32;
	ENDPOINT_REGS	*endp;

	htd->active	=	FALSE;

	if( pipe->type == enHPIPE_CTRL ) {

		switch( htd->ctlTransStat ) {
		case enCSTAGE_SETUP:
			if( htd->len == 0 ) 
				htd->ctlTransStat = enCSTAGE_STATUS;
			else 
				htd->ctlTransStat = enCSTAGE_DATA;
			break;
		
		case enCSTAGE_DATA:
			if( htd->dataIn ) {
				size = *usbhostReg->endp0->rxbc;
				
				DBG_assert( size + htd->transLen <= htd->len );
				for( i = 0; i < size; i ++ ) 
					htd->buffer[i]	=	usbhostReg->endp0->rxdata[i];	
				
				htd->buffer 	+=	size;
				htd->transLen	+=	size;
				
				if( size < pipe->maxpkt )	htd->shortPkt = TRUE;

				if( htd->shortPkt || (htd->transLen == htd->len) ) {
					htd->ctlTransStat = enCSTAGE_STATUS;
				}
				
			}
			else {
				if( htd->transLen == htd->len ) {
					htd->ctlTransStat = enCSTAGE_STATUS;
				}
			}
			break;

		case enCSTAGE_STATUS:
			htd->ctlTransStat = enCSTAGE_COMPLETE;
			htd->result	=	enURBRST_SUCESS;
			htd->complete	=	TRUE;
			break;

		case enCSTAGE_COMPLETE:
			break;
		default:
			DBG_assert(0);
		}
	}
	else {

		if( pipe->type == enHPIPE_INT ) {
			if( pipe->dirin )
				endp	=	usbhostReg->endp2In;
			else
				endp	=	usbhostReg->endp2Out;
		}
		else {
			//enPIPE_BULK, enPIPE_ISO
			if( pipe->dirin )
				endp	=	usbhostReg->endp1In;
			else
				endp	=	usbhostReg->endp1Out;
		}
		
		if( pipe->dirin ) {
			size	=	*endp->bc;
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
						*ptr32 ++	=	*endp->fifoDword;
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
						*ptr16 ++	=	*endp->fifoWord;
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
					*ptr8 ++	=	*endp->fifoByte;
				}

				#else
			
				tmp = size/4;
				ptr32	=	(U32*)htd->buffer;
				DBG_assert( ( (U32)ptr32 & 0x03 ) == 0 );
				for( i = 0; i < tmp; i ++ )
				{
					*ptr32 ++	=	*endp->fifoDword;
				}
				
				tmp = size % 4;
				if( tmp > 0 )
				{
					ptr8 = (U8*)ptr32;
					for( i = 0; i < tmp; i ++ )
					{
						*ptr8 ++	=	*endp->fifoByte;
					}
				}
				#endif

				//FIXME: alignment
				
				htd->buffer +=	size;
				htd->transLen +=	size;
				
				if( pipe->type == enHPIPE_ISO )
					htd->shortPkt = FALSE;
				else  if( size < pipe->maxpkt )	
					htd->shortPkt = TRUE;
				
				break;
				
			case enTRANS_DMA32:
				break;
				
			case enTRANS_PIO64:
			case enTRANS_DMA64:
			default:
				DBG_assert(0);
			}

			//FIXME
			if( pipe->type == enHPIPE_ISO ) {
				htd->complete	=	TRUE;
				htd->result		=	enURBRST_SUCESS;
			}
			else {
				if( htd->shortPkt || (htd->transLen == htd->len) ) {
					htd->complete = TRUE;
					
					if( htd->transLen == htd->len )
						htd->result	=	enURBRST_SUCESS;
					else
						htd->result	=	enURBRST_SUCCESS_UNDERRUN;
				}
			}
			
		}
		else {
			if( htd->transLen >= htd->len ) {
				htd->complete = TRUE;
				htd->result	=	enURBRST_SUCESS;
			}
		}
		
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
	case HC_ERR_UNDERRUN:
		result	=	enURBRST_SUCCESS_UNDERRUN;
		break;
	case HC_ERR_PID:
	case HC_ERR_OVERRUN:
	default:
		result	=	enURBRST_FAIL;
		break;
	}
}


//FIXME: error recovery ?
static int htdErr( HTD *htd )
{
	HCD	*phcd	=	&hcd;
	PIPE	*pipe	=	htd->pipe;
	volatile U8 reg8;
	U8	tmp;
	ENDPOINT_REGS	*endp;

	htd->active	=	FALSE;

	if( pipe->type == enHPIPE_CTRL ) {
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
			htd->complete	=	TRUE;
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
			htd->complete	=	TRUE;
			break;
		}
	}
	else {

		if( pipe->type == enHPIPE_INT ) {
			if( pipe->dirin )
				endp	=	usbhostReg->endp2In;
			else
				endp	=	usbhostReg->endp2Out;

			reg8	=	*endp->hcerr;
			reg8	&=	HC_ERR_MASK;
			if( reg8 == 0x00 ) {
				//NAK, do nothing
				if( !phcd->intModeSof ) {
					htd->result	=	enURBRST_INT_NAK;
					htd->complete	=	TRUE;			
				}
			}
			else {
				htd->result	=	endpErr2htdErr( reg8 );
				htd->complete	=	TRUE;			
			}
		}
		else {
			//enPIPE_BULK, enPIPE_ISO
			if( pipe->dirin )
				endp	=	usbhostReg->endp1In;
			else
				endp	=	usbhostReg->endp1Out;
			
			reg8	=	*endp->hcerr;
			reg8	&=	HC_ERR_MASK;

			//FIXME
			if( reg8 == HC_ERR_UNDERRUN ) {
				asm( "nop" );
			}

			//iso should ignore err
			if( pipe->type == enHPIPE_ISO ) {
				htd->complete	=	TRUE;			
				htd->result	=	endpErr2htdErr( reg8 );
			}
			else {
				htd->result	=	endpErr2htdErr( reg8 );
				htd->complete	=	TRUE;		
			}
		}

	}
}

static HTD *getHtd( int endp, int dirin )
{
	HCD	*phcd	=	&hcd;
	HTD	*htd	=	NULL;

	if( endp == 0 ) {
		htd	=	phcd->htdCtl;
	}
	else if( endp == 2 ) {
		htd	=	phcd->htdInt;
		DBG_assert( dirin == htd->pipe->dirin );
	}
	else if( endp == 1 ) {

		#ifndef IsoIN_Feature
		DBG_assert( (phcd->htdBulk == NULL) || (phcd->htdIso == NULL ) );
		#else
		DBG_assert( (phcd->htdBulk == NULL) || ((phcd->htdIso == NULL) && (!dirin) )  || 
			((phcd->htdIsoIN == NULL) && (dirin) ));
		#endif
		
		if( phcd->htdBulk != NULL )
			htd	=	phcd->htdBulk;
		else if( (phcd->htdIso != NULL) && !dirin)
		{
			htd	=	phcd->htdIso;
		}
		#ifdef IsoIN_Feature
		else if( (phcd->htdIsoIN != NULL) && dirin)
		{
			htd	=	phcd->htdIsoIN;
		}
		DBG_assert( dirin == htd->pipe->dirin );
		#endif
	}
	else {
		DBG_assert(0);
	}

	DBG_assert( htd != NULL );
	return htd;
}

static int setHtdNull( HTD *htd )
{
	HCD	*phcd	=	&hcd;
	int ret = TRUE;

	if( htd == phcd->htdCtl )
		phcd->htdCtl	=	NULL;
	else if( htd == phcd->htdBulk )
		phcd->htdBulk	=	NULL;
	else if( htd == phcd->htdInt )
		phcd->htdInt	=	NULL;
	#ifndef IsoIN_Feature
	else if( htd == phcd->htdIso )
		phcd->htdIso	=	NULL;
	#else
	else if( htd == phcd->htdIso || htd == phcd->htdIsoIN )
	{
		phcd->htdIso	=	NULL;
		phcd->htdIsoIN	=	NULL;
	}
	#endif
	else
		ret = FALSE;

	DBG_assert( ret );
	return ret;
}

static int schedulerErr( int endp, int dirin )
{
	HCD	*phcd	=	&hcd;
	HTD *htd	=	getHtd( endp, dirin );
	if( htd == NULL )	return FALSE;

	htdErr( htd );

	//FIXME:	do schedule
	if( htd->complete ) {
		if( htd->waitEvent ) 
			usbmsg_setHTDevent();

		if( htd->callback != NULL )
			(*htd->callback) (htd );

		if( htd->waitEvent )
			setHtdNull( htd );
	}

	return TRUE;
}

static int schedulerRun( int endp, int dirin )
{
	HCD	*phcd	=	&hcd;

	HTD *htd	=	getHtd( endp, dirin );
	U8	*buf;
	int highspeed = (phcd->devSpeed == enSPEED_HIGH);//jj+

	if( htd == NULL )	return FALSE;

	htdComplete( htd );

	if( htd->complete ) {

		if( htd->waitEvent ) 
			usbmsg_setHTDevent();

		if( htd->callback != NULL )
			(*htd->callback) (htd );

		//special process for int/iso
		if( phcd->intModeSof ) {

			#ifndef IsoIN_Feature
			if( (phcd->htdInt == htd) || (phcd->htdIso == htd) ) {
			#else
			if( (phcd->htdInt == htd) || (phcd->htdIso == htd) || (phcd->htdIsoIN == htd) ) {
			#endif
				htd->transLen = 0;
				htd->complete = FALSE;
				htd->active = FALSE;
				htd->buffer	=	htd->buf0;

				#ifndef IsoIN_Feature
				if(phcd->htdIso == htd) 
				#else
				if(phcd->htdIso == htd || phcd->htdIsoIN == htd) 
				#endif
					if( (!highspeed && (phcd->timeTick % htd->pipe->interval) == 0) ||
						( highspeed && (phcd->highSpeedTick % htd->pipe->interval) == 0) )//jj+
					{
					htdIssue( htd );
					}
			}
			else if( htd->waitEvent )
				setHtdNull( htd );
		}
		else {
			setHtdNull( htd );
		}
	}
	else {
		if( !phcd->intModeSof ) 
			htdIssue( htd );
		else {
			//FIXME:	add more transaction ?
			//FIXME: if no iso, can do ctl/int mutiple times
			//could further optimse
			#ifndef IsoIN_Feature
			if( phcd->htdIso == NULL ) {
			#else
			if( phcd->htdIso == NULL && phcd->htdIsoIN == NULL ) {
			#endif
				if( htd->pipe->type == enHPIPE_CTRL )
					if( ++htd->transPerFrame < phcd->maxCtlTransPerFrame )
						htdIssue( htd );
			}
		}
	}

	return TRUE;
}

static int schedulerRun0( void )
{
	HCD	*phcd	=	&hcd;
	HTD	*htd;
	int tmask = 0;
	int highspeed = (phcd->devSpeed == enSPEED_HIGH);


	if( phcd->intModeSof ) {
		//check for active
		if( (phcd->htdCtl != NULL) && phcd->htdCtl->active )	return FALSE;
		if( (phcd->htdInt != NULL) && phcd->htdInt->active )	return FALSE;
		if( (phcd->htdBulk != NULL) && phcd->htdBulk->active )	return FALSE;
	} 

	//endp0
	if( phcd->htdCtl != NULL ) {
		tmask	|=	0x01;
		htd	=	phcd->htdCtl;
		DBG_assert( htd->active == FALSE );

		if( !htd->active ) {
			htd->transPerFrame	=	0;
			htdIssue( htd );
		}
	}

	//endp2
	if( phcd->htdInt != NULL ) {
		tmask	|=	0x02;
		htd	=	phcd->htdInt;
		DBG_assert( htd->active == FALSE );

		if( !htd->active ) {
			
			htd->transPerFrame	=	0;
			if( phcd->intModeSof )
			{
				if( highspeed )
				{
					if( (phcd->highSpeedTick % htd->pipe->interval) == 0 )
					{
						htdIssue( htd );
					}
				}
				else
				{
					if( (phcd->timeTick % htd->pipe->interval) == 0 )
					{
						htdIssue( htd );
					}
				}
			}
			else
				htdIssue( htd );
		}
	}

	//endp1
	if( phcd->htdIso != NULL ) {
		tmask	|=	0x04;
		DBG_assert( phcd->htdBulk == NULL );
		htd	=	phcd->htdIso;
		DBG_assert( htd->active == FALSE );
		htd->transPerFrame	=	0;

		//FIXME
		if( htd->firstTrans ) {
			htd->firstTrans	=	FALSE;
			DBG_Printf("\n+>sR00\n");

			if( phcd->intModeSof ) {
				if( highspeed ) {
					if( phcd->highSpeedTick == 1 ) {	//avoid conflict with int
						if( (phcd->highSpeedTick % htd->pipe->interval) == 0 ) {
							htdIssue( htd );
						}
					}
				}
				else {
					if( (phcd->timeTick % htd->pipe->interval) == 0 ) {
						htdIssue( htd );
					}
				}
			}
			else
				htdIssue( htd );
		}
		else	//jj+ for Not htd->firstTrans
		{
			if( (!highspeed && (phcd->timeTick % htd->pipe->interval) == 0) ||
				(highspeed && (phcd->highSpeedTick % htd->pipe->interval) == 0) )
			{
				htdIssue( htd );
			}

		}
	} //htdIso



#ifdef IsoIN_Feature
	//endp1
	if( phcd->htdIsoIN != NULL ) {
		tmask	|=	0x04;
		DBG_assert( phcd->htdBulk == NULL );
		htd	=	phcd->htdIsoIN;
		DBG_assert( htd->active == FALSE );
		htd->transPerFrame	=	0;

		//FIXME
		if( htd->firstTrans ) {
			htd->firstTrans	=	FALSE;

			if( phcd->intModeSof )
			{
				if( highspeed )
				{
					if( phcd->highSpeedTick == 1 )
					{
						//avoid conflict with int
						if(	(phcd->highSpeedTick % htd->pipe->interval) == 0 )
						{
							htdIssue( htd );
						}
					}
				}
				else
				{
					if( (phcd->timeTick % htd->pipe->interval) == 0 )
					{
						htdIssue( htd );
					}
				}
			}
			else
				htdIssue( htd );
		}
		else //jj+ for Not htd->firstTrans
		{
			if( (!highspeed && (phcd->timeTick % htd->pipe->interval) == 0) ||
				(highspeed && (phcd->highSpeedTick % htd->pipe->interval) == 0) )
			{
				htdIssue( htd );
			}

		}
	} //htdIsoIN
#endif //IsoIN_Feature
	
	if( phcd->htdBulk != NULL )
	{
		tmask	|=	0x08;
        #ifndef IsoIN_Feature
		DBG_assert( phcd->htdIso == NULL );
        #else
		DBG_assert( phcd->htdIso == NULL && phcd->htdIsoIN == NULL );
        #endif
		htd	=	phcd->htdBulk;
		DBG_assert( htd->active == FALSE );

		if( !htd->active )
		{
			htd->transPerFrame	=	0;
			htdIssue( htd );
		}
	}

	if(!(phcd->intModeSof))
	{
		DBG_assert( (tmask == 0x01) || (tmask == 0x02) || (tmask == 0x04) || (tmask == 0x08) );
		DBG_assert( (tmask & 0x0c) != 0x0c );
	}

	return TRUE;
}


static int configEndp( PIPE *pipe, ENDPOINT_REGS *endpin,  ENDPOINT_REGS *endpout )
{
	U8	ptype;
	volatile U8 reg8;
	ENDPOINT_REGS	*endp;

	//dir & pipe type
	ptype	=	0;
	switch( pipe->type ) {
	case enHPIPE_ISO:
		ptype	=	0x04;
		break;
	case enHPIPE_BULK:
		ptype	=	0x08;
		break;
	case enHPIPE_INT:
		ptype	=	0x0c;
		break;
	default:
		DBG_assert(0);
	}

	if( pipe->dirin )	
		endp	=	endpin;
	else
		endp	=	endpout;
	
	*endp->hcctrl	=	pipe->endpnum;
	*endp->maxpkt	=	pipe->maxpkt;
	
	reg8 = *endp->con;
	reg8 &= ~0x0c;
	reg8 |= ptype;

	*endp->con	=	reg8;	
		
	return TRUE;
}
static int configPipe( PIPE *pipe )
{

	*usbhostReg->ctrl->funcaddr	=	pipe->usbdev->addr;

	if( pipe->type == enHPIPE_CTRL ) {
		//endp0
		*usbhostReg->endp0->hcctrl		=	pipe->endpnum;
		*usbhostReg->endp0->hcmaxpkt	=	pipe->maxpkt;
	}
	else if( pipe->type == enHPIPE_INT) {
		//endp2
		configEndp( pipe, usbhostReg->endp2In, usbhostReg->endp2Out );
	}
	else {
		//endp1
		configEndp( pipe, usbhostReg->endp1In, usbhostReg->endp1Out );
	}

	return TRUE;
}

int hcd_addHtd( HTD *htd )
{
	HCD	*phcd	=	&hcd;
	PIPE *pipe	=	htd->pipe;

	htd->active	=	FALSE;

	USB_INT_DISABLE;	//actually can remove
	
	switch( pipe->type ) {
	case enHPIPE_CTRL:
		DBG_assert( phcd->htdCtl == NULL );
		phcd->htdCtl	=	htd;
		break;
	case enHPIPE_ISO:
		DBG_assert( phcd->htdIso == NULL );
		if (!htd->dataIn)
			phcd->htdIso	=	htd;
		#ifdef IsoIN_Feature
		else
			phcd->htdIsoIN	=	htd;
		#endif
		break;
	case enHPIPE_BULK:
		DBG_assert( phcd->htdBulk == NULL );
		phcd->htdBulk	=	htd;
		break;
	case enHPIPE_INT:
		DBG_assert( phcd->htdInt == NULL );
		phcd->htdInt	=	htd;
		break;
	}

	configPipe( pipe );

	USB_INT_ENABLE;

	if( !phcd->intModeSof ) {
		schedulerRun0();
	}

	return TRUE;
}


HTD *hcd_removeHtd( PIPE *pipe )
{
	HCD	*phcd	=	&hcd;
	HTD	*htd	=	NULL;

	USB_INT_DISABLE;

	switch( pipe->type ) {
	case enHPIPE_CTRL:
		DBG_assert( phcd->htdCtl != NULL );
		htd	=	phcd->htdCtl;
		phcd->htdCtl	=	NULL;
		break;
	case enHPIPE_BULK:
		DBG_assert( phcd->htdBulk != NULL );
		htd	=	phcd->htdBulk;
		phcd->htdBulk	=	NULL;
		break;

	case enHPIPE_INT:
		DBG_assert( phcd->htdInt != NULL );
		htd	=	phcd->htdInt;
		phcd->htdInt	=	NULL;
		break;
	case enHPIPE_ISO:
		DBG_assert( phcd->htdIso != NULL );
		if ( !pipe->dirin ) {
		htd	=	phcd->htdIso;
		phcd->htdIso	=	NULL;
		}
		#ifdef IsoIN_Feature
		else {
			htd	=	phcd->htdIsoIN;
			phcd->htdIsoIN	=	NULL;
		}
		#endif
		break;
	default:
		DBG_assert(0);
	}
	
	USB_INT_ENABLE;

	DBG_assert( htd != NULL );
	if( htd == NULL )	return NULL;

	htd->result	=	enURBRST_TERMINATED;
	if( htd->waitEvent )
		usbmsg_setHTDevent();
	if( htd->callback != NULL )
		(*htd->callback) ( htd );
	

	return htd;
}




int hcd_setScheduleMode( int sof )
{
	HCD	*phcd	=	&hcd;
	
	if( sof != phcd->intModeSof ) {
		//wait all htd
		#ifdef IsoIN_Feature
		DBG_assert( (phcd->htdBulk == NULL) && (phcd->htdCtl == NULL) && (phcd->htdInt == NULL) && (phcd->htdIso == NULL) );
		#else
		DBG_assert( (phcd->htdBulk == NULL) && (phcd->htdCtl == NULL) && (phcd->htdInt == NULL) && (phcd->htdIso == NULL && phcd->htdIsoIN == NULL) );
		#endif

		USB_INT_DISABLE;

		phcd->intModeSof = sof;
		if( sof ) {
			*usbhostReg->irq->usbirq		=	0xff;
			*usbhostReg->irq->usbien		|=	0x02;
		}
		else {
			*usbhostReg->irq->usbien		&=	~0x02;
			*usbhostReg->irq->usbirq		=	0xff;
		}

		USB_INT_ENABLE;
	}

	return TRUE;
}

//===============================================================================================

int hcd_devUnplug( void )
{
	HCD	*phcd	=	&hcd;


	if( phcd->htdCtl != NULL ) {
		phcd->htdCtl->result	=	enURBRST_DEV_DISCONNECT;
		phcd->htdCtl->transLen	=	0;

		if( phcd->htdCtl->waitEvent )
			usbmsg_setHTDevent();
		phcd->htdCtl	=	NULL;
	}

	if( phcd->htdBulk != NULL ) {
		phcd->htdBulk->result	=	enURBRST_DEV_DISCONNECT;
		phcd->htdBulk->transLen	=	0;

		if( phcd->htdBulk->waitEvent )
			usbmsg_setHTDevent();
		phcd->htdBulk	=	NULL;
	}

	if( phcd->htdInt != NULL ) {
		phcd->htdInt->result	=	enURBRST_DEV_DISCONNECT;
		DBG_assert( phcd->htdInt->callback != NULL );
		phcd->htdInt->transLen	=	0;

		if( phcd->htdInt->callback != NULL )
			(*phcd->htdInt->callback) ( phcd->htdInt );
		phcd->htdInt	=	NULL;
	}

	if( phcd->htdIso != NULL ) {
		phcd->htdIso->result	=	enURBRST_DEV_DISCONNECT;
		DBG_assert( phcd->htdIso->callback != NULL );
		phcd->htdIso->transLen	=	0;

		if( phcd->htdIso->callback != NULL )
			(*phcd->htdIso->callback) ( phcd->htdIso );
		phcd->htdIso	=	NULL;
	}

	#ifdef IsoIN_Feature
	if( phcd->htdIsoIN != NULL ) {//jj+
		phcd->htdIsoIN->result	=	enURBRST_DEV_DISCONNECT;
		DBG_assert( phcd->htdIsoIN->callback != NULL );
		phcd->htdIsoIN->transLen	=	0;

		if( phcd->htdIsoIN->callback != NULL )
			(*phcd->htdIsoIN->callback) ( phcd->htdIsoIN );
		phcd->htdIsoIN	=	NULL;
	}
	#endif

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
	HCD	*phcd	=	&hcd;
	hcd.devSpeed		=	enSPEED_UNKNOWN;


	phcd->maxCtlTransPerFrame	=	MAX_CTL_INT_TRANS_PER_FRAME_FULL_SPEED;

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
	HCD	*phcd	=	&hcd;

	phcd->maxCtlTransPerFrame	=	MAX_CTL_INT_TRANS_PER_FRAME_FULL_SPEED;
	phcd->intModeSof	=	FALSE;

	*usbhostReg->portctrl	|=	0x20;
	while( *usbhostReg->portctrl & 0x20 );
	timer_delayms(PORT_RESET_DELAY); //10

	//fifo ctrl
	*usbhostReg->ctrl->fifoctl		=	0x81;
	*usbhostReg->ctrl->fifoctl		=	0x91;
	*usbhostReg->ctrl->fifoctl		=	0x82;
	*usbhostReg->ctrl->fifoctl		=	0x92;


	//0: ctl, 1: iso/bulk, 2: int

	*usbhostReg->endp0->hcmaxpkt	=	0;
	*usbhostReg->endp1In->maxpkt	=	0;
	*usbhostReg->endp1Out->maxpkt	=	0;

	*usbhostReg->endp1In->staddr	=	ENDPOINT0_BUF_SIZE;
	*usbhostReg->endp1In->con		=	0;

#ifdef USB_HOST_AUDIO_ENABLE 	//jj test with 
	*usbhostReg->endp1Out->staddr	=	ENDPOINT0_BUF_SIZE + 256;
	*usbhostReg->endp1Out->con		=	0;

	*usbhostReg->endp2In->maxpkt	=	0;
	*usbhostReg->endp2Out->maxpkt	=	0;
#else
	*usbhostReg->endp1Out->staddr	=	ENDPOINT0_BUF_SIZE;
	*usbhostReg->endp1Out->con		=	0;

	*usbhostReg->endp2Out->maxpkt	=	0;
	*usbhostReg->endp2Out->maxpkt	=	0;
#endif

	*usbhostReg->endp2In->staddr	=	ENDPOINT0_BUF_SIZE + ENDPOINT1_BUF_SIZE;
	*usbhostReg->endp2In->con		=	0;

	*usbhostReg->endp2Out->staddr	=	ENDPOINT0_BUF_SIZE + ENDPOINT1_BUF_SIZE;
	*usbhostReg->endp2Out->con		=	0;

	//toggle & fifo	
	*usbhostReg->ctrl->enprst	=	0x00;
	*usbhostReg->ctrl->enprst	=	0x60;
	*usbhostReg->ctrl->enprst	=	0x10;
	*usbhostReg->ctrl->enprst	=	0x70;

	*usbhostReg->irq->hcinerrIrq	=	0xff;
	*usbhostReg->irq->hcouterrIrq	=	0xff;
	*usbhostReg->irq->txirq			=	0xff;
	*usbhostReg->irq->rxirq			=	0xff;

	*usbhostReg->irq->hcinerrIen	=	0x07;
	*usbhostReg->irq->hcouterrIen	=	0x07;
	*usbhostReg->irq->txien			=	0x07;
	*usbhostReg->irq->rxien			=	0x07;

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

#endif //HCD_V2

#endif //USB_HOST_ENABLE
