#include "os_config.h"

#include "mem_reloc.h"

#include "fifo.h"
#include "audio_task.h"


//#pragma section fifo_code ".internal_sram"
//#pragma use_section fifo_code fifo_get_free_space,fifo_get_data_len
//#pragma use_section fifo_code fifo_put_data, fifo_get_data



void fifo_init ( FIFO *pFifo, U8 *pBuf, unsigned long len, int empty) __INTERNAL_RAM_TEXT;
long fifo_get_free_space (FIFO *pFifo) __INTERNAL_RAM_TEXT;
long fifo_get_data_len (FIFO *pFifo) __INTERNAL_RAM_TEXT;

long fifo_get_fifo_data_wlen (FIFO *pFifo) __INTERNAL_RAM_TEXT;
long fifo_put_data (FIFO *pFifo, U8 *pBuf, unsigned long len) __INTERNAL_RAM_TEXT;
long fifo_put_data_one_byte (FIFO *pFifo, U8 *pBuf) __INTERNAL_RAM_TEXT;
long fifo_get_data (FIFO *pFifo, U8 *pBuf, unsigned long len) __INTERNAL_RAM_TEXT;


#ifdef BT_HCI_ENABLE
long fifo_get_OLA_data (FIFO *pFifo, U8 *pBuf, long rlen, long slen) __INTERNAL_RAM_TEXT;
long fifo_put_data_by_dma (FIFO *pFifo, U8 *pBuf) __INTERNAL_RAM_TEXT;
long fifo_get_fifo_4byte_data_len (FIFO *pFifo) __INTERNAL_RAM_TEXT;
long fifo_get_4byte_data (FIFO *pFifo, U8 *pBuf, long len) __INTERNAL_RAM_TEXT;
long fifo_get_4byte_ir_data (FIFO *pFifo, U16 *pBuf) __INTERNAL_RAM_TEXT;
#endif

long fifo_discard_data (FIFO *pFifo, unsigned long len) __INTERNAL_RAM_TEXT;


void fifo_init ( FIFO *pFifo, U8 *pBuf, unsigned long len, int empty)
{
	pFifo->valid = FALSE;
	pFifo->fifo_start_addr = pBuf;
	pFifo->fifo_end_addr = pBuf + len;
	pFifo->fifo_length = len;
	pFifo->pRead = pFifo->fifo_start_addr;
	pFifo->pWrite = pFifo->fifo_start_addr;
	pFifo->wlen_H = 0;
	
	if (empty)
	{
		pFifo->wLen = 0;
		pFifo->rLen = 0;
	}
	else
	{
		pFifo->wLen = pFifo->fifo_length;
		pFifo->rLen = 0;
	}

	pFifo->start_pos = 0;
	pFifo->stream_len = STREAM_LENTH_UNKNOWN;
	pFifo->entry = 0;
	pFifo->stream_margin = 0;
}

void fifo_init_w_data ( FIFO *pFifo, U8 *pBuf, unsigned long len, unsigned long dlen)
{
	if (dlen > len) {
		dlen = 0;
		DBG_Assert (FALSE);
	}
	
	pFifo->valid = FALSE;
	pFifo->fifo_start_addr = pBuf;
	pFifo->fifo_end_addr = pBuf + len;
	pFifo->fifo_length = len;
	pFifo->pRead = pFifo->fifo_start_addr;
	pFifo->pWrite = pFifo->fifo_start_addr;
	pFifo->wlen_H = 0;


	memset (pBuf,0,len);
	
	if (dlen > 0) {
		pFifo->wLen = dlen;
		pFifo->pWrite = pFifo->fifo_start_addr + dlen;
		pFifo->rLen = 0;
	}
	else {
		pFifo->wLen = 0;
		pFifo->rLen = 0;
	}

	pFifo->start_pos = 0;
	pFifo->stream_len = STREAM_LENTH_UNKNOWN;
	pFifo->entry = 0;
	pFifo->stream_margin = 0;
}

/****************use opened fifo to manage one music stream***************************************/
void fifo_create_file_stream ( FIFO *pFifo, unsigned long start_pos, unsigned long stream_len, U16 entry,U16 stream_margin)
{
	pFifo->start_pos = start_pos;
	pFifo->stream_len = stream_len;
	pFifo->wLen = start_pos;
	pFifo->rLen = start_pos;
	pFifo->entry = entry;
	pFifo->stream_margin = stream_margin;
	pFifo->valid = TRUE;	
}

