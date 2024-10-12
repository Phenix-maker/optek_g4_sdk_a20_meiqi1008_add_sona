/*should be same as bt_lib_mask.h in bt lib*/
#ifndef BT_LIB_MASK
#define BT_LIB_MASK

//sdp
#define SDP_A2DP_SNK_MASK		    (0x1<<0)
#define disA2DP_CODEC_AAC_MASK      (0x1<<1)
#define A2DP_SNK_RESERVED1_MASK	    (0x1<<2)
#define A2DP_SNK_RESERVED2_MASK	    (0x1<<3)

#define SDP_A2DP_SRC_MASK		    (0x1<<4)
#define A2DP_SRC_RESERVED1_MASK     (0x1<<5)
#define A2DP_SRC_RESERVED2_MASK     (0x1<<6)
#define A2DP_SRC_RESERVED3_MASK     (0x1<<7)

#define SDP_AVRCP_CT_MASK		    (0x1<<8)
#define AVRCP_CT_METADATA_MASK	    (0x1<<9)		/*Music Metadata (Artist/Title..)*/
#define AVRCP_CT_AUTOPLAY_MASK	    (0x1<<10)
#define AVRCP_CT_VOL_SYNC_MASK	    (0x1<<11)
#define AVRCP_CT_RESERVED1_MASK	    (0x1<<12)

#define SDP_AVRCP_TG_MASK		    (0x1<<13)
#define AVRCP_TG_RESERVED1_MASK		(0x1<<14)
#define AVRCP_TG_RESERVED2_MASK		(0x1<<15)

#define SDP_HFP_HF_MASK			    (0x1<<16)
#define disSDP_PBAP_PCE_MASK		(0x1<<17)		/*Phone Book Access Profile (Phone Book Client Equipment)*/
#define HFP_HF_BATT_LEV_MASK	    (0x1<<18)		/*Batt Level Indicator Support*/
#define HFP_HF_WIDEBAND_MASK	    (0x1<<19)		/*HFP Wideband Support*/

#define SDP_HFP_AG_MASK			    (0x1<<20)
#define disSDP_PBAP_PSE_MASK		(0x1<<21)		/*Phone Book Access Profile (Phone Book Server Equipment)*/
#define disHFP_AG_BATT_LEV_MASK	    (0x1<<22)		/*Batt Level Indicator Support*/
#define disHFP_AG_WIDEBAND_MASK	    (0x1<<23)		/*HFP Wideband Support*/

#define BT_APP_NEED_MASTER_MASK		(0x1<<24)		/*for tws, multiroom, need device is master*/
#define BT_APP_NEED_SLAVE_MASK		(0x1<<25)

#define BLE_EXT_ADV_MASK		    (0x1<<26)

#define BT_NAME_EXT_MASK		    (0x1<<27)		/*Name + some of MAC ID*/
#define BT_w_EEPROM_MASK		    (0x1<<28)		/*BT data in eeprom*/

#define BT_RESERVED1_MASK		    (0x1<<29)	

#define TEST_DUT_MASK			    (0x1<<30)
#define TEST_BQB_MASK			    (0x1<<31)

#endif		//BT_LIB_MASK



