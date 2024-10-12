#include "os_config.h"

#include "interrupt.h"
#include "otg.h"

#include "mem_reloc.h"
#include "CFasm_memcpy.h"
#include "app_sdram.h"
#include "app_main.h"
#include "app_string_oem.h"
//#include "audio_dec.h"
#include <string.h>


/************************************************************************************************************
 *																											*							
 *									SDRAM STREAM BUFFER APPLICATION														*
 *																											*
 ************************************************************************************************************/											


extern U16 file_name[MAX_DISP_FILE_FOLDER_NAME+1];
extern U16 folder_name[MAX_DISP_FILE_FOLDER_NAME+1];
extern U16 unicode_name_to_oem[MAX_DISP_FILE_FOLDER_NAME+1];

#ifdef LCD_DISP_IPHONE_ID3_MESSAGE
#if 1
extern ID3_TAG id3_tag;
#else
U16 iPhone_title_buf[MAX_TITLE_LEN];
U16 iPhone_artist_buf[MAX_TITLE_LEN];
U16 iPhone_album_buf[MAX_TITLE_LEN];
#endif
#endif


//ID3_TAG entry_id3_tag;
//ID3_TAG entry_id3_tag __attribute__ ((section (".sdram_bss")));

PLAYING_INFO playing_info; //__attribute__ ((section (".sdram_bss")));

//U8 disp_folder_name[64];	//__attribute__ ((section (".sdram_bss")));
//U8 disp_file_name[64];	//__attribute__ ((section (".sdram_bss")));
//U16 disp_copy_file_name[256]	__attribute__ ((section (".sdram_bss")));

U8 entry_id3_flag = ID3_UNKNOWN;

//U8 c_disc_info[MAX_C_DISC_INFO_SPACE];

//C_ENTRY cEntry[MAX_ENTRY];
//C_SUBDIR cFolder[MAX_FOLDER];

//U8 *pFolderName;
//U8 *pEntryName;
//U8 *pCInfoStart;
//U16 wCInfoLen;


#if 0

typedef struct {
	long start_addr;
	long start_pos;
	long stream_start_pos;						/*audio stream start pos*/	
	long stream_len;
	long len;
	long pos;
	U16 entry;
	U8	overwrite	:	1;
	U8	valid		:	1;
	U8	user_data	:	1;
	U8	open		:	5;

} STREAM;

typedef struct {
	I16 total_streams;
	I16 overwrite;
	
	STREAM_ID rStream;						/*only one reading stream*/
	STREAM_ID wStream;						/*only one writing stream*/
	
	long wAddr;								/*current stream write address*/
	long ptAddr;							/*current stream protect address*/
	long rAddr;								/*current stream read address*/
	long space;			
	long ptspace;										
	
	I16 windows;							/*stream protect windows*/
	
	long start_addr;						/*must be in block addr*/
	long end_addr;							/*must be in block addr*/
	long valid_len;							//cdda stream	
	STREAM streams [TOTAL_OF_STREAMS];
} SDRAM_STREAMS;



SDRAM_STREAMS sdram_streams;// 	__attribute__ ((section (".sdram_bss")));;


#define STREAM_PRE_GAP	0x4000L


//SEC_BUF sec_buf;

//#pragma CODE_SECTION (app_sdram_stream_init, ".EXTERNAL_CODE"); 
//#pragma CODE_SECTION (app_sdram_stream_open, ".EXTERNAL_CODE"); 
//#pragma CODE_SECTION (app_sdram_stream_close, ".EXTERNAL_CODE"); 
//#pragma CODE_SECTION (app_sdram_stream_fini, ".EXTERNAL_CODE"); 


void app_sdram_stream_init (void)
{
	//sec_buf.size = 0;
	//sec_buf.addr = 0l;
}
 
void app_sdram_stream_open (SDRAM_STREAM_CONFIG *stream_config)
{
	I16 i;

	//sec_buf.size = 0;
	//sec_buf.addr = 0l;

	/*debug*/
	/*stream_config->start_addr = 0;
	stream_config->end_addr = 0x8000;*/


	sdram_streams.total_streams = 0;
	sdram_streams.overwrite = stream_config->overwrite;
	
	sdram_streams.rStream = INVALID_STREAM_ID;
	sdram_streams.wStream = INVALID_STREAM_ID;

	sdram_streams.windows = stream_config->windows;
	sdram_streams.wAddr = stream_config->start_addr;
	//sdram_streams.ptAddr = STREAM_INVALID_ADDR;
	sdram_streams.rAddr = stream_config->start_addr;
	sdram_streams.start_addr = stream_config->start_addr;
	sdram_streams.end_addr = stream_config->end_addr;
	sdram_streams.space = stream_config->end_addr - stream_config->start_addr;	
	sdram_streams.ptspace = stream_config->end_addr - stream_config->start_addr;	
	
	for (i=0; i<TOTAL_OF_STREAMS; i++)
	{
		sdram_streams.streams[i].start_addr = 0l;
		sdram_streams.streams[i].len = 0l;
		sdram_streams.streams[i].pos = 0l;
		sdram_streams.streams[i].overwrite = FALSE;
		sdram_streams.streams[i].open = 0;
		sdram_streams.streams[i].valid = FALSE;
	}
}

void app_sdram_stream_close ()
{
	I16 i;

	//sec_buf.size = 0;
	//sec_buf.addr = 0l;

	for (i=0; i<TOTAL_OF_STREAMS; i++)
	{
		sdram_streams.streams[i].valid = FALSE;
	}
}

void app_sdram_stream_fini (void)
{
}

//#pragma CODE_SECTION (app_sdram_stream_point, ".EXTERNAL_CODE"); 

long app_sdram_stream_point (long addr, I16 len) 
{
	
	addr += (long) len;
	
	while (addr >= sdram_streams.end_addr)
	{		
		addr -= (sdram_streams.end_addr -
			sdram_streams.start_addr);

	}

	while (addr < sdram_streams.start_addr)
	{
		addr += (sdram_streams.end_addr -
			sdram_streams.start_addr);
	}

	return addr;
	
}

//#pragma CODE_SECTION (app_sdram_stream_create_wstream, ".EXTERNAL_CODE"); 

STREAM_ID app_sdram_stream_create_wstream (long stream_pos, long stream_len, I16 entry)
{
	I16 i;
	STREAM_ID s;
	
	//only one opened wstream
	DBG_Assert (sdram_streams.wStream == INVALID_STREAM_ID);

	//stream_pos &= ~(1023l);

	if (sdram_streams.rStream == INVALID_STREAM_ID)
	{
		s = 0;
	}
	else
	{
		s = sdram_streams.rStream;

		for (i=0; i<TOTAL_OF_STREAMS; i++)
		{
			if (sdram_streams.streams[s].valid == FALSE)
			{
				break;
			}
			else
			{
				s++;
				if (s >= TOTAL_OF_STREAMS) {
					s = 0;
				}
			}
				
		}
	}

	if (sdram_streams.streams[s].valid == FALSE)
	{			
			//Interrupt_Disable;

			/*if (sdram_streams.wAddr & 0xFF)
			{
				sdram_streams.wAddr &= 0xFFFFFF00;				
				sdram_streams.wAddr = app_sdram_stream_point (sdram_streams.wAddr,0x100);
				
				sdram_streams.space &= 0xFFFFFF00;
				if (sdram_streams.ptAddr != STREAM_INVALID_ADDR)
				{
					sdram_streams.ptspace &= 0xFFFFFF00;
				}
			}*/
			

			sdram_streams.wStream = s;
			sdram_streams.valid_len = stream_pos;

			//sdram_streams.streams[s].start_addr = sdram_streams.wAddr;

			sdram_streams.streams[s].start_addr = (sdram_streams.wAddr + 15) & 0xFFFFFFF0;
			sdram_streams.wAddr = sdram_streams.streams[s].start_addr;

			if (sdram_streams.streams[s].start_addr == sdram_streams.end_addr) {
					sdram_streams.streams[s].start_addr = sdram_streams.start_addr;
			}
			sdram_streams.streams[s].stream_len = stream_len; 
			sdram_streams.streams[s].len = stream_pos;			
			sdram_streams.streams[s].pos = stream_pos;
			sdram_streams.streams[s].start_pos = stream_pos;
			sdram_streams.streams[s].stream_start_pos = stream_pos;
			
			sdram_streams.streams[s].entry = entry;
			//sdram_streams.streams[s].subq = 0;
			sdram_streams.streams[s].valid = 1;		
			sdram_streams.streams[s].open = 1;		
			sdram_streams.streams[s].user_data = 0;		
			
			if (sdram_streams.ptAddr == STREAM_INVALID_ADDR)
			{
				sdram_streams.ptAddr = sdram_streams.wAddr;
				
				sdram_streams.ptspace = sdram_streams.rAddr - sdram_streams.streams[s].start_addr;

				if (sdram_streams.ptspace <= 0l)
				{
					sdram_streams.ptspace += (sdram_streams.end_addr - sdram_streams.start_addr);
				}

			}
			
			//Interrupt_Enable;

			if (stream_pos == 0)
			{
				sdram_streams.streams[s].overwrite = FALSE;
			}
			else
			{
				sdram_streams.streams[s].overwrite = TRUE;
			}

			DBG_Printf ("Create Stream %d\n\r", s);

			return	s;	
	}
	else
	{
		sdram_streams.wStream = INVALID_STREAM_ID;
		
		DBG_Printf ("Failed to Create Stream\n\r");
		
		return INVALID_STREAM_ID;
	}
}