I16 fifo_file_stream_flush ( FIFO *pFifo)
{
	pFifo->valid = FALSE;
	pFifo->pRead = pFifo->fifo_start_addr;
	pFifo->pWrite = pFifo->fifo_start_addr;
	
	/*sometimes, need to wake the decode process, ask decode process to quit*/

	return TRUE;
}

long fifo_get_data_len (FIFO *pFifo)
{
	if (pFifo->wLen < pFifo->rLen)
	{
		return ( pFifo->wLen + ((0x100000000 - pFifo->rLen)) );
	}
	else
	{
		return (pFifo->wLen - pFifo->rLen);
	}
}

long fifo_get_free_space (FIFO *pFifo)
{
	int data_len = fifo_get_data_len(pFifo);

	return (pFifo->fifo_length - data_len - pFifo->stream_margin);
}

long fifo_get_fifo_data_wlen (FIFO *pFifo)
{
	if (pFifo->wlen_H > 0x0)
	{
		return 0xFFFFFFFF;
	}
	else 
	{
		return pFifo->wLen;
	}
}	

long fifo_put_data (FIFO *pFifo, U8 *pBuf, unsigned long len)
{
	unsigned long l;
	unsigned long pre_wlen;

#ifdef WAV_DEC//for special wave files
	if ( pFifo->wLen > pFifo->stream_len )
	{
		if ( ((global_vari.audio_source == SOURCE_USB_MSC) || 
			(global_vari.audio_source == MEDIA_SDMMC)) && 
			(app_media_data.playing_stream_type == STREAM_WAV) )
		{
			return 0;
		}
	}
#endif


	if (pFifo->stream_len != STREAM_LENTH_UNKNOWN) 
	{
		l = (pFifo->stream_len - pFifo->wLen);
		if (len > l) 
		{
			len = l;
		}
	}

	if (pFifo->wLen < pFifo->rLen)
	{
		l = pFifo->wLen + ((0x100000000 - pFifo->rLen));
	}
	else
	{
		l = pFifo->wLen - pFifo->rLen;
	}
	

	if (len > (pFifo->fifo_length - l - pFifo->stream_margin))
	{
		return 0;//overflow
	}

	
	pre_wlen = pFifo->wLen;

	if (len == 0)
	{
		return 0;
	}

	if ( (pFifo->pWrite + len) > pFifo->fifo_end_addr)
	{	
		l = pFifo->fifo_end_addr - pFifo->pWrite;

		CFasm_memcpy ( (void*) pFifo->pWrite, (void*) pBuf, l);
		
		pFifo->pWrite = pFifo->fifo_start_addr;
		pBuf += l;

		l = len - l;
		CFasm_memcpy ( (void*) pFifo->pWrite, (void*) pBuf, l);

		pFifo->pWrite += l;

		pFifo->wLen += len;
	}
	else
	{
		CFasm_memcpy ( (void*) pFifo->pWrite, (void*) pBuf, len);

		pFifo->pWrite += len;
		
		if (pFifo->pWrite == pFifo->fifo_end_addr)
		{
			pFifo->pWrite = pFifo->fifo_start_addr;
		}

		pFifo->wLen += len;
	}

	if (pFifo->wLen < pre_wlen)
	{
		pFifo->wlen_H = 0x01;
	}

	return len;
}

long fifo_put_data_one_byte (FIFO *pFifo, U8 *pBuf)
{
	U8 len = 1;

	DBG_Assert ( len <= ( pFifo->fifo_length - (pFifo->wLen - pFifo->rLen)) );

	if ( (pFifo->pWrite + len) > pFifo->fifo_end_addr)
	{
		pFifo->pWrite = pFifo->fifo_start_addr;
		*pFifo->pWrite++ = *pBuf;
		pFifo->wLen += 1;
	}
	else
	{
		*pFifo->pWrite++ = *pBuf;

		if (pFifo->pWrite == pFifo->fifo_end_addr)
		{
			pFifo->pWrite = pFifo->fifo_start_addr;
		}

		pFifo->wLen += 1;
	}

	return 1;
}

