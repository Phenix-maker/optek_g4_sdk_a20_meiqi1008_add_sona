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

#ifdef BT_HFP_ENABLE

#define AG_NREC_MASK					0x0002
#define AG_VOICE_RECONGNITION_MASK		0x0004
#define AG_IN_BAND_RING_MASK			0x0008
#define AG_CODE_NEGOTIATION_MASK		0x0200					

void BtHCI_HFP_call(u8 call);
void BtHCI_HFP_callsetup(u8 callsetup);
void BtHCI_HFP_callheld(u8 callheld);
void BtHCI_ring(void);
void BtHCI_clip(u8 *s);
void BtHCI_hfp_vgm(u8 v);
void BtHCI_hfp_vgs(u8 v);
u8 Bt_HCI_get_hfp_speaker_vol(void);

u8 hfp_iphone_batt_cb (void);

#define APPLE_ACCESSORY

void hfp_hf_init(struct rfcomm_dlc *pDLC)
{
	u8 *at_buf;
	u8 len;
	u8 *pData;

	DBG_Printf("HFP hf init\n\r");

	at_buf = otk_malloc(MAX_AT_COMAND_BUF);

	if (at_buf == NULL)
	{
		DBG_Assert(FALSE);
		return;
	}

	pData = rfcomm_get_dlc_user_data(pDLC);
	DBG_Assert (pData != NULL);
	pData[HFP_STATE] = HFP_BRSF;
	pData[HFP_CIEV_CALL] = 0;
	pData[HFP_CIEV_CALLSETUP] = 0;
	pData[HFP_CIEV_CALLHELD] = 0;
	pData[HFP_FEAT_L] = 0;
	pData[HFP_FEAT_H] = 0;
    pData[HFP_CODEC]  = HFP_CODEC_CVSD;

	BtHCI_hfp_init();

	len = at_send_BRSF(at_buf);
	hfp_data_send(pDLC,at_buf,(u8) len);	
}


u8 * hfp_next_atc(u8 *buf, int len)
{
	int i;
	int tmp_1, tmp_2;

	tmp_1 = 0;

	for (i=0; i<len;i++) {
		if (buf[i] != 0x0D && buf[i] != 0x0A) {
			break;
		} 
	}

	tmp_1 = i;

	if (i<len) {
		len -= i;
	}
	else {
		len = 0;
	}

	for (i=0; i<len;i++) {
		if (buf[tmp_1+i] == 0x0D || buf[tmp_1+i] == 0x0A) {
			break;
		} 
	}

	tmp_2 = i;

	if (i<len) {
		len -= i;
	}
	else {
		len = 0;
	}

	for (i=0; i<len;i++) {
		if (buf[tmp_1+tmp_2+i] != 0x0D && buf[tmp_1+tmp_2+i] != 0x0A) {
			break;
		} 
	}


	return &buf[tmp_1+tmp_2+i];

}

void rfcomm_hfp_close (struct rfcomm_dlc *d);
void avctp_set_top_by_hfp(void);



extern struct rfcomm_dlc *hfp_rfcomm_dlc;

//#define HFP_CALL_CALLSETUP_ROLE_SWITCH

void hfp_multi_call(struct rfcomm_dlc *pDLC,u8 call)
{
	u8 *pData;
	u8 curr_call, curr_callsetup, curr_callheld;

	pData = rfcomm_get_dlc_user_data(pDLC);
	DBG_Assert (pData != NULL);

	curr_call = pData[HFP_CALL];
	curr_callsetup = pData[HFP_CALLSETUP];
	curr_callheld = pData[HFP_CALLHELD];

	DBG_Printf ("HFP Call %d\n\r", call);

	if (call == 0) {
		//call fini
		func_w_one_param_msgSend(BtHCI_HFP_call,0);

		//if (curr_callsetup == 0) {
		//if (curr_callsetup != 1) {			//microsoft phone, call ==1, callsetup =2
		if (1) {								//????
			//no call setup
			if (hfp_rfcomm_dlc == pDLC) {
#if 0//def HFP_CALL_CALLSETUP_ROLE_SWITCH
				rfcomm_hfp_call_setup_cb(pDLC);
#endif
				//hfp_rfcomm_dlc = NULL;
		//	DBG_Printf ("dlc2:0x%x\n\r",hfp_rfcomm_dlc);

			}
		}
	}
	else {
		//call in progress
		if (hfp_rfcomm_dlc == NULL || pDLC == hfp_rfcomm_dlc) {
			hfp_rfcomm_dlc = pDLC;
			
#if 0//def HFP_CALL_CALLSETUP_ROLE_SWITCH
			rfcomm_hfp_call(pDLC);
#endif
			func_w_one_param_msgSend(BtHCI_HFP_call,1);

			//avctp_set_top_by_hfp();
			//DBG_Assert (FALSE);
		}
		else {
			if (pDLC != hfp_rfcomm_dlc) {
				//rfcomm_hfp_close(pDLC);
				DBG_Printf("Disconn Call device\n\r");
			}
		}
	}
}