U32 app_sdram_stream_get_start_pos (STREAM_ID s)
{
	U32 len;
	int distance;

	DBG_Assert(s != INVALID_STREAM_ID);

	len = sdram_streams.streams[s].len - sdram_streams.streams[s].start_pos;
		
	if (len <= (sdram_streams.end_addr - sdram_streams.start_addr) )
	{
		//stream not overwrited
		return sdram_streams.streams[s].start_pos;
	}
	else
	{
		//stream overwrited
		distance = sdram_streams.rAddr - sdram_streams.wAddr;
		if (distance <= 0)
		{
			distance += (sdram_streams.end_addr - sdram_streams.start_addr);
		}

		return (sdram_streams.streams[s].pos - distance);
	}
}

//#pragma CODE_SECTION (app_sdram_stream_add_wstream, ".EXTERNAL_CODE"); 

void app_sdram_stream_add_wstream (STREAM_ID s, long stream_len)
{
	DBG_Assert(s != INVALID_STREAM_ID);
	DBG_Assert(s == sdram_streams.wStream);
			
	sdram_streams.streams[s].stream_len = stream_len + 1024l * (long) USER_DATA_SECTORS; 
	//sdram_streams.streams[s].len = stream_pos;			
	//sdram_streams.streams[s].pos = 1024l * (long)USER_DATA_SECTORS;
	//sdram_streams.streams[s].entry = entry;
	sdram_streams.streams[s].user_data = TRUE;
	//sdram_streams.streams[s].valid = 1;		
			
	//if (stream_pos == 0)
	//{
	//	sdram_streams.streams[s].overwrite = FALSE;
	//}
	//else
	//{
	//	sdram_streams.streams[s].overwrite = TRUE;
	//}

	return; 
}

void app_sdram_stream_wstream_append (STREAM_ID s, long len)
{

	DBG_Assert(s != INVALID_STREAM_ID);
	DBG_Assert(s == sdram_streams.wStream);
				
	sdram_streams.streams[s].stream_len += len; 
	//sdram_streams.streams[s].len = stream_pos;			
	//sdram_streams.streams[s].pos = 1024l * (long)USER_DATA_SECTORS;
	//sdram_streams.streams[s].entry = entry;
	//sdram_streams.streams[s].user_data = TRUE;
	//sdram_streams.streams[s].valid = 1;		
			
	//if (stream_pos == 0)
	//{
	//	sdram_streams.streams[s].overwrite = FALSE;
	//}
	//else
	//{
	//	sdram_streams.streams[s].overwrite = TRUE;
	//}

	return; 
		

}

//#pragma CODE_SECTION (app_sdram_stream_set_len, ".EXTERNAL_CODE");

void app_sdram_stream_set_len (STREAM_ID s,long stream_len)
{
	DBG_Assert(s != INVALID_STREAM_ID);
	DBG_Assert(s == sdram_streams.wStream);

	sdram_streams.streams[s].stream_len = stream_len;
}

void app_sdram_stream_set_wlen (STREAM_ID s,long wlen)
{
	DBG_Assert(s != INVALID_STREAM_ID);
	DBG_Assert(s == sdram_streams.wStream);			//must be current write stream
		
	if (sdram_streams.streams[s].pos == 0 && wlen == 0)
	{
		return;
	}

	if (sdram_streams.streams[s].pos >= wlen)
	{
		DBG_Assert (FALSE);
		return;
	}
	
	/*if (sdram_streams.streams[s].len < wlen)
	{
		DBG_Assert (FALSE);
		return;
	}*/

	sdram_streams.wAddr -= (sdram_streams.streams[s].len - wlen);
	
	if (sdram_streams.wAddr < sdram_streams.start_addr)
	{
		sdram_streams.wAddr += (sdram_streams.end_addr - sdram_streams.start_addr);
	}
	
	DBG_Assert ( (sdram_streams.wAddr >= sdram_streams.start_addr) &&
		(sdram_streams.wAddr < sdram_streams.end_addr) );

	sdram_streams.streams[s].len = wlen;
}

//#pragma CODE_SECTION (app_sdram_stream_del_stream, ".EXTERNAL_CODE"); 

I16 app_sdram_stream_del_stream (I16 stream_id)
{

	if ( (stream_id == (I16) INVALID_STREAM_ID) )
	{
		DBG_Assert (FALSE);
		return FALSE;
	}

	if (sdram_streams.streams[stream_id].open == 0)
	{
		sdram_streams.streams[stream_id].valid = FALSE;
		
		//Interrupt_Disable;

		stream_id++;

		if (stream_id >= TOTAL_OF_STREAMS)
		{
			stream_id = 0;
		
		}

		if (sdram_streams.streams[stream_id].valid == TRUE)
		{
			sdram_streams.ptAddr = sdram_streams.streams[stream_id].start_addr;
			
			sdram_streams.ptspace = sdram_streams.streams[stream_id].start_addr - sdram_streams.wAddr;

			if (sdram_streams.ptspace <= 0)
			{
				sdram_streams.ptspace += (sdram_streams.end_addr - sdram_streams.start_addr);
			}
			
			sdram_streams.rAddr = sdram_streams.streams[stream_id].start_addr;
					
			sdram_streams.space = sdram_streams.rAddr - sdram_streams.wAddr;

			if (sdram_streams.space <= 0l)
			{
				sdram_streams.space += sdram_streams.end_addr - sdram_streams.start_addr;
			}
		}
		else
		{
			//sdram_streams.ptAddr = STREAM_INVALID_ADDR;
			sdram_streams.rAddr = sdram_streams.start_addr;
			sdram_streams.wAddr = sdram_streams.start_addr;
			sdram_streams.ptspace = sdram_streams.end_addr - sdram_streams.start_addr;
			sdram_streams.space = sdram_streams.end_addr - sdram_streams.start_addr;
		}
		
		//Interrupt_Enable;

		return TRUE;
	}
	else
	{
		DBG_Assert (FALSE);	
		return FALSE;								/*stream be opened*/
	}
}

//#pragma CODE_SECTION (app_sdram_stream_get_wstream, ".EXTERNAL_CODE"); 

STREAM_ID app_sdram_stream_get_wstream(void)
{
	return sdram_streams.wStream;
}

//#pragma CODE_SECTION (app_sdram_stream_get_rstream, ".EXTERNAL_CODE"); 

STREAM_ID app_sdram_stream_get_rstream(void)
{
	return sdram_streams.rStream;
} 

//#pragma CODE_SECTION (app_sdram_stream_get_entry, ".EXTERNAL_CODE"); 

I16 app_sdram_stream_get_entry(STREAM_ID s)
{
	return sdram_streams.streams[s].entry;
}

//#pragma CODE_SECTION (app_sdram_stream_open_rstream, ".EXTERNAL_CODE"); 

STREAM_ID app_sdram_stream_open_rstream(STREAM_ID stream_id)
{
	
	if (sdram_streams.streams[stream_id].valid)
	{
		sdram_streams.rStream = stream_id;
		//sdram_streams.streams[stream_id].cd_type = CD_UNKNOWN;

		sdram_streams.rAddr = sdram_streams.streams[stream_id].start_addr;
		//sdram_streams.streams[stream_id].open++;
		
		//sec_buf.size = 0;
		//sec_buf.addr = sdram_streams.streams[stream_id].pos;

		return stream_id;
	}
	else
	{
		return	INVALID_STREAM_ID; 
	}

}

//#pragma CODE_SECTION (app_sdram_stream_close_wstream, ".EXTERNAL_CODE"); 

