#ifndef __OPTEKRF_H__
#define __OPTEKRF_H__

#define TEST_BOARD_1
//#define TEST_BOARD_2

#ifdef TEST_BOARD_1
#define TEST_BOARD_1_TX
#endif

#ifdef TEST_BOARD_2
#define TEST_BOARD_2_RX
//#define TEST_BOARD_2_TX
#endif

#define OPTEKRF_TEST_BY_CEVA
//#if ((defined TEST_BOARD_1_RX) || (defined TEST_BOARD_2_RX))
//#define OPTEKRF_RX_TEST_BY_REG //for mid freq test
//#endif
#define OPTEKRF_TEST_BY_OPTEK


#define MDM_BASE_ADDR           	0x18500000


#define OPTEKRF_RATE_TX_ADD      	0x800
#define OPTEKRF_CHANNEL_TX_ADD   	0x801
#define OPTEKRF_PA_PWR_ADD       	0x802
#define OPTEKRF_TX_STARTUP_ADD   	0x803

#define OPTEKRF_TX_STARTUP       	0x808

#define OPTEKRF_RATE_RX_ADD      	0x810
#define OPTEKRF_CHANNEL_RX_ADD   	0x811
#define OPTEKRF_RX_STARTUP_ADD   	0x812
#define OPTEKRF_SYNC_WORD_0       	0x813
#define OPTEKRF_SYNC_WORD_1       	0x814
#define OPTEKRF_SYNC_WORD_2       	0x815
#define OPTEKRF_SYNC_WORD_3       	0x816
#define OPTEKRF_SYNC_WORD_4       	0x817
#define OPTEKRF_SYNC_WORD_5       	0x818
#define OPTEKRF_SYNC_WORD_6       	0x819
#define OPTEKRF_SYNC_WORD_7       	0x81A


#define OPTEKRF_RX_STARTUP       	0x818

#define OPTEKRF_RX_PWROFF_ADD    	0x820
#define OPTEKRF_RSSI_ADD         	0x824
#define OPTEKRF_TX_PWROFF_ADD    	0x830

#define OPTEKRF_AM_TABLE_DATA_ADDR      0x400 + 0xC6
#define OPTEKRF_AM_TABLE_INDEX_ADDR     0x400 + 0xC8

#define OPTEKRF__FCVO_1_ADD             ( 0x400 + 0 )
#define OPTEKRF__FCVO_2_ADD             ( 0x400 + 1 )
#define OPTEKRF__FCVO_3_ADD             ( 0x400 + 2 )
#define OPTEKRF__FCVO_4_ADD             ( 0x400 + 3 )
#define OPTEKRF__FCVO_5_ADD             ( 0x400 + 4 )
#define OPTEKRF__FCVO_6_ADD             ( 0x400 + 5 )
#define OPTEKRF__FCVO_7_ADD             ( 0x400 + 6 )
#define OPTEKRF__FCVO_8_ADD             ( 0x400 + 7 )
#define OPTEKRF__FCVO_9_ADD             ( 0x400 + 8 )
#define OPTEKRF__FCVO_10_ADD            ( 0x400 + 9 )
#define OPTEKRF__FCVO_11_ADD            ( 0x400 + 10 )
#define OPTEKRF__FCVO_12_ADD            ( 0x400 + 11 )
#define OPTEKRF__FCVO_13_ADD            ( 0x400 + 12 )
#define OPTEKRF__FCVO_14_ADD            ( 0x400 + 13 )
#define OPTEKRF__FCVO_15_ADD            ( 0x400 + 14 )
#define OPTEKRF__FCVO_16_ADD            ( 0x400 + 15 )

#define OPTEKRF__DCO_GA_BT1M_1_ADDR     ( 0x400 + 16 )
#define OPTEKRF__DCO_GA_BT1M_2_ADDR     ( 0x400 + 17 )
#define OPTEKRF__DCO_GA_BT1M_3_ADDR     ( 0x400 + 18 )
#define OPTEKRF__DCO_GA_BT1M_4_ADDR     ( 0x400 + 19 )
#define OPTEKRF__DCO_GA_BT1M_5_ADDR     ( 0x400 + 20 )
#define OPTEKRF__DCO_GA_BT1M_6_ADDR     ( 0x400 + 21 )
#define OPTEKRF__DCO_GA_BT1M_7_ADDR     ( 0x400 + 22 )
#define OPTEKRF__DCO_GA_BT1M_8_ADDR     ( 0x400 + 23 )
#define OPTEKRF__DCO_GA_BT1M_9_ADDR     ( 0x400 + 24 )
#define OPTEKRF__DCO_GA_BT1M_10_ADDR    ( 0x400 + 25 )
#define OPTEKRF__DCO_GA_BT1M_11_ADDR    ( 0x400 + 26 )
#define OPTEKRF__DCO_GA_BT1M_12_ADDR    ( 0x400 + 27 )
#define OPTEKRF__DCO_GA_BT1M_13_ADDR    ( 0x400 + 28 )
#define OPTEKRF__DCO_GA_BT1M_14_ADDR    ( 0x400 + 29 )
#define OPTEKRF__DCO_GA_BT1M_15_ADDR    ( 0x400 + 30 )
#define OPTEKRF__DCO_GA_BT1M_16_ADDR    ( 0x400 + 31 )