void hfp_multi_callsetup(struct rfcomm_dlc *pDLC,u8 callsetup)
{
	u8 *pData;
	u8 curr_call, curr_callsetup, curr_callheld;
	pData = rfcomm_get_dlc_user_data(pDLC);
	DBG_Assert (pData != NULL);
	
	curr_call = pData[HFP_CALL];
	curr_callsetup = pData[HFP_CALLSETUP];
	curr_callheld = pData[HFP_CALLHELD];

	DBG_Printf ("HFP Callsetup %d\n\r", callsetup);

	if (callsetup == 0) {
		//callsetp fini
		func_w_one_param_msgSend(BtHCI_HFP_callsetup,0);

		if (curr_call == 0) {
			//no call
			if (hfp_rfcomm_dlc == pDLC) {
#if 0//def HFP_CALL_CALLSETUP_ROLE_SWITCH
				rfcomm_hfp_call_setup_cb(pDLC);
#endif
				//hfp_rfcomm_dlc = NULL;
			//	DBG_Printf ("dlc4:0x%x\n\r",hfp_rfcomm_dlc);

			}
		}
	}
	else {
		//callsetup in progress
		if (hfp_rfcomm_dlc == NULL) {
			hfp_rfcomm_dlc = pDLC;
#if 0//def HFP_CALL_CALLSETUP_ROLE_SWITCH
			rfcomm_hfp_call(pDLC);
#endif
			func_w_one_param_msgSend(BtHCI_HFP_callsetup,callsetup);

			//avctp_set_top_by_hfp();
		}
		else {
			if (pDLC != hfp_rfcomm_dlc) {
				//rfcomm_hfp_close(pDLC);
				DBG_Printf("Disconn Callsetup device\n\r");
			}
		}
	}
	
}

void hfp_multi_ring(struct rfcomm_dlc *pDLC)
{

	if (hfp_rfcomm_dlc == NULL) {
		hfp_rfcomm_dlc = pDLC;
		//avctp_set_top_by_hfp();
		//DBG_Assert (FALSE);
	}
	else {
		if (pDLC != hfp_rfcomm_dlc) {
			//rfcomm_hfp_close(pDLC);
			DBG_Printf("Disconn Ring device\n\r");
		}
	}	
}

void hci_conn_del_rfcomm_by_hfp(void);
u16 ml_device_get_conns(void);

#define HFP_HF_CODEC_NEGOTIATION_MASK	0x2