I16 app_sdram_stream_close_wstream (STREAM_ID stream_id)
{
	//if (sdram_streams.streams[stream_id].open > 0)
	//	sdram_streams.streams[stream_id].open--;
	if ( (stream_id == INVALID_STREAM_ID) )
	{
		DBG_Assert (FALSE);
		return FALSE;
	}

	if (sdram_streams.streams[stream_id].stream_len == STREAM_LENTH_UNKNOWN)
	{
		sdram_streams.streams[stream_id].stream_len = 
			sdram_streams.streams[stream_id].len;
	}

	sdram_streams.wStream = INVALID_STREAM_ID;
	return TRUE;
	
	//return FALSE;
}

//#pragma CODE_SECTION (app_sdram_stream_get_stream_wlen, ".EXTERNAL_CODE"); 

long app_sdram_stream_get_stream_wlen (STREAM_ID stream_id)
{
	return sdram_streams.streams[stream_id].len;
}

//#pragma CODE_SECTION (app_sdram_stream_get_stream_len, ".EXTERNAL_CODE"); 

long app_sdram_stream_get_stream_len (STREAM_ID stream_id)
{
	return sdram_streams.streams[stream_id].stream_len;
}

long app_sdram_stream_get_stream_dlen (STREAM_ID stream_id) __attribute__ ((section (".internal_ram_1_text")));

long app_sdram_stream_get_stream_dlen (STREAM_ID stream_id)
{
	volatile long tmp;
	//unsigned int interrupt_save;

	TX_DISABLE;	
	tmp = sdram_streams.streams[stream_id].len - sdram_streams.streams[stream_id].pos;
	TX_RESTORE;

	return tmp;
}

//#pragma CODE_SECTION (app_sdram_stream_get_stream_pos, ".EXTERNAL_CODE"); 

long app_sdram_stream_get_stream_pos (STREAM_ID stream_id)
{
	return sdram_streams.streams[stream_id].pos;
}

//#pragma CODE_SECTION (app_sdram_stream_close_rstream, ".EXTERNAL_CODE"); 

I16 app_sdram_stream_close_rstream (STREAM_ID stream_id)
{
	sdram_streams.rStream = INVALID_STREAM_ID;

	/*sometimes, need to wake the decode process, ask decode process to quit*/
	//decode_wakeup ();
	
	if ( (stream_id == INVALID_STREAM_ID) )
	{
		DBG_Assert (FALSE);
		return FALSE;
	}
		
	if (sdram_streams.streams[stream_id].open > 0)
	{
		sdram_streams.streams[stream_id].open--;
		
		//sec_buf.size = 0;
		//sec_buf.addr = 0l;

		return TRUE;
	}
	return FALSE;
}

//#pragma CODE_SECTION (app_sdram_stream_get_overwrite_status, ".EXTERNAL_CODE"); 

I16 app_sdram_stream_get_overwrite_status (STREAM_ID s)
{
	return sdram_streams.streams[s].overwrite;
}

//#pragma CODE_SECTION (app_sdram_stream_get_userdata_status, ".EXTERNAL_CODE"); 

I16 app_sdram_stream_get_userdata_status (STREAM_ID s)
{
	return sdram_streams.streams[s].user_data;
}

//#pragma CODE_SECTION (app_sdram_stream_find_next_valid_stream, ".EXTERNAL_CODE"); 

STREAM_ID app_sdram_stream_find_next_valid_stream (STREAM_ID s)
{
	I16 i;
	for (i=0; i<TOTAL_OF_STREAMS; i++)
	{		
		s++;
		if (s >= TOTAL_OF_STREAMS)
		{
			s = 0;
		}
		
		if (sdram_streams.streams[s].valid == TRUE)
		{
			return s;
		}		
	}

	/*failed to find valid stream*/
	return INVALID_STREAM_ID;

}

//#pragma CODE_SECTION (app_sdram_streams_find_prev_stream, ".EXTERNAL_CODE"); 

STREAM_ID app_sdram_streams_find_prev_stream(STREAM_ID stream)
{
	return (U16) (-1);
}


void app_sdram_stream_set_valid_len (long len)
{
	 sdram_streams.valid_len = len;
}


I32 app_sdram_stream_read (STREAM_ID stream_id, U8 *buf, U32 len)
{
	I32 i, l;

	if ( (sdram_streams.streams[stream_id].pos + len) > 
		sdram_streams.streams[stream_id].len )
	{
		//for (i=0; i<len; i++)
		//{
		//	*(buf + i) = 0x0;						/*init*/
		//}

		if ( sdram_streams.streams[stream_id].len != 
			sdram_streams.streams[stream_id].stream_len )
		{
			/*buffer underflow*/
			return (I32) STREAM_UNDERFLOW;
		}

		len = sdram_streams.streams[stream_id].len - sdram_streams.streams[stream_id].pos;
		if (len == 0)
		{
			//asm("nop");
		}
	}

	if (len == 0)
	{
		//asm("nop");
		return 0;
	}

	if ( (sdram_streams.rAddr + len) >= sdram_streams.end_addr )
	{
		l = sdram_streams.end_addr - sdram_streams.rAddr;
		CFasm_memcpy ( (void*) buf, (void*) sdram_streams.rAddr, l);
		
		buf += l;
		sdram_streams.rAddr = sdram_streams.start_addr;		
		l = (len - l);

		if ( l > 0 )
		{
			CFasm_memcpy ( (void*) buf, (void*) sdram_streams.rAddr, l);
			sdram_streams.rAddr += l;
		}
	}
	else
	{
		CFasm_memcpy ( (void*) buf ,(void*) sdram_streams.rAddr, len);
		sdram_streams.rAddr += len;		
	}

	sdram_streams.streams[stream_id].pos += len;
	if (len == 0)
	{
		//asm("nop");
	}

	return len;
}

I32 app_sdram_stream_iphone_read (STREAM_ID stream_id, U8 *buf, I32 len)
{
	I32 i,l;
	TX_INTERRUPT_SAVE_AREA;

	if ( (sdram_streams.streams[stream_id].pos + len) > 
		sdram_streams.streams[stream_id].len )
	{
		//for (i=0; i<len; i++) {
		//		*(buf + i) = 0x0;						/*init*/
		//}
		
		if ( sdram_streams.streams[stream_id].len != 
			sdram_streams.streams[stream_id].stream_len )
		{	
			/*buffer under flow*/		
			return (I32) STREAM_UNDERFLOW;
		}

		len = sdram_streams.streams[stream_id].len - sdram_streams.streams[stream_id].pos;
	}

	if (len == 0) {
		return 0;
	}

	if ( (sdram_streams.rAddr + len) >=
		sdram_streams.end_addr )
	{
		l = sdram_streams.end_addr - sdram_streams.rAddr;
		CFasm_memcpy ( (void*) buf, (void*) sdram_streams.rAddr, l);

		buf += l;
		sdram_streams.rAddr = sdram_streams.start_addr;		
		l = (len - l);

		if ( l > 0 )
		{
			CFasm_memcpy ( (void*) buf, (void*) sdram_streams.rAddr, l);
			sdram_streams.rAddr += l;
		}
	}
	else
	{
		CFasm_memcpy ( (void*) buf ,(void*) sdram_streams.rAddr, len);
		sdram_streams.rAddr += len;
	}

	//sdram_streams.streams[stream_id].pos += len;
	TX_DISABLE;
	sdram_streams.streams[stream_id].len -= len;
	TX_RESTORE;

	return len;
}

I32 app_sdram_stream_ad_pcm_read (STREAM_ID stream_id, U8 *buf, I32 len)
{
	I32 i,l;
	TX_INTERRUPT_SAVE_AREA;

	if ( (sdram_streams.streams[stream_id].pos + len) > 
		sdram_streams.streams[stream_id].len )
	{
		//for (i=0; i<len; i++)
		//{
		//	*(buf + i) = 0x0;    /*init*/
		//}

		if ( sdram_streams.streams[stream_id].len != 
			sdram_streams.streams[stream_id].stream_len )
		{	
			/*buffer under flow*/		
			return (I32) STREAM_UNDERFLOW;
		}

		len = sdram_streams.streams[stream_id].len - sdram_streams.streams[stream_id].pos;
	}

	if (len == 0)
	{
		return 0;
	}

	if ( (sdram_streams.rAddr + len) >=
		sdram_streams.end_addr )
	{
		l = sdram_streams.end_addr - sdram_streams.rAddr;
		CFasm_memcpy ( (void*) buf, (void*) sdram_streams.rAddr, l);

		buf += l;
		sdram_streams.rAddr = sdram_streams.start_addr;		
		l = (len - l);

		if ( l > 0 )
		{
			CFasm_memcpy ( (void*) buf, (void*) sdram_streams.rAddr, l);
			sdram_streams.rAddr += l;
		}
	}
	else
	{
		CFasm_memcpy ( (void*) buf ,(void*) sdram_streams.rAddr, len);
		sdram_streams.rAddr += len;
	}

	//sdram_streams.streams[stream_id].pos += len;
	TX_DISABLE;
	sdram_streams.streams[stream_id].len -= len;
	TX_RESTORE;

	return len;
}

