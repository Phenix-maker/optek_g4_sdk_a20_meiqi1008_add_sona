#ifndef __XA_GLOBAL_VARIBLE_H__
#define __XA_GLOBAL_VARIBLE_H__

#include "oem.h"


#if (defined ATMOS_CODEC_ENABLE)

#define MAX_MEM_ALLOCS        100

#define XA_MAX_ARGS           16
#define MAX_CMD_LINE_LENGTH   32

#else

#define MAX_MEM_ALLOCS        50

#define XA_MAX_ARGS           20
#define MAX_CMD_LINE_LENGTH   64

#endif


#define XA_MAX_CMD_LINE_LENGTH    MAX_CMD_LINE_LENGTH


extern signed short int g_w_malloc_count;
extern unsigned char xa_ui_chanmap[9];
extern unsigned char xa_ui_chan_number;


#endif //__XA_GLOBAL_VARIBLE_H__
