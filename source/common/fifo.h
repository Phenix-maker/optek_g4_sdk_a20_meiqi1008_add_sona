#ifndef _FIFO_H_
#define _FIFO_H_

typedef struct {
	U8		*fifo_start_addr;
	U8		*fifo_end_addr;
	unsigned long	fifo_length;
	U8		*pRead;
	U8		*pWrite;
	unsigned long	rLen;
	unsigned long	wLen;	
	U8		wlen_H;

	unsigned long start_pos;			//start read write position in file
	unsigned long stream_len; 		//file len
	U16 entry;				//flie index
	U16 stream_margin;
	U8 valid;
	
} FIFO;

#define STREAM_LENTH_UNKNOWN    0xFFFFFFFF

extern FIFO pFifo;

void fifo_init ( FIFO *pFifo, U8 *pBuf,unsigned long len, int empty);
void fifo_init_w_data ( FIFO *pFifo, U8 *pBuf,unsigned long len,unsigned long dlen);
void fifo_create_file_stream ( FIFO *pFifo,unsigned long start_pos, unsigned long stream_len, U16 entry,U16 stream_margin);
I16 fifo_file_stream_flush ( FIFO *pFifo);

long fifo_get_free_space (FIFO *pFifo);
long fifo_get_data_len (FIFO *pFifo);
long fifo_put_data (FIFO *pFifo, U8 *pBuf,unsigned long len);
long fifo_get_data (FIFO *pFifo, U8 *pBuf,unsigned long len);
I8 fifo_file_stream_seek (FIFO *pFifo,unsigned long pos);
long fifo_get_OLA_data (FIFO *pFifo, U8 *pBuf, long rlen, long slen);
BOOL fifo_set_data_len (FIFO *pFifo, long len);

long fifo_put_data_one_byte (FIFO *pFifo, U8 *pBuf);

long fifo_get_fifo_4byte_data_len (FIFO *pFifo);
long fifo_get_4byte_data (FIFO *pFifo, U8 *pBuf, long len);
long fifo_get_4byte_ir_data (FIFO *pFifo, U16 *pBuf);

long fifo_get_fifo_data_wlen (FIFO *pFifo);
void fifo_discard_all_data (FIFO *pFifo);
long fifo_discard_data (FIFO *pFifo,unsigned long len);


#endif //_FIFO_H_