void hfp_HF_recv_process(struct rfcomm_dlc *pDLC, u8 *pBuf, u8 len)
{
	int val;
	u8 *at_buf;//[MAX_AT_COMAND_BUF];
	u8 at_cmd_len;
	u8 cind_ciev_buf[CIND_callheld_index];
	u16 i;
	u8 *atc_next;
	u8 vol;
	u8 *pData;
	u8 hfp_state;
	u16 ag_feat;
	u8 ciev_call, ciev_callsetup, ciev_callheld;
	u8 *hfp_clip_buf;
	u16 conn_nums;
	u8 codec;

	at_buf = otk_malloc(MAX_AT_COMAND_BUF);
	if (at_buf == NULL) {
		DBG_Assert(FALSE);
		return;
	}

	pData = rfcomm_get_dlc_user_data(pDLC);
	DBG_Assert (pData != NULL);

	hfp_state = pData[HFP_STATE];

	ag_feat = (pData[HFP_FEAT_L] | pData[HFP_FEAT_H] << 8);

	ciev_call = pData[HFP_CIEV_CALL];
	ciev_callsetup = pData[HFP_CIEV_CALLSETUP];
	ciev_callheld = pData[HFP_CIEV_CALLHELD];
	hfp_clip_buf = &pData[HFP_CLIP_BUF];

	switch (hfp_state)
	{
	case HFP_CLOSED:
		break;

	case HFP_BRSF:
		if (at_BRSF(pBuf, &val) == TRUE) {

		#if 0//def BT_MULTILINK_ONLY_ONE_HFP
			conn_nums = ml_device_get_conns();
			if (conn_nums > 1) {
				hci_conn_del_rfcomm_by_hfp();
				//DBG_Assert (FALSE);
				hfp_state = HFP_CLOSED;
				return;
			} 
		#endif

			//ag_feat = (U16) val;
			pData[HFP_FEAT_L] = (val & 0xFF);
			pData[HFP_FEAT_H] = (val >> 8) & 0xFF;

			atc_next = hfp_next_atc(pBuf,len);

			if (atc_next < pBuf + len)
			{
				pBuf = atc_next;			
			}
			else {
				break;
			}

		}
		
		if (at_OK(pBuf) == TRUE) {
		#if 0
		#if 0
			//hfp_state = HFP_BAC;
			pData[HFP_STATE] = HFP_BAC;
			at_cmd_len = at_send_BAC(at_buf);
			hfp_data_send(at_buf,(u8) at_cmd_len);
			#endif
			
			#if 1
			//hfp_state = HFP_CIND_1;
			pData[HFP_STATE] = HFP_CIND_1;
			at_cmd_len = at_send_CIND_1(at_buf);
			hfp_data_send(pDLC,at_buf,(u8) at_cmd_len);			
		#endif
		#endif
		
			if ((pData[HFP_FEAT_H] & HFP_HF_CODEC_NEGOTIATION_MASK) != 0) {
				//hfp_state = HFP_BAC;
				pData[HFP_STATE] = HFP_BAC;
				at_cmd_len = at_send_BAC(at_buf);
				hfp_data_send(pDLC,at_buf,(u8) at_cmd_len);
			}
			else {	
				//hfp_state = HFP_CIND_1;
				pData[HFP_STATE] = HFP_CIND_1;
				at_cmd_len = at_send_CIND_1(at_buf);
				hfp_data_send(pDLC,at_buf,(u8) at_cmd_len);

			}

		}
		else if (at_ERROR(pBuf) == TRUE) {
			DBG_Assert(FALSE);
			//hfp_init ();		
		}
		else {
			DBG_Assert(FALSE);
		}
		break;
		
	case HFP_BAC:

		if ((at_BCS(pBuf,&codec)) == TRUE) {
			if (codec > 2 || codec == 0) {
				at_cmd_len = at_send_ERROR(at_buf);
				hfp_data_send(pDLC,at_buf,(u8) at_cmd_len);		
				DBG_Assert (FALSE);
				pData[HFP_CODEC] = 1;
			}
			else {
				pData[HFP_CODEC] = codec;
				DBG_Printf ("hfp codec %d\n\r",pData[HFP_CODEC]);
				at_cmd_len = at_send_BCS(at_buf,codec);
				hfp_data_send(pDLC,at_buf,(u8) at_cmd_len);		
			}
			
			atc_next = hfp_next_atc(pBuf,len);

			if (atc_next < pBuf + len)
			{
				pBuf = atc_next;			
			}
			else {
				break;
			}			
		}

		if (at_OK(pBuf) == TRUE) {
			pData[HFP_STATE] = HFP_CIND_1;
			at_cmd_len = at_send_CIND_1(at_buf);
			hfp_data_send(pDLC,at_buf,(u8) at_cmd_len);						
		}
		else {
			DBG_Assert(FALSE);
		}
		break;
		
	case HFP_CIND_1:
		if(at_CIND(pBuf) == TRUE) {

			memset(cind_ciev_buf,0,sizeof(cind_ciev_buf));
			at_CIND_process(pBuf,cind_ciev_buf);
		
			for (i=0;i<CIND_callheld_index;i++) {
				switch (cind_ciev_buf[i]) {
				case CIND_call_index:
					ciev_call = (i+1);
					pData[HFP_CIEV_CALL] = (i+1);
					break;
				case CIND_callsetup_index:
					ciev_callsetup = (i+1);
					pData[HFP_CIEV_CALLSETUP] = (i+1);
					break;
				case CIND_callheld_index:
					ciev_callheld = (i+1);
					pData[HFP_CIEV_CALLHELD] = (i+1);
					break;
				default:
					break;
				}
			}

			atc_next = hfp_next_atc(pBuf,len);
			
			if (atc_next < pBuf + len)
			{
				pBuf = atc_next;			
			}
			else {
				break;
			}
		}
		
		if (at_OK(pBuf) == TRUE || at_ERROR(pBuf) == TRUE) {
			//hfp_state = HFP_CIND_2;
			pData[HFP_STATE] = HFP_CIND_2;
			at_cmd_len = at_send_CIND_2(at_buf);
			hfp_data_send(pDLC,at_buf,(u8) at_cmd_len);	
		}
		else {
			DBG_Assert(FALSE);
		}
		break;
	
	case HFP_CIND_2:
		if(at_CIND(pBuf) == TRUE) {

		#if 1
			//when hfp conn, get init status
			//DBG_Assert (FALSE);
			if (pData[HFP_CIEV_CALL] == 0 && pData[HFP_CIEV_CALLSETUP] == 0) {
				//DBG_Assert (FALSE);
				ciev_call = 2;
				ciev_callsetup = 3;
				ciev_callheld = 4;				
			}
		#endif

			memset(cind_ciev_buf,0,sizeof(cind_ciev_buf));
			at_CIND_process_cstatus(pBuf,cind_ciev_buf);

			for (i=0;i<CIND_callheld_index;i++) {
				if (i == (ciev_call-1)) {

					pData[HFP_CALL] = cind_ciev_buf[i];

					#if 0
					//if (cind_ciev_buf[i] != 0) {
					if (pDLC == hfp_rfcomm_dlc) {
						//call is in progressing
						func_w_one_param_msgSend(BtHCI_HFP_call,cind_ciev_buf[i]);
					}
					#endif

					hfp_multi_call(pDLC,pData[HFP_CALL]);

					#if 0
					if (cind_ciev_buf[i] != 0) {
						//call finished
						pData[HFP_ROLE_SWITCH] = 0;
						rfcomm_hfp_call(pDLC);
					}
					#endif
				}
				else if (i == (ciev_callsetup-1)) {
					//if (cind_ciev_buf[i] != 0) {
					if (1) {
						//callsetup is in progressing
						//DBG_Assert(FALSE);
						pData[HFP_CALLSETUP] = cind_ciev_buf[i];


					#if 0
						if (pDLC == hfp_rfcomm_dlc) {
							func_w_one_param_msgSend(BtHCI_HFP_callsetup,cind_ciev_buf[i]);
						}
					#endif
						
						hfp_multi_callsetup(pDLC,pData[HFP_CALLSETUP]);

					#if 0
						if (cind_ciev_buf[i] == 0 && pData[HFP_ROLE_SWITCH] == 1) {
							if (pData[HFP_CALL] == 0) {
								//no call and setup finished
								pData[HFP_ROLE_SWITCH] = 0;
								rfcomm_hfp_call(pDLC);
							}
						}
						else {
							pData[HFP_ROLE_SWITCH] = 1;
							rfcomm_hfp_call_setup_cb(pDLC);
						}
					#endif
					}	
					
				}
				else if (i == (ciev_callheld-1)) {
					if (cind_ciev_buf[i] != 0) {
						//callheld is in progressing
						pData[HFP_CALLHELD] = cind_ciev_buf[i];
						func_w_one_param_msgSend(BtHCI_HFP_callheld,cind_ciev_buf[i]);
					}										
				}			
			}

			atc_next = hfp_next_atc(pBuf,len);
			
			if (atc_next < pBuf + len)
			{
				pBuf = atc_next;			
			}
			else {
				break;
			}
		}
		
		if (at_OK(pBuf) == TRUE) {
			//DBG_Assert(FALSE);
			DBG_Printf("AT CMER\n\r");
			//hfp_state = HFP_CMER;
			pData[HFP_STATE] = HFP_CMER;
			at_cmd_len = at_send_CMER(at_buf);
			hfp_data_send(pDLC,at_buf,(u8) at_cmd_len);
		}
		else {
			DBG_Assert(FALSE);
		}
		break;
		
	case HFP_CMER:
		if (at_OK(pBuf) != TRUE) {
			DBG_Assert(FALSE);
		}

		DBG_Printf("AT CLIP\n\r");
		//hfp_state = HFP_CLIP;		
		pData[HFP_STATE] = HFP_CLIP;
		at_cmd_len = at_send_CLIP(at_buf);
		hfp_data_send(pDLC,at_buf,(u8) at_cmd_len);		
		
		break;
/*
	case HFP_CHLD:
		break;
*/

#if 0
	case HFP_NREC:
		if (at_OK(pBuf) != TRUE) {
			//DBG_Assert(FALSE);
		}

		//hfp_state = HFP_VGS;	
		pData[HFP_STATE] = HFP_VGS;
		//vol = Bt_HCI_get_hfp_speaker_vol();
		at_cmd_len = at_send_VGS(at_buf,15);
		hfp_data_send(at_buf,(u8) at_cmd_len);		
		break;

	case HFP_VGS:
		if (at_OK(pBuf) != TRUE) {
			//DBG_Assert(FALSE);
		}

		//hfp_state = HFP_INITed;					
		pData[HFP_STATE] = HFP_INITed;
		break;

	case HFP_CLIP:	
		if (at_OK(pBuf) != TRUE) {
			//DBG_Assert(FALSE);
		}
			
		{
			if ( (ag_feat & AG_NREC_MASK) == 0) {
			//if (1) {									//test, no local echo cancellation
				//not support echo cancellation
				//hfp_state = HFP_VGS;	
				pData[HFP_STATE] = HFP_VGS;
				//vol = Bt_HCI_get_hfp_speaker_vol();
				at_cmd_len = at_send_VGS(at_buf,15);
				hfp_data_send(at_buf,(u8) at_cmd_len);		
			}
			else {
				//hfp_state = HFP_NREC;		
				pData[HFP_STATE] = HFP_NREC;
				at_cmd_len = at_send_NREC(at_buf);
				hfp_data_send(at_buf,(u8) at_cmd_len);		
			}
		}
		break;
#endif


	default:
		if (at_OK(pBuf) == TRUE || at_ERROR(pBuf) == TRUE) {

			if (at_ERROR(pBuf) == TRUE) {
				if (pData[HFP_STATE] == HFP_APPLE_XAPL) {
					//hfp_state = HFP_INITed;								
					pData[HFP_STATE] = HFP_INITed;
					pData[HFP_BATT_LEV] = 0;
					
					//iphone_batt = FALSE;						//iphone accessory batt level disp
					DBG_Printf ("iPhone Accessory not support\n\r");
				}	
				else {
					DBG_Printf("At cmd errpr\n");
					DBG_Assert (FALSE);
				}
			}
			else {

				switch (hfp_state) {	
					case HFP_CLIP:
					#if 1
						//if add NRCE, the connection will slow down???	
						if ( (ag_feat & AG_NREC_MASK) == 0) {
							//if (1) {									//test, no local echo cancellation
							//not support echo cancellation
							//hfp_state = HFP_VGS;	
							pData[HFP_STATE] = HFP_VGS;
							//vol = Bt_HCI_get_hfp_speaker_vol();
							at_cmd_len = at_send_VGS(at_buf,15);
							hfp_data_send(pDLC,at_buf,(u8) at_cmd_len);		
						}
						else {
							//hfp_state = HFP_NREC;		
							pData[HFP_STATE] = HFP_NREC;
							at_cmd_len = at_send_NREC(at_buf);
							hfp_data_send(pDLC, at_buf,(u8) at_cmd_len);		
						}
					#else
						//at current time, no support echo cancellation in local
						//hfp_state = HFP_VGS;	
						pData[HFP_STATE] = HFP_VGS;
						//vol = Bt_HCI_get_hfp_speaker_vol();
						at_cmd_len = at_send_VGS(at_buf,15);
						hfp_data_send(pDLC, at_buf,(u8) at_cmd_len);
					#endif
						break;
					
					case HFP_NREC:
						//hfp_state = HFP_VGS;	
						pData[HFP_STATE] = HFP_VGS;
						//vol = Bt_HCI_get_hfp_speaker_vol();
						at_cmd_len = at_send_VGS(at_buf,15);
						hfp_data_send(pDLC, at_buf,(u8) at_cmd_len);				
						break;

#if 1//def APPLE_ACCESSORY
					case HFP_VGS:
						if ((sdp_get_mask() & HFP_HF_BATT_LEV_MASK) != 0) {
							//hfp_state = HFP_APPLE_XAPL;	
							pData[HFP_STATE] = HFP_APPLE_XAPL;
							//vol = Bt_HCI_get_hfp_speaker_vol();
							at_cmd_len = at_send_XAPL(at_buf);
							hfp_data_send(pDLC,at_buf,(u8) at_cmd_len);			
						}
						else {
							//hfp_state = HFP_INITed;								
							pData[HFP_STATE] = HFP_INITed;
							
							//hci_conn_del_rfcomm_by_HFP();						}
							break;						
						}

					//case HFP_APPLE_XAPL:
					//	hfp_state = HFP_INITed;								
					//	break;

					#else
					case HFP_VGS:
						//hfp_state = HFP_INITed;								
						pData[HFP_STATE] = HFP_INITed;

						//hci_conn_del_rfcomm_by_HFP();
						break;
					#endif

					default:
						break;		
				}
			}				
		}
		else if (at_CIEV(pBuf,cind_ciev_buf) > 0) {
			
			if (pData[HFP_CIEV_CALL] == 0 && pData[HFP_CIEV_CALLSETUP] == 0) {
				//DBG_Assert (FALSE);
				ciev_call = 2;
				ciev_callsetup = 3;
				ciev_callheld = 4;
			}

			if (cind_ciev_buf[0] == ciev_call) {
				//DBG_Assert(FALSE);
				pData[HFP_CALL] = cind_ciev_buf[1];

			#if 0
				if (pDLC == hfp_rfcomm_dlc) {
					func_w_one_param_msgSend(BtHCI_HFP_call,cind_ciev_buf[1]);
				}
			#endif
				
				hfp_multi_call(pDLC,pData[HFP_CALL]);

			#if 0
				if (cind_ciev_buf[1] == 0 && pData[HFP_ROLE_SWITCH] == 1) {
					//call finished
					pData[HFP_ROLE_SWITCH] = 0;
					rfcomm_hfp_call(pDLC);
				}
			#endif
			}
			else if (cind_ciev_buf[0] == ciev_callsetup) {

				pData[HFP_CALLSETUP] = cind_ciev_buf[1];

				
				hfp_multi_callsetup(pDLC,pData[HFP_CALLSETUP]);

			#if 0
				if (pDLC == hfp_rfcomm_dlc) {
					func_w_one_param_msgSend(BtHCI_HFP_callsetup,cind_ciev_buf[1]);
				}
			#endif

			#if 0
				if (cind_ciev_buf[1] == 0) {
					if (pData[HFP_CALL] == 0) {
						//no call and setup finished
						pData[HFP_ROLE_SWITCH] = 0;
						rfcomm_hfp_call(pDLC);
					}
				}
				else {
					pData[HFP_ROLE_SWITCH] = 1;
					rfcomm_hfp_call_setup_cb(pDLC);
				}
			#endif
			}
			else if (cind_ciev_buf[0] == ciev_callheld) {
				//DBG_Assert(FALSE);
				pData[HFP_CALLHELD] = cind_ciev_buf[1];
				func_w_one_param_msgSend(BtHCI_HFP_callheld,cind_ciev_buf[1]);				
			}			
		}
		else if((at_RING(pBuf)) == TRUE) {
			//incoming ring

			DBG_Printf ("HFP Ring\n\r");

			hfp_multi_ring(pDLC);

			if (pDLC == hfp_rfcomm_dlc) {
				func_wo_param_msgSend(BtHCI_ring);
			}

			atc_next = hfp_next_atc(pBuf,len);
			
			if (atc_next < pBuf + len)
			{
				if((at_CLIP(atc_next, hfp_clip_buf,HFP_CLIP_BUF_SIZE)) == TRUE) {
					if (pDLC == hfp_rfcomm_dlc) {
						func_w_one_param_msgSend(BtHCI_clip,hfp_clip_buf);
					}
				}
			}
		}
		else if((at_CLIP(pBuf, hfp_clip_buf,HFP_CLIP_BUF_SIZE)) == TRUE) {
			func_w_one_param_msgSend(BtHCI_clip,hfp_clip_buf);				
		}
		else if((at_VGS(pBuf,&vol)) == TRUE) {
			func_w_one_param_msgSend(BtHCI_hfp_vgs,(u32) vol);							
		}
		else if((at_VGM(pBuf,&vol)) == TRUE) {
			func_w_one_param_msgSend(BtHCI_hfp_vgm,(u32)vol);				
		}
		else if (at_XAPL(pBuf) == TRUE) {
			u8 batt;
			//hfp_state = HFP_INITed;								
			pData[HFP_STATE] = HFP_INITed;
			//iphone_batt = TRUE;						//iphone accessory batt level disp
			pData[HFP_BATT_LEV] = 0x01; 

			batt = hfp_iphone_batt_cb();
			at_cmd_len = at_send_iphone_accev(at_buf,batt);
			hfp_data_send(pDLC,at_buf,(u8) at_cmd_len);	
			
			DBG_Printf ("iPhone Accessory support\n\r");
		}
		else if ((at_BCS(pBuf,&codec)) == TRUE) {
			if (codec > 2 || codec == 0) {
				at_cmd_len = at_send_ERROR(at_buf);
				hfp_data_send(pDLC,at_buf,(u8) at_cmd_len);		
				DBG_Assert (FALSE);
				pData[HFP_CODEC] = 1;
			}
			else {
				pData[HFP_CODEC] = codec;
				DBG_Printf ("hfp codec %d\n\r",pData[HFP_CODEC]);
				at_cmd_len = at_send_BCS(at_buf,codec);
				hfp_data_send(pDLC,at_buf,(u8) at_cmd_len);		
			}
		}
		else {
			//DBG_Assert (FALSE);
		}		
		break;
	}

	otk_free(at_buf);
}