#define OPTEKRF__DCO_GA_BLE2M_1_ADDR    ( 0x400 + 32 )
#define OPTEKRF__DCO_GA_BLE2M_2_ADDR    ( 0x400 + 33 )
#define OPTEKRF__DCO_GA_BLE2M_3_ADDR    ( 0x400 + 34 )
#define OPTEKRF__DCO_GA_BLE2M_4_ADDR    ( 0x400 + 35 )
#define OPTEKRF__DCO_GA_BLE2M_5_ADDR    ( 0x400 + 36 )
#define OPTEKRF__DCO_GA_BLE2M_6_ADDR    ( 0x400 + 37 )
#define OPTEKRF__DCO_GA_BLE2M_7_ADDR    ( 0x400 + 38 )
#define OPTEKRF__DCO_GA_BLE2M_8_ADDR    ( 0x400 + 39 )
#define OPTEKRF__DCO_GA_BLE2M_9_ADDR    ( 0x400 + 40 )
#define OPTEKRF__DCO_GA_BLE2M_10_ADDR   ( 0x400 + 41 )
#define OPTEKRF__DCO_GA_BLE2M_11_ADDR   ( 0x400 + 42 )
#define OPTEKRF__DCO_GA_BLE2M_12_ADDR   ( 0x400 + 43 )
#define OPTEKRF__DCO_GA_BLE2M_13_ADDR   ( 0x400 + 44 )
#define OPTEKRF__DCO_GA_BLE2M_14_ADDR   ( 0x400 + 45 )
#define OPTEKRF__DCO_GA_BLE2M_15_ADDR   ( 0x400 + 46 )
#define OPTEKRF__DCO_GA_BLE2M_16_ADDR   ( 0x400 + 47 )


#define OPTEKRF__DCO_GA_BTEDR_1_ADDR    ( 0x400 + 48 )
#define OPTEKRF__DCO_GA_BTEDR_2_ADDR    ( 0x400 + 49 )
#define OPTEKRF__DCO_GA_BTEDR_3_ADDR    ( 0x400 + 50 )
#define OPTEKRF__DCO_GA_BTEDR_4_ADDR    ( 0x400 + 51 )
#define OPTEKRF__DCO_GA_BTEDR_5_ADDR    ( 0x400 + 52 )
#define OPTEKRF__DCO_GA_BTEDR_6_ADDR    ( 0x400 + 53 )
#define OPTEKRF__DCO_GA_BTEDR_7_ADDR    ( 0x400 + 54 )
#define OPTEKRF__DCO_GA_BTEDR_8_ADDR    ( 0x400 + 55 )
#define OPTEKRF__DCO_GA_BTEDR_9_ADDR    ( 0x400 + 56 )
#define OPTEKRF__DCO_GA_BTEDR_10_ADDR   ( 0x400 + 57 )
#define OPTEKRF__DCO_GA_BTEDR_11_ADDR   ( 0x400 + 58 )
#define OPTEKRF__DCO_GA_BTEDR_12_ADDR   ( 0x400 + 59 )
#define OPTEKRF__DCO_GA_BTEDR_13_ADDR   ( 0x400 + 60 )
#define OPTEKRF__DCO_GA_BTEDR_14_ADDR   ( 0x400 + 61 )
#define OPTEKRF__DCO_GA_BTEDR_15_ADDR   ( 0x400 + 62 )
#define OPTEKRF__DCO_GA_BTEDR_16_ADDR   ( 0x400 + 63 )
#define OPTEKRF_SEL_VC_CLK_ADDR        	( 0x400 + 64 )
#define OPTEKRF_RX_FL_GAIN_ADDR        	( 0x400 + 65 )
#define OPTEKRF_LDOS_SETTING_ADDR      	( 0x400 + 66 )
#define OPTEKRF_RFPLL_SETTING_ADDR     	( 0x400 + 67 )
#define OPTEKRF_TXPD_SETTING_ADDR      	( 0x400 + 68 )
#define OPTEKRF_RX_GAIN_ADDR           	( 0x400 + 69 )
#define OPTEKRF_FM_AM_DT_ADDR          	( 0x400 + 70 )
#define OPTEKRF_DCO_DIV_DT_ADDR        	( 0x400 + 71 )
#define OPTEKRF_GFSK_AM_NOM_ADDR    	( 0x400 + 72 )
#define OPTEKRF_AM_DEN_ADDR    	 		( 0x400 + 73 )
#define OPTEKRF_PA_ANT_SEL_ADDR    	    ( 0x400 + 74 )
//#define OPTEKRF_PSK_AM_NOM_ADDR    		( 0x400 + 74 )
#define OPTEKRF_AM_VAL_ADDR    	 		( 0x400 + 75 )
#define OPTEKRF_MDM_CTRL_ADDR    		( 0x400 + 76 )