BOOL app_sdram_stream_enough_write (STREAM_ID stream_id, I16 len)
{	
	I32 l,i;
	STREAM_ID s = stream_id;
	BOOL stream_handle_full;

	DBG_Assert (sdram_streams.streams[s].valid);
	
	/*check start valid stream*/	
	stream_handle_full = TRUE;
	for (i=0; i < TOTAL_OF_STREAMS; i++)
	{
		if ( sdram_streams.streams[s].valid == FALSE ) 
		{
			s++;
			if (s >= TOTAL_OF_STREAMS) 
			{
				s = 0;
			}
			stream_handle_full = FALSE;
			break;
		}

		if (s == 0) 
		{
			s = TOTAL_OF_STREAMS - 1;
		}
		else
		{
			s--;
		}
	}

	if (stream_handle_full == TRUE)
	{
		s = stream_id + 1;
		if (s >= TOTAL_OF_STREAMS) 
		{
			s = 0;
		}
	}

	if ( s == stream_id ) 
	{
		/*only one valid stream, the current write stream*/
		if (sdram_streams.streams[s].overwrite == FALSE) 
		{
			l = sdram_streams.streams[s].start_addr - sdram_streams.wAddr;

			if ( l == 0 ) 
			{
				if ( sdram_streams.streams[s].len == sdram_streams.streams[s].start_pos ) 
				{
					l = (sdram_streams.end_addr - sdram_streams.start_addr);					
				}
			}
			else if ( l < 0 ) 
			{
				l += (sdram_streams.end_addr - sdram_streams.start_addr);
			}
		
			if ( l < len ) 
			{
				sdram_streams.streams[s].overwrite = TRUE;
			}
		}

		if (sdram_streams.streams[s].overwrite == TRUE) 
		{
			l = sdram_streams.rAddr - sdram_streams.wAddr;

			if ( l == 0 ) 
			{
				if ( sdram_streams.streams[s].len == sdram_streams.streams[s].pos ) 
				{
					l = (sdram_streams.end_addr - sdram_streams.start_addr);					
				}
			}
			
			else if ( l < 0 ) 
			{
				l += (sdram_streams.end_addr - sdram_streams.start_addr);
			}

			if (l < len) 
			{
				return FALSE;
			}
		}
	}

	else 
	{
		/*there are several valid streams in sdram buf*/
		if (sdram_streams.streams[s].overwrite == TRUE)
		{
			l = sdram_streams.rAddr - sdram_streams.wAddr;		
		
		}
		else
		{
			l = sdram_streams.streams[s].start_addr - sdram_streams.wAddr;
		
		}

		if (l < 0) 
		{
			l += (sdram_streams.end_addr - sdram_streams.start_addr);
		}

		if (l < len) 
		{
			/*buffer is over*/
			return FALSE;
		}
	}

	return TRUE;
	
}

I32 app_sdram_stream_write (STREAM_ID stream_id, U8 *buf, I32 len)
{
	I32 l,i;
	STREAM_ID s = stream_id;
	//U8 *p;
	U8 *saddr;
	BOOL stream_handle_full;

	DBG_Assert (sdram_streams.streams[s].valid);

	/*check the write len*/
	if (sdram_streams.streams[stream_id].stream_len != STREAM_LENTH_UNKNOWN) 
	{
		if ( (sdram_streams.streams[stream_id].len + len) >
			sdram_streams.streams[stream_id].stream_len ) 
		{
			len = (I16) (sdram_streams.streams[stream_id].stream_len -
			sdram_streams.streams[stream_id].len);
		}
	}
	
	/*check start valid stream*/
	stream_handle_full = TRUE;
	for (i=0; i < TOTAL_OF_STREAMS; i++)
	{
		if ( sdram_streams.streams[s].valid == FALSE ) 
		{
			s++;
			if (s >= TOTAL_OF_STREAMS) 
			{
				s = 0;
			}
			stream_handle_full = FALSE;
			break;
		}

		if (s == 0) 
		{
			s = TOTAL_OF_STREAMS - 1;
		}
		else
		{
			s--;
		}
	}

	if (stream_handle_full == TRUE)
	{
		s = stream_id + 1;
		if (s >= TOTAL_OF_STREAMS) 
		{
			s = 0;
		}
	}

	if ( s == stream_id ) 
	{
		/*only one valid stream, the current write stream*/
		if (sdram_streams.streams[s].overwrite == FALSE) 
		{
			l = sdram_streams.streams[s].start_addr - sdram_streams.wAddr;

			if ( l == 0 ) 
			{
				if ( sdram_streams.streams[s].len == sdram_streams.streams[s].start_pos ) 
				{
					l = (sdram_streams.end_addr - sdram_streams.start_addr);					
				}
			}
			else if ( l < 0 ) 
			{
				l += (sdram_streams.end_addr - sdram_streams.start_addr);
			}
		
			if ( l < len ) 
			{
				sdram_streams.streams[s].overwrite = TRUE;
			}
		}

		if (sdram_streams.streams[s].overwrite == TRUE) 
		{
			l = sdram_streams.rAddr - sdram_streams.wAddr;

			if ( l == 0 ) 
			{
				if ( sdram_streams.streams[s].len == sdram_streams.streams[s].pos ) 
				{
					l = (sdram_streams.end_addr - sdram_streams.start_addr);					
				}
			}
			
			else if ( l < 0 ) 
			{
				l += (sdram_streams.end_addr - sdram_streams.start_addr);
			}

			if (l < (len + WRITE_STREAM_MARGIN) ) 
			{
				return 0;
			}
		}
	}

	else 
	{
		/*there are several valid streams in sdram buf*/
		if (sdram_streams.streams[s].overwrite == TRUE)
		{
			l = sdram_streams.rAddr - sdram_streams.wAddr;			
		}
		else
		{
			l = sdram_streams.streams[s].start_addr - sdram_streams.wAddr;
		}

		if (l < 0) 
		{
			l += (sdram_streams.end_addr - sdram_streams.start_addr);
		}

		if (l < (len+WRITE_STREAM_MARGIN) ) 
		{
			/*buffer is over*/
			return 0;
		}
	}
	
	if ( (sdram_streams.wAddr + len) >= sdram_streams.end_addr ) 
	{
		l = sdram_streams.end_addr - sdram_streams.wAddr;
		CFasm_memcpy ( (void*) sdram_streams.wAddr, (void*) buf, l);

		/*saddr = (U8 *) sdram_streams.wAddr;
		for (i=0; i<l; i++)
		{
			*saddr++ = *buf++;
		}*/

		buf += l;
		sdram_streams.wAddr = sdram_streams.start_addr;		
		l = (len - l);

		if ( l > 0 ) 
		{
			CFasm_memcpy ( (void*) sdram_streams.wAddr, (void*) buf, l);
			
			/*saddr = (U8 *) sdram_streams.wAddr;
			for (i=0; i<l; i++) {
				*saddr++ = *buf++;
			}*/
			
			sdram_streams.wAddr += l;
		}
	}
	else 
	{
		CFasm_memcpy ( (void*) sdram_streams.wAddr, (void*) buf,  len);
		
		/*saddr = (U8 *) sdram_streams.wAddr;
		for (i=0; i<len; i++) {
			*saddr++ = *buf++;
		}*/
		
		sdram_streams.wAddr += len;
	}

	sdram_streams.streams[stream_id].len += len;
	
	return len;
	
}


void app_sdram_set_stream_start_pos (STREAM_ID s, long len)
{

	DBG_Assert ( sdram_streams.streams[s].valid );
	DBG_Assert ( sdram_streams.streams[s].len >= len);

	sdram_streams.streams[s].stream_start_pos = len;
}

long app_sdram_get_stream_start_pos (STREAM_ID s)
{
	DBG_Assert ( sdram_streams.streams[s].valid );
	return sdram_streams.streams[s].stream_start_pos;
}


//#pragma CODE_SECTION (app_sdram_stream_wstream_status, ".EXTERNAL_CODE"); 

I16 app_sdram_stream_wstream_status (void)
{
	if (sdram_streams.streams[sdram_streams.wStream].len <
		sdram_streams.streams[sdram_streams.wStream].stream_len)
	{
		return SDRAM_STREAM_WSTREAM_PROGRESS;
	}
	else
	{
		return SDRAM_STREAM_WSTREAM_END;
	}
}

