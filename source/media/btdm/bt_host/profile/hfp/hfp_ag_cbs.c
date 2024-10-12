#include "os_config.h"

#include "c_def.h"
#include "debug.h"
#include "oem.h"

#include "string.h"
#include "at_command.h"

#include "bt_def.h"
#include "bt_lib_mask.h"
#include "hfp.h"
#include "app_bt_tx.h"
#include "hfp_hf.h"
#include "app_task.h"


//callbacks of hfg ag role, btdm task


/**
 ****************************************************************************************
 * @brief remove end support codec negotiation notify
 *
 * @param[in]  con    	    yes or no
 *
 ****************************************************************************************
 */
void hfp_ag_remove_codec_nego_notify(U8 con)
{
    printf_info("codec negotiation:%d\n", con);
}

/**
 ****************************************************************************************
 * @brief remove end support codec negotiation notify
 *
 * @param[in]  codec    	    HFP_CODEC_MSBC, HFP_CODEC_CVSD or others
 *
 ****************************************************************************************
 */
void hfp_ag_codec_confirmed_notify(U8 codec)
{
    printf_info("codec confirmed:%d\n", codec);

    if (codec == HFP_CODEC_MSBC)
    {
        hfp_audio_switch();//start sco conn
    }
}
