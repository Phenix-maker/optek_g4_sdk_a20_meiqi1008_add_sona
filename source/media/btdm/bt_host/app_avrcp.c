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

#include "audio_device.h"
#if 1 //def BT_VOLUME_SYC
// #define DBG_Puts  myprintf
u8 Bt_HCI_get_avrcp_speaker_vol(void)
{
	return ((audio_vari.volume * 127)/VOLUME_MAX);	/*avrcp vol 0=127*/
}

void app_nav_bt_avrcp_vol(void)
{
#ifdef BT_VOLUME_SET
	u8 vol;


	if ( (global_vari.audio_source != SOURCE_BT) && (bt_get_hp_status() == FALSE) )
	{
		return;
	}

	DBG_Puts("set vol to bt dev\n\r");
	//app_cmd_bt_vol_up(TRUE);

	vol = Bt_HCI_get_avrcp_speaker_vol();
	kBtHCI_avrcp_vol(vol);					//vol sync
#endif


}

void app_nav_bt_avrcp_vol_up(void)
{
#if 1	
	u8 vol;

	app_cmd_bt_vol_up(TRUE);
	
	vol = Bt_HCI_get_avrcp_speaker_vol();
	kBtHCI_avrcp_vol(vol);					//vol sync
#endif
}

void app_nav_avrcp_vol_up_hold(void)
{
#if 0	
	u8 vol;
	
	app_timer_key_hold_set_timer (300);
	app_cmd_bt_vol_up(TRUE);
	
	vol = Bt_HCI_get_avrcp_speaker_vol();
	kBtHCI_avrcp_vol(vol);					//vol sync
#endif
}

void app_nav_bt_avrcp_vol_down(void)
{
#if 1	
	u8 vol;
	app_cmd_bt_vol_up(FALSE);
	
	vol = Bt_HCI_get_avrcp_speaker_vol();
	kBtHCI_avrcp_vol(vol);					//vol sync
#endif
}

void app_nav_bt_avrcp_vol_down_hold(void)
{
#if 0
	u8 vol;
	app_timer_key_hold_set_timer (300);
	app_cmd_bt_vol_up(FALSE);
	
	vol = Bt_HCI_get_avrcp_speaker_vol();
	kBtHCI_avrcp_vol(vol);					//vol sync
#endif
}

#endif