//#pragma CODE_SECTION (app_sdram_stream_get_free_space, ".EXTERNAL_CODE"); 

long app_sdram_stream_get_free_space (void) 
{	
	I32 l,i;
	STREAM_ID s = sdram_streams.wStream;
	STREAM_ID wStream = sdram_streams.wStream;

	if ( wStream == INVALID_STREAM_ID)
	{
		/*no write stream*/
		return 0;
	}
	
	/*check start valid stream*/	
	for (i=0; i < TOTAL_OF_STREAMS; i++)
	{
		if ( sdram_streams.streams[s].valid == FALSE ) 
		{
			s++;
			if (s >= TOTAL_OF_STREAMS) 
			{
				s = 0;
			}
			break;
		}

		if (s == 0) 
		{
			s = TOTAL_OF_STREAMS - 1;
		}
		else
		{
			s--;
		}
	}

	if ( s == wStream ) 
	{
		/*only one valid stream, the current write stream*/
		l = sdram_streams.rAddr - sdram_streams.wAddr;

		if ( l == 0 ) 
		{
			if ( sdram_streams.streams[s].len == sdram_streams.streams[s].pos ) 
			{
				l = (sdram_streams.end_addr - sdram_streams.start_addr);					
			}
		}
			
		else if ( l < 0 ) 
		{
			l += (sdram_streams.end_addr - sdram_streams.start_addr);
		}

	}

	else 
	{
		//DBG_Assert (FALSE);

		/*there are several valid streams in sdram buf*/
		if (sdram_streams.streams[s].overwrite == TRUE)
		{
			l = sdram_streams.rAddr - sdram_streams.wAddr;
		}
		else
		{
			l = sdram_streams.streams[s].start_addr - sdram_streams.wAddr;
		}

		if (l < 0) 
		{
			l += (sdram_streams.end_addr - sdram_streams.start_addr);
		}

	}

	if (l >= WRITE_STREAM_MARGIN)
	{
		l -= WRITE_STREAM_MARGIN;
	}
	else
	{
		l = 0;
	}

	return l;
	
}

BOOL app_sdram_stream_pos_is_valid (STREAM_ID s, long pos)
{
	if (sdram_streams.streams[s].overwrite == TRUE)
	{
		if ( (pos >= sdram_streams.streams[s].start_pos) &&
			(pos < sdram_streams.streams[s].len) )
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}

	}
	else
	{
		if (pos > sdram_streams.streams[s].len)
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
}

//#pragma CODE_SECTION (app_sdram_stream_rstream_seek, ".EXTERNAL_CODE"); 

I16 app_sdram_stream_rstream_seek (STREAM_ID s, long pos)
{
//	long raddr,bufaddr;

	DBG_Assert (sdram_streams.streams[s].valid == TRUE);
	DBG_Assert (s == sdram_streams.rStream);
	DBG_Assert (pos >= 0l);

	if (pos < sdram_streams.streams[s].start_pos)
	{
		pos = sdram_streams.streams[s].start_pos;
	}

	if (sdram_streams.streams[s].pos == pos)
		return TRUE;
	
	sdram_streams.streams[s].pos = pos;

	sdram_streams.rAddr = sdram_streams.streams[s].start_addr + (pos - sdram_streams.streams[s].start_pos);

	while (sdram_streams.rAddr >= sdram_streams.end_addr)
	{
		sdram_streams.rAddr -= (sdram_streams.end_addr - sdram_streams.start_addr);
	}
	

#if 0	
	raddr = ((pos - sdram_streams.streams[s].start_pos) / 1024l);
    bufaddr =  ((pos) / 1024l);
	
	if (sec_buf.addr  !=  bufaddr*1024L)
	{
		sec_buf.addr = bufaddr*1024L;
		sec_buf.size = 0;
	
		sdram_streams.rAddr = sdram_streams.streams[s].start_addr + raddr * 1170l;

		while (sdram_streams.rAddr >= sdram_streams.end_addr)
		{
			sdram_streams.rAddr -= (sdram_streams.end_addr - sdram_streams.start_addr);
		}
	}
#endif

	return TRUE;
}



/*
	added for wma seek, sometimes wma decoder seek backward
*/

I16 app_sdram_stream_codec_seek (STREAM_ID s, long pos)
{
//	long raddr,bufaddr;
	TX_INTERRUPT_SAVE_AREA;
	U32 len;
	int ret;

	DBG_Assert (sdram_streams.streams[s].valid == TRUE);
	DBG_Assert (s == sdram_streams.rStream);
	DBG_Assert (pos >= 0l);
	DBG_Assert (pos <= sdram_streams.streams[s].stream_len);


	//added by Li Ping in July 26, 2011
	if (pos < sdram_streams.streams[s].start_pos)
	{
		return -1;
	}

		
	if (pos == sdram_streams.streams[s].pos) 
	{
		ret = 0;
	}
	else if (pos > sdram_streams.streams[s].pos) 
	{
		//seek forward

		if (pos > sdram_streams.streams[s].len)
		{
			//seek out of stream write len
			ret = -1;
		}
		else
		{
			sdram_streams.rAddr += (pos - sdram_streams.streams[s].pos);

			while (sdram_streams.rAddr >= sdram_streams.end_addr) 
			{
				sdram_streams.rAddr -= (sdram_streams.end_addr - sdram_streams.start_addr);
			}

			sdram_streams.streams[s].pos = pos;

			ret = 0;
		}
	}
	else 
	{
		//seek backward 
		//pos < sdram_streams.streams[s].pos
		
		TX_DISABLE;		//seek backward need to lock stream to prevent any write
				
		if (sdram_streams.rAddr == sdram_streams.wAddr) 
		{
			if (sdram_streams.streams[s].len == sdram_streams.streams[s].pos) 
			{
				//all stream data has been used, no new data
				len = (sdram_streams.end_addr - sdram_streams.start_addr);
			}
			else 
			{
				//no stream data has not been used, no space for new data
				len = 0;
			}
		}
		else if (sdram_streams.rAddr > sdram_streams.wAddr) 
		{
			len = sdram_streams.rAddr - sdram_streams.wAddr;
		}
		else 
		{
			//sdram_streams.rAddr < sdram_streams.wAddr
			len =  (sdram_streams.end_addr - sdram_streams.start_addr) - (sdram_streams.wAddr - sdram_streams.rAddr); 
		}


		if ( len >= (sdram_streams.streams[s].pos - pos) )
		{	
			sdram_streams.rAddr -= (sdram_streams.streams[s].pos - pos);

			while (sdram_streams.rAddr < sdram_streams.start_addr) 
			{
					sdram_streams.rAddr += (sdram_streams.end_addr - sdram_streams.start_addr);
			}

			sdram_streams.streams[s].pos = pos;

			ret = 0;
		}
		else {
			//seek backward out of reserved space
			ret = -1;
		}

		TX_RESTORE;
	}
	
	return ret;
}

U32 app_sdram_stream_get_rstream_len (void)
{
	DBG_Assert (sdram_streams.rStream != INVALID_STREAM_ID);
	return (sdram_streams.streams[sdram_streams.rStream].stream_len);
}


//#pragma CODE_SECTION (app_sdram_stream_wstream_seek, ".EXTERNAL_CODE"); 

void app_sdram_stream_wstream_seek (long wlen)
{
	long distance;

	DBG_Assert (sdram_streams.wStream != INVALID_STREAM_ID);

	distance = sdram_streams.streams[sdram_streams.wStream].len - wlen;

	//if (distance < 0l) {
	//	NOP; 
	//	return;
	//}
	
	DBG_Assert (distance >= 0l);
    
	sdram_streams.streams[sdram_streams.wStream].len = wlen;
	
	if (sdram_streams.streams[sdram_streams.wStream].pos >= wlen)
	{
		sdram_streams.streams[sdram_streams.wStream].pos = wlen;
	}
	
	sdram_streams.wAddr -= distance;

	if (sdram_streams.wAddr < sdram_streams.start_addr)
	{
		sdram_streams.wAddr += (sdram_streams.end_addr - sdram_streams.start_addr);
	}

}

//#pragma CODE_SECTION (app_sdram_stream_flush, ".EXTERNAL_CODE"); 