#define  OPTEKRF_RX_RFGA_EFFECT_T_ADDR  0x400 + 77
#define  OPTEKRF_RX_AGC_TARGET_ADDR     0x400 + 78
#define  OPTEKRF_RX_AGC_0db_ADJ_ADDR    0x400 + 79
#define  OPTEKRF_RX_BLE_ADDR            0x400 + 80

//16b w
#define OPTEKRF_RFADC_VAL_0_ADDR_0     	( 0x400 + 0xC0 + 0 )
#define OPTEKRF_RFADC_VAL_0_ADDR_1     	( 0x400 + 0xC0 + 1 )
#define OPTEKRF_RXIF_DIV_ADDR_0        	( 0x400 + 0xC0 + 2 )
#define OPTEKRF_RXIF_DIV_ADDR_1        	( 0x400 + 0xC0 + 3 )
#define OPTEKRF_RXIF_2M_DIV_ADDR_0     	( 0x400 + 0xC0 + 4 )
#define OPTEKRF_RXIF_2M_DIV_ADDR_1     	( 0x400 + 0xC0 + 5 )


//32b w
#define OPTEKRF_CLK_REG_CORE_ADDR_0    	( 0x400 + 0x100 + 0 )
#define OPTEKRF_CLK_REG_CORE_ADDR_1    	( 0x400 + 0x100 + 1 )
#define OPTEKRF_CLK_REG_CORE_ADDR_2    	( 0x400 + 0x100 + 2 )
#define OPTEKRF_CLK_REG_CORE_ADDR_3    	( 0x400 + 0x100 + 3 )

#define OPTEKRF_LDO_REG_CORE_ADDR_0    	( 0x400 + 0x104 + 0 )
#define OPTEKRF_LDO_REG_CORE_ADDR_1    	( 0x400 + 0x104 + 1 )
#define OPTEKRF_LDO_REG_CORE_ADDR_2    	( 0x400 + 0x104 + 2 )
#define OPTEKRF_LDO_REG_CORE_ADDR_3    	( 0x400 + 0x104 + 3 )

#define OPTEKRF_RFE_REG_ADDR_0         	( 0x400 + 0x108 + 0 )
#define OPTEKRF_RFE_REG_ADDR_1         	( 0x400 + 0x108 + 1 )
#define OPTEKRF_RFE_REG_ADDR_2         	( 0x400 + 0x108 + 2 )
#define OPTEKRF_RFE_REG_ADDR_3         	( 0x400 + 0x108 + 3 )

#define OPTEKRF_RFPLL_REG_ADDR_0       	( 0x400 + 0x10C + 0 )
#define OPTEKRF_RFPLL_REG_ADDR_1       	( 0x400 + 0x10C + 1 )
#define OPTEKRF_RFPLL_REG_ADDR_2       	( 0x400 + 0x10C + 2 )
#define OPTEKRF_RFPLL_REG_ADDR_3       	( 0x400 + 0x10C + 3 )

#define OPTEKRF_VCO_REG_ADDR_0         	( 0x400 + 0x110 + 0 )
#define OPTEKRF_VCO_REG_ADDR_1         	( 0x400 + 0x110 + 1 )
#define OPTEKRF_VCO_REG_ADDR_2         	( 0x400 + 0x110 + 2 )
#define OPTEKRF_VCO_REG_ADDR_3         	( 0x400 + 0x110 + 3 )

#define OPTEKRF_RXABB_REG_ADDR_0       	( 0x400 + 0x114 + 0 )
#define OPTEKRF_RXABB_REG_ADDR_1       	( 0x400 + 0x114 + 1 )
#define OPTEKRF_RXABB_REG_ADDR_2       	( 0x400 + 0x114 + 2 )
#define OPTEKRF_RXABB_REG_ADDR_3       	( 0x400 + 0x114 + 3 )

