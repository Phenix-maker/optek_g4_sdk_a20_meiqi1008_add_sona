//#include "os_config.h"

#include "c_def.h"
#include "debug.h"
#include "oem.h"

#ifdef OPTEK_ARC_CEC_ENABLE

#include "regmap.h"

#include "hw_timer.h"
#include "hw_cec.h"
#include "hal_gpio.h"

#include "interrupt.h"
#include "fifo.h"
#include "sdk_com.h"
#include "optek_cec.h"
#include "optek_cec_cmd.h"
#include "arc_process.h"

#include "audio_device.h"


//#define CEC_CMD_TX_WAITING_ENABLE

//#define ARC_CEC_OSD_NAME_CHECKOUT

#define MAX_MESSAGE_SIZE    16


//U8 cec_tx_buf_index;
U8 cec_tx_buf_len;
U8 cec_tx_err_cnt;
U8 cec_tx_request;

const U8 osd_name[] = {"DOLBY\0\0"};  

U8 cec_direct_addr;
U8 key_hold_counter;

volatile U8 cec_rx_byte_cnt_bak;


U8 cec_audio_volum_mute_status_get(void)
{
	U8 tmp;

	//vol bit0-bit6; max is 100
	//mute is bit 7; 0->mute off, 1 mute on
	tmp = (audio_vari.volume * 100)/VOLUME_MAX;

	#ifdef CEC_SYNC_STATUS_MUTE
	if(audio_vari.mute)
	{
		tmp |= 0x80;
	}
	#endif

	return tmp;
}

#ifdef CEC_CMD_TX_WAITING_ENABLE
U8 cec_cmd_tx_waiting (void)
{
	int i;

	DBG_Printf ("%s\n\r", __func__);

	for (i=0; i<200; i++)
	{
		if (cec_tx_request == 0)
		{
			//DBG_Printf ("cec_cmd_tx_waiting end\n\r");
			//DBG_Printf ("%s end\n\r", __func__);
			DBG_Printf ("%s end,counter=%d\n\r", __func__, i);
			return TRUE;
		}

		delayms(1);
	}

	//DBG_Printf ("cec_cmd_tx_waiting timeout\n\r");
	DBG_Printf ("%stimeout\n\r", __func__);
	return FALSE;
}
#endif //CEC_CMD_TX_WAITING_ENABLE

void cec_report_audio_volume_mute_status (void)
{
	DBG_Printf ("%s\n\r", __func__);
	#if ((defined CEC_SYNC_STATUS_MUTE)||(defined CEC_SYNC_STATUS_VOLUME))

	#ifdef CEC_CMD_TX_WAITING_ENABLE
	if (cec_cmd_tx_waiting() == FALSE)
	{
		return;
	}
	#endif

	cec_tx_buf[0] = (cec_logical_addr << 4) | ((cec_direct_addr & 0xF0) >> 4);
	cec_tx_buf[1] = CEC_OPCODE_REPORT_AUDIO_STATUS;
	cec_tx_buf[2] = cec_audio_volum_mute_status_get();
				
	cec_tx_buf_len = 3;
	cec_tx_err_cnt = 0;
	cec_tx_request = 1;
	hw_cec_tx(cec_tx_buf, cec_tx_buf_len);
	#endif
}


void opetk_cec_key_down (U8 key)
{
	DBG_Printf ("%s\n\r", __func__);

	if (global_vari.audio_source != SOURCE_ARC)
	{
		return;
	}
	
	key_hold_counter = FALSE;

	if (key == CEC_REMOTE_BUTTON_POWER)
	{
		#ifdef STANDBY_SOURCE_ENABLE
		if (app_source_get_standby_mode_status())
		{
			#ifdef CEC_SYNC_STATUS_RELEASE_POWER_STANBY
			app_source_convert_to_standby_release();
			#endif
		}
		else
		{
			#ifdef CEC_SYNC_STATUS_POWER_STANBY
			app_source_convert_to_standby_enter();
			#endif
		}
		#endif
	}
	else if (key == CEC_REMOTE_BUTTON_VOLUME_UP)
	{
		#ifdef CEC_SYNC_STATUS_VOLUME
		//update vol	
		#ifdef AUDIO_PROCESSOR
		if (audio_vari.mute)//mute status change//LG TV
		{
			AUDIO_DEV_CODEC_MUTE_OFF;
		}
		app_audio_device_volume_set(TRUE);
		#endif
		//#else
		//report vol
		//cec_report_audio_volume_mute_status();
		#endif
	}
	else if (key == CEC_REMOTE_BUTTON_VOLUME_DOWN)
	{
		#ifdef CEC_SYNC_STATUS_VOLUME
		//update vol
		app_audio_device_volume_set(FALSE);
		//#else
		//report vol
		//cec_report_audio_volume_mute_status();
		#endif
	}
	else if (key == CEC_REMOTE_BUTTON_MUTE)
	{	
		#ifdef AUDIO_PROCESSOR
		if(audio_vari.mute)
		{
			AUDIO_DEV_CODEC_MUTE_OFF;
		}
		else
		{
			AUDIO_DEV_CODEC_MUTE_ON;
		}
		#endif
	}
}

void optek_cec_key_up (u8 key)
{
	DBG_Printf ("%s\n\r", __func__);

	#ifdef OPTEK_CEC_CODE_CHANGED
	if (key == CEC_REMOTE_BUTTON_VOLUME_UP || key == CEC_REMOTE_BUTTON_VOLUME_DOWN || key == CEC_REMOTE_BUTTON_MUTE)
	#else
	if (key == CEC_REMOTE_BUTTON_VOLUME_UP || key == CEC_REMOTE_BUTTON_VOLUME_DOWN)
	#endif
	{
		//update vol

		//report vol
		//cec_report_audio_volume_mute_status();
	}
}


