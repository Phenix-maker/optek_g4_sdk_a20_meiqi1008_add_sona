#include "os_config.h"

#include "c_def.h"
#include "debug.h"
#include "oem.h"


#ifdef USB_HOST_ENABLE

#include "mem_reloc.h"
#include "IRP.h"
#include "hw_timer.h"
#include "usbd.h"
#include "otg.h"
#include "hcd.h"
#include "hstack.h"




#ifdef	IPOD_MEM_SIZE_DOWN
#define	MAX_HTDS	8
#else
#define	MAX_HTDS	16
#endif

typedef	struct {
	HTD	*head;
	HTD *tail;
	int	maxItem;
	int	item;

	HTD	htdArray[MAX_HTDS];
}	HTD_QUEUE;


HTD_QUEUE	htdQueue	__USBHOST_BSS;

static HTD *htdQueue_get( void )
{
	HTD_QUEUE	*que	=	&htdQueue;
	HTD *htd;

	DBG_assert( que->item > 0 );
	if( que->item == 0 )	return NULL;

	htd	=	que->head;
	if ( que->head == que->tail ) 
		que->head = que->tail = NULL;
	else
		que->head = que->head->next;

	--que->item;
	return htd;
}

static int htdQueue_put( HTD *htd )
{
	HTD_QUEUE	*que	=	&htdQueue;

	DBG_assert( que->item < que->maxItem );

	if( que->tail == NULL ) {
		DBG_assert( que->head == NULL );
		que->head = que->tail = htd;
	}
	else {
		que->tail->next	=	htd;
		que->tail	=	htd;
	}

	htd->next	=	NULL;
	++que->item;

	return TRUE;
}

static int htdQueue_init(void)
{
	HTD_QUEUE	*que	=	&htdQueue;
	int i;

	memset( que, 0, sizeof(HTD_QUEUE) );

	que->maxItem	=	MAX_HTDS;
	for( i = 0; i < que->maxItem; i ++ ) {
		htdQueue_put( que->htdArray + i );
	}

	return TRUE;

}

int hstk_htdRetire( HTD *htd )
{
	int ret;

	if( htd == NULL )	return FALSE;

	ret = htdQueue_put( htd );
	DBG_assert( ret );

	return TRUE;
}

int hstk_transExt( PIPE *pipe, U8 dataIn, U8 *setuppkt, U8 *buf, U32 *len, U8 zeroPkt, PHTD_CALLBACK callback, int waitEvt )
{
	int ret;
	HTD *htd;
	int result;

	if( !hcd_isDevConnected() )	{
		*len	=	0;
		return enURBRST_DEV_DISCONNECT;
	}

	htd	=	htdQueue_get();
	DBG_assert( htd != NULL );
	if( htd == NULL )	return enURBRST_FAIL;

	htd->buf0		=	buf;
	htd->buffer		=	buf;
	htd->len			=	*len;
	htd->transLen	=	0;
	htd->zeroPkt		=	zeroPkt;

	htd->setupPkt	=	setuppkt;
	htd->ctlTransStat=	enCSTAGE_SETUP;
	htd->transResult	=	enHTD_FAIL;
	htd->dataIn		=	dataIn;
	htd->shortPkt	=	FALSE;
	htd->result		=	enURBRST_FAIL;

	htd->inPkts	=	0;

	htd->firstTrans	=	TRUE;
	htd->pipe		=	pipe;
	
	htd->callback	=	callback;
	htd->active		=	FALSE;
	htd->complete	=	FALSE;
	htd->waitEvent	=	waitEvt;

	if( waitEvt ) {
		DBG_assert( (pipe->type == enPIPE_CTRL) || (pipe->type == enPIPE_BULK)  );

		usbmsg_clearHTDevent();
		hcd_addHtd( htd );
		ret = usbmsg_waitHTDevent();
		if( !ret ) htd->result = enURBRST_TIMEOUT;

		*len	=	htd->transLen;
		DBG_assert( htd->result == enURBRST_SUCESS );

		result	=	htd->result;

		ret = htdQueue_put( htd );
		DBG_assert( ret );

		return result;
	}
	else {
		DBG_assert( (pipe->type == enPIPE_INT) || (pipe->type == enPIPE_ISO) );
		DBG_assert( callback != NULL );

		hcd_addHtd( htd );

		return enURBRST_SUCESS;		//submit success
	}

}


int hstk_trans( PIPE *pipe, U8 dataIn, U8 *setuppkt, U8 *buf, U32 *len, U8 zeroPkt )
{
	
	DBG_assert( (pipe->type == enPIPE_CTRL) || (pipe->type == enPIPE_BULK) );

	return hstk_transExt( pipe, dataIn, setuppkt, buf, len, zeroPkt, NULL, TRUE );
}


static PIPE *pipeConvert( USB_DEVICE *dev, U16 pipe )
{
	PIPE *pipe2	=	NULL;

	switch( usb_pipetype( pipe ) ) {
	case PIPE_CONTROL:
		pipe2 = &dev->pipes[enPIPE_INDX_CTRL];
		break;
	case PIPE_BULK:
		if( usb_pipein( pipe ) ) 
			pipe2 = &dev->pipes[enPIPE_INDX_BULK_IN];
		else
			pipe2 = &dev->pipes[enPIPE_INDX_BULK_OUT];
		break;
	case PIPE_INTERRUPT:
		if( usb_pipein( pipe ) ) 
			pipe2 = &dev->pipes[enPIPE_INDX_INT_IN];
		else
			pipe2 = &dev->pipes[enPIPE_INDX_INT_OUT];
		break;
	case PIPE_ISOCHRONOUS:
		if( usb_pipein( pipe ) ) 
			pipe2 = &dev->pipes[enPIPE_INDX_ISO_IN];
		else
			pipe2 = &dev->pipes[enPIPE_INDX_ISO_OUT];
		break;
	default:
		HALT;
	}

	DBG_assert( pipe2 != NULL );
	return pipe2;
}


int hstk_xferUsbdGlue( USB_DEVICE *usbDev, U8 *setupPacket, U8 *buf, U32 *totalLen, U16 pipe, U32 interval )
{
	int ret;
	U8	din;

	PIPE *pipe2 = pipeConvert( usbDev, pipe );
	DBG_assert( pipe2 != NULL );

	din	=	usb_pipein(pipe);
	if( (pipe2->type == enPIPE_CTRL) && (0 == *totalLen) ) din = TRUE;

	ret = hstk_trans( pipe2, din, setupPacket, buf, totalLen, ( (*totalLen) == 0) );
	DBG_assert( ret == enURBRST_SUCESS );

	return ret;
}

PIPE *hstk_getCtlPipe(void)
{
	int r;
	USB_DEVICE *dev;
	PIPE *pipe = NULL;

	r = usbd_getActiveDev( &dev );
	if( dev != NULL )
		pipe = &dev->pipes[enPIPE_INDX_CTRL];

	return pipe;
}

int hstk_init( void )
{
	htdQueue_init();
	return TRUE;
}


int hstk_open( void )
{
	htdQueue_init();
	return TRUE;
}

int hstk_close( void )
{
	htdQueue_init();
	return TRUE;
}

#endif //USB_HOST_ENABLE