#define OPTEKRF_TRX_REG_ADDR_0         	( 0x400 + 0x118 + 0 )
#define OPTEKRF_TRX_REG_ADDR_1         	( 0x400 + 0x118 + 1 )
#define OPTEKRF_TRX_REG_ADDR_2         	( 0x400 + 0x118 + 2 )
#define OPTEKRF_TRX_REG_ADDR_3         	( 0x400 + 0x118 + 3 )

#define OPTEKRF_XCVR_REG_ADDR_0        	( 0x400 + 0x11C + 0 )
#define OPTEKRF_XCVR_REG_ADDR_1        	( 0x400 + 0x11C + 1 )
#define OPTEKRF_XCVR_REG_ADDR_2        	( 0x400 + 0x11C + 2 )
#define OPTEKRF_XCVR_REG_ADDR_3        	( 0x400 + 0x11C + 3 )

#define OPTEKRF_CAV_VAL_ADDR_0         	( 0x400 + 0x120 + 0 )
#define OPTEKRF_CAV_VAL_ADDR_1         	( 0x400 + 0x120 + 1 )
#define OPTEKRF_CAV_VAL_ADDR_2         	( 0x400 + 0x120 + 2 )
#define OPTEKRF_CAV_VAL_ADDR_3         	( 0x400 + 0x120 + 3 )

#define OPTEKRF_DEBUG_ADDR_0           	( 0x400 + 0x124 + 0 )
#define OPTEKRF_DEBUG_ADDR_1           	( 0x400 + 0x124 + 1 )
#define OPTEKRF_DEBUG_ADDR_2           	( 0x400 + 0x124 + 2 )
#define OPTEKRF_DEBUG_ADDR_3           	( 0x400 + 0x124 + 3 )

#define OPTEKRF_AM_AM           		0x600
#define OPTEKRF_AM_HAM           		0xA00
#define OPTEKRF_AM_PM           		0xC00



