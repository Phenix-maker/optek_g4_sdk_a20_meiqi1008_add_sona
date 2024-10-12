/******************************************************************************
  * @file           hfp_ag.h
  * @author         optek L
  * @version        V1.0.0
  * @date           2021-09-02
  * @brief          bt hfp ag role header file
******************************************************************************/

#ifndef HFP_AG_H
#define HFP_AG_H

void hfp_ag_init(struct rfcomm_dlc *pDLC);

void hfp_audio_conn(void);
void hfp_audio_disconn(void);

void hfp_AG_recv_process(struct rfcomm_dlc *pDLC, u8 *pBuf, u8 len);

#endif