I16 app_sdram_stream_flush (void)
{
	I16 i;

	//sec_buf.ptr = sec_buf.buf;
	//sec_buf.size = 0;
	//sec_buf.addr = 0;

	sdram_streams.total_streams = 0;
	
	sdram_streams.rStream = INVALID_STREAM_ID;
	sdram_streams.wStream = INVALID_STREAM_ID;
	sdram_streams.wAddr = sdram_streams.start_addr;
//	sdram_streams.ptAddr = STREAM_INVALID_ADDR;
	sdram_streams.rAddr = sdram_streams.start_addr;
	
	sdram_streams.space = sdram_streams.end_addr - sdram_streams.start_addr;	
	sdram_streams.ptspace = sdram_streams.space;	

	for (i=0; i<TOTAL_OF_STREAMS; i++)
	{
		sdram_streams.streams[i].valid = FALSE;
		sdram_streams.streams[i].open = 0;
	}

	/*sometimes, need to wake the decode process, ask decode process to quit*/
	//decode_wakeup ();
	
	return TRUE;
}

//#pragma CODE_SECTION (app_sdram_stream_get_total_streams, ".EXTERNAL_CODE"); 

I16 app_sdram_stream_get_total_streams (void)
{
	return 1;
}

//#pragma CODE_SECTION (app_sdram_stream_get_curr_streams, ".EXTERNAL_CODE"); 

I16 app_sdram_stream_get_curr_streams (void)
{
	return 1;
}

//#pragma CODE_SECTION (app_sdram_streams_get_stream_entry, ".EXTERNAL_CODE"); 

I16 app_sdram_streams_get_stream_entry (STREAM_ID s)
{
	DBG_Assert ( (s != INVALID_STREAM_ID) );

	return sdram_streams.streams[s].entry;

}
#endif




/****************************************************************************************
 *					UDF Descriptor Write/READ											*
 ****************************************************************************************/


//c_entry len is 0xc80, c_subdir len is 0x12c

void app_sdram_disc_info_compress (void)
{
#if 0
	U16 i;
	U16	len;
	U8 *pByte;

	pCInfoStart = (U8 *) &c_disc_info;
	pCEntry = (C_ENTRY *) &c_disc_info;
	for (i=0; i<app_main_data.total_tracks; i++)
	{
		pCEntry[i].start_addr = disc_info.entry[i].start_addr;
		pCEntry[i].index = disc_info.entry[i].index;
		pCEntry[i].fragment = disc_info.entry[i].fragment;
		pCEntry[i].len = disc_info.entry[i].len;
		pCEntry[i].stream_type = disc_info.entry[i].stream_type;
		pCEntry[i].fdb_index = disc_info.entry[i].fdb_index;
	}

	pCSubdir = (C_SUBDIR *) &pCEntry[app_main_data.total_tracks];

	DBG_Assert ( ((U8 *) pCSubdir) < (pCInfoStart + MAX_C_DISC_INFO_SPACE));
	
	for (i=0; i<app_main_data.total_folders; i++)
	{
		pCSubdir[i].first_entry = disc_info.subdir[i].first_entry;	
		pCSubdir[i].location = disc_info.subdir[i].location;			
	}

	pFolderName = (U8 *) &pCSubdir[app_main_data.total_folders];
	DBG_Assert ( ((U8 *) pFolderName) <= (pCInfoStart + MAX_C_DISC_INFO_SPACE));

	pByte = pFolderName;

	for (i=0; i<app_main_data.total_folders; i++)
	{
		len = strlen ((BYTE *) disc_info.subdir[i].name);

		if ( (pByte + len + 1) >= (pCInfoStart + (MAX_C_DISC_INFO_SPACE-1) ) )
		{
			
			/*no enough space*/
			while (pByte <= (pCInfoStart + (MAX_C_DISC_INFO_SPACE -1)) )
			{
				*pByte++ = 0x0;
			}

			break;
		}
		else
		{
			strcpy (pByte, (BYTE *) disc_info.subdir[i].name);
			pByte += len + 1;
		}
	}

	pEntryName = pByte;

	DBG_Assert ( ((U8 *) pEntryName) <= (pCInfoStart + MAX_C_DISC_INFO_SPACE));

	for (i=0; i<app_main_data.total_tracks; i++)
	{
		len = strlen ((BYTE *) disc_info.entry[i].name);
		if ( (pByte + len + 1) >= (pCInfoStart + (MAX_C_DISC_INFO_SPACE-1) ) )
		{			
			/*no enough space*/
			while (pByte <= (pCInfoStart + (MAX_C_DISC_INFO_SPACE -1)) )
			{
				*pByte++ = 0x0;
			}

			break;
		}
		else
		{
			strcpy (pByte, (BYTE *) disc_info.entry[i].name);
			pByte += len + 1;
		}
	}

	wCInfoLen = pByte - pCInfoStart;

	DBG_Assert ( (wCInfoLen) <= (MAX_C_DISC_INFO_SPACE));
#endif
}

U8 * app_sdram_get_file_id3_title (void)
{
#if 0
	//U16 i;
	//U8 *pByte, *pName;

	//if (index >= app_main_data.total_tracks)
	//{
	//	return ((U8 *) STRING_NO_NAME);	
	//}

	_unicode_to_oem (id3_tag.title, disp_folder_name);

	return disp_folder_name;
#endif
}

U8 * app_sdram_get_file_id3_artist (void)
{
#if 0
	//	U16 i;
	//U8 *pByte, *pName;

	//if (index >= app_main_data.total_tracks)
	//{
	//	return ((U8 *) STRING_NO_NAME);	
	//}

	_unicode_to_oem (id3_tag.artist, disp_folder_name);

	return disp_folder_name;
#endif
}

U8 * app_sdram_get_folder_name (U16 folder_index)
{
#if 0
	U16 *pName;
	U8 len;
	U8 *pByte;
	
	{
#ifdef MULTI_LANGUAGE

#ifdef LCD_DISP_FOLDER_NAME_FOLDER_SKIP_IN_STOP
		if (app_main_data.playback_state == PLAYING_MODE_STOP)
		{
			U32 start_addr;
			U32 cluster;
			
#ifdef FILE_SYSTEM_ENABLE
			//folder_index = app_file_get_folder_of_entry(app_main_data.entry);
			//track_index = app_file_get_entry_in_folder(app_main_data.entry) + 1;		
			//folder_index = app_main_data.fold;

			app_file_search_folder (folder_info[folder_index].parent_cluster, 
				folder_info[folder_index].index, folder_name, MAX_DISP_FILE_FOLDER_NAME, &start_addr);

			//app_file_search_file (start_addr, track_index, file_name, MAX_DISP_FILE_FOLDER_NAME, &(app_main_data.playing_stream_type), &cluster, &len);
#endif
		}
#endif

		len = (U8) wstrlen ((const char *) folder_name);
		if (len > (MAX_DISP_LENTH-2))
		{
			len = MAX_DISP_LENTH - 2;
		}

		unicode_to_English_oem_n(folder_name, (U8 *)unicode_name_to_oem, len);
		return (U8 *) unicode_name_to_oem;
#else
		return (U8 *) folder_name;
#endif
	}

#else
	return (U8 *) folder_name;
#endif

#if 0
	U16 i;
	U8 *pByte, *pName;
	U16 match = 0;

	if (index >= app_main_data.total_folders)\
	{
		return ((U8 *) STRING_NO_NAME);	
	}

	_unicode_to_oem (disc_info.subdir[index].name, disp_folder_name);

	return disp_folder_name;
#endif

#if 0
	pByte = pFolderName;

	if ( pByte >= (pCInfoStart + MAX_C_DISC_INFO_SPACE - 1) ) {		/* ...x- */
		//no folder name
		return ((U8 *) STRING_NO_NAME);
	}	
	else if (index == 0) {
		return pByte;
	}

	pName = (U8 *) STRING_NO_NAME;

	while (1)
	{		
		if (pByte >= (pCInfoStart + MAX_C_DISC_INFO_SPACE - 2))
		{
			/* ...xx- */
			break;
		}
		
		if (*pByte++ == 0x0)
		{
			match++;
			if (match == index)
			{
				pName = pByte;
				break;
			}
		}
	}

	if (pByte >= (pCInfoStart + MAX_C_DISC_INFO_SPACE - 1))
	{
		/* ...x- */
		return ((U8 *) STRING_NO_NAME);	
	}
	else if (*pByte == 0x0)
	{
		return ((U8 *) STRING_NO_NAME);
	}
	else
	{
		return pByte;
	}
#endif
}


