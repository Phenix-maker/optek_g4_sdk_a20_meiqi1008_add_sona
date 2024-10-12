#ifndef _HFP_HF_H_
#define _HFP_HF_H_

//u16 ag_feat;
//u8 hfp_state;
//u8 ciev_call, ciev_callsetup, ciev_callheld;



void uiBtHCI_hfp_accept(void);
void uiBtHCI_hfp_term(void);

void hfp_vgm(u8 vol);
void hfp_vgs(u8 vol);


#if 1//zero
void hfp_ata_test(void);
void hfp_ata_chup_test(void);
void hfp_ata_bvra_test(void);
void hfp_ata_chld_test_0(void);
void hfp_ata_chld_test_1(void);
void hfp_ata_chld_test_2(void);
void hfp_ata_chld_test_3(void);
void hfp_ata_chld_test_1or2(void);
void hfp_ata_chld_test_1_idx(U8 index);
void hfp_ata_chld_test_2_idx(U8 index);
#endif

void hfp_hf_init(struct rfcomm_dlc *pDLC);
void hfp_HF_recv_process(struct rfcomm_dlc *pDLC, u8 *pBuf, u8 len);
u8 hfp_get_codec (void);

#endif //_HFP_H_