#define BTDM_MDM_VERSION_ADDR                 	(0x80<<2)
#define BTDM_MDM_MDM_CNTL_ADDR                	(0x81<<2)
#define BTDM_MDM_CLKCNTL_ADDR                 	(0x82<<2)
#define BTDM_MDM_RX_STARTUPDEL_ADDR           	(0x83<<2)
#define BTDM_MDM_TX_STARTUPDEL_ADDR           	(0x84<<2)
#define BTDM_MDM_TX_GFSKMODE_ADDR             	(0x85<<2)
#define BTDM_MDM_DIAGCNTL_ADDR                	(0x86<<2)
#define BTDM_MDM_RX_PWR_ADDR                  	(0x87<<2)
#define BTDM_MDM_OVERSAMPLING_CNTL_ADDR       	(0x88<<2)
#define BTDM_MDM_RXFE_CNTL_ADDR               	(0x90<<2)
#define BTDM_MDM_FCS_IFMHZ_ADDR               	(0x91<<2)
#define BTDM_MDM_RXGFSK_CNTL_ADDR             	(0xA0<<2)
#define BTDM_MDM_GFO_P2PTHR_BT_ADDR           	(0xA1<<2)
#define BTDM_MDM_GFO_P2PTHR_BLE_ADDR          	(0xA2<<2)
#define BTDM_MDM_GFO_GFSKDETECT_ADDR          	(0xA3<<2)
#define BTDM_MDM_GFO_INIT_ADDR                	(0xA4<<2)
#define BTDM_MDM_GFO_SETKDEL_ADDR             	(0xA5<<2)
#define BTDM_MDM_GFO_EST_ADDR                 	(0xA6<<2)
#define BTDM_MDM_GFO_PEAKTIMETHR_ADDR         	(0xA7<<2)
#define BTDM_MDM_GFO_DPLATENCY_ADDR           	(0xA8<<2)
#define BTDM_MDM_STO_CNTL_ADDR                	(0xA9<<2)
#define BTDM_MDM_STO_LR_CNTL_ADDR             	(0xAA<<2)
#define BTDM_MDM_PE_POWERTHR_ADDR             	(0xAB<<2)
#define BTDM_MDM_DPLATENCY_CNTL_ADDR          	(0xAC<<2)
#define BTDM_MDM_LPF_TUNE_ADDR                	(0xAD<<2)
#define BTDM_MDM_PE_POWERTHR1_ADDR            	(0xAE<<2)
#define BTDM_MDM_STO_QUALITY_ADDR             	(0xAF<<2)
#define BTDM_MDM_RXDPSK_CNTL_ADDR             	(0xB0<<2)
#define BTDM_MDM_TE_TIMEINIT_ADDR             	(0xB1<<2)
#define BTDM_MDM_DFD_KFACTOR_ADDR             	(0xB2<<2)
#define BTDM_MDM_COC_KFACTOR_ADDR             	(0xB3<<2)
#define BTDM_MDM_COC_THR_ADDR                 	(0xB4<<2)
#define BTDM_MDM_COC_TD_ADDR                  	(0xB5<<2)
#define BTDM_MDM_COC_TENABLE_ADDR             	(0xB6<<2)
#define BTDM_MDM_POW_DIGGAINOFF_ADDR          	(0xB7<<2)
#define BTDM_MDM_FOE_STATUS_ADDR              	(0xB8<<2)
#define BTDM_MDM_TECOC_STATUS_ADDR            	(0xB9<<2)
#define BTDM_MDM_GSG_DEN_ADDR                 	(0xC0<<2)
#define BTDM_MDM_GSG_LSTVAL_ADDR              	(0xC1<<2)
#define BTDM_MDM_GSG_NOM_ADDR                 	(0xC2<<2)
#define BTDM_MDM_GSG_THREPS_ADDR              	(0xC3<<2)
#define BTDM_MDM_GSG_VCO_DEN_ADDR             	(0xC4<<2)
#define BTDM_MDM_GSG_VCO_NOM_ADDR             	(0xC5<<2)
#define BTDM_MDM_FM2PSW_LAT_ADDR              	(0xC6<<2)
#define BTDM_MDM_GSG_DPHI_DEN_ADDR            	(0xC7<<2)
#define BTDM_MDM_GSG_DPHI_NOM_ADDR            	(0xC8<<2)
#define BTDM_MDM_DSG_DEN_ADDR                 	(0xD0<<2)
#define BTDM_MDM_DSG_NOM_ADDR                 	(0xD1<<2)
#define BTDM_MDM_CORRELATION_CODE_BT_ADDR     	(0xD8<<2)
#define BTDM_MDM_CORRELATION_CODE_BT_BLE_ADDR  	(0xD9<<2)
#define BTDM_MDM_CORR_ERR_CNTL_ADDR           	(0xDA<<2)
#define BTDM_MDM_LR_CORR_THR_CNTL_ADDR        	(0xDB<<2)
#define BTDM_MDM_RXLPF_1M_C0_ADDR             	(0xE0<<2)
#define BTDM_MDM_RXLPF_1M_C1_ADDR             	(0xE1<<2)
#define BTDM_MDM_RXLPF_1M_C2_ADDR             	(0xE2<<2)
#define BTDM_MDM_RXLPF_1M_C3_ADDR             	(0xE3<<2)
#define BTDM_MDM_RXLPF_1M_C4_ADDR             	(0xE4<<2)
#define BTDM_MDM_RXLPF_2M_C0_ADDR             	(0xE5<<2)
#define BTDM_MDM_RXLPF_2M_C1_ADDR             	(0xE6<<2)
#define BTDM_MDM_RXLPF_2M_C2_ADDR             	(0xE7<<2)
#define BTDM_MDM_RXLPF_2M_C3_ADDR             	(0xE8<<2)
#define BTDM_MDM_RXLPF_2M_C4_ADDR             	(0xE9<<2)
#define BTDM_MDM_RXLPF_LR_C0_ADDR             	(0xEA<<2)
#define BTDM_MDM_RXLPF_LR_C1_ADDR             	(0xEB<<2)
#define BTDM_MDM_RXLPF_LR_C2_ADDR             	(0xEC<<2)
#define BTDM_MDM_RXLPF_LR_C3_ADDR             	(0xED<<2)
#define BTDM_MDM_RXLPF_LR_C4_ADDR             	(0xEE<<2)
#define BTDM_MDM_RXLPF_LRSEARCH_C0_ADDR      	(0xEF<<2)
#define BTDM_MDM_RXLPF_LRSEARCH_C1_ADDR      	(0xF0<<2)
#define BTDM_MDM_RXLPF_LRSEARCH_C2_ADDR      	(0xF1<<2)
#define BTDM_MDM_RXLPF_LRSEARCH_C3_ADDR      	(0xF2<<2)
#define BTDM_MDM_RXLPF_LRSEARCH_C4_ADDR      	(0xF3<<2)


