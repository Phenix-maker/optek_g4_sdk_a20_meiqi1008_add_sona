#include "os_config.h"

#include "audio_task.h"
#include "app_window.h"
#include "app_source.h"
#ifdef DISPLAY_MODULE_ENABLE
#include "DisplayModule.h"
#endif

const APP_SOURCE_STRU *source_table_for_sw[] = 
{
	// &internal_source,
	#ifdef AUX_ENABLE
    &aux_source,
	#endif
	#ifdef HDMI_ENABLE 
    &hdmi_source,
	#endif
	#ifdef OPTICAL_ENABLE
    // &optical_source,
	#endif
	#ifdef COAXIAL_ENABLE
	&coaxial_source,
	#endif
	#ifdef ARC_ENABLE 
    &arc_source,
	#endif
	#ifdef USB_HOST_ENABLE
	&usb_msc_source,
	#endif
	#ifdef USB_SPEAKER
	&usb_speaker_source,
	#endif
	#if ((defined BTDM52_ENABLE)&&(!(defined BT_TX_ROLE_ENABLE)))
	&bt_source,	
	#endif
};

const APP_SOURCE_STRU *source_table[] = 
{
	// &internal_source,
	#ifdef AUX_ENABLE
    &aux_source,
	#endif
	#ifdef HDMI_ENABLE 
    &hdmi_source,
	#endif
	#ifdef OPTICAL_ENABLE 
    // &optical_source,
	#endif
	#ifdef COAXIAL_ENABLE
	&coaxial_source,
	#endif
	#ifdef ARC_ENABLE 
    &arc_source,
	#endif
	#ifdef USB_HOST_ENABLE
	&usb_msc_source,
	#endif
	#if (defined USB_SPEAKER)
	&usb_speaker_source,
	#endif
	#ifdef BTDM52_ENABLE
	&bt_source,
	#ifdef BT_HFP_ENABLE
	&hfp_source,
	#endif
	#endif
	#ifdef STANDBY_SOURCE_ENABLE
	&standby_source,
	#endif
	#ifdef OPL_SLAVE_ENBALE
	&opteklink_slave_source,
	#endif		

};

static void exit_cur_source(void)
{
	int i;
	int loop;

	loop = sizeof(source_table)/sizeof(source_table[0]);

	for (i=0; i<loop; i++)
	{
		if (global_vari.audio_source == source_table[i]->id)
		{
            source_table[i]->exit_from_source();
			break;
		}
	}
}

void enter_fix_source(U8 audio_source)
{
	int i;
	int loop;
	printf_info("%s\naudio_source : %d\n\r", __func__, audio_source);

	loop = sizeof(source_table)/sizeof(source_table[0]);

	for (i=0; i<loop; i++)
	{
		if (audio_source == source_table[i]->id)
		{
			global_vari.audio_source = SOURCE_UNKNOWN;
           	source_table[i]->switch_to_source();
			break;
		}
	}
}

APP_SOURCE_STRU* get_source_entry_by_source_id(enumSOURCE_ID src_id)
{
	int i;
	int loop;

	loop = sizeof(source_table)/sizeof(source_table[0]);

	for (i=0; i<loop; i++)
	{
		if (src_id == source_table[i]->id)
		{
            return (APP_SOURCE_STRU *)source_table[i];
		}
	}
	return NULL;
}

void app_source_finish(void)
{
    decode_exit_wait_forever_msgSend();
    exit_cur_source();	
}

void app_source_next_enter(void)
{
	int i;
	int loop;

	loop = sizeof(source_table_for_sw)/sizeof(source_table_for_sw[0]);
	printf_info("%d;%d;%d\n\r", loop, sizeof(source_table_for_sw), sizeof(source_table_for_sw[0]));

	if (loop > 0)
	{
		for (i=0; i<loop; i++)
		{
			if (global_vari.audio_source == source_table_for_sw[i]->id)
			{
				printf_info("%d;%s;%d\n\r", global_vari.audio_source, source_table_for_sw[i]->string, source_table_for_sw[i]->id);
				if (i<(loop-1))
				{
					printf_info("%d;%s;%d\n\r", global_vari.audio_source, source_table_for_sw[i+1]->string, source_table_for_sw[i+1]->id);
					source_table_for_sw[i+1]->switch_to_source();
				}
				else
				{
					printf_info("%d;%s;%d\n\r", global_vari.audio_source, source_table_for_sw[0]->string, source_table_for_sw[0]->id);
					source_table_for_sw[0]->switch_to_source();
				}
				break;
			}
		}		
	}
	else
	{
		source_table[0]->switch_to_source();
	}
}

U32 app_source_next_enter_for_keyfunction(KEY_HANDLE_PARAM_STRU param)
{
	int i;
	int loop;

    printf_info("key 0 up,combo tiemrs:%d, hold times:%d\r\n", param.combo_times, param.hold_times);

    if (param.combo_times == 0)
    {
		app_source_next_enter();
        printf_info("pass the key once\n\r");
    }
    else if (param.combo_times == 1)
    {
        printf_info("pass the key twice\n\r");
    }
    else if(param.combo_times == 2)
    {
        printf_info("pass the key thrice\n\r");    
    }
	return 0;
}
