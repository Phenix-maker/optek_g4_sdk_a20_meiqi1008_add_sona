#include "os_config.h"

#include "sdk_com.h"

#include "bt_def.h"
#include "bluetooth.h"
#include "hci.h"
#include "hci_core.h"
#include "app_bt_tx.h"
#include "ht_hci_main.h"
#include "bt_lib_mask.h"
#include "bt_common.h"

#include "btdm_task.h"
#include "interrupt.h"
#include "hw_timer.h"
#include "hal_uart.h"
#include "hw_timer.h"
#include "fifo.h"
#include "audio_com.h"
#include "app_source.h"
#include "audio_codec.h"

#ifdef BT_HFP_ENABLE

void uiBtHCI_hfp_iphone_batt(u8 batt);//bt stack function for update batt

/*****************************/
extern int hfp_mic_in_1st_chk;
extern int hfp_mic_stream_wlen;

u8 hfp_calling, hfp_ring;

enumSOURCE_ID last_source_id;

void BtHCI_sco_conn(void)
{
	APP_SOURCE_STRU* pSrcFunction;

    last_source_id = global_vari.audio_source;

	pSrcFunction = get_source_entry_by_source_id(SOURCE_HFP);

	if (pSrcFunction != NULL)
	{
		pSrcFunction->switch_to_source();
	}
	else
	{
		printf_warn("hfp source entry not found\n");
	}    
}

void BtHCI_sco_disconn(void)
{
	APP_SOURCE_STRU* pSrcFunction;

	pSrcFunction = get_source_entry_by_source_id(last_source_id);

	if (pSrcFunction != NULL)
	{
		pSrcFunction->switch_to_source();
	}
	else
	{
		printf_warn("source id:%d entry not found\n", last_source_id);
	}
}

void BtHCI_hfp_init(void)
{
	hfp_calling = 0;
	hfp_ring = 0;
}

bool BtHCI_hfp_is_calling(void)
{
	return 	(hfp_calling || hfp_ring);
}

void BtHCI_HFP_call(u8 call)
{
	if (call == 1) {
		hfp_calling = 0x01;

        printf_info("enter calling\n");
	}
	else {
		hfp_calling = 0x0;

        printf_info("exit calling\n");        
	}

	hfp_ring = 0x0;
}

void BtHCI_HFP_callsetup(u8 callsetup)
{
	if (callsetup == 1) {
		hfp_ring = 0x01;
        printf_info("enter ring\n");
	}
	else {
		hfp_ring = 0x0;
        printf_info("exit ring\n");
	}
}

void BtHCI_HFP_callheld(u8 callheld)
{
	printf_info("enter call hold\n");
}

void BtHCI_ring(void)
{
    printf_info("call ring\n");
}

void BtHCI_clip(u8 *s)
{
	printf_info("hfp clip\n");
}

u8 hfp_iphone_batt_cb (void)
{
	return 7;			//0-9
}

u8 Bt_HCI_get_hfp_speaker_vol(void)
{
}

/*Callback from mobile phone, VGM value should be 0-15*/
void BtHCI_hfp_vgm(u8 v)
{
	DBG_Assert(FALSE);
}

/*Callback from mobile phone, VGS value should be 0-15*/
void BtHCI_hfp_vgs(u8 v)
{
#if 0	
	u8 vol;
	DBG_Printf ("VGS %d\n\r", v);

	if (v > 15)
		v = 15;

	vol = (v * VOLUME_MAX) / 15;
	#if 0
	if (vol > 0) {
		vol -= 1;
		audio_vari.volume = vol;
		app_cmd_vol_up(TRUE);	
	}
	else {
		vol = 1;
		audio_vari.volume = vol;
		app_cmd_vol_up(FALSE);	
	}
	#else
	audio_vari.volume = vol;

	#ifdef AUDIO_CODEC_USED_VOL
	AUDIOdevice.Set_Volume(audio_vari.volume);
	#endif
	#ifdef AUDIO_AMP_USED_VOL
	for (i=0;i<AMP_DEV_COUNT_MAX;i++)
	{
		AudioAmpDev[i].Set_Volume(audio_vari.volume);
	}
	#endif	
	#endif
#endif	
}
void BtHCI_phone_key (void)
{
	if (hfp_ring == 0x01) {
		//accept phone
		//DBG_Assert(FALSE);
		uiBtHCI_hfp_accept ();
	}
	else if (hfp_calling == 0x01) {
		//terminate calling
		//DBG_Assert(FALSE);
		uiBtHCI_hfp_term();
	}
}