void hfp_ata(void)
{
	u8 *at_buf;//[MAX_AT_COMAND_BUF];
	u8 at_cmd_len;
	struct rfcomm_dlc *pDLC;
	u8 *pData;
	u8 hfp_state;

	pDLC = rfcomm_get_top_dlc();
	if (pDLC == NULL)
		return;

	pData = rfcomm_get_dlc_user_data(pDLC);
	DBG_Assert (pData != NULL);
	
	hfp_state = pData[HFP_STATE];

	if(hfp_state != HFP_INITed)
		return;


	DBG_Printf ("ATA\n\r");
	at_buf = otk_malloc(MAX_AT_COMAND_BUF);
	if (at_buf == NULL) {
		DBG_Assert(FALSE);
		return;
	}

	at_cmd_len = at_send_ATA(at_buf);
	hfp_data_send(pDLC, at_buf,(u8) at_cmd_len);		

	otk_free(at_buf);
}


#ifdef BT_HFP_THREE_WAY_CALLING
void hfp_comm_send(char *comm,U8 comm_len)
{
	u8 *at_buf;//[MAX_AT_COMAND_BUF];
	u8 at_cmd_len;
	struct rfcomm_dlc *pDLC;
	u8 *pData;
	u8 hfp_state;

	pDLC = rfcomm_get_top_dlc();
	if (pDLC == NULL)
		return;

	pData = rfcomm_get_dlc_user_data(pDLC);
	DBG_Assert (pData != NULL);
	
	hfp_state = pData[HFP_STATE];

	if(hfp_state != HFP_INITed)
		return;

	at_buf = otk_malloc(MAX_AT_COMAND_BUF);
	if (at_buf == NULL) {
		DBG_Assert(FALSE);
		return;
	}

	//at_cmd_len = at_send_ATA(at_buf,);

	DBG_Assert(comm_len + 1 <= MAX_AT_COMAND_BUF);
	
	CFasm_memcpy (&at_buf[0],comm,(comm_len));
	at_buf[(comm_len)] = 0x0D;
	
	
	at_cmd_len = (1+(comm_len));


	hfp_data_send(pDLC, at_buf,(u8) at_cmd_len);		

	otk_free(at_buf);
}