long fifo_get_data (FIFO *pFifo, U8 *pBuf, unsigned long len)
{
	unsigned long l;
	
	if (pFifo->wLen < pFifo->rLen)
	{
		l = pFifo->wLen + ((0x100000000 - pFifo->rLen));
	}
	else
	{
		l = pFifo->wLen - pFifo->rLen;
	}

	if (len > l)
	{
		if ( (decode_type_get() == DECODE_OPTICAL) ||
			(decode_type_get() == DECODE_DD_AC3) )
		{
			len = 0;//buf underflow
		}
		else
		{
			len = l;
		}
	}

	if (len == 0)
	{
		return 0;
	}

	if ( (pFifo->pRead + len) > pFifo->fifo_end_addr)
	{	
		l = pFifo->fifo_end_addr - pFifo->pRead;

		CFasm_memcpy ((void*) pBuf, (void*) pFifo->pRead, l);
		
		pFifo->pRead = pFifo->fifo_start_addr;
		pBuf += l;

		l = len - l;
		CFasm_memcpy ( (void*) pBuf, (void*) pFifo->pRead, l);

		pFifo->pRead += l;

		pFifo->rLen += len;
	}
	else
	{
		CFasm_memcpy ( (void*) pBuf, (void*) pFifo->pRead, len);

		pFifo->pRead += len;
		
		if (pFifo->pRead == pFifo->fifo_end_addr)
		{
			pFifo->pRead = pFifo->fifo_start_addr;
		}

		pFifo->rLen += len;
	}

	return len;
}

I8 fifo_file_stream_seek (FIFO *pFifo, unsigned long pos)
{
	TX_INTERRUPT_SAVE_AREA;
	U32 len;
	int ret = 0;

	DBG_Assert (pos <= pFifo->stream_len);
	DBG_Assert (pFifo->stream_len != STREAM_LENTH_UNKNOWN);

	if (pos < pFifo->start_pos)
	{
		return -1;
	}

		
	if (pos == pFifo->rLen) 
	{

	}
	else if (pos > pFifo->rLen) 
	{
		//seek forward

		if (pos > pFifo->wLen)
		{	
			//seek out of stream write len
			ret = -1;
		}
		else
		{
			pFifo->pRead += (pos - pFifo->rLen);

			while (pFifo->pRead >= pFifo->fifo_end_addr) 
			{
				pFifo->pRead -= pFifo->fifo_length;
			}

			pFifo->rLen = pos;
		}		
	}
	else 
	{
		//seek backward 
		
		TX_DISABLE;		//seek backward need to lock stream to prevent any write

		len = pFifo->fifo_length - (pFifo->wLen - pFifo->rLen);

		if ( len >= (pFifo->rLen - pos) )
		{	
			pFifo->pRead -= (pFifo->rLen - pos);

			while (pFifo->pRead < pFifo->fifo_start_addr) 
			{
				pFifo->pRead += pFifo->fifo_length;
			}

			pFifo->rLen = pos;
		}
		else
		{
			//seek backward out of reserved space
			ret = -1;
		}

		TX_RESTORE;
	}
	
	return ret;

}

long fifo_get_OLA_data (FIFO *pFifo, U8 *pBuf, long rlen, long slen)
{
	long l;
	
	DBG_Assert ( rlen <= (pFifo->wLen - pFifo->rLen) );
	DBG_Assert (slen <= rlen);

	if (rlen == 0)
	{
		return 0;
	}
	
	if ( (pFifo->pRead + rlen) > pFifo->fifo_end_addr)
	{		
		l = pFifo->fifo_end_addr - pFifo->pRead;
		
		CFasm_memcpy ((void*) pBuf, (void*) pFifo->pRead, l);
		
		pBuf += l;
		
		l = rlen - l;
		CFasm_memcpy ( (void*) pBuf, (void*) pFifo->fifo_start_addr, l);
		
		pFifo->pRead += slen;
		
		if (pFifo->pRead >= pFifo->fifo_end_addr)
		{
			pFifo->pRead -= pFifo->fifo_length;
		}
		
		pFifo->rLen += slen;
	}
	else
	{
		CFasm_memcpy ( (void*) pBuf, (void*) pFifo->pRead, rlen);
		
		pFifo->pRead += slen;
		
		if (pFifo->pRead >= pFifo->fifo_end_addr)
		{
			pFifo->pRead -= pFifo->fifo_length;
		}
		
		pFifo->rLen += slen;
	}
	
	return rlen;
}