#define BTDM_MDM_VERSION_ADDR_CT               		REG(MDM_BASE_ADDR + BTDM_MDM_VERSION_ADDR                )                
#define BTDM_MDM_MDM_CNTL_ADDR_CT              		REG(MDM_BASE_ADDR + BTDM_MDM_MDM_CNTL_ADDR               )
#define BTDM_MDM_CLKCNTL_ADDR_CT               		REG(MDM_BASE_ADDR + BTDM_MDM_CLKCNTL_ADDR                )
#define BTDM_MDM_RX_STARTUPDEL_ADDR_CT         		REG(MDM_BASE_ADDR + BTDM_MDM_RX_STARTUPDEL_ADDR          )
#define BTDM_MDM_TX_STARTUPDEL_ADDR_CT         		REG(MDM_BASE_ADDR + BTDM_MDM_TX_STARTUPDEL_ADDR          )
#define BTDM_MDM_TX_GFSKMODE_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_TX_GFSKMODE_ADDR            )
#define BTDM_MDM_DIAGCNTL_ADDR_CT              		REG(MDM_BASE_ADDR + BTDM_MDM_DIAGCNTL_ADDR               )
#define BTDM_MDM_RX_PWR_ADDR_CT                		REG(MDM_BASE_ADDR + BTDM_MDM_RX_PWR_ADDR                 )
#define BTDM_MDM_OVERSAMPLING_CNTL_ADDR_CT     		REG(MDM_BASE_ADDR + BTDM_MDM_OVERSAMPLING_CNTL_ADDR      )
#define BTDM_MDM_RXFE_CNTL_ADDR_CT             		REG(MDM_BASE_ADDR + BTDM_MDM_RXFE_CNTL_ADDR              )
#define BTDM_MDM_FCS_IFMHZ_ADDR_CT             		REG(MDM_BASE_ADDR + BTDM_MDM_FCS_IFMHZ_ADDR              )
#define BTDM_MDM_RXGFSK_CNTL_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_RXGFSK_CNTL_ADDR            )
#define BTDM_MDM_GFO_P2PTHR_BT_ADDR_CT         		REG(MDM_BASE_ADDR + BTDM_MDM_GFO_P2PTHR_BT_ADDR          )
#define BTDM_MDM_GFO_P2PTHR_BLE_ADDR_CT        		REG(MDM_BASE_ADDR + BTDM_MDM_GFO_P2PTHR_BLE_ADDR         )
#define BTDM_MDM_GFO_GFSKDETECT_ADDR_CT        		REG(MDM_BASE_ADDR + BTDM_MDM_GFO_GFSKDETECT_ADDR         )
#define BTDM_MDM_GFO_INIT_ADDR_CT              		REG(MDM_BASE_ADDR + BTDM_MDM_GFO_INIT_ADDR               )
#define BTDM_MDM_GFO_SETKDEL_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_GFO_SETKDEL_ADDR            )
#define BTDM_MDM_GFO_EST_ADDR_CT               		REG(MDM_BASE_ADDR + BTDM_MDM_GFO_EST_ADDR                )
#define BTDM_MDM_GFO_PEAKTIMETHR_ADDR_CT       		REG(MDM_BASE_ADDR + BTDM_MDM_GFO_PEAKTIMETHR_ADDR        )
#define BTDM_MDM_GFO_DPLATENCY_ADDR_CT         		REG(MDM_BASE_ADDR + BTDM_MDM_GFO_DPLATENCY_ADDR          )
#define BTDM_MDM_STO_CNTL_ADDR_CT              		REG(MDM_BASE_ADDR + BTDM_MDM_STO_CNTL_ADDR               )
#define BTDM_MDM_STO_LR_CNTL_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_STO_LR_CNTL_ADDR            )
#define BTDM_MDM_PE_POWERTHR_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_PE_POWERTHR_ADDR            )
#define BTDM_MDM_DPLATENCY_CNTL_ADDR_CT        		REG(MDM_BASE_ADDR + BTDM_MDM_DPLATENCY_CNTL_ADDR         )
#define BTDM_MDM_LPF_TUNE_ADDR_CT              		REG(MDM_BASE_ADDR + BTDM_MDM_LPF_TUNE_ADDR               )
#define BTDM_MDM_PE_POWERTHR1_ADDR_CT          		REG(MDM_BASE_ADDR + BTDM_MDM_PE_POWERTHR1_ADDR           )
#define BTDM_MDM_STO_QUALITY_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_STO_QUALITY_ADDR            )
#define BTDM_MDM_RXDPSK_CNTL_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_RXDPSK_CNTL_ADDR            )
#define BTDM_MDM_TE_TIMEINIT_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_TE_TIMEINIT_ADDR            )
#define BTDM_MDM_DFD_KFACTOR_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_DFD_KFACTOR_ADDR            )
#define BTDM_MDM_COC_KFACTOR_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_COC_KFACTOR_ADDR            )
#define BTDM_MDM_COC_THR_ADDR_CT               		REG(MDM_BASE_ADDR + BTDM_MDM_COC_THR_ADDR                )
#define BTDM_MDM_COC_TD_ADDR_CT                		REG(MDM_BASE_ADDR + BTDM_MDM_COC_TD_ADDR                 )
#define BTDM_MDM_COC_TENABLE_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_COC_TENABLE_ADDR            )
#define BTDM_MDM_POW_DIGGAINOFF_ADDR_CT        		REG(MDM_BASE_ADDR + BTDM_MDM_POW_DIGGAINOFF_ADDR         )
#define BTDM_MDM_FOE_STATUS_ADDR_CT            		REG(MDM_BASE_ADDR + BTDM_MDM_FOE_STATUS_ADDR             )
#define BTDM_MDM_TECOC_STATUS_ADDR_CT          		REG(MDM_BASE_ADDR + BTDM_MDM_TECOC_STATUS_ADDR           )
#define BTDM_MDM_GSG_DEN_ADDR_CT               		REG(MDM_BASE_ADDR + BTDM_MDM_GSG_DEN_ADDR                )
#define BTDM_MDM_GSG_LSTVAL_ADDR_CT            		REG(MDM_BASE_ADDR + BTDM_MDM_GSG_LSTVAL_ADDR             )
#define BTDM_MDM_GSG_NOM_ADDR_CT               		REG(MDM_BASE_ADDR + BTDM_MDM_GSG_NOM_ADDR                )
#define BTDM_MDM_GSG_THREPS_ADDR_CT            		REG(MDM_BASE_ADDR + BTDM_MDM_GSG_THREPS_ADDR             )
#define BTDM_MDM_GSG_VCO_DEN_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_GSG_VCO_DEN_ADDR            )
#define BTDM_MDM_GSG_VCO_NOM_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_GSG_VCO_NOM_ADDR            )
#define BTDM_MDM_FM2PSW_LAT_ADDR_CT            		REG(MDM_BASE_ADDR + BTDM_MDM_FM2PSW_LAT_ADDR             )
#define BTDM_MDM_GSG_DPHI_DEN_ADDR_CT          		REG(MDM_BASE_ADDR + BTDM_MDM_GSG_DPHI_DEN_ADDR           )
#define BTDM_MDM_GSG_DPHI_NOM_ADDR_CT          		REG(MDM_BASE_ADDR + BTDM_MDM_GSG_DPHI_NOM_ADDR           )
#define BTDM_MDM_DSG_DEN_ADDR_CT               		REG(MDM_BASE_ADDR + BTDM_MDM_DSG_DEN_ADDR                )
#define BTDM_MDM_DSG_NOM_ADDR_CT               		REG(MDM_BASE_ADDR + BTDM_MDM_DSG_NOM_ADDR                )
#define BTDM_MDM_CORRELATION_CODE_BT_ADDR_CT   		REG(MDM_BASE_ADDR + BTDM_MDM_CORRELATION_CODE_BT_ADDR    )
#define BTDM_MDM_CORRELATION_CODE_BT_BLE_ADDR_CT	REG(MDM_BASE_ADDR + BTDM_MDM_CORRELATION_CODE_BT_BLE_ADDR )
#define BTDM_MDM_CORR_ERR_CNTL_ADDR_CT         		REG(MDM_BASE_ADDR + BTDM_MDM_CORR_ERR_CNTL_ADDR          )
#define BTDM_MDM_LR_CORR_THR_CNTL_ADDR_CT      		REG(MDM_BASE_ADDR + BTDM_MDM_LR_CORR_THR_CNTL_ADDR       )
#define BTDM_MDM_RXLPF_1M_C0_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_RXLPF_1M_C0_ADDR            )
#define BTDM_MDM_RXLPF_1M_C1_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_RXLPF_1M_C1_ADDR            )
#define BTDM_MDM_RXLPF_1M_C2_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_RXLPF_1M_C2_ADDR            )
#define BTDM_MDM_RXLPF_1M_C3_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_RXLPF_1M_C3_ADDR            )
#define BTDM_MDM_RXLPF_1M_C4_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_RXLPF_1M_C4_ADDR            )
#define BTDM_MDM_RXLPF_2M_C0_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_RXLPF_2M_C0_ADDR            )
#define BTDM_MDM_RXLPF_2M_C1_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_RXLPF_2M_C1_ADDR            )
#define BTDM_MDM_RXLPF_2M_C2_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_RXLPF_2M_C2_ADDR            )
#define BTDM_MDM_RXLPF_2M_C3_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_RXLPF_2M_C3_ADDR            )
#define BTDM_MDM_RXLPF_2M_C4_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_RXLPF_2M_C4_ADDR            )
#define BTDM_MDM_RXLPF_LR_C0_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_RXLPF_LR_C0_ADDR            )
#define BTDM_MDM_RXLPF_LR_C1_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_RXLPF_LR_C1_ADDR            )
#define BTDM_MDM_RXLPF_LR_C2_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_RXLPF_LR_C2_ADDR            )
#define BTDM_MDM_RXLPF_LR_C3_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_RXLPF_LR_C3_ADDR            )
#define BTDM_MDM_RXLPF_LR_C4_ADDR_CT           		REG(MDM_BASE_ADDR + BTDM_MDM_RXLPF_LR_C4_ADDR            )
#define BTDM_MDM_RXLPF_LRSEARCH_C0_ADDR_CT     		REG(MDM_BASE_ADDR + BTDM_MDM_RXLPF_LRSEARCH_C0_ADDR     )
#define BTDM_MDM_RXLPF_LRSEARCH_C1_ADDR_CT     		REG(MDM_BASE_ADDR + BTDM_MDM_RXLPF_LRSEARCH_C1_ADDR     )
#define BTDM_MDM_RXLPF_LRSEARCH_C2_ADDR_CT     		REG(MDM_BASE_ADDR + BTDM_MDM_RXLPF_LRSEARCH_C2_ADDR     )
#define BTDM_MDM_RXLPF_LRSEARCH_C3_ADDR_CT     		REG(MDM_BASE_ADDR + BTDM_MDM_RXLPF_LRSEARCH_C3_ADDR     )
#define BTDM_MDM_RXLPF_LRSEARCH_C4_ADDR_CT     		REG(MDM_BASE_ADDR + BTDM_MDM_RXLPF_LRSEARCH_C4_ADDR     ) 