#if 0//def LCD_DISP_FOLDER_NAME_FOLDER_SKIP_IN_STOP
U8 * app_sdram_get_track_index_folder_name (U16 track_index)
{
	U16 *pName;
	U8 len;
	U8 *pByte;
	U16 folder_index;
	
	{
#ifdef MULTI_LANGUAGE
		if (app_main_data.playback_mode == PLAYING_MODE_STOP)
		{
			U32 start_addr;
			U32 cluster;
			
#ifdef FILE_SYSTEM_ENABLE
			folder_index = app_file_get_folder_of_entry(track_index);
			//track_index = app_file_get_entry_in_folder(track_index) + 1;

			app_file_search_folder (folder_info[folder_index].parent_cluster, 
				folder_info[folder_index].index, folder_name, MAX_DISP_FILE_FOLDER_NAME, &start_addr);

			//app_file_search_file (start_addr, track_index, file_name, MAX_DISP_FILE_FOLDER_NAME, &(app_main_data.playing_stream_type), &cluster, &len);
#endif
		}

		len = (U8) wstrlen ((const char *) folder_name);
		if (len > (MAX_DISP_LENTH-2))
		{
			len = MAX_DISP_LENTH - 2;
		}

		unicode_to_English_oem_n(folder_name, (U8 *)unicode_name_to_oem, len);
		return (U8 *) unicode_name_to_oem;
#else
		return (U8 *) folder_name;
#endif
	}
}
#endif


U16 * app_sdram_get_cpy_file_name (U16 index)
{
#if 0
	U16 i;
#ifdef MULTI_LANGUAGE
	U16	*pName;
#else
	U8	*pName;
#endif
	U16 len;

	if (index >= app_main_data.total_tracks)
	{
		DBG_Assert (FALSE);
		return ((U16 *) NULL);	
	}

	pName =	app_cd_get_entry_name(index);

#ifdef MULTI_LANGUAGE
	len = (U8) wstrlen (pName);
#else
	len = (U8) strlen ((const char *) pName);
#endif

	if (len > MAX_DISP_LENTH)
	{
		len = MAX_DISP_LENTH;
	}

	memset(song_information, 0, sizeof (song_information));

	for (i=0; i<len; i++)
	{
		song_information[i] = pName[i];
	}

	//if (iso9660_disc() == TRUE)
	//{
		//OEM code
	//	oem_to_unicode ( (U8 *) disc_info.entry[index].name, disp_copy_file_name);
	//}
	//else
	//{
	//	wstrcpy (song_information, pName);
	//}

	len = wstrlen (song_information);

	//if (song_information[len-4] != '.')
	if (1)
	{
		//no extntion
		//if (disc_info.entry[index].stream_type == STREAM_MP3)
		if (app_main_data.playing_stream_type == STREAM_MP3)
		{
			song_information[len] = '.';
			song_information[len+1] = 'm';
			song_information[len+2] = 'p';
			song_information[len+3] = '3';
		}
		else
		{
			song_information[len] = '.';
			song_information[len+1] = 'w';
			song_information[len+2] = 'm';
			song_information[len+3] = 'a';
		}

		song_information[len+4] = 0x0;
	}

	return song_information;
#endif
}

U8 *app_sdram_get_iPhone_id3_info (int index)
{
#ifdef LCD_DISP_IPHONE_ID3_MESSAGE
	U16 *pName;
	U8 len;
	U8 *pByte;

	switch (index)
	{
	case DISP_INFO_TITLE:
		memset(file_name, 0, MAX_TITLE_LEN*2);
		//CFasm_memcpy(file_name, iPhone_title_buf, MAX_TITLE_LEN);
		CFasm_memcpy(file_name, id3_tag.title, MAX_TITLE_LEN);
		break;

	case DISP_INFO_ARTIST:
		memset(file_name, 0, MAX_TITLE_LEN*2);
		//CFasm_memcpy(file_name, iPhone_artist_buf, MAX_TITLE_LEN);
		CFasm_memcpy(file_name, id3_tag.artist, MAX_TITLE_LEN);
		break;

	case DISP_INFO_ALBUM:
		memset(file_name, 0, MAX_TITLE_LEN*2);
		//CFasm_memcpy(file_name, iPhone_album_buf, MAX_TITLE_LEN);
		CFasm_memcpy(file_name, id3_tag.album, MAX_TITLE_LEN);
		break;
	}

	len = (U8) wstrlen ((const char *) file_name);
	if (len > (MAX_DISP_LENTH-2))
	{
		len = MAX_DISP_LENTH - 2;
	}

	unicode_to_English_oem_n(file_name, (U8 *)unicode_name_to_oem, len);
	return (U8 *) unicode_name_to_oem;
#endif
}

U8 *app_sdram_get_volume_lable_name(void)
{
	U16 *pName;
	U8 len;
	U8 *pByte;
	U8 temp;

	{
#if 0
		pName = app_usb_get_vlome_lable_name();
		
		//mp3/wma disc
		len = (U8) wstrlen ((const char *) pName);
		if (len > USB_VOLUME_LABLE_LEN)
		{
			len = USB_VOLUME_LABLE_LEN;
		}

		//unicode_to_English_oem_n(pName, (U8 *)&unicode_name_to_oem, len);
		memset(unicode_name_to_oem, 0, sizeof(unicode_name_to_oem));
		CFasm_memcpy(unicode_name_to_oem, pName, len);
		return (U8 *) unicode_name_to_oem;
#endif
	}
}

U8 *app_sdram_get_file_name (U16 track_index)
{
#if 1
	U16 *pName;
	U32 len;
	U8 *pByte;
	U8 temp;
	U16 folder_index;
	{
#ifdef MULTI_LANGUAGE

#if 1//def LCD_DISP_FILE_NAME_TRACK_SKIP_IN_STOP
		if (app_main_data.playback_state == PLAYING_MODE_STOP)
		{
			U32 start_addr;
			U32 cluster;
			U32 track_number;
			U16 cdate,ctime;

#ifdef FILE_SYSTEM_ENABLE
			folder_index = app_file_get_folder_of_entry(track_index);
			track_number = app_file_get_entry_in_folder(track_index) + 1;

			app_file_search_folder (folder_info[folder_index].parent_cluster,
				folder_info[folder_index].index, folder_name, MAX_DISP_FILE_FOLDER_NAME, &start_addr);

#ifndef NEW_FS_LIB
			app_file_search_file (start_addr, track_number, file_name, MAX_DISP_FILE_FOLDER_NAME, &(app_main_data.playing_stream_type), &cluster, &len);
#else
			app_file_search_file (start_addr,  track_number, file_name, MAX_DISP_FILE_FOLDER_NAME, &(app_main_data.playing_stream_type), 
				&cluster, &len, &cdate, &ctime);
#endif

#endif
		}
#endif

		len = (U8) wstrlen ((const char *) file_name);
		if (len > (MAX_DISP_LENTH-2))
		{
			len = MAX_DISP_LENTH - 2;
		}
		unicode_to_English_oem_n(file_name, (U8 *)unicode_name_to_oem, len);

		//DBG_Printf ("file name:%s\n\r", unicode_name_to_oem);

		return (U8 *) unicode_name_to_oem;
#else
		return (U8 *) file_name;
#endif
	}
#else
	return (U8 *) file_name;
#endif
	
#if 0
	U16 i;
	U8 *pByte, *pName;
	U16 match = 0;

	if (index >= app_main_data.total_tracks) {
		return ((U8 *) STRING_NO_NAME);	
	}

	_unicode_to_oem (disc_info.entry[index].name, disp_file_name);

	return disp_file_name;
#endif

#if 0
	pByte = pEntryName;

	if ( pByte >= (pCInfoStart + MAX_C_DISC_INFO_SPACE - 1) )
	{
		/* ...x- */
		//no folder name
		return ((U8 *) STRING_NO_NAME);
	}	
	else if (index == 0)
	{
		return pByte;
	}

	pName = (U8 *) STRING_NO_NAME;

	while (1)
	{
		if (pByte >= (pCInfoStart + MAX_C_DISC_INFO_SPACE - 2))
		{
			/* ...xx- */
			break;
		}
		
		if (*pByte++ == 0x0)
		{
			match++;
			if (match == index)
			{
				pName = pByte;
				break;
			}
		}
	}

	if (pByte >= (pCInfoStart + MAX_C_DISC_INFO_SPACE - 1))
	{
		/* ...x- */
		return ((U8 *) STRING_NO_NAME);	
	}
	else if (*pByte == 0x0)
	{
		return ((U8 *) STRING_NO_NAME);
	}
	else
	{
		return pByte;
	}
#endif
}

