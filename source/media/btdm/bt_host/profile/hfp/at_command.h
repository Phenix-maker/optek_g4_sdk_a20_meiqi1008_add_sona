#ifndef _AT_COMMAND_H
#define _AT_COMMAND_H


#define MAX_AT_COMAND_BUF    64


//HSP
extern const u8 AT_OK[];
extern const u8 AT_ERROR[];
extern const u8 AT_HSP_BUTT[];
extern const u8 VGS[];
extern const u8 VGM[];
extern const u8 AT_VGS[];
extern const u8 AT_VGM[];
extern const u8 AT_BLDN[];
extern const u8 AT_BCC[];

//HFP
#ifndef HFP_WIDEBAND
extern const u8 AT_BRSF[]; //feat 0x15
extern const u8 AT_BAC[];  //only CVSD
#else
extern const u8 AT_BRSF[]; //support wideband
extern const u8 AT_BAC[];  //CVSD and mSBC
#endif //HFP_WIDEBAND

extern const u8 AT_BCS[];
extern const u8 BCS[];

extern const u8 BRSF[];


extern const u8 AT_CIND_1[];
extern const u8 AT_CIND_2[];
extern const u8 CIND[];
extern const u8 CIEV[];

extern const u8 AT_CMER[];

extern const u8 ATA[];
extern const u8 AT_CHUP[];
//extern const u8 AT_CHUP[];
extern const u8 AT_BVRA_ACTI[];
extern const u8 AT_BVRA_DEACTI[];

//AT+BTRH=1
extern const u8 AT_BTRH_1[];
extern const u8 AT_BTRH_0[];

extern const u8 AT_NREC[];
extern const u8 AT_CLIP[];
extern const u8 CLIP[];
extern const u8 RING[];


extern const u8 CIND_service[];
extern const u8 CIND_call[];
extern const u8 CIND_callsetup[];
extern const u8 CIND_battchg[];
extern const u8 CIND_signal[];
extern const u8 CIND_roam[];
extern const u8 CIND_callheld[];

extern const u8 AT_XAPL[];
extern const u8 AT_IPHONEACCEV[];

extern const u8 AT_XAPL_RESP[];


enum {
	CIND_service_index = 1,
	CIND_call_index,
	CIND_callsetup_index,
	CIND_battchg_index,
	CIND_signal_index,
	CIND_roam_index,
	CIND_callheld_index = (CIND_roam_index + 9)		/*BlackBerry is more than spec*/
};



u8 at_send_BRSF (u8 *buf);
u8 at_send_BAC (u8 *buf);
u8 at_send_CIND_1 (u8 *buf);
u8 at_send_CIND_2 (u8 *buf);
u8 at_send_CMER (u8 *buf);
u8 at_send_ATA (u8 *buf);
u8 at_send_CHUP (u8 *buf);
u8 at_send_NREC (u8 *buf);
u8 at_send_CLIP (u8 *buf);
u8 at_send_VGS (u8 *buf, u8 vol);
u8 at_send_VGM (u8 *buf, u8 vol);
u8 at_send_BLDN (u8 *buf);
u8 at_send_BCC (u8 *buf);


u8 at_BRSF (u8 *buf, int *feat);
u8 at_OK(u8 *buf);
u8 at_ERROR(u8 *buf);
u8 at_CIND(u8 *buf);

u8 at_CIND_process(U8 *buf, u8 *cind_index);
u8 at_CIND_process_cstatus(u8 *buf, u8 *cind_cstatus);
u8 at_CIEV(u8 *buf, u8 *ciev_ind);
u8 at_RING(u8 *buf);
u8 at_CLIP(u8 *buf, U8 *clip_buf, u8 n);
u8 at_send_BVRA_ACTI (u8 *buf);

u8 at_VGS (u8 *buf, u8 *vol);
u8 at_VGM (u8 *buf, u8 *vol);


u8 at_CIEV_test(u8 *buf);

u8 at_send_XAPL (u8 *buf);
u8 at_send_iphone_accev (u8 *buf, u8 batt);
u8 at_XAPL(u8 *buf);

u8 at_BCS (u8 *buf, u8 *codec);
u8 at_send_BCS (u8 *buf, u8 codec);
u8 at_send_ERROR (u8 *buf);


#endif