void hci_hfp_comm_send(u32 parm1,u32 parm2,u32 parm3)
{
	hfp_comm_send(parm1,parm2);
}

void uiBtHCI_hfp_comm_send(char *comm,U8 comm_len)
{
	U32 data[WIDTH_OF_BtHCI_QUEUE];
	
	data[0] = (U32) hci_hfp_comm_send;
	data[1] = (U32)comm;
	data[2] = (U32)comm_len;
	data[3] = 0;

	QUEUE_BT_HCI_SEND;
}

u8 ATA_test[] = {"ATA"};
const u8 AT_CHUP_test[] = {"AT+CHUP"};
const u8 AT_BVRA_test[] = {"AT+BVRA"};
const u8 AT_CHLD_test_0[] = {"AT+CHLD=0"};
const u8 AT_CHLD_test_1[] = {"AT+CHLD=1"};
const u8 AT_CHLD_test_1_0[] = {"AT+CHLD=10"};
const u8 AT_CHLD_test_1_1[] = {"AT+CHLD=11"};
const u8 AT_CHLD_test_2[] = {"AT+CHLD=2"};
const u8 AT_CHLD_test_2_0[] = {"AT+CHLD=2<0>"};
const u8 AT_CHLD_test_2_1[] = {"AT+CHLD=2<1>"};
const u8 AT_CHLD_test_1or2[] = {"AT+CHLD=1 or 2"};
const u8 AT_CHLD_test_3[] = {"AT+CHLD=3"};