void cec_cmd_rx_err (U32 len)
{
	U32 i;
	U8 tmp;

	//DBG_Printf ("cec rx error\n\r");
	DBG_Printf ("cec rx error,len=%d\n\r", len);

	#if 0
	for (i=0; i<len; i++)
	{
		fifo_get_data(&cec_rx_fifo, &tmp, 1);
	}
	#endif
}

void cec_cmd_tx_err (U32 opcode)
{	
	//DBG_Printf ("cec tx error\n\r");
	DBG_Printf ("cec tx error,opcode=0x%x\n\r", opcode);
}

U8 cec_report_physical_address (void)
{
	DBG_Printf("%s\n\r", __func__);

	#ifdef CEC_CMD_TX_WAITING_ENABLE
	if (cec_cmd_tx_waiting() == FALSE)
	{
		return FALSE;
	}
	#endif

	if (cec_read_physcial_addr_status != CEC_READ_PHYSIC_ADDR_SUCCESS)
	{
		DBG_Printf ("cec_read_physcial_addr_status=%d,unfit,return\n\r", cec_read_physcial_addr_status);
		return FALSE;
	}

	if ( !((cec_physical_addr[0] == 0xFF) && (cec_physical_addr[1] == 0xFF)) )
	{
		#ifdef CEC_CMD_TX_WAITING_ENABLE
		if (cec_cmd_tx_waiting() == FALSE)
		{
			return FALSE;
		}
		#endif
	
		cec_tx_buf[0] = (cec_logical_addr << 4) | cec_logical_bc_addr;

		cec_tx_buf[1] = CEC_OPCODE_REPORT_PHYSICAL_ADDRESS;
		cec_tx_buf[2] = cec_physical_addr[0];
		cec_tx_buf[3] = cec_physical_addr[1];
		cec_tx_buf[4] = cec_logical_addr;
		
		cec_tx_buf_len = 5;
		cec_tx_err_cnt = 0;
		cec_tx_request = 1;
		hw_cec_tx(cec_tx_buf, cec_tx_buf_len);
		return TRUE;
	}

	return FALSE;
}

U8 cec_abort_feature_buf_filled (U8 *pBuf, U8 header, U8 opcode, U8 reason)
{
	DBG_Printf ("%s\n\r", __func__);

	//About feature
	pBuf[0] = (cec_logical_addr << 4) | ((header & 0xF0) >> 4);
	pBuf[1] = CEC_OPCODE_FEATURE_ABORT;
	pBuf[2] = opcode;
	pBuf[3] = reason;
	
	return 4;
}

U8 cec_set_osd_name(U8 *pBuf, U8 header)
{
	U8 len;

	DBG_Printf ("%s\n\r", __func__);

	//osd name
	pBuf[0] = (cec_logical_addr << 4) | ((header & 0xF0) >> 4);
	pBuf[1] = CEC_OPCODE_SET_OSD_NAME;

	strcpy(&pBuf[2], osd_name);
	len = strlen(osd_name) + 2;
	
	#ifdef ARC_CEC_OSD_NAME_CHECKOUT
	U8 i;
	for (i=0; i<len; i++)
	{
		if (i == 0)
		{
			DBG_Printf ("TxData:%x ", pBuf[i]);
		}
		else if (i == (len-1))
		{
			DBG_Printf ("%x\n\r", pBuf[i]);
		}
		else
		{
			DBG_Printf ("%x ", pBuf[i]);
		}
	}
	#endif //ARC_CEC_OSD_NAME_CHECKOUT
	
	return len;
}

U8 cec_set_version_num(U8 *pBuf, U8 header)
{
	U8 len;

	DBG_Printf ("%s\n\r", __func__);

	pBuf[0] = (cec_logical_addr << 4) | ((header & 0xF0) >> 4);
	pBuf[1] = CEC_OPCODE_CEC_VERSION;
	pBuf[2] = CEC_VERSION_1_4;
	len = 3;

	return len;
}

void cec_request_arc_termination(void)
{
	DBG_Printf("%s\n\r", __func__);

	#ifdef CEC_CMD_TX_WAITING_ENABLE
	if (cec_cmd_tx_waiting() == FALSE)
	{
		return;
	}
	#endif
	
	if (cec_read_physcial_addr_status != CEC_READ_PHYSIC_ADDR_SUCCESS)
	{
		DBG_Printf ("cec_read_physcial_addr_status=%d,unfit,return\n\r", cec_read_physcial_addr_status);
		return;
	}

	cec_tx_buf[0] = (cec_logical_addr << 4) | ((cec_direct_addr & 0xF0) >> 4);

	//Resonse ARC RX device sends a <Terminate ARC>
	cec_tx_buf[1] = CEC_OPCODE_REQUEST_ARC_TERMINATION;

	cec_tx_buf_len = 2;
	cec_tx_err_cnt = 0;
	cec_tx_request = 1;
	hw_cec_tx(cec_tx_buf, cec_tx_buf_len);
	
	#ifdef CEC_CMD_TX_WAITING_ENABLE
	if (cec_cmd_tx_waiting() == FALSE)
	{
		cec_terminate_flag = FALSE;
		return;
	}
	else
	{
		cec_terminate_flag = TRUE;
	}
	#endif
}

