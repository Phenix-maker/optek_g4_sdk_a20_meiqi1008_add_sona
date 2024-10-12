#include "c_def.h"
#include "debug.h"
#include "oem.h"

#include "os_config.h"

#include "xa_global_variable.h"


#if 1//( (defined DOLBY_DDPLUS71_DEC_ENABLE) ||\
      (#define DOLBY_AC3_DEC_ENABLE) ||\
      (#define DTS_DMP_DEC_EANBLE) ||\
      (defined ATMOS_CODEC_ENABLE) )

signed short int g_w_malloc_count;
void *g_pv_arr_alloc_memory[MAX_MEM_ALLOCS];

unsigned char xa_ui_chanmap[9];
unsigned char xa_ui_chan_number;


#endif //(defined DOLBY_DDPLUS71_DEC_ENABLE) || (defined ATMOS_CODEC_ENABLE)