void app_sdram_shift_c_disc_info (U16 decode_mode)
{
#if 0
	U16 i;
	U8* pName;

	switch (decode_mode)
	{
	case DECODE_MP3:
		if (pCInfoStart != MP3_C_DISC_INFO)
		{
			for (i=0; i<wCInfoLen; i++)
			{
				MP3_C_DISC_INFO[i] = pCInfoStart[i];
			}

			pCInfoStart = MP3_C_DISC_INFO;
			pCEntry = (C_ENTRY *) pCInfoStart;
			pCSubdir = (C_SUBDIR *) &pCEntry[app_main_data.total_tracks];
			pName = (U8 *) &pCSubdir[app_main_data.total_folders];

			pEntryName = pName + (pEntryName - pFolderName);
			pFolderName = pName;
		}
		break;

	case DECODE_WMA:
		if (pCInfoStart != WMA_C_DISC_INFO)
		{
			for (i=0; i<wCInfoLen; i++)
			{
				WMA_C_DISC_INFO[i] = pCInfoStart[i];
			}

			pCInfoStart = WMA_C_DISC_INFO;
			pCEntry = (C_ENTRY *) pCInfoStart;
			pCSubdir = (C_SUBDIR *) &pCEntry[app_main_data.total_tracks];
			pName = (U8 *) &pCSubdir[app_main_data.total_folders];

			pEntryName = pName + (pEntryName - pFolderName);
			pFolderName = pName;
		}
		break;

	case DECODE_CDDA:
		if (pCInfoStart != CDDA_C_DISC_INFO)
		{
			for (i=0; i<wCInfoLen; i++)
			{
				CDDA_C_DISC_INFO[i] = pCInfoStart[i];	
			}

			pCInfoStart = CDDA_C_DISC_INFO;
			pCEntry = (C_ENTRY *) pCInfoStart;
			pCSubdir = (C_SUBDIR *) &pCEntry[app_main_data.total_tracks];
			pName = (U8 *) &pCSubdir[app_main_data.total_folders];

			pEntryName = pName + (pEntryName - pFolderName);
			pFolderName = pName;
		}
		break;

	default:
		DBG_Assert (FALSE);
		break;
	}
#endif
}


U16 app_sdram_buf_flush (U16 decode_mode)
{
	U16 retVal = FALSE;

#if 0
	if (sdram_streams.end_addr != (long) BUF_END)
	{
		retVal = TRUE;
	}
	else
	{
		switch (decode_mode)
		{
		case DECODE_MP3:
			if (pCInfoStart != MP3_C_DISC_INFO)
			{
				retVal = TRUE;
			}
		break;

		case DECODE_WMA:
			if (pCInfoStart != WMA_C_DISC_INFO)
			{
				retVal = TRUE;
			}
		break;

		case DECODE_CDDA:
			if (pCInfoStart != CDDA_C_DISC_INFO)
			{
				retVal = TRUE;
			}
		break;

		default:
			DBG_Assert (FALSE);
			retVal = TRUE;
			break;
		}
	}
#endif

	return retVal;
}

U32 app_sdram_get_stream_buf_len (void)
{
	//return (sdram_streams.end_addr - sdram_streams.start_addr);
}

/*added for delete function*/
BOOL app_sdram_delete_entry (U16 index)
{
#if 0
	U16 i,folder;
	U8 *pDelteFolderName, *pNextFolderName, *pLastFolderName, *p;
	U8 *pDelteEntryName, *pNextEntryName, *pLastEntryName;
	U16 num;

	if (index >= app_main_data.total_tracks) {
		DBG_Assert (FALSE);
		return FALSE;
	}

	DBG_Assert (app_main_data.total_tracks > 0);

	folder = app_file_get_folder_of_entry (index);

	pDelteFolderName = app_sdram_get_folder_name (folder);
	pDelteEntryName = app_sdram_get_file_name(index);

	if (index < (app_main_data.total_tracks-1) )
	{
		pNextEntryName = app_sdram_get_file_name(index+1);
		pLastEntryName = app_sdram_get_file_name(app_main_data.total_tracks - 1);
		pLastEntryName += strlen (pLastEntryName);
		pLastEntryName++;				/*including null*/
	}

	if (folder < (app_main_data.total_folders-1))
	{
		pNextFolderName = app_sdram_get_folder_name (index+1);
		pLastFolderName = app_sdram_get_folder_name (app_main_data.total_folders-1);
		pLastFolderName += strlen (pLastFolderName);
		pLastFolderName++;				/*including null*/
	}
	
	DBG_Assert (folder < app_main_data.total_folders);

	/*delete one entry*/
	if (index == (app_main_data.total_tracks-1) )
	{
		app_main_data.total_tracks--;
	}
	else
	{
		for (i=index; i<app_main_data.total_tracks-1; i++)
		{
			pCEntry[i] = pCEntry[i+1];
		}

		for (p=pNextEntryName; p<pLastEntryName; p++)
		{
			*pDelteEntryName++ = *p;			
		}

		app_main_data.total_tracks--;
	}
	
	/*adjust folder first entry*/
	for (i=0;i<app_main_data.total_folders;i++)
	{
		if (pCSubdir[i].first_entry > index)
		{
			pCSubdir[i].first_entry--;
		}	
	}

	if (app_main_data.total_folders == 1) {
	}
	else if (folder == app_main_data.total_folders - 1)
	{
		//last folder
		if (pCSubdir[folder].first_entry == pCSubdir[folder-1].first_entry)
		{
			/*no entry in last folder*/
			app_main_data.total_folders--;
		}
	}
	else
	{
		if (pCSubdir[folder].first_entry == pCSubdir[folder+1].first_entry)
		{
			/*no entry in this folder*/
			for (i=folder; i<app_main_data.total_folders - 1; i++)
			{
				pCSubdir[i] = pCSubdir[i+1];
			}
			
			app_main_data.total_folders--;
			
			for (p=pNextFolderName; p<pLastFolderName; p++)
			{
				*pDelteFolderName++ = *p;			
			}
		}
	}
#endif

}


/*added for delete function*/
BOOL app_sdram_delete_folder (U16 index)
{
#if 0
	U16 i,j;
	U8 *pDelteFolderName, *pNextFolderName, *pLastFolderName, *p;
	U8 *pDelteEntryName, *pNextEntryName, *pLastEntryName;
	U16 num, entries;

	pDelteFolderName = app_sdram_get_folder_name (index);
	pDelteEntryName = app_sdram_get_file_name(pCSubdir[index].first_entry);

	if (index < (app_main_data.total_folders-1) )
	{
		pNextFolderName = app_sdram_get_folder_name (index+1);
		pLastFolderName = app_sdram_get_folder_name (app_main_data.total_folders-1);
		pLastFolderName += strlen (pLastFolderName);
		pLastFolderName++;				/*including null*/
	
		pNextEntryName = app_sdram_get_file_name(pCSubdir[index+1].first_entry);
		pLastEntryName = app_sdram_get_file_name(app_main_data.total_tracks - 1);
		pLastEntryName += strlen (pLastEntryName);
		pLastEntryName++;				/*including null*/
	}

	if (index >= app_main_data.total_folders)
	{
		DBG_Assert (FALSE);
		return FALSE;
	}

	DBG_Assert (app_main_data.total_tracks > 0);

	if (index == app_main_data.total_folders - 1)
	{
		/*last folder*/
		app_main_data.total_folders--;
		app_main_data.total_tracks = app_main_data.total_tracks - 
			(app_main_data.total_tracks - pCSubdir[index].first_entry);
	}
	else
	{
		/*delete folder entry*/	
		num = app_main_data.total_tracks - (pCSubdir[index+1].first_entry - pCSubdir[index].first_entry);

		j = 0;
		for (i=pCSubdir[index+1].first_entry; i<app_main_data.total_tracks; i++)
		{
			pCEntry[pCSubdir[index].first_entry+j++] = pCEntry[i];
		}

		entries = pCSubdir[index+1].first_entry - pCSubdir[index].first_entry;

		for (i=index; i<app_main_data.total_folders - 1; i++)
		{
			pCSubdir[i] = pCSubdir[i+1];
			pCSubdir[i].first_entry -= entries;
		}

		app_main_data.total_tracks = num;
		
		app_main_data.total_folders--;

		for (p=pNextFolderName; p<pLastFolderName; p++)
		{
			*pDelteFolderName++ = *p;			
		}

		for (p=pNextEntryName; p<pLastEntryName; p++)
		{
			*pDelteEntryName++ = *p;			
		}
	}
#endif
}

U8 app_sdram_stream_type_get(U16 entry_index)
{
#if 0
	U32 temp = entry_index;
	U8 ret = STREAM_UNKNOWN;

	if (pCD_entry[temp].stream_type == STREAM_MP3)
	{
		ret = STREAM_MP3;
	}
	else if (pCD_entry[temp].stream_type == STREAM_WMA)
	{
		ret = STREAM_WMA;
	}

	return ret;
#endif
}