//The function below is for terminate TV at once.
U8 cec_set_arc_terminate(void)
{
	DBG_Printf("%s\n\r", __func__);

	#ifdef CEC_CMD_TX_WAITING_ENABLE
	if (cec_cmd_tx_waiting() == FALSE)
	{
		return FALSE;
	}
	#endif
	
	if (cec_read_physcial_addr_status != CEC_READ_PHYSIC_ADDR_SUCCESS)
	{
		DBG_Printf ("cec_read_physcial_addr_status=%d,unfit,return\n\r", cec_read_physcial_addr_status);
		return FALSE;
	}

	#if 1//def SONY_TV_KD_49X7500F
	cec_tx_buf[0] = (cec_logical_addr << 4) | cec_logical_bc_addr;
	#else
	cec_tx_buf[0] = (cec_logical_addr << 4) | ((cec_direct_addr & 0xF0) >> 4);
	#endif

	//Response:The ARC functionality in the ARC TX device is deactivated
	cec_tx_buf[1] = CEC_OPCODE_TERMINATE_ARC;

	cec_tx_buf_len = 2;
	cec_tx_err_cnt = 0;
	cec_tx_request = 1;
	hw_cec_tx(cec_tx_buf, cec_tx_buf_len);
	
	#ifdef CEC_CMD_TX_WAITING_ENABLE
	if (cec_cmd_tx_waiting() == FALSE)
	{
		cec_terminate_flag = FALSE;
	}
	else
	{
		cec_terminate_flag = TRUE;
	}
	#endif

	return TRUE;
}

void cec_report_power_on(void)
{
	DBG_Printf ("%s\n\r", __func__);
	
	#ifdef CEC_CMD_TX_WAITING_ENABLE
	if (cec_cmd_tx_waiting() == FALSE)
	{
		return;
	}
	#endif

	cec_tx_buf[0] = (cec_logical_addr << 4) | ((cec_direct_addr & 0xF0) >> 4);
	cec_tx_buf[1] = CEC_OPCODE_REPORT_POWER_STATUS;

	cec_tx_buf[2] = CEC_POWER_STATUS_ON;	
	
	cec_tx_buf_len = 3; 	
	cec_tx_err_cnt = 0;
	cec_tx_request = 1;
	hw_cec_tx(cec_tx_buf, cec_tx_buf_len);
}

void cec_report_power_standby(void)
{
	DBG_Printf ("%s\n\r", __func__);
	
	#ifdef CEC_CMD_TX_WAITING_ENABLE
	if (cec_cmd_tx_waiting() == FALSE)
	{
		return;
	}
	#endif

	cec_tx_buf[0] = (cec_logical_addr << 4) | ((cec_direct_addr & 0xF0) >> 4);
	cec_tx_buf[1] = CEC_OPCODE_REPORT_POWER_STATUS;

	cec_tx_buf[2] = CEC_POWER_STATUS_STANDBY;	
	
	cec_tx_buf_len = 3; 	
	cec_tx_err_cnt = 0;
	cec_tx_request = 1;
	hw_cec_tx(cec_tx_buf, cec_tx_buf_len);
}

void cec_report_power_on_to_power_standby(void)
{
	DBG_Printf ("%s\n\r", __func__);
	
	#ifdef CEC_CMD_TX_WAITING_ENABLE
	if (cec_cmd_tx_waiting() == FALSE)
	{
		return;
	}
	#endif

	cec_tx_buf[0] = (cec_logical_addr << 4) | ((cec_direct_addr & 0xF0) >> 4);
	cec_tx_buf[1] = CEC_OPCODE_REPORT_POWER_STATUS;

	cec_tx_buf[2] = CEC_POWER_STATUS_TRANSITION_ON_TO_STANDBY;	
	
	cec_tx_buf_len = 3; 	
	cec_tx_err_cnt = 0;
	cec_tx_request = 1;
	hw_cec_tx(cec_tx_buf, cec_tx_buf_len);
}

void cec_report_power_standby_to_power_on(void)
{
	DBG_Printf ("%s\n\r", __func__);
	
	#ifdef CEC_CMD_TX_WAITING_ENABLE
	if (cec_cmd_tx_waiting() == FALSE)
	{
		return;
	}
	#endif

	cec_tx_buf[0] = (cec_logical_addr << 4) | ((cec_direct_addr & 0xF0) >> 4);
	cec_tx_buf[1] = CEC_OPCODE_REPORT_POWER_STATUS;

	cec_tx_buf[2] = CEC_POWER_STATUS_TRANSITION_STANDBY_TO_ON;	
	
	cec_tx_buf_len = 3; 	
	cec_tx_err_cnt = 0;
	cec_tx_request = 1;
	hw_cec_tx(cec_tx_buf, cec_tx_buf_len);
}

void cec_report_device_verdor_id(void)
{
	DBG_Printf ("cec report device verdor id\n\r");

	#ifdef CEC_CMD_TX_WAITING_ENABLE
	if (cec_cmd_tx_waiting() == FALSE)
	{
		return;
	}
	#endif
	
	//Broadcast
	cec_tx_buf[0] = (cec_logical_addr << 4) | cec_logical_bc_addr;

	//Response:Any other interested device may store the Vendor ID of the device.
	//cec_tx_buf[1] = CEC_OPCODE_DEVICE_VENDOR_ID;
	cec_tx_buf[1] = CEC_OPCODE_REPORT_DEVICE_VENDOR_ID;

	cec_tx_buf[2] = 0x8; //Sharp
	cec_tx_buf[3] = 0x0;
	cec_tx_buf[4] = 0xF;
	
	cec_tx_buf_len = 5; 	
	cec_tx_err_cnt = 0;
	cec_tx_request = 1;
	hw_cec_tx(cec_tx_buf, cec_tx_buf_len);
}

#if 0
void cec_give_device_verdor_id(void)
{
	DBG_Printf ("cec give device verdor id\n\r");

	#ifdef CEC_CMD_TX_WAITING_ENABLE
	if (cec_cmd_tx_waiting() == FALSE)
	{
		return;
	}
	#endif
	
	//Directly addressed
	cec_tx_buf[0] = (cec_logical_addr << 4) | ((cec_direct_addr & 0xF0) >> 4);

	//Response:Device Vendor ID
	//cec_tx_buf[1] = CEC_OPCODE_GIVE_DEVICE_VENDOR_ID;
	cec_tx_buf[1] = CEC_OPCODE_REPORT_DEVICE_VENDOR_ID;
	
	cec_tx_buf[2] = 0x8; //Sharp
	cec_tx_buf[3] = 0x0;
	cec_tx_buf[4] = 0xF;
	
	cec_tx_buf_len = 5; 	
	cec_tx_err_cnt = 0;
	cec_tx_request = 1;
	hw_cec_tx(cec_tx_buf, cec_tx_buf_len);	
}
#endif