void hfp_ata_test(void)
{
	DBG_Printf("hfp_ata_test\n");
	U32 len = sizeof(ATA_test)-1;
	uiBtHCI_hfp_comm_send(ATA,len);
}

void hfp_ata_chup_test(void)
{
	DBG_Printf("hfp_ata_chup_test\n");
	U32 len = sizeof(AT_CHUP_test)-1;
	uiBtHCI_hfp_comm_send(AT_CHUP_test,len);
}

void hfp_ata_bvra_test(void)
{
	DBG_Printf("hfp_ata_chup_test\n");
	U32 len = sizeof(AT_BVRA_test)-1;
	uiBtHCI_hfp_comm_send(AT_BVRA_test,len);
}

//to send the User Busy indication to the AG:(AT+CHLD=0).
void hfp_ata_chld_test_0(void)
{
	DBG_Printf("hfp_ata_chld_test_0\n");
	U32 len = sizeof(AT_CHLD_test_0)-1;
	uiBtHCI_hfp_comm_send(AT_CHLD_test_0,len);
}

//to end an active call and accept the other (held or waiting) call:(AT+CHLD=1).
void hfp_ata_chld_test_1(void)
{
	DBG_Printf("hfp_ata_chld_test_1\n");
	U32 len = sizeof(AT_CHLD_test_1)-1;
	uiBtHCI_hfp_comm_send(AT_CHLD_test_1,len);
}

void hfp_ata_chld_test_1_idx(U8 index)
{
	DBG_Printf("hfp_ata_chld_test_1_idx %d\n",index);
	U32 len;
	
	switch (index)
	{
		case 0:
			len = sizeof(AT_CHLD_test_1_0)-1;
			uiBtHCI_hfp_comm_send(AT_CHLD_test_1_0,len);
			break;
		case 1:
			len = sizeof(AT_CHLD_test_1_1)-1;
			uiBtHCI_hfp_comm_send(AT_CHLD_test_1_1,len);
			break;
		default :
			DBG_Printf("hfp_ata_chld_test_1_idx err %d\n",index);
			break;
	}
}

void hfp_ata_chld_test_2(void)
{
	DBG_Printf("hfp_ata_chld_test_2\n");
	U32 len = sizeof(AT_CHLD_test_2)-1;
	uiBtHCI_hfp_comm_send(AT_CHLD_test_2,len);
}

void hfp_ata_chld_test_2_idx(U8 index)
{
	DBG_Printf("hfp_ata_chld_test_2_idx %d\n",index);
	U32 len;
	
	switch (index)
	{
		case 0:
			len = sizeof(AT_CHLD_test_2_0)-1;
			uiBtHCI_hfp_comm_send(AT_CHLD_test_2_0,len);
			break;
		case 1:
			len = sizeof(AT_CHLD_test_2_1)-1;
			uiBtHCI_hfp_comm_send(AT_CHLD_test_2_1,len);
			break;
		default :
			DBG_Printf("hfp_ata_chld_test_2_idx err %d\n",index);
			break;
	}
}

void hfp_ata_chld_test_3(void)
{
	DBG_Printf("hfp_ata_chld_test_3\n");
	U32 len = sizeof(AT_CHLD_test_3)-1;
	uiBtHCI_hfp_comm_send(AT_CHLD_test_3,len);
}

void hfp_ata_chld_test_1or2(void)
{
	DBG_Printf("hfp_ata_chld_test_1or2\n");
	U32 len = sizeof(AT_CHLD_test_1or2)-1;
	uiBtHCI_hfp_comm_send(AT_CHLD_test_1or2,len);
}
#endif //BT_HFP_THREE_WAY_CALLING