typedef volatile struct {
	U8	 fm_dt      :4; 
	U8	 am_dt      :4;	
} OPTEKRF_FM_AM_DT;

typedef volatile struct {
	U8	 div_dt      :4; 
	U8	 dco_dt      :4;	
} OPTEKRF_DIV_DCO_DT;

typedef volatile struct {
    U8   tx_power    :7;        //max 0x7f
    U8   rx_power    :1;        //1->pa1 max 13dB, 0->pa0 max 4dB
} OPTEKRF_RX_POWER;

//OPTEKRF_PA_ANT_SEL_ADDR
typedef volatile struct {
    U8   tx_ant                 :1;                 //manual tx ant      
    U8   rx_ant                 :1;                 //manual rx ant
    U8   tx_dyn_ant             :1;                 //1'b0->tx ant by tx_ant, 1'b1->tx ant dynamic                     
    U8   rx_dyn_ant             :1;                 //1'b0->rx ant by rx_ant, 1'b1->rx ant dynamic
    U8   tx_toggle_ant          :1;                 //dynamic ant toggled by every tx                         
    U8   rx_toggle_ant          :1;                 //dynamic ant toggled by every rx
    U8   pa_sel                 :1;                 //1'b1->manual pa sel, 1'b0->ps sel from btdm ma      
    U8   pa                     :1;                 //manual pa sel
} OPTEKRF_PA_ANT_SEL;


#define AM_AM_ADDR		0x600
#define AM_HAM_ADDR		0xA00
#define AM_PM_ADDR		0xC00

//OPTEKRF_MDM_CTRL_ADDR

typedef volatile struct {
	U8    am_9b_14b     : 1;		    
	U8    adc_iq_swap   : 1;	
	U8    am_sdm_en     : 1;
	U8    dco_sdm_en    : 1;
	U8    rf_am_9b      : 1;
    U8    spi_miso_sel  : 1;
	U8    polar_fm_in   : 1;
    U8    rx_2m         : 1;                        //always seting rf ip rx in 2m mode  
} OPTEKRF_MDM_CTRL;

//OPTEKRF_RX_RFGA_EFFECT_T_ADDR
typedef volatile struct {
	U8    rf_ga_effect_t    : 5;	            //default 16 (16/12=1.2us)	    
	U8    rx_agc_avg_t      : 1;	            //default is 0
	U8    tx_rampup_en      : 1;                //default is 0
	U8    tx_rampdn_en      : 1;                //default is 0
} OPTEKRF_RX_RFGA_EFFECT_T;



#endif //__OPTEKRF_H__