void fifo_discard_all_data (FIFO *pFifo)
{
	pFifo->pRead = pFifo->pWrite;
	
	if (pFifo->pRead == pFifo->fifo_end_addr)
	{
		pFifo->pRead = pFifo->fifo_start_addr;
	}

	pFifo->rLen = pFifo->wLen;
}

long fifo_discard_data (FIFO *pFifo, unsigned long len)
{
	DBG_Assert ( len <= fifo_get_data_len(pFifo) );
	
	if (len == 0)
	{
		return 0;
	}

	pFifo->rLen += len;

	pFifo->pRead += len;

	while (pFifo->pRead >= pFifo->fifo_end_addr)
	{
		pFifo->pRead -= pFifo->fifo_length;
	}
	
	return len;
}


long fifo_put_data_by_dma (FIFO *pFifo, U8 *pBuf)
{
	U32 wlen;

	DBG_Assert (pBuf >= pFifo->fifo_start_addr && pBuf <= (pFifo->fifo_end_addr));

	if (pBuf == pFifo->pWrite)
	{
		//no new in data
		return 0;
	}

	if (pBuf >= pFifo->fifo_end_addr)
	{
		//end addr
		pBuf = pFifo->fifo_start_addr;	
	}

	if (pBuf > pFifo->pWrite)
	{
		wlen = (U32) (pBuf - pFifo->pWrite);
		pFifo->wLen += wlen;
		pFifo->pWrite = pBuf;
	}
	else
	{
		//pBuf < pFifo->pWrite
		wlen = (U32) (pBuf + pFifo->fifo_length - pFifo->pWrite);
		pFifo->wLen += wlen;
		pFifo->pWrite = pBuf;
	}

	return wlen;
}
 
long fifo_get_fifo_4byte_data_len (FIFO *pFifo)
{
	long wlen;
	wlen = 	pFifo->wLen - pFifo->rLen;

	return ((wlen >> 2));
}

long fifo_get_4byte_data (FIFO *pFifo, U8 *pBuf, long len)
{
	long l,i;
	U32 *pU32Buf;
	U32 lenU32;
	TX_INTERRUPT_SAVE_AREA;


	if (len == 0)
	{
		return 0;
	}

	//4 byte len
	lenU32 = (len << 2);
	
	pU32Buf = (U32 *) pFifo->pRead;

	if ( (pFifo->pRead + lenU32) > pFifo->fifo_end_addr)
	{	
		l = (pFifo->fifo_end_addr - pFifo->pRead) >> 2;

		for (i=0;i<l;i++)
		{
			*pBuf++ =  (U8) (*pU32Buf++);
		}
		
		pFifo->pRead = pFifo->fifo_start_addr;
		pU32Buf = (U32 *) pFifo->pRead;
		
		l = len - l;
		for (i=0;i<l;i++)
		{
			*pBuf++ =  (U8) (*pU32Buf++);
		}

		
		pFifo->pRead = (U8 *) pU32Buf;
		
		if (pFifo->pRead == pFifo->fifo_end_addr)
		{
			pFifo->pRead = pFifo->fifo_start_addr;
		}
		
		pFifo->rLen += lenU32;

	}
	else
	{
		for (i=0;i<len;i++)
		{
			*pBuf++ =  (U8) (*pU32Buf++);
		}

		
		pFifo->pRead = (U8 *) pU32Buf;
		
		if (pFifo->pRead == pFifo->fifo_end_addr)
		{
			pFifo->pRead = pFifo->fifo_start_addr;
		}
		
		pFifo->rLen += lenU32;
	}

	return len;
}

long fifo_get_4byte_ir_data (FIFO *pFifo, U16 *pBuf)
{
	U32 *pU16Buf;
			
	if ( pFifo->wLen <= pFifo->rLen)
		return 0;
	
	pU16Buf = (U16 *) pFifo->pRead;

	*pBuf = *pU16Buf;

	pFifo->pRead += 4;

	if (pFifo->pRead == pFifo->fifo_end_addr)
	{
		pFifo->pRead = pFifo->fifo_start_addr;
	}

	pFifo->rLen += 4;
	
	return 1;
}