void cec_set_arc_initiate(void)
{
	DBG_Printf ("%s\n\r", __func__);

	#ifdef CEC_CMD_TX_WAITING_ENABLE
	if (cec_cmd_tx_waiting() == FALSE)
	{
		return;
	}
	#endif
	
	cec_tx_buf[0] = (cec_logical_addr << 4) | ((cec_direct_addr & 0xF0) >> 4);
	cec_tx_buf[1] = CEC_OPCODE_INITIATE_ARC;
	
	cec_tx_buf_len = 2;
	cec_tx_err_cnt = 0;
	cec_tx_request = 1;
	hw_cec_tx(cec_tx_buf, cec_tx_buf_len);

	#ifdef CEC_CMD_TX_WAITING_ENABLE
	if (cec_cmd_tx_waiting() == FALSE)
	{
		return;
	}
	#endif
}

void cec_request_arc_initiation(void)
{
	DBG_Printf ("%s\n\r", __func__);

	#ifdef CEC_CMD_TX_WAITING_ENABLE
	if (cec_cmd_tx_waiting() == FALSE)
	{
		return;
	}
	#endif
	
	cec_tx_buf[0] = (cec_logical_addr << 4) | ((cec_direct_addr & 0xF0) >> 4);
	cec_tx_buf[1] = CEC_OPCODE_REQUEST_ARC_INITIATION;
	
	cec_tx_buf_len = 2;
	cec_tx_err_cnt = 0;
	cec_tx_request = 1;
	hw_cec_tx(cec_tx_buf, cec_tx_buf_len);
}

void cec_set_system_audio_mode(CEC_AUDIO_MODE on_off, CEC_ADDRESS_SEL cec_addr_sel)
{
	DBG_Printf ("%s\n\r", __func__);

	#ifdef CEC_CMD_TX_WAITING_ENABLE
	if (cec_cmd_tx_waiting() == FALSE)
	{
		return;
	}
	#endif

	if (cec_read_physcial_addr_status != CEC_READ_PHYSIC_ADDR_SUCCESS)
	{
		DBG_Printf ("cec_read_physcial_addr_status=%d,unfit,return\n\r", cec_read_physcial_addr_status);
		return;
	}

	//The comand may select either Directly address or Broadcast. 	
	if (cec_addr_sel == enCEC_BROADCAST_ADDRESS)
	{
		cec_tx_buf[0] = (cec_logical_addr << 4) | cec_logical_bc_addr;
	}
	else
	{
		cec_tx_buf[0] = (cec_logical_addr << 4) | ((cec_direct_addr & 0xF0) >> 4);
	}

	cec_tx_buf[1] = CEC_OPCODE_SET_SYSTEM_AUDIO_MODE;

	//if set to 1(On), the TV muets its speakers.The TV sends relevant <User Control Pressed> or <User Control Released> as necessary.
	//if set to 0(Off), the TV unmuets its speakers.The TV stops sending the volume-related <User Control Pressed> or <User Control Released> messages
	cec_tx_buf[2] = on_off;

	cec_tx_buf_len = 3;
	cec_tx_err_cnt = 0;
	cec_tx_request = 1;
	hw_cec_tx(cec_tx_buf, cec_tx_buf_len);
}

void cec_give_device_power_status(void)
{
	DBG_Printf ("%s\n\r", __func__);

	#ifdef CEC_CMD_TX_WAITING_ENABLE
	if (cec_cmd_tx_waiting() == FALSE)
	{
		return;
	}
	#endif

	//Directly addressed
	cec_tx_buf[0] = (cec_logical_addr << 4) | ((cec_direct_addr & 0xF0) >> 4);

	cec_tx_buf[1] = CEC_OPCODE_GIVE_DEVICE_POWER_STATUS;
	
	cec_tx_buf_len = 2; 	
	cec_tx_err_cnt = 0;
	cec_tx_request = 1;
	hw_cec_tx(cec_tx_buf, cec_tx_buf_len);

	#ifdef CEC_CMD_TX_WAITING_ENABLE
	if (cec_cmd_tx_waiting() == FALSE)
	{
		return;
	}
	#endif
}

void cec_report_device_power_status(void)
{
	DBG_Printf ("%s\n\r", __func__);

	#ifdef CEC_CMD_TX_WAITING_ENABLE
	if (cec_cmd_tx_waiting() == FALSE)
	{
		return;
	}
	#endif
	
	cec_tx_buf[0] = (cec_logical_addr << 4) | ((cec_direct_addr & 0xF0) >> 4);
	cec_tx_buf[1] = CEC_OPCODE_REPORT_POWER_STATUS;

	#ifdef STANDBY_SOURCE_ENABLE
	if (app_source_get_standby_mode_status())
	{
		#ifdef CEC_SYNC_STATUS_POWER_STANBY
		cec_tx_buf[2] = CEC_POWER_STATUS_STANDBY;
		#endif
	}
	else
	{
		#ifdef CEC_SYNC_STATUS_POWER_ON
		cec_tx_buf[2] = CEC_POWER_STATUS_ON;
		#endif
	}
	#else
	cec_tx_buf[2] = CEC_POWER_STATUS_ON;
	//cec_tx_buf[2] = CEC_POWER_STATUS_STANDBY;
	#endif


	cec_tx_buf_len = 3;
	cec_tx_err_cnt = 0;
	cec_tx_request = 1;
	hw_cec_tx(cec_tx_buf, cec_tx_buf_len);

	#ifdef CEC_CMD_TX_WAITING_ENABLE
	if (cec_cmd_tx_waiting() == FALSE)
	{
		return;
	}
	#endif
}

void cec_cmd_connect_timeout(void)
{	
	DBG_Printf("%s\n\r", __func__);
}