void hfp_chup(void)
{
	u8 *at_buf;//[MAX_AT_COMAND_BUF];
	u8 at_cmd_len;
	struct rfcomm_dlc *pDLC;
	u8 *pData;
	u8 hfp_state;
	
	pDLC = rfcomm_get_top_dlc();
	if (pDLC == NULL)
		return;
	
	pData = rfcomm_get_dlc_user_data(pDLC);
	DBG_Assert (pData != NULL);
	
	hfp_state = pData[HFP_STATE];
	
	if(hfp_state != HFP_INITed)
		return;
	
	DBG_Printf ("AT+CHUP\n\r");

	at_buf = otk_malloc(MAX_AT_COMAND_BUF);
	if (at_buf == NULL) {
		DBG_Assert(FALSE);
		return;
	}
	
	at_cmd_len = at_send_CHUP(at_buf);
	hfp_data_send(pDLC, at_buf,(u8) at_cmd_len);		

	otk_free(at_buf);
}

void hfp_last_call(void)
{
	u8 *at_buf;//[MAX_AT_COMAND_BUF];
	u8 at_cmd_len;
	struct rfcomm_dlc *pDLC;
	u8 *pData;
	u8 hfp_state;
	
	pDLC = rfcomm_get_top_dlc();
	if (pDLC == NULL)
		return;
	
	pData = rfcomm_get_dlc_user_data(pDLC);
	DBG_Assert (pData != NULL);
	
	hfp_state = pData[HFP_STATE];
		
	if(hfp_state != HFP_INITed)
		return;
	
	DBG_Printf ("AT+BLDN\n\r");
	
	at_buf = otk_malloc(MAX_AT_COMAND_BUF);
	if (at_buf == NULL) {
		DBG_Assert(FALSE);
		return;
	}
	
	at_cmd_len = at_send_BLDN(at_buf);
	hfp_data_send(pDLC, at_buf,(u8) at_cmd_len);		
	
	otk_free(at_buf);
}

void hfp_hf_trigger_audio_conn(void)
{
	u8 *at_buf;//[MAX_AT_COMAND_BUF];
	u8 at_cmd_len;
	struct rfcomm_dlc *pDLC;
	u8 *pData;
	u8 hfp_state;
	
	pDLC = rfcomm_get_top_dlc();
	if (pDLC == NULL)
		return;
	
	pData = rfcomm_get_dlc_user_data(pDLC);
	DBG_Assert (pData != NULL);
	
	hfp_state = pData[HFP_STATE];

		
	if(hfp_state != HFP_INITed)
		return;

		
	DBG_Printf ("AT+BCC\n\r");
	
	at_buf = otk_malloc(MAX_AT_COMAND_BUF);
	if (at_buf == NULL) {
		DBG_Assert(FALSE);
		return;
	}
	
	at_cmd_len = at_send_BCC(at_buf);
	hfp_data_send(pDLC, at_buf,(u8) at_cmd_len);		
	
	otk_free(at_buf);
}

void hci_sco_conn_switch(void);

void hfp_audio_switch(void)
{
	DBG_Printf ("audio switch\n\r");
	hci_sco_conn_switch();
}



void hfp_vgm(u8 vol)
{
	//no need support
	DBG_Assert(FALSE);
}

void hfp_vgs(u8 vol)
{
	u8 *at_buf;//[MAX_AT_COMAND_BUF];
	u8 at_cmd_len;
	struct rfcomm_dlc *pDLC;
	u8 *pData;
	u8 hfp_state;
	
	pDLC = rfcomm_get_top_dlc();
	if (pDLC == NULL)
		return;
	
	pData = rfcomm_get_dlc_user_data(pDLC);
	DBG_Assert (pData != NULL);
	
	hfp_state = pData[HFP_STATE];
	
	if(hfp_state != HFP_INITed)
		return;
	
	DBG_Printf ("AT+VGS=%d\n\r", vol);
	at_buf = otk_malloc(MAX_AT_COMAND_BUF);
	if (at_buf == NULL) {
		DBG_Assert(FALSE);
		return;
	}
	
	if (vol > 15)
		vol = 15;

	at_cmd_len = at_send_VGS(at_buf,vol);
	hfp_data_send(pDLC,at_buf,(u8) at_cmd_len);		
	
	otk_free(at_buf);
}

struct rfcomm_dlc  *rfcomm_session_get_dlc_by_index(u16 index);
void hfp_BVRA_ACTI(void)
{
	u8 *at_buf;//[MAX_AT_COMAND_BUF];
	u8 at_cmd_len;
	struct rfcomm_dlc *pDLC;
	u8 *pData;
	u8 hfp_state;
	u16 index;
	index = 0;
	
	while (1) {
		pDLC = rfcomm_session_get_dlc_by_index(index++);
		if (pDLC == NULL) {
			return;
		}
		pData = rfcomm_get_dlc_user_data(pDLC);
		DBG_Assert (pData != NULL);
		hfp_state = pData[HFP_STATE];
		if(hfp_state != HFP_INITed)
			return;
		DBG_Printf ("%s\n\r", AT_BVRA_ACTI);		
		at_buf = otk_malloc(MAX_AT_COMAND_BUF);
		if (at_buf == NULL) {
			DBG_Assert(FALSE);
			return;
		}
		at_cmd_len = at_send_BVRA_ACTI(at_buf);
		hfp_data_send(pDLC,at_buf,(u8) at_cmd_len);		
		otk_free(at_buf);
	}
}