void BtHCI_hfp_source_key (void)
{
/*
	if (hfp_ring == 0x01) {
		uiBtHCI_hfp_term();
		delayms(100);	
	}
	else if (hfp_calling == 0x01) {
		//terminate calling
		//DBG_Assert(FALSE);
		uiBtHCI_hfp_term();
		delayms(100);
	}
*/

	uiBtHCI_hfp_term();
	delayms(100);

	//app_nav_source_released ();
}

void app_nav_hfp_vol_up(void)
{
#if 0
	u8 vol;
	app_cmd_bt_vol_up(TRUE);
	
	vol = Bt_HCI_get_hfp_speaker_vol();
	uiBtHCI_hfp_vgs(vol);					//vol sync
#endif
}

void app_nav_hfp_vol_up_hold(void)
{
#if 0
	u8 vol;
	
	app_timer_key_hold_set_timer (300);
	app_cmd_bt_vol_up(TRUE);
	
	vol = Bt_HCI_get_hfp_speaker_vol();
	uiBtHCI_hfp_vgs(vol);					//vol sync
#endif
}

void app_nav_hfp_vol_down(void)
{
#if 0
	u8 vol;
	app_cmd_bt_vol_up(FALSE);
	
	vol = Bt_HCI_get_hfp_speaker_vol();
	uiBtHCI_hfp_vgs(vol);					//vol sync
#endif
}

void app_nav_hfp_vol_down_hold(void)
{
#if 0
	u8 vol;
	app_timer_key_hold_set_timer (300);
	app_cmd_bt_vol_up(FALSE);
	
	vol = Bt_HCI_get_hfp_speaker_vol();
	uiBtHCI_hfp_vgs(vol);					//vol sync
#endif
}

void app_dec_hfp_close_wstream (void)
{
	DBG_Printf("HFP close wstream\n\r");
}

void hfp_dsp_init(void)
{
	//hfpG711Handle = NULL;
	DBG_Printf("HFP DSP init\n\r");
}

void hfp_dsp_open(void)
{	
	DBG_Printf("HFP DSP open\n\r");
}

void hfp_dsp_fini(void)
{
	DBG_Printf("HFP DSP fini\n\r");
}

void hfp_dsp_close(void)
{
	DBG_Printf("HFP DSP close\n\r");
}

U16 hfp_size;
U32 app_dec_hfp_stream_write(U8 *ptr, U32 len)
{
	//printf_info("sco rev:%d\n", len);
	//return len;
#if 1
#if 0
	static U32 l_time;
	U32 ctime,wtime;

	ctime = read_ccount();

	if (ctime < l_time)
	{
		wtime = (ctime - l_time)/48;
		l_time = ctime;
		if (wtime > 10000 || wtime < 3000)
		{
			DBG_Printf("loss:%d\r\n",wtime);
			btdm_debug_pin(31, 0);
			btdm_debug_pin(31, 1); 		
		}
	}

	DBG_Assert(len == 60);
	//return len;
#endif

	U32 xlen;

	btdm_debug_pin(31, 0);
	btdm_debug_pin(31, 1); 

	if (hfp_get_codec() != 2)
	{
		printf_info("cvsd codec, study later! \n");
		return len;
	}	

	if (decode_type_get() != DECODE_MSBC && decode_type_get() != DECODE_HFP)
		return len;

	if (AUDIO_FIFO_STREAM_IS_VAILD() == FALSE)
	{
		return len;
	}

	xlen = (U32) AUDIO_FIFO_STREAM_PUT_DATA(ptr, len);
	if (xlen != len)
	{
		//asm ("break 1,1");
		DBG_Printf("hfp Stream overflow\n\r");
		//codec err, dead???

		//codec err, dead???
		//ukMsgSend(app_nav_bt_codec_reinit);
		//return;
	}

	xlen = (U32) AUDIO_FIFO_STREAM_DATA_LEN ();

	if (audio_env.playing_stream_status == STREAM_DEC_INIT || audio_env.playing_stream_status == STREAM_WAITING_DATA) 
	{
		if (xlen > len*4)
		{
			//app_dac_mute_disable();
			decode_pause_disable();
			sbc_decocde_start_w_init();
			audio_env.playing_stream_status = STREAM_MEDIA;
		}
	}

	return len;
#endif
}