void cec_cmd_rx_handle (U32 rx_data_len)
{
	U32 i;
	U8 cec_header;
	U8 opcode;
	U8 data_block[16];
	U8 tmp;
	U8 len = rx_data_len;
	static U8 ecc_power_status= 0xFF;

	#if 0
	if (len > MAX_MESSAGE_SIZE)
	{
		//from spec
		DBG_Assert (FALSE);

		//discard command
		for (i=0; i<len; i++)
		{
			fifo_get_data(&cec_rx_fifo, &tmp, 1);
			DBG_Printf ("0x%x ",tmp);
		}

		//abort feature
		return;
	}
	#endif

	if (!fifo_get_data_len(&cec_rx_fifo))
	{
		return;
	}

	#ifdef CEC_RX_ORIGINAL_DATA_CHECK
	DBG_Printf ("CEC rx:");

	for (i=0; i<len; i++)
	{
		fifo_get_data(&cec_rx_fifo, &tmp, 1);
		DBG_Printf ("0x%x ", tmp);
	}

	DBG_Printf ("\n\r");

	#else
	
	fifo_get_data(&cec_rx_fifo, &len, 1);
	fifo_get_data(&cec_rx_fifo, &cec_header, 1);
	DBG_Printf ("\n\rCEC Rx Data:0x%x ", cec_header);
	data_block[0] = cec_header;

	cec_direct_addr = cec_header;
	
	if (len > 1)
	{
		fifo_get_data(&cec_rx_fifo, &opcode, 1);
		DBG_Printf ("0x%x ", opcode);
		data_block[1] = opcode;

		fifo_get_data(&cec_rx_fifo, &data_block[2], (len-2));

		for (i=0; i<(len-2); i++)
		{
			if (i==len-2-1)
			{
				DBG_Printf ("0x%x", data_block[i]);
			}
			else
			{
				DBG_Printf ("0x%x ", data_block[i]);
			}
		}

		#if 1
		DBG_Printf (",len=%d\n\r", len);
		#else
		DBG_Printf (",len=%d\n\r", cec_rx_byte_cnt_bak);
		#endif

		if ( (cec_header & 0xF) == 0xF)
		{
			//braodcast, don't need action
			//return;
		}

		#ifdef CEC_CMD_TX_WAITING_ENABLE
		if (cec_cmd_tx_waiting() == FALSE)
		{
			return;
		}
		#endif

		//parse command
		switch (opcode)
		{
		case CEC_OPCODE_STANDBY:
			if ( (cec_header & 0xF) == 0xF)
			{
				DBG_Printf ("CEC All Devices Standby\n\r");
			}
			else
			{
				DBG_Printf ("CEC Some Device Standby\n\r");
			}

			app_timer_cec_reject_com_time_set(1000);

			if (global_vari.audio_source == SOURCE_ARC)
			{
				#if ((defined CEC_SYNC_STATUS_POWER_STANBY) && (defined STANDBY_SOURCE_ENABLE))
				app_source_convert_to_standby_enter();
				#endif
			}
			break;
			
		case CEC_OPCODE_FEATURE_ABORT:
			DBG_Printf ("CEC Abort Feature\n\r");
			break;
		case CEC_OPCODE_ROUTING_CHANGE:
			DBG_Printf ("CEC Routing Channel\n\r");
			break;

		case CEC_OPCODE_GIVE_PHYSICAL_ADDRESS:
			DBG_Printf ("CEC Give Physical Addr\n\r");
			
			#ifdef CEC_CMD_TX_WAITING_ENABLE
			if (cec_cmd_tx_waiting() == FALSE)
			{
				return;
			}
			#endif

			cec_tx_buf[0] = (cec_logical_addr << 4) | cec_logical_bc_addr;
			cec_tx_buf[1] = CEC_OPCODE_REPORT_PHYSICAL_ADDRESS;
			cec_tx_buf[2] = cec_physical_addr[0];
			cec_tx_buf[3] = cec_physical_addr[1];
			cec_tx_buf[4] = cec_logical_addr;

			cec_tx_buf_len = 5;
			cec_tx_err_cnt = 0;
			cec_tx_request = 1;
			hw_cec_tx(cec_tx_buf, cec_tx_buf_len);
			break;

		case CEC_OPCODE_GIVE_SYSTEM_AUDIO_MODE_STATUS:
			DBG_Printf ("CEC Give Audio Mode Status\n\r");
			
			#ifdef CEC_CMD_TX_WAITING_ENABLE
			if (cec_cmd_tx_waiting() == FALSE)
			{
				return;
			}
			#endif

			cec_tx_buf[0] = (cec_logical_addr << 4) | ((cec_header & 0xF0) >> 4);
			
			//Response: Report System Audio Mode Status
			//cec_tx_buf[1] = CEC_OPCODE_SYSTEM_AUDIO_MODE_STATUS;
			cec_tx_buf[1] = CEC_OPCODE_REPORT_SYSTEM_AUDIO_MODE_STATUS;
			
		    if (global_vari.audio_source != SOURCE_ARC)
			{
				cec_tx_buf[2] = enCEC_AUDIO_MODE_OFF;
			}
			else
			{
				cec_tx_buf[2] = enCEC_AUDIO_MODE_ON;
			}

			cec_tx_buf_len = 3;
			cec_tx_err_cnt = 0;
			cec_tx_request = 1;
			hw_cec_tx(cec_tx_buf, cec_tx_buf_len);

			#ifdef SAMSUNG_TV_UA43TU8000JXXZ
			#ifdef STANDBY_SOURCE_ENABLE
			if (app_source_get_standby_mode_status())
			{
				if (global_vari.audio_source != SOURCE_ARC)
				{
					global_vari.audio_source = SOURCE_ARC;
				}
				
				key_hold_counter = FALSE;
				app_source_convert_to_standby_release();
			}
			else
			{
				if (global_vari.audio_source != SOURCE_ARC)
				{
					//enter_fix_source(SOURCE_ARC);
				}
			}
			#endif
			#endif //SAMSUNG_TV_UA43TU8000JXXZ

			break;

		case CEC_OPCODE_REQUEST_ARC_INITIATION:
			DBG_Printf ("CEC Request ARC Init\n\r");
			
			#ifdef CEC_CMD_TX_WAITING_ENABLE
			if (cec_cmd_tx_waiting() == FALSE)
			{
				return;
			}
			#endif

			app_timer_cec_reject_com_time_set(1500);

			cec_tx_buf[0] = (cec_logical_addr << 4) | ((cec_header & 0xF0) >> 4);
			cec_tx_buf[1] = CEC_OPCODE_INITIATE_ARC;
			
			cec_tx_buf_len = 2;
			cec_tx_err_cnt = 0;
			cec_tx_request = 1;
			hw_cec_tx(cec_tx_buf, cec_tx_buf_len);
			break;

		case CEC_OPCODE_GIVE_OSD_NAME:
			DBG_Printf ("CEC Give OSD Name\n\r");
			
			#ifdef CEC_CMD_TX_WAITING_ENABLE
			if (cec_cmd_tx_waiting() == FALSE)
			{
				return;
			}
			#endif
			
			cec_tx_buf_len = cec_set_osd_name(cec_tx_buf, cec_header); 
			cec_tx_err_cnt = 0;
			cec_tx_request = 1;	
			hw_cec_tx(cec_tx_buf, cec_tx_buf_len);
			break;

		case CEC_OPCODE_GET_CEC_VERSION:
			DBG_Printf ("CEC OPCODE GET CEC VERSION\n\r");
			
			#ifdef CEC_CMD_TX_WAITING_ENABLE
			if (cec_cmd_tx_waiting() == FALSE)
			{
				return;
			}
			#endif
			
			cec_tx_buf_len = cec_set_version_num(cec_tx_buf, cec_header); 
			cec_tx_err_cnt = 0;
			cec_tx_request = 1;	
			hw_cec_tx(cec_tx_buf, cec_tx_buf_len);
			break;

		case CEC_OPCODE_GIVE_DEVICE_VENDOR_ID:
			DBG_Printf ("CEC Give Device Vendor ID\n\r");
			
			#ifdef CEC_CMD_TX_WAITING_ENABLE
			if (cec_cmd_tx_waiting() == FALSE)
			{
				return;
			}
			#endif
			
			//Broadcast
			cec_tx_buf[0] = (cec_logical_addr << 4) | cec_logical_bc_addr;
			
			//Response:Device Vendor ID
			//cec_tx_buf[1] = CEC_OPCODE_DEVICE_VENDOR_ID;
			cec_tx_buf[1] = CEC_OPCODE_REPORT_DEVICE_VENDOR_ID;

			cec_tx_buf[2] = 0x8; //Sharp
			cec_tx_buf[3] = 0x0;
			cec_tx_buf[4] = 0xF;
			
			cec_tx_buf_len = 5;
			cec_tx_err_cnt = 0;
			cec_tx_request = 1;
			hw_cec_tx(cec_tx_buf, cec_tx_buf_len);
			break;

		case CEC_OPCODE_REPORT_DEVICE_VENDOR_ID:
			DBG_Printf ("CEC Report Device Vendor ID\n\r");
			
			#ifdef CEC_CMD_TX_WAITING_ENABLE
			if (cec_cmd_tx_waiting() == FALSE)
			{
				return;
			}
			#endif
			
			//Broadcast
			cec_tx_buf[0] = (cec_logical_addr << 4) | cec_logical_bc_addr;

			//Response:Any other interested device may store the Vendor ID of the device.
			cec_tx_buf[1] = CEC_OPCODE_REPORT_DEVICE_VENDOR_ID;

			cec_tx_buf[2] = 0x8; //Sharp
			cec_tx_buf[3] = 0x0;
			cec_tx_buf[4] = 0xF;
			
			cec_tx_buf_len = 5;
			cec_tx_err_cnt = 0;
			cec_tx_request = 1;
			hw_cec_tx(cec_tx_buf, cec_tx_buf_len);
			break;
		
		case CEC_OPCODE_GIVE_DEVICE_POWER_STATUS:
			DBG_Printf ("CEC GIVE DEVICE POWER STATUS\n\r");
			
			#ifdef CEC_CMD_TX_WAITING_ENABLE
			if (cec_cmd_tx_waiting() == FALSE)
			{
				return;
			}
			#endif

			cec_tx_buf[0] = (cec_logical_addr << 4) | ((cec_header & 0xF0) >> 4);

			//Response:Report Power Status
			cec_tx_buf[1] = CEC_OPCODE_REPORT_POWER_STATUS;

			#ifdef STANDBY_SOURCE_ENABLE
			if (app_source_get_standby_mode_status())
			{
				#ifdef CEC_SYNC_STATUS_POWER_STANBY
				cec_tx_buf[2] = CEC_POWER_STATUS_STANDBY;
				#endif
			}
			else
			{
				#ifdef CEC_SYNC_STATUS_POWER_ON
				cec_tx_buf[2] = CEC_POWER_STATUS_ON;
				#endif
			}
			#else
			cec_tx_buf[2] = CEC_POWER_STATUS_ON;
			//cec_tx_buf[2] = CEC_POWER_STATUS_STANDBY;
			#endif

			cec_tx_buf_len = 3;
			cec_tx_err_cnt = 0;
			cec_tx_request = 1;
			hw_cec_tx(cec_tx_buf, cec_tx_buf_len);
			break;

		case CEC_OPCODE_REPORT_POWER_STATUS:
			//DBG_Printf("CEC Report Power Status\n\r");
			DBG_Printf("CEC Report Power Status=%d\n\r", data_block[2]);

			#ifdef CEC_CMD_TX_WAITING_ENABLE
			if (cec_cmd_tx_waiting() == FALSE)
			{
				return;
			}
			#endif

			if (ecc_power_status != data_block[2])
			{
				ecc_power_status = data_block[2];

				#ifdef SAMSUNG_TV_UA43TU8000JXXZ
				#ifdef STANDBY_SOURCE_ENABLE
				if (app_source_get_standby_mode_status())
				{
					if (!app_timer_cec_standby_time_get())
					{
						return;
					}

					if (data_block[2] == CEC_POWER_STATUS_ON)
					{
						if (global_vari.audio_source != SOURCE_ARC)
						{
							global_vari.audio_source = SOURCE_ARC;
						}

						app_timer_cec_standby_time_clr();					
						key_hold_counter = FALSE;
						app_source_convert_to_standby_release();
					}
				}
				#endif
				#endif //SAMSUNG_TV_UA43TU8000JXXZ
			}

			#if 0
			cec_tx_buf[0] = (cec_logical_addr << 4) | ((cec_header & 0xF0) >> 4);
			cec_tx_buf[1] = CEC_OPCODE_REPORT_POWER_STATUS;

			cec_tx_buf[2] = CEC_POWER_STATUS_ON;
			//cec_tx_buf[2] = CEC_POWER_STATUS_STANDBY;

			cec_tx_buf_len = 3;
			cec_tx_err_cnt = 0;
			cec_tx_request = 1;
			hw_cec_tx(cec_tx_buf, cec_tx_buf_len);
			#endif

			break;

		case CEC_OPCODE_SET_AUDIO_RATE:
			DBG_Printf ("CEC Set Audio Rate\n\r");
			break;

		case CEC_OPCODE_REPORT_ARC_TERMINATED:
			DBG_Printf ("CEC Report ARC Terminated\n\r");
			break;

		case CEC_OPCODE_REQUEST_ARC_TERMINATION:
			DBG_Printf ("CEC Request ARC Termination\n\r");
			
			#ifdef CEC_CMD_TX_WAITING_ENABLE
			if (cec_cmd_tx_waiting() == FALSE)
			{
				return;
			}
			#endif

			cec_tx_buf[0] = (cec_logical_addr << 4) | ((cec_header & 0xF0) >> 4);
			
			//Resonse ARC RX device sends a <Terminate ARC>
			cec_tx_buf[1] = CEC_OPCODE_REQUEST_ARC_TERMINATION;
			
			cec_tx_buf_len = 2; 	
			cec_tx_err_cnt = 0;
			cec_tx_request = 1;
			hw_cec_tx(cec_tx_buf, cec_tx_buf_len);
			break;

		case CEC_OPCODE_TERMINATE_ARC:
			DBG_Printf ("CEC Terminate ARC\n\r");
			break;

		case CEC_OPCODE_SET_SYSTEM_AUDIO_MODE:
			DBG_Printf ("CEC Set Audio Mode\n\r");
			break;


		case CEC_OPCODE_GIVE_AUDIO_STATUS:
			DBG_Printf ("CEC Give Audio Status\n\r");

			#ifdef CEC_CMD_TX_WAITING_ENABLE
			if (cec_cmd_tx_waiting() == FALSE)
			{
				return;
			}
			#endif

			cec_tx_buf[0] = (cec_logical_addr << 4) | ((cec_header & 0xF0) >> 4);
			
			//Response:Report Audio Status
			cec_tx_buf[1] = CEC_OPCODE_REPORT_AUDIO_STATUS;

			cec_tx_buf[2] = cec_audio_volum_mute_status_get();
			
			cec_tx_buf_len = 3;
			cec_tx_err_cnt = 0;
			cec_tx_request = 1;
			hw_cec_tx(cec_tx_buf, cec_tx_buf_len);

			#if 0 //Sony KDL soundbar & TV stanby -> remote power on soundbar, can't power on
			if(app_main_data.first_hpd_status)
			{
				cec_set_system_audio_mode(enCEC_AUDIO_MODE_ON, enCEC_DIRECT_ADDRESS); //send 0x72 0x01
			}
			#endif
			break;	

		case CEC_OPCODE_REQUEST_SHORT_AUDIO_DESC:
			DBG_Printf ("CEC Request Short Audio Desc\n\r");
			
			#ifdef CEC_CMD_TX_WAITING_ENABLE
			if (cec_cmd_tx_waiting() == FALSE)
			{
				return;
			}
			#endif

			#if 1
			cec_tx_buf[0] = (cec_logical_addr << 4) | ((cec_header & 0xF0) >> 4);
			cec_tx_buf[1] = CEC_OPCODE_REPORT_SHORT_AUDIO_DESC;
			cec_tx_buf[2] = 0x15;							
			cec_tx_buf[3] = 0x07;
			cec_tx_buf[4] = 0x50;
			
			cec_tx_buf_len = 5; 	
			cec_tx_err_cnt = 0;
			cec_tx_request = 1;
			#else
			cec_tx_buf_len = cec_abort_feature_buf_filled(cec_tx_buf, cec_header, opcode, CEC_ABORT_INVALID_OPRAND);
			cec_tx_err_cnt = 0;
			cec_tx_request = 1;
			#endif
			
			hw_cec_tx(cec_tx_buf, cec_tx_buf_len);
			break;			
			
		case CEC_OPCODE_REPORT_ARC_INITIATED:
			DBG_Printf ("CEC Report ARC Initiated\n\r");
			
			#ifdef CEC_CMD_TX_WAITING_ENABLE
			if (cec_cmd_tx_waiting() == FALSE)
			{
				return;
			}
			#endif

			app_timer_cec_reject_com_time_set(1500);

			cec_tx_buf[0] = (cec_logical_addr << 4) | ((cec_header & 0xF0) >> 4);

			//Response:Report Audio Status
			cec_tx_buf[1] = CEC_OPCODE_REPORT_AUDIO_STATUS;
			cec_tx_buf[2] = cec_audio_volum_mute_status_get();
			cec_tx_buf_len = 3;
			cec_tx_err_cnt = 0;
			cec_tx_request = 1;
			hw_cec_tx(cec_tx_buf, cec_tx_buf_len);

			cec_set_system_audio_mode(enCEC_AUDIO_MODE_ON, enCEC_DIRECT_ADDRESS); //send 0x72 0x01

			#if ((defined CEC_SYNC_STATUS_RELEASE_POWER_STANBY) && (defined STANDBY_SOURCE_ENABLE))
			if (app_source_get_standby_mode_status())
			{
				#if	0//def HISENSE_HZ32E35A
				asm("nop");
				#else
				//global_vari.audio_source = SOURCE_ARC;
				//key_hold_counter = FALSE;
				app_source_convert_to_standby_release();
				#endif
			}
			else
			{
				if (global_vari.audio_source != SOURCE_ARC)
				{
					enter_fix_source(SOURCE_ARC);
				}
			}
			#endif
			break;

		case CEC_OPCODE_USER_CONTROL_PRESSED:
			DBG_Printf ("CEC User Control Pressed=0x%x\n\r", data_block[2]);

			if (!app_timer_cec_reject_com_time_get())
			{
				#if 0
				if (app_source_get_standby_mode_status() == FALSE)
				#endif
				{
					opetk_cec_key_down (data_block[2]);
				}
			}
			break;
			
		case CEC_OPCODE_USER_CONTROL_RELEASED:
			DBG_Printf ("CEC User Control Released\n\r");

			if (!app_timer_cec_reject_com_time_get())
			{
				if(app_source_get_standby_mode_status() == FALSE)
				{
					optek_cec_key_up (data_block[2]);
				}
			}
			break;

		case CEC_OPCODE_SYSTEM_AUDIO_MODE_REQUEST:
			if (len > 2)
			{
				DBG_Printf ("CEC Audio Mode Request On\n\r");
			
				#ifdef CEC_CMD_TX_WAITING_ENABLE
				if (cec_cmd_tx_waiting() == FALSE)
				{
					return;
				}
				#endif

				cec_set_system_audio_mode(enCEC_AUDIO_MODE_ON, enCEC_DIRECT_ADDRESS); //send 0x72 0x01

				#ifdef STANDBY_SOURCE_ENABLE
				if (app_source_get_standby_mode_status())
				{
					#ifdef CEC_SYNC_STATUS_RELEASE_POWER_STANBY
					app_source_convert_to_standby_release();
					#endif
				}
				#endif				
			}
			else
			{
				DBG_Printf ("CEC Audio Mode Request Off\n\r");
			
				#ifdef CEC_CMD_TX_WAITING_ENABLE
				if (cec_cmd_tx_waiting() == FALSE)
				{
					return;
				}
				#endif
				
				cec_set_system_audio_mode(enCEC_AUDIO_MODE_OFF, enCEC_DIRECT_ADDRESS); //send 0x72 0x01

				#ifdef STANDBY_SOURCE_ENABLE
				if (!app_source_get_standby_mode_status())
				{
					#ifdef CEC_SYNC_STATUS_POWER_STANBY
					app_source_convert_to_standby_enter();
					#endif
				}
				#endif				
			}
			break;

		case CEC_OPCODE_VENDOR_COMMAND_WITH_ID:
			DBG_Printf ("CEC Vendor Command With ID\n\r");
			
			#if 0
			cec_tx_buf_len = cec_abort_feature_buf_filled(cec_tx_buf, cec_header, opcode, CEC_ABORT_N0T_IN_CORRECT_MOD);
			cec_tx_err_cnt = 0;
			cec_tx_request = 1;
			hw_cec_tx(cec_tx_buf, cec_tx_buf_len);
			break;
			#endif

		case CEC_OPCODE_ACTIVE_SOURCE:
			DBG_Printf ("CEC Active Source\n\r");
			break;

		case CEC_OPCODE_REPORT_PHYSICAL_ADDRESS:
			DBG_Printf ("CEC Report Physical Address\n\r");
			break;

		default:
			DBG_Printf ("CEC Abort Opcode=0x%x\n\r", opcode);
			
			#ifdef CEC_CMD_TX_WAITING_ENABLE
			if (cec_cmd_tx_waiting() == FALSE)
			{
				return;
			}
			#endif

			//DBG_Assert (FALSE);

			cec_tx_buf_len = cec_abort_feature_buf_filled(cec_tx_buf, cec_header, opcode, CEC_ABORT_N0T_IN_CORRECT_MOD);
			cec_tx_err_cnt = 0;
			cec_tx_request = 1;			
			hw_cec_tx(cec_tx_buf, cec_tx_buf_len);

			#if 0
			//About feature
			cec_tx_buf[0] = (cec_logical_addr << 4) | ((cec_header & 0xF0) >> 4);
			cec_tx_buf[1] = CEC_OPCODE_FEATURE_ABORT;
			cec_tx_buf[2] = opcode;
			cec_tx_buf[3] = CEC_ABORT_REFUSED;
			
			cec_tx_buf_len = 4; 	
			cec_tx_err_cnt = 0;
			cec_tx_request = 1;
			hw_cec_tx(cec_tx_buf, cec_tx_buf_len);
			#endif

			#if 0
			//Abort Message
			cec_tx_buf[0] = (cec_logical_addr << 4) | ((cec_header & 0xF0) >> 4);
			cec_tx_buf[1] = CEC_OPCODE_ABORT_MESSAGE;			
			cec_tx_buf_len = 2; 	
			cec_tx_err_cnt = 0;
			cec_tx_request = 1;
			hw_cec_tx(cec_tx_buf, cec_tx_buf_len);
			#endif
			
			break;
		}
	}
	#endif //CEC_RX_ORIGINAL_DATA_CHECK
}
#endif //ARC_ENABLE