void hfp_BVRA_DEACTI(void)
{
	u8 *at_buf;//[MAX_AT_COMAND_BUF];
	u8 at_cmd_len;
	struct rfcomm_dlc *pDLC;
	u8 *pData;
	u8 hfp_state;
	u16 index;
	index = 0;
	
	while (1)
	{
		pDLC = rfcomm_session_get_dlc_by_index(index++);
		if (pDLC == NULL) {
			return;
		}
		pData = rfcomm_get_dlc_user_data(pDLC);
		DBG_Assert (pData != NULL);
		hfp_state = pData[HFP_STATE];
		if(hfp_state != HFP_INITed)
		{
			return;
		}
		DBG_Printf ("%s\n\r", AT_BVRA_DEACTI);
		at_buf = otk_malloc(MAX_AT_COMAND_BUF);
		if (at_buf == NULL) {
			DBG_Assert(FALSE);
			return;
		}
		at_cmd_len = at_send_BVRA_DEACTI(at_buf);
		hfp_data_send(pDLC,at_buf,(u8) at_cmd_len);		
		otk_free(at_buf);
	}
}

u8 at_send_BTRH_1 (u8 *buf);

void hfp_BTRH_1(void)
{
	u8 *at_buf;//[MAX_AT_COMAND_BUF];
	u8 at_cmd_len;
	struct rfcomm_dlc *pDLC;
	u8 *pData;
	u8 hfp_state;
	u16 index;
	index = 0;
	
	while (1)
	{
		pDLC = rfcomm_session_get_dlc_by_index(index++);
		if (pDLC == NULL) {
			return;
		}
		pData = rfcomm_get_dlc_user_data(pDLC);
		DBG_Assert (pData != NULL);
		hfp_state = pData[HFP_STATE];
		if(hfp_state != HFP_INITed)
			return;
		DBG_Printf ("AT+BTRH=1\n\r");
		at_buf = otk_malloc(MAX_AT_COMAND_BUF);
		if (at_buf == NULL) {
			DBG_Assert(FALSE);
			return;
		}
		at_cmd_len = at_send_BTRH_1(at_buf);
		hfp_data_send(pDLC,at_buf,(u8) at_cmd_len);		
		otk_free(at_buf);
	}
}

u8 at_send_at_cmd (u8 *buf,const U8 *str);

void hfp_send_at_cmd(const U8 *str)
{
	u8 *at_buf;//[MAX_AT_COMAND_BUF];
	u8 at_cmd_len;
	struct rfcomm_dlc *pDLC;
	u8 *pData;
	u8 hfp_state;
	u16 index;
	index = 0;
	
	while (1)
	{
		pDLC = rfcomm_session_get_dlc_by_index(index++);
		if (pDLC == NULL) {
			return;
		}
		pData = rfcomm_get_dlc_user_data(pDLC);
		DBG_Assert (pData != NULL);
		hfp_state = pData[HFP_STATE];
		if(hfp_state != HFP_INITed)
			return;
		DBG_Printf ("AT+BTRH=1\n\r");
		at_buf = otk_malloc(MAX_AT_COMAND_BUF);
		if (at_buf == NULL) {
			DBG_Assert(FALSE);
			return;
		}
		at_cmd_len = at_send_at_cmd(at_buf,str);
		hfp_data_send(pDLC,at_buf,(u8) at_cmd_len);		
		otk_free(at_buf);
	}
}


struct rfcomm_dlc  *rfcomm_session_get_dlc_by_index(u16 index);

void hfp_iphone_batt(u8 batt)
{
	u8 *at_buf;//[MAX_AT_COMAND_BUF];
	u8 at_cmd_len;

	struct rfcomm_dlc *pDLC;
	u8 *pData;
	u8 hfp_state;
	u16 index;

	index = 0;

	while (1) {

		pDLC = rfcomm_session_get_dlc_by_index(index++);
		if (pDLC == NULL) {
			return;
		}

		//DBG_Assert (FALSE);
		
		//pDLC = rfcomm_get_top_dlc();
		//if (pDLC == NULL)
		//	return;
		
		pData = rfcomm_get_dlc_user_data(pDLC);
		DBG_Assert (pData != NULL);
		
		hfp_state = pData[HFP_STATE];
		
		if(hfp_state != HFP_INITed)
			return;

		if (pData[HFP_BATT_LEV] == 0x0)
			return;
		
		DBG_Printf ("AT+IPHONEACCEV=%d\n\r", batt);
		at_buf = otk_malloc(MAX_AT_COMAND_BUF);
		if (at_buf == NULL) {
			DBG_Assert(FALSE);
			return;
		}
		
		if (batt > 9)
			batt = 9;
		
		at_cmd_len = at_send_iphone_accev(at_buf,batt);
		hfp_data_send(pDLC, at_buf,(u8) at_cmd_len);		
		
		otk_free(at_buf);
	}
}

u8 HFP_is_active(void)
{
	if (hfp_rfcomm_dlc == NULL)
		return FALSE;
	else
		return TRUE;
}
#else
void hfp_hf_init(struct rfcomm_dlc *pDLC)
{

}


u8 * hfp_next_atc(u8 *buf, int len)
{

}

void hfp_multi_call(struct rfcomm_dlc *pDLC,u8 call)
{

}

void hfp_multi_callsetup(struct rfcomm_dlc *pDLC,u8 callsetup)
{

}

void hfp_multi_ring(struct rfcomm_dlc *pDLC)
{
	
}

void hfp_HF_recv_process(struct rfcomm_dlc *pDLC, u8 *pBuf, u8 len)
{

}


void hfp_ata(void)
{

}

void hfp_chup(void)
{

}

void hfp_last_call(void)
{

}

void hfp_hf_trigger_audio_conn(void)
{

}

void hfp_audio_switch(void)
{

}



void hfp_vgm(u8 vol)
{

}

void hfp_vgs(u8 vol)
{

}

void hfp_BVRA_ACTI(void)
{

}

void hfp_BVRA_DEACTI(void)
{

}

void hfp_BTRH_1(void)
{

}

void hfp_send_at_cmd(const U8 *str)
{

}

void hfp_iphone_batt(u8 batt)
{

}

u8 HFP_is_active(void)
{
	return FALSE;
}

#endif