U32 app_dec_hfp_mic_stream_read(U8 *ptr, U32 len)
{
	//printf_info("sco read:%d\n", len);
	//memset(ptr, 0x55, len);
	//return len;

	return hfp_msbc_encData_read(ptr, len);
#if 0	
#if 1
	static U8 seq_num = 0x08;
	u32 dlen,wlen;

#if 0
	if (hfp_upstream_fifo == NULL)
	{
		memset(ptr,0,len);
		return 0;
	}

	wlen = fifo_get_fifo_data_wlen(hfp_upstream_fifo);
	dlen = fifo_get_fifo_data_len(hfp_upstream_fifo);

	if (hfp_get_codec() != 2)
	{
		if (dlen >= len) 
		{
			dlen = fifo_get_data (hfp_upstream_fifo, ptr,len);
		}
		else
		{
			memset(ptr,0,len);
			dlen = 0;		
		}
				
		return dlen;
	}
#endif

	ptr[0] = 0x01;
	ptr[len-1] = 0;

	switch (seq_num)
	{
		case 0x08:
			seq_num = 0x38;
			break;
		case 0x38:
			seq_num = 0xc8;
			break;
		case 0xc8:
			seq_num = 0xf8;
			break;
		case 0xf8:
			seq_num = 0x08;
			break;
		default :
			seq_num = 0x08;
			break;
	}
	ptr[1] = seq_num;

	if (wlen <= 57*2)
	{
		memset(ptr,0,len);
		return 0;
	}
	else if (dlen >= 57) 
	{
		dlen = fifo_get_data (hfp_upstream_fifo, ptr+2,57);
	}
	else 
	{
		DBG_Printf ("MIC data underflow 1\n\r");
		memset(ptr,0,len);
		return 0;		
	}

	return len;		// 120 bytes
#else	
//	memset(ptr,0xa0,len);
//	CFasm_memcpy(ptr,p_msbc,len);
//	p_msbc += len;
//	return len;
	//DBG_Printf("hfp read len:%d\n",len);
	short *p = ptr;
	short data,i;
	if (index < 1)
	{
		//memset(ptr,-0x50,len);
		data = -0x3000;
	} 
	else if(index < 2)
	{
		//memset(ptr,0x40,len);
		data = 0x3000;
	}

	for (i=0;i<len/2;i++)
	{
		*p++ = data;
	}
	if (++index >= 2)
		index = 0;
	return len;
#endif
#endif	
}
#else
void BtHCI_sco_conn(void)
{
   
}

void BtHCI_sco_disconn(void)
{

}

void BtHCI_hfp_init(void)
{

}

bool BtHCI_hfp_is_calling(void)
{

}

void BtHCI_HFP_call(u8 call)
{

}

void BtHCI_HFP_callsetup(u8 callsetup)
{

}

void BtHCI_HFP_callheld(u8 callheld)
{

}

void BtHCI_ring(void)
{

}

void BtHCI_clip(u8 *s)
{

}

u8 hfp_iphone_batt_cb (void)
{

}

u8 Bt_HCI_get_hfp_speaker_vol(void)
{
}

/*Callback from mobile phone, VGM value should be 0-15*/
void BtHCI_hfp_vgm(u8 v)
{

}

/*Callback from mobile phone, VGS value should be 0-15*/
void BtHCI_hfp_vgs(u8 v)
{

}
void BtHCI_phone_key (void)
{

}


void BtHCI_hfp_source_key (void)
{

}

void app_nav_hfp_vol_up(void)
{


}

void app_nav_hfp_vol_up_hold(void)
{

}

void app_nav_hfp_vol_down(void)
{

}

void app_nav_hfp_vol_down_hold(void)
{

}

void app_dec_hfp_close_wstream (void)
{

}

void hfp_dsp_init(void)
{

}

void hfp_dsp_open(void)
{	
}

void hfp_dsp_fini(void)
{

}

void hfp_dsp_close(void)
{

}

U16 hfp_size;
U32 app_dec_hfp_stream_write(U8 *ptr, U32 len)
{

}

U32 app_dec_hfp_mic_stream_read(U8 *ptr, U32 len)
{

}
#endif
