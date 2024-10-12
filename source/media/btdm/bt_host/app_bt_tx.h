/**
 ****************************************************************************************
 *
 * @file app_bt_tx.h
 *
 * @brief This file contains the definitions for management and use of bt tx module
 *
 ****************************************************************************************
 */

#ifndef _APP_BT_TX_
#define _APP_BT_TX_
/**
 ****************************************************************************************
 * @defgroup bt_tx (implement class bt profiles of a2dp source,avrcp target,hfp AG)
 * @ingroup bt
 * @brief definitions for management and use of bt tx module
 *
 * @{
 ****************************************************************************************
 */

/*
 * configs
 ****************************************************************************************
 */

#ifdef BT_TX_ROLE_ENABLE
#define A2DP_SOURCE
//#define AVRCP_TARGET  //study later
#ifdef BT_HFP_ENABLE
#define HFP_AG
#endif
#endif

#ifdef A2DP_SOURCE
//#define A2DP_CODEC_SR_44100
#define A2DP_CODEC_SR_48000
#endif

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
void app_nav_bt_tx_init (void);
void app_nav_bt_hp_search (void);
void app_hp_tx_play(void);
void app_hp_tx_pause(void);

///@} bt_tx
#endif