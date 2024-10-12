#include <string.h>        // for memcpy
#include <stdint.h>        // for memcpy
#if 0
/**
****************************************************************************************
*
* @file rf_icytrxdm.c
*
* @brief IcyTRxDM radio initialization and specific functions
*
* Copyright (C) RivieraWaves 2009-2015
*
* $Rev: $
*
****************************************************************************************
*/

/**
****************************************************************************************
* @addtogroup RF_ICYDM
* @ingroup RF
* @brief IcyTRxDM Radio Driver
*
* This is the driver block for IcyTRxDM radio
* @{
****************************************************************************************
*/

/**
 *****************************************************************************************
 * INCLUDE FILES
 *****************************************************************************************
 */
#include <string.h>        // for memcpy
//#include "co_utils.h"      // common utility definition
//#include "co_math.h"       // common math functions
//#include "co_endian.h"     // endian definitions
#include "rf.h"            // RF interface
#include "em_map.h"        // RF area mapping

#include "rwip.h"          // for RF API structure definition
#include "reg_ipcore.h"    // DM core registers

#include "plf.h"           // Platform register
#include "flash.h"         // Flash interface
//#include "lcd.h"           // DBG interface
#include "reg_iqgen.h"     // Add DF Generator register h file here

#if (BLE_EMB_PRESENT)
#include "reg_blecore.h"   // ble core registers
#include "reg_em_ble_cs.h" // control structure definitions
#endif //BLE_EMB_PRESENT

#if (BT_EMB_PRESENT)
#include "reg_btcore.h"    // bt core registers
#include "reg_em_bt_cs.h"  // control structure definitions
#endif //BT_EMB_PRESENT

/**
****************************************************************************************
 * DEFINES
 ****************************************************************************************
 **/
#include "gallifrey_v3a/bt_dm/bt_dm.h"
#include "gallifrey_t22_v1a/bt_dm/bt_dm.h"

#define SET_DATA_IN_PARALLEL_WAY

#define RF_EM_SPI_ADRESS              (EM_BASE_ADDR + EM_RF_SW_SPI_OFFSET)

#define ICYDM_SPIRD                   0x00
#define ICYDM_SPIWR                   0x80

#define ICYDM_PWR_TBL_SIZE            0x10
#define ICYDM_PA_TBL_MSK              0x0F
#define ICYDM_PA_PWR_MSK              0x1F

#define ICYDM_MAX_BURST_SIZE          0x80
#define ICYDM_INIT_TBL_18_SIZE        0xD0
#define ICYDM_INIT_TBL_20_SIZE        0xFC
#define ICYDM_INIT_TBL_30_SIZE        0xF7

#define ICYDM_RSSI_20dB_THRHLD        -20
#define ICYDM_RSSI_40dB_THRHLD        -40
#define ICYDM_RSSI_45dB_THRHLD        -45
#define ICYDM_RSSI_48dB_THRHLD        -48
#define ICYDM_RSSI_55dB_THRHLD        -55
#define ICYDM_RSSI_60dB_THRHLD        -60
#define ICYDM_RSSI_70dB_THRHLD        -70


#define REG_ICYDM_RD                rf_icydm_reg_rd
#define REG_ICYDM_WR                rf_icydm_reg_wr

/* The offset value given below is the offset to add to the frequency table index to
   get the value to be programmed in the radio for each channel                      */
#define ICYDM_FREQTAB_OFFSET          0   // Offset for ICYDM radio

// TX max power
#define ICYDM_POWER_MIN                0x20
#define ICYDM_POWER_MAX                0xe0
#define ICYDM_POWER_STEP               0x20
#define ICYDM_POWER_MSK                0xE0

// icyTRxDM hash table
#define ICYDM_T22_V1A                  0x8779e61c 

// Workaround RSSI value issues identified in EBQ testing
#define RF_EBQ_RSSI_WORKAROUND          1
/**
****************************************************************************************
* MACROS
*****************************************************************************************
*/
// Max burst register
__STATIC uint8_t rf_icydm_reg_buf[ICYDM_MAX_BURST_SIZE + 8]; // max burst size + buffer controls


// IcyTRxDM register definitions and access functions
__STATIC uint32_t rf_icydm_reg_rd (uint32_t addr);
__STATIC void rf_icydm_reg_wr (uint32_t addr, uint32_t value);


/// IcyTRx EM Write Macro for HW driven SPI chained structures
#define RF_ICTRXDM_EM_WR(addr, val) \
    EM_WR((((uint32_t) (addr)) + REG_EM_ET_BASE_ADDR), (val))


/**
 ****************************************************************************************
 * GLOBAL VARIABLE DEFINITIONS
 *****************************************************************************************
 **/

 
/* --------------------------- TestChip gallifrey_v3a Init Tables ---------------------------------------------*/

#define RF_ICYDM_REG_TBL_18_INIT           GALLIFREY_V3A_MAIN_REGS
#define RF_ICYDM_REG_TBL_18_ADPLL          GALLIFREY_V3A_ADPLL_REGS
#define RF_ICYDM_REG_TBL_18_SEQUENCER      GALLIFREY_V3A_SEQ_CODE
#define RF_ICYDM_REG_TBL_18_TX_PKT_HANDLER GALLIFREY_V3A_TXPH_CODE
#define RF_ICYDM_REG_TBL_18_RX_PKT_HANDLER GALLIFREY_V3A_RXPH_CODE

__STATIC  const uint8_t RF_ICYDM_REG_TBL_18_GPIOS[] =
{
  0x1d,  0x1a,  0x01,  0x1b,
  0x1c,  0x29,  0x19,  0x16,
  0x17,  0x18
};

__STATIC  const uint8_t RF_ICYDM_REG_TBL_18_AGC[] =
{
    0x6b, 0x3f, 0xe9, 0x92,   0x1f, 0xf9, 0x9a, 0x9f,   0xf9, 0x83, 0x0f, 0xf9,   0x98, 0xcf, 0xf9, 0x82,     // 0x00*
    0x9f, 0xf5, 0x01, 0x0a,   0xc0, 0xd1, 0x2f, 0xf9,   0x81, 0xaf, 0xf9, 0x89,   0x9f, 0xf7, 0x00, 0x3c,     // 0x00*
    0xf8, 0x90, 0x8f, 0xf9,   0x9c, 0x0f, 0xc0, 0xd1,   0x3f, 0xd0, 0x32, 0x2a,   0x8c, 0xfa, 0xfa, 0xf9,     // 0x00*
    0xc8, 0xbf, 0xfd, 0x98,   0xcf, 0xf9, 0xc4, 0x0f,   0xc0, 0x05, 0xaa, 0x87,   0x00, 0x2c, 0xa1, 0xd1,     // 0x00*
    0xaf, 0xaf, 0x8f, 0x0f,   0xdc, 0x1b, 0xf0, 0xdd,   0xf0, 0xf8, 0xf9, 0xe2,   0x08, 0xdd, 0x81, 0x9f,     // 0x00*
    0x87, 0x05, 0xaa, 0x87,   0xd8, 0xcf, 0xe1, 0x0c,   0x0d, 0xc0, 0x18, 0x8e,   0xd0, 0x00, 0x0c, 0xe1,     // 0x00*
    0x00, 0x0d, 0xc0, 0x40,   0x1c, 0xfd, 0x8f, 0xaf,   0xa7, 0xc8, 0xdd, 0x02,   0xdf, 0x8d, 0x8f, 0x9f,     // 0x00*
    0x1f, 0x51, 0x05, 0x8a,   0xfd, 0xd1, 0xff, 0xf8,   0x3a, 0xca, 0xdc, 0x45,   0xf0, 0xdc, 0xfc, 0x18,     // 0x00*
    0x51, 0x09, 0x0a, 0xc0,   0x6a, 0xae, 0xd1, 0x05,   0x0a, 0xc0, 0x7a, 0x41,   0xc0, 0x1a, 0x9d, 0xa0,     // 0x00*
    0x00, 0xac, 0x13, 0x04,   0xac, 0xd1, 0x0d, 0x0a,   0xc0, 0x8a, 0x0f, 0xc0,   0x00, 0x00, 0x00, 0x00,     // 0x00*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x00*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x00*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x00*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x00*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x00*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x0e, 0x00, 0x00, 0x00,   0x00, 0x15, 0x00, 0x00,     // 0x01*
    0x00, 0x00, 0x00, 0x00,   0x1a, 0x29, 0x2a, 0x43,   0x4e, 0x39, 0x00, 0x58,   0x00, 0x00, 0x00, 0x00,     // 0x01*
    0x00, 0x01, 0x00, 0x00,   0x1c, 0x0d, 0x0d, 0x28,   0x0a, 0x00, 0x00, 0x00
};

/* --------------------------- TestChip gallifrey_t22_v1a Init Tables ---------------------------------------------*/

#define RF_ICYDM_REG_TBL_T22V1A_INIT           GALLIFREY_T22_V1A_MAIN_REGS
#define RF_ICYDM_REG_TBL_T22V1A_ADPLL          GALLIFREY_T22_V1A_ADPLL_REGS
#define RF_ICYDM_REG_TBL_T22V1A_SEQUENCER      GALLIFREY_T22_V1A_SEQ_CODE
#define RF_ICYDM_REG_TBL_T22V1A_TX_PKT_HANDLER GALLIFREY_T22_V1A_TXPH_CODE
#define RF_ICYDM_REG_TBL_T22V1A_RX_PKT_HANDLER GALLIFREY_T22_V1A_RXPH_CODE

__STATIC  const uint8_t RF_ICYDM_REG_TBL_T22V1A_GPIOS[] =
{
  0x1d,  0x1a,  0x01,  0x1b,
  0x1c,  0x29,  0x19,  0x16,
  0x17,  0x18
};

__STATIC  const uint8_t RF_ICYDM_REG_TBL_T22V1A_AGC[] =
{
  0x6b, 0x3f, 0xe9, 0x92,   0x1f, 0xf9, 0x9a, 0x9f,   0xf9, 0x83, 0x0f, 0xf8,   0x98, 0xcf, 0xf9, 0x84,     // 0x00*
  0x2f, 0xf8, 0x59, 0x1f,   0xa0, 0x00, 0x1c, 0xfd,   0x92, 0x1f, 0xf8, 0x9a,   0x9f, 0xf8, 0x79, 0x0f,     // 0x00*
  0xc0, 0x83, 0x0f, 0xf9,   0x98, 0xcf, 0xf9, 0x00,   0x1c, 0xfd, 0x43, 0xe1,   0xc0, 0x32, 0x2a, 0x8c,     // 0x00*
  0xfa, 0xfa, 0xf9, 0xc8,   0xbf, 0xfd, 0x98, 0xcf,   0xf9, 0xc4, 0x0f, 0xc0,   0x05, 0xaa, 0x87, 0x00,     // 0x00*
  0x0c, 0xf8, 0x03, 0xfd,   0xc6, 0x54, 0x2c, 0xa1,   0xd1, 0xaf, 0xaf, 0x8f,   0x0f, 0x13, 0x02, 0xcc,     // 0x00*
  0x01, 0xdf, 0x0d, 0xd0,   0x9f, 0x2f, 0x8e, 0x05,   0xaa, 0x87, 0xd8, 0x1f,   0xfd, 0xfa, 0xfa, 0xf8,     // 0x00*
  0x04, 0x2e, 0xc0, 0x71,   0xfc, 0xdd, 0x04, 0xfd,   0xf9, 0x7a, 0x58, 0xa0,   0xd8, 0x1f, 0xfd, 0xfa,     // 0x00*
  0x5a, 0xa0, 0x1c, 0xce,   0xd0, 0x00, 0x8c, 0xe1,   0x08, 0x0d, 0xc0, 0x94,   0x4d, 0x8f, 0x00, 0x0c,     // 0x00*
  0xd9, 0xf0, 0x08, 0xc0,   0x56, 0x1c, 0xfd, 0x8f,   0xaf, 0xa7, 0xc8, 0x3d,   0x03, 0xdf, 0x8d, 0x8f,     // 0x00*
  0x9f, 0x1f, 0x51, 0x05,   0x8a, 0xfd, 0xd1, 0xff,   0xf8, 0x3a, 0xca, 0xdc,   0x5b, 0xf0, 0xdc, 0xfc,     // 0x00*
  0x18, 0x51, 0x09, 0x0a,   0xc0, 0x6a, 0xae, 0xd1,   0x05, 0x0a, 0xc0, 0x7a,   0x41, 0xc0, 0x1a, 0x9d,     // 0x00*
  0xa0, 0x00, 0xac, 0x13,   0x04, 0xac, 0xd1, 0x0d,   0x0a, 0xc0, 0x8a, 0x0f,   0xc0, 0x00, 0x00, 0x00,     // 0x00*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x00*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x00*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x00*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,     // 0x01*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x0f, 0x00, 0x00, 0x00,   0x00, 0x16, 0x00, 0x00,     // 0x01*
  0x00, 0x00, 0x00, 0x00,   0x1b, 0x2f, 0x3d, 0x59,   0x64, 0x49, 0x00, 0x6e,   0x00, 0x00, 0x00, 0x00,     // 0x01*
  0x00, 0x01, 0x00, 0x00,   0x23, 0x10, 0x10, 0x32,   0x0d, 0x00, 0x00, 0x00
};

/* IcyTRx dynamic register */
enum
{
    ICYDM_RATE_TX        = 0x800,
    ICYDM_CHANNEL_RM_TX  = 0x801,
    ICYDM_PA_PWR_RM      = 0x802,
    ICYDM_FSM_MODE_RM_TX = 0x803,
    ICYDM_RATE_RX        = 0x810,
    ICYDM_CHANNEL_RM_RX  = 0x811,
    ICYDM_FSM_MODE_RM_RX = 0x812,
    ICYDM_SYNC_WORD_LOW  = 0x813,
    ICYDM_SYNC_WORD_HIGH = 0x817,
    ICYDM_RX_POWER_OFF   = 0x820,
    ICYDM_RSSI_READYOUT  = 0x824,
    ICYDM_PACKET_LEN     = 0x828
};

// Power table - ref. Table 10, icyTRXDigital_architecture_3v3.docx
__STATIC const int8_t RF_ICYDM_TX_PW_CONV_TBL[ICYDM_PWR_TBL_SIZE] = {
        [0xD/*-3*/] = -40,
        [0xE/*-2*/] = -35,
        [0xF/*-1*/] = -30,
        [0] = -18,
        [1] = -16,
        [2] = -14,
        [3] = -13,
        [4] = -11,
        [5] = -10,
        [6] = -8,
        [7] = -7,
        [8] = -5,
        [9] = -4,
        [10] = -2,
        [11] = -1,
        [12] = 1,
};

/**
 ****************************************************************************************
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief SPI access
 ***************************************************************************************
 */
__STATIC void rf_icydm_spi_tf(void)
{
    //launch SPI transfer
    ip_radiocntl0_spigo_setf(1);

    //wait for transfer to be completed
    while (!ip_radiocntl0_spicomp_getf());
}

/**
 ****************************************************************************************
 * @brief IcyTRxDM specific read access
 *
 * @param[in] addr    register address
 *
 * @return uint32_t value
 *****************************************************************************************
 */
__STATIC uint32_t rf_icydm_reg_rd (uint32_t addr)
{
    // Next Pointr to 0x0
    rf_icydm_reg_buf[0] = (uint8_t)(0);  // Next Pointer set to 0x0000 to stop the SPI Chained access
    rf_icydm_reg_buf[1] = (uint8_t)(0);

    //copy control and number of u32 to send
    rf_icydm_reg_buf[2] = (uint8_t)(ICYDM_SPIRD + 1);

    //copy address
    rf_icydm_reg_buf[3] = (uint8_t)(addr & 0x00FF);
    rf_icydm_reg_buf[4] = (uint8_t)((addr & 0xFF00) >> 8 );

    // Padding
    rf_icydm_reg_buf[5] = (uint8_t)(0);
    rf_icydm_reg_buf[6] = (uint8_t)(0);
    rf_icydm_reg_buf[7] = (uint8_t)(0);

    memcpy((void *)RF_EM_SPI_ADRESS, rf_icydm_reg_buf, 8);

    //do the transfer
    rf_icydm_spi_tf();

    return (uint32_t)(*((uint8_t *)(RF_EM_SPI_ADRESS + 8)));
}

/**
 ****************************************************************************************
 * @brief IcyTRxDM specific read access
 *
 * @param[in] addr    register address
 * @param[in] size    read size
 * @param[in] data    buf to store data
 *
 * @return uint32_t value
 ****************************************************************************************
 **/
__STATIC void rf_icydm_reg_burst_rd (uint16_t addr, uint8_t size, uint8_t *buf)
{
    rf_icydm_reg_buf[0] = (uint8_t)(0);  // Next Pointer set to 0x0000 to stop the SPI Chained access
    rf_icydm_reg_buf[1] = (uint8_t)(0);

    //copy control and number of u8 to read
    rf_icydm_reg_buf[2] = (uint8_t)(ICYDM_SPIWR + size);

    //copy address
    rf_icydm_reg_buf[3] = (uint8_t)(addr & 0x00FF);
    rf_icydm_reg_buf[4] = (uint8_t)((addr & 0xFF00) >> 8 );

    // Padding
    rf_icydm_reg_buf[5] = (uint8_t)(0);
    rf_icydm_reg_buf[6] = (uint8_t)(0);
    rf_icydm_reg_buf[7] = (uint8_t)(0);

    memcpy((void *)RF_EM_SPI_ADRESS, rf_icydm_reg_buf, 8);

    //do the transfer
    rf_icydm_spi_tf();

    for(int i =0;i < size;i++)//for(int i =0;i < size + 2;i++)
    {
      buf[i] = *((uint8_t *)(RF_EM_SPI_ADRESS + 8 + i));
    }    
}

/**
 ****************************************************************************************
 * @brief IcyTRxDM specific write access
 *
 * @param[in] addr    register address
 * @param[in] value   value to write
 *
 * @return uint32_t value
 ****************************************************************************************
 */
__STATIC void rf_icydm_reg_wr (uint32_t addr, uint32_t value)
{
    rf_icydm_reg_buf[0] = (uint8_t)(0);  // Next Pointer set to 0x0000 to stop the SPI Chained access
    rf_icydm_reg_buf[1] = (uint8_t)(0);

    //inversion for EM reading by U8 on ATL SPI side
    //copy control and number of u32 to send
    rf_icydm_reg_buf[2] = (uint8_t)(ICYDM_SPIWR + 1);

    //copy address
    rf_icydm_reg_buf[3] = (uint8_t)(addr & 0x00FF);
    rf_icydm_reg_buf[4] = (uint8_t)((addr & 0xFF00) >> 8 );

    // Padding
    rf_icydm_reg_buf[5] = (uint8_t)(0);
    rf_icydm_reg_buf[6] = (uint8_t)(0);
    rf_icydm_reg_buf[7] = (uint8_t)(0);

    // Byte to be written
    rf_icydm_reg_buf[8]= (uint8_t)value;

    memcpy((void *)RF_EM_SPI_ADRESS, rf_icydm_reg_buf, 9);

    //do the transfer
    rf_icydm_spi_tf();
}

#if 1
  void rf_icydm_reg_32bit_wr (uint32_t addr, uint32_t value)
{
    unsigned short size = 4;

    rf_icydm_reg_buf[0] = (uint8_t)(0);  // Next Pointer set to 0x0000 to stop the SPI Chained access
    rf_icydm_reg_buf[1] = (uint8_t)(0);

    //inversion for EM reading by U8 on ATL SPI side
    //copy control and number of u32 to send
    rf_icydm_reg_buf[2] = (uint8_t)(ICYDM_SPIWR + size);

    //copy address
    rf_icydm_reg_buf[3] = (uint8_t)(addr & 0x00FF);
    rf_icydm_reg_buf[4] = (uint8_t)((addr & 0xFF00) >> 8 );

    // Padding
    rf_icydm_reg_buf[5] = (uint8_t)(0);
    rf_icydm_reg_buf[6] = (uint8_t)(0);
    rf_icydm_reg_buf[7] = (uint8_t)(0);

    #if 1
    for(int i = 0;i < size;i++)//for(int i = 0;i < size + 2;i++)
    {
        rf_icydm_reg_buf[i + 8] = (uint8_t)(value >> (8 * i));
    }
    #else
    // Byte to be written
    rf_icydm_reg_buf[8] = (uint8_t)(value>>24);
    rf_icydm_reg_buf[9] = (uint8_t)(value>>16);
    rf_icydm_reg_buf[10] = (uint8_t)(value>>8);
    rf_icydm_reg_buf[11] = (uint8_t)value;
    #endif

    memcpy((void *)RF_EM_SPI_ADRESS, rf_icydm_reg_buf, (8 + size));

    //do the transfer
    rf_icydm_spi_tf();
}
#endif

/**
 ****************************************************************************************
 * @brief IcyTRxDM specific read access
 *
 * @param[in] addr    register address
 * @param[in] size    transfer size
 * @param[in] data    pointer to the data array
 *
 * @return uint32_t value
 ****************************************************************************************
 **/
__STATIC void rf_icydm_reg_burst_wr (uint16_t addr, uint8_t size, uint8_t *data)
{
    rf_icydm_reg_buf[0] = (uint8_t)(0);  // Next Pointer set to 0x0000 to stop the SPI Chained access
    rf_icydm_reg_buf[1] = (uint8_t)(0);

    //copy control and number of u8 to send
    rf_icydm_reg_buf[2] = (uint8_t)(ICYDM_SPIWR + size);

    //copy address
    rf_icydm_reg_buf[3] = (uint8_t)(addr & 0x00FF);
    rf_icydm_reg_buf[4] = (uint8_t)((addr & 0xFF00) >> 8 );

    // Padding
    rf_icydm_reg_buf[5] = (uint8_t)(0);
    rf_icydm_reg_buf[6] = (uint8_t)(0);
    rf_icydm_reg_buf[7] = (uint8_t)(0);

    for(int i =0;i < size + 2;i++)
    {
        rf_icydm_reg_buf[i + 8] = *(data + i);
    }

    memcpy((void *)RF_EM_SPI_ADRESS, rf_icydm_reg_buf, 8 + size);

    //do the transfer
    rf_icydm_spi_tf();
}

/**
 *****************************************************************************************
 * @brief Init Frequency Table.in Exchange Memory
 *****************************************************************************************
 */
static void rf_em_init(void)
{
    uint8_t idx = 0;
    uint8_t temp_freq_tbl[EM_RF_FREQ_TABLE_LEN];

#if BT_EMB_PRESENT
    // First half part of frequency table is for the even frequencies
    while(idx < EM_RF_FREQ_TABLE_LEN/2)
    {
        temp_freq_tbl[idx] = 2*idx + ICYDM_FREQTAB_OFFSET;
        idx++;
    }
    while(idx < EM_RF_FREQ_TABLE_LEN)
    {
        temp_freq_tbl[idx] = 2*(idx-(EM_RF_FREQ_TABLE_LEN/2)) + 1 + ICYDM_FREQTAB_OFFSET;
        idx++;
    }
#elif BLE_EMB_PRESENT
    while(idx < EM_RF_FREQ_TABLE_LEN)
    {
        temp_freq_tbl[idx] = 2*idx + ICYDM_FREQTAB_OFFSET;
        idx++;
    }
#endif // BT_EMB_PRESENT/BLE_EMB_PRESENT
    em_wr(&temp_freq_tbl[0], EM_FT_OFFSET, EM_RF_FREQ_TABLE_LEN);
}

/**
 *****************************************************************************************
 * @brief Init RF sequence after reset.
 *****************************************************************************************
 */
__STATIC void rf_reset(void)
{
}

/**
 ****************************************************************************************
 * @brief ISR to be called in BLE ISR routine when RF Interrupt occurs.
 *****************************************************************************************
 */
__STATIC void rf_force_agc_enable(bool en)
{
}

/**
 *****************************************************************************************
 * @brief Get TX power in dBm from the index in the control structure
 *
 * @param[in] txpwr_idx  Index of the TX power in the control structure
 * @param[in] modulation Modulation: 1 or 2 or 3 MBPS
 *
 * @return The TX power in dBm
 *
 *****************************************************************************************
 */
__STATIC int8_t rf_txpwr_dbm_get(uint8_t txpwr_idx, uint8_t modulation)
{
    // power table
    return(RF_ICYDM_TX_PW_CONV_TBL[txpwr_idx & ICYDM_PA_TBL_MSK]);
}

/**
 *****************************************************************************************
 * @brief Sleep function for IcyTRxDM RF.
 *****************************************************************************************
 */
__STATIC void rf_sleep(void)
{
    ip_deepslcntl_set(ip_deepslcntl_get() |
                      IP_DEEP_SLEEP_ON_BIT |    // RW BLE Core sleep
                      IP_RADIO_SLEEP_EN_BIT |   // Radio sleep
                      IP_OSC_SLEEP_EN_BIT);     // Oscillator sleep
}

/**
 *****************************************************************************************
 * @brief Convert RSSI to dBm
 *
 * @param[in] rssi_reg RSSI read from the HW registers
 *
 * @return The converted RSSI
 *
 *****************************************************************************************
 */
__STATIC int8_t rf_rssi_convert (uint8_t rssi_reg)
{
    int8_t RssidBm = 0;
    RssidBm = ((rssi_reg) >> 1) - 118;
    #if RF_EBQ_RSSI_WORKAROUND
    RssidBm += 20;
    #endif // RF_EBQ_RSSI_WORKAROUND
    return(RssidBm);
}

#if  defined(CFG_BT_EMB)
/**
 *****************************************************************************************
 * @brief Decrease the TX power by one step
 *
 * @param[in] link_id Link ID for which the TX power has to be decreased
 *
 * @return true when minimum power is reached, false otherwise
 *****************************************************************************************
 */
__STATIC bool rf_txpwr_dec(uint8_t link_id)
{
    bool boMinpow = true;
    uint8_t tx_pwr = em_bt_pwrcntl_txpwr_getf(EM_BT_CS_ACL_INDEX(link_id)) & ICYDM_POWER_MSK;

    if (tx_pwr > ICYDM_POWER_MIN)
    {
        //Increase the TX power value
        if (tx_pwr > (ICYDM_POWER_MIN+ICYDM_POWER_STEP))
          tx_pwr = tx_pwr-ICYDM_POWER_STEP;
        else
          tx_pwr = ICYDM_POWER_MIN;
        em_bt_pwrcntl_txpwr_setf(EM_BT_CS_ACL_INDEX(link_id), tx_pwr);
        boMinpow = false;
    }

    return(boMinpow);
}

/**
 *****************************************************************************************
 * @brief Increase the TX power by one step
 *
 * @param[in] link_id Link ID for which the TX power has to be increased
 *
 * @return true when maximum power is reached, false otherwise
 *****************************************************************************************
 */
__STATIC bool rf_txpwr_inc(uint8_t link_id)
{
    bool boMaxpow = true;
    uint8_t tx_pwr = em_bt_pwrcntl_txpwr_getf(EM_BT_CS_ACL_INDEX(link_id)) & ICYDM_POWER_MSK;

    if (tx_pwr < ICYDM_POWER_MAX)
    {
        //Increase the TX power value
        if (tx_pwr < (ICYDM_POWER_MAX-ICYDM_POWER_STEP))
            tx_pwr += ICYDM_POWER_STEP;
        else
            tx_pwr = ICYDM_POWER_MAX;
        em_bt_pwrcntl_txpwr_setf(EM_BT_CS_ACL_INDEX(link_id), tx_pwr);
        boMaxpow = false;
    }

    return(boMaxpow);
}

/**
 ****************************************************************************************
 * @brief Set the TX power to max
 *
 * @param[in] link_id     Link Identifier
 ****************************************************************************************
 */
__STATIC void txpwr_max_set(uint8_t link_id)
{
    //Increase the TX power value
    em_bt_pwrcntl_txpwr_setf(EM_BT_CS_ACL_INDEX(link_id), ICYDM_POWER_MAX);
}
#endif //CFG_BT_EMB

/**
 *****************************************************************************************
 * @brief Get the TX power as control structure TX power field from a value in dBm.
 *
 * @param[in] txpwr_dbm   TX power in dBm
 * @param[in] option      If TXPWR_CS_LOWER, return index equal to or lower than requested
 *                        If TXPWR_CS_HIGHER, return index equal to or higher than requested
 *                        If TXPWR_CS_NEAREST, return index nearest to the desired value
 *
 * @return The index of the TX power
 *
 *****************************************************************************************
 */
__STATIC uint8_t rf_txpwr_cs_get (int8_t txpwr_dbm, uint8_t option)
{
    ASSERT_ERR(option <= TXPWR_CS_NEAREST);

    uint8_t i;

    for (i = ICYDM_POWER_MIN; i < ICYDM_POWER_MAX; i++)
    {
        // Loop until we find a power higher than or equal to the requested one
        if (RF_ICYDM_TX_PW_CONV_TBL[i & ICYDM_PA_TBL_MSK] >= txpwr_dbm)
            break;
    }

    if ((RF_ICYDM_TX_PW_CONV_TBL[i & ICYDM_PA_TBL_MSK] > txpwr_dbm) && (i > ICYDM_POWER_MIN))
    {
        if (    (option == TXPWR_CS_LOWER)
            || ((option == TXPWR_CS_NEAREST) && (co_abs(txpwr_dbm - RF_ICYDM_TX_PW_CONV_TBL[(i - 1) & ICYDM_PA_TBL_MSK]) < co_abs(txpwr_dbm - RF_ICYDM_TX_PW_CONV_TBL[i & ICYDM_PA_TBL_MSK])))   )
        {
            i--;
        }
    }

    return(i & ICYDM_PA_PWR_MSK);
}

/**
 ****************************************************************************************
 * RADIO FUNCTION INTERFACE
 ****************************************************************************************
 **/
#include "gallifrey_v3a_reg_map.h"
#include "gallifrey_t22_v1a_reg_map.h"
#define DIS_RRMU_GAL_V3A \
  rf_icydm_set_field((icydm_field)GALLIFREY_V3A_DIG_RRMU_SPI_FORWARD_FIELD, 0); \
  rf_icydm_set_field((icydm_field)GALLIFREY_V3A_DIG_IFACE_OCTA_SPI_SLV_IS_SYNC_FIELD, 1) 
#define EN_RRMU_GAL_V3A \
  rf_icydm_set_field((icydm_field)GALLIFREY_V3A_DIG_RRMU_SPI_FORWARD_FIELD, 1); \
  rf_icydm_set_field((icydm_field)GALLIFREY_V3A_DIG_IFACE_OCTA_SPI_SLV_IS_SYNC_FIELD, 0) 
#define DIS_RRMU_GAL_T22V1A \
  rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_DIG_RRMU_SPI_FORWARD_FIELD, 0); \
  rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_DIG_IFACE_OCTA_SPI_SLV_IS_SYNC_FIELD, 1) 
#define EN_RRMU_GAL_T22V1A \
  rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_DIG_RRMU_SPI_FORWARD_FIELD, 1); \
  rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_DIG_IFACE_OCTA_SPI_SLV_IS_SYNC_FIELD, 0) 

typedef struct icydm_field_struct {
  int pos;
  int len;
  short cst;
} icydm_field;

void rf_icydm_set_field(icydm_field f,int data){
    int mask;
    int data_l;
    int addr;
    if(f.cst==1)
        return;
    data_l = data<<(f.pos%8);
    addr   = f.pos>>3;
    mask = ((1<<f.len)-1)<<(f.pos%8);
    while(mask!=0){
        uint8_t mask_b;
        mask_b = (uint8_t)(mask & 0xff);
        if((mask_b)!=0){
            uint8_t val = 0;
            if((mask_b)!=0xff)
                val = rf_icydm_reg_rd(addr);
            val = (val&(0xff^mask_b))+(mask_b&((uint8_t)data_l));
            rf_icydm_reg_wr(addr,val); 
        }
        data_l >>=8;
        mask >>=8;
        addr++;
    }
}

int rf_icydm_read_field(icydm_field f){
    int mask;
    int data_l;
    int addr;
    uint8_t k;
    if(f.cst==1)
        return 0;
    data_l = 0;
    addr   = f.pos>>3;
    mask = ((1<<f.len)-1)<<(f.pos%8);
    k = 0;
    while(mask!=0){
        uint8_t mask_b;
        mask_b = (uint8_t)(mask & 0xff);
        if((mask_b)!=0){
            uint8_t val = 0;
            val = rf_icydm_reg_rd(addr);
            data_l |= ((int)val)<<(8*k);
        }
        mask >>=8;
        addr++;
        k++;
    }
    data_l = data_l>>(f.pos%8);
    return data_l;
}

void rf_calib_adpll_v3a()
{
   uint16_t kdco[11];
   uint16_t kdtc[11];
   uint16_t kdco_avg=0;
   uint16_t kdtc_avg=0;

   // initial calibration
   EN_RRMU_GAL_V3A; 
   rf_icydm_set_field((icydm_field)GALLIFREY_V3A_ADPLLDIG_TMOD_FIELD, 15); 
   rf_icydm_reg_wr(GALLIFREY_V3A_ADPLLDIG_DTC_CONFIG_ADDR,0x3e); 
   rf_icydm_set_field((icydm_field)GALLIFREY_V3A_ADPLLDIG_KDCOESTDEV_FIELD, 3); 
   rf_icydm_set_field((icydm_field)GALLIFREY_V3A_ADPLLDIG_KDCOCALRX_FIELD, 1); 
   rf_icydm_set_field((icydm_field)GALLIFREY_V3A_ADPLLDIG_UPDATE_KDCOHF_IC_FIELD, 1); 
   rf_icydm_set_field((icydm_field)GALLIFREY_V3A_ADPLLDIG_HOLD_STATE_FIELD, 8); 
   DIS_RRMU_GAL_V3A; 
   rf_icydm_reg_wr(GALLIFREY_V3A_COMMANDS_ADDR,0x12); 
   for(uint32_t cpt=0;cpt<1000;cpt++); //~ 1ms wait
   EN_RRMU_GAL_V3A; 
   rf_icydm_set_field((icydm_field)GALLIFREY_V3A_ADPLLDIG_HOLD_STATE_FIELD, 15); 
   DIS_RRMU_GAL_V3A; 
   rf_icydm_reg_wr(GALLIFREY_V3A_COMMANDS_ADDR,0x13); 
   EN_RRMU_GAL_V3A; 
   rf_icydm_reg_wr(GALLIFREY_V3A_ADPLLDIG_DTC_CONFIG_ADDR,0x2e); 
   rf_icydm_set_field((icydm_field)GALLIFREY_V3A_ADPLLDIG_PHRDLY_FIELD, 3); 
   rf_icydm_set_field((icydm_field)GALLIFREY_V3A_ADPLLDIG_INV_CKTDC_FIELD, 1); 
   rf_icydm_set_field((icydm_field)GALLIFREY_V3A_ADPLLDIG_KDTCCALLG_FIELD, 3); 
   rf_icydm_set_field((icydm_field)GALLIFREY_V3A_ADPLLDIG_KDCOESTDEV_FIELD, 0); 

   // calibrate channels
   for(int k=0;k<11;k++){
     rf_icydm_reg_wr(GALLIFREY_V3A_ADPLLDIG_CHANNEL_REG_ADDR,k*8); 
     DIS_RRMU_GAL_V3A; 
     rf_icydm_reg_wr(GALLIFREY_V3A_COMMANDS_ADDR,0x12); 
     for(uint32_t cpt=0;cpt<100;cpt++); //~ 1ms wait
     rf_icydm_reg_wr(GALLIFREY_V3A_COMMANDS_ADDR,0x13); 
     EN_RRMU_GAL_V3A; 
     kdco[k]  =(uint16_t)rf_icydm_read_field((icydm_field)GALLIFREY_V3A_ADPLLDIG_KDCO_HF_FIELD);
     kdco_avg += kdco[k];
     kdtc[k]  =(uint16_t)rf_icydm_read_field((icydm_field)GALLIFREY_V3A_ADPLLDIG_KDTC_OUT_FIELD);
     kdtc_avg += kdtc[k];
   }
   kdtc_avg /= 11;
   kdco_avg /= 11;

   rf_icydm_set_field((icydm_field)GALLIFREY_V3A_ADPLLDIG_KDCOHFIC_FIELD, kdco_avg); 
   rf_icydm_set_field((icydm_field)GALLIFREY_V3A_ADPLLDIG_KDTCIC_FIELD, kdtc_avg); 

   for(int k=0;k<11;k++){
     rf_icydm_reg_wr(GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_LUT_0_ADDR+k,0x1f&(kdco[k]-kdco_avg));
     rf_icydm_reg_wr(GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_LUT_0_ADDR+k,0x1f&(kdtc[k]-kdtc_avg));
   }
   rf_icydm_set_field((icydm_field)GALLIFREY_V3A_ADPLLDIG_UPDATE_KDCOHF_IC_FIELD, 0); 
   rf_icydm_set_field((icydm_field)GALLIFREY_V3A_ADPLLDIG_KDCOCALTX_FIELD, 0); 
   rf_icydm_set_field((icydm_field)GALLIFREY_V3A_ADPLLDIG_UPDATE_KDTC_IC_FIELD, 0); 
   rf_icydm_set_field((icydm_field)GALLIFREY_V3A_ADPLLDIG_KDTCCALEN_FIELD, 0); 
   rf_icydm_set_field((icydm_field)GALLIFREY_V3A_ADPLLDIG_TMOD_FIELD, 10); 
   rf_icydm_set_field((icydm_field)GALLIFREY_V3A_ADPLLDIG_KDCO_LUT_E_FIELD, 1); 
   rf_icydm_set_field((icydm_field)GALLIFREY_V3A_ADPLLDIG_KDTC_LUT_E_FIELD, 1); 
   DIS_RRMU_GAL_V3A;
}

void rf_calib_adpll_t22_v1a()
{
   uint16_t kdco[11];
   uint16_t kdtc[11];
   uint16_t kdco_avg=0;
   uint16_t kdtc_avg=0;

   // initial calibration
   EN_RRMU_GAL_T22V1A; 
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_FILTER_MOD_TC_FIELD, 15); 
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_E_FIELD, 1); 
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_MODE_FIELD, 1); 
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_INV_FIELD, 1); 
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_PHR_DLY_FIELD, 2); 
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_LOOP_GAIN_FIELD, 6); 
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_UPDATE_IC_FIELD, 1); 

   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_EST_DEV_FIELD, 3); 
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_POS_NEG_FIELD, 1); 
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_RX_E_FIELD, 1); 
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_UPDATE_IC_FIELD, 1); 
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_SYSTEM_HOLD_STATE_FIELD, 8); 
   DIS_RRMU_GAL_T22V1A; 
   rf_icydm_reg_wr(GALLIFREY_T22_V1A_COMMANDS_ADDR,0x12); 
   for(uint32_t cpt=0;cpt<1000;cpt++); //~ 1ms wait
   EN_RRMU_GAL_T22V1A; 
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_SYSTEM_HOLD_STATE_FIELD, 15); 
   DIS_RRMU_GAL_T22V1A; 
   rf_icydm_reg_wr(GALLIFREY_T22_V1A_COMMANDS_ADDR,0x13); 
   EN_RRMU_GAL_T22V1A; 

   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_E_FIELD, 1); 
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_MODE_FIELD, 0); 
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_PHR_DLY_FIELD, 3); 
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_CLOCKS_INV_CK_TDC_FIELD, 1); 
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_UPDATE_IC_FIELD, 1); 
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_EST_DEV_FIELD, 0); 
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_LOOP_GAIN_FIELD, 4); 

   // calibrate channels
   for(int k=0;k<11;k++){
     rf_icydm_reg_wr(GALLIFREY_T22_V1A_ADPLLDIG_CHANNEL_REG_ADDR,k*8); 
     DIS_RRMU_GAL_T22V1A; 
     rf_icydm_reg_wr(GALLIFREY_T22_V1A_COMMANDS_ADDR,0x12); 
     for(uint32_t cpt=0;cpt<100;cpt++); //~ 1ms wait
     rf_icydm_reg_wr(GALLIFREY_T22_V1A_COMMANDS_ADDR,0x13); 
     EN_RRMU_GAL_T22V1A; 
     kdco[k]  =(uint16_t)rf_icydm_read_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDCO_HF_FIELD);
     kdco_avg += kdco[k];
     kdtc[k]  =(uint16_t)rf_icydm_read_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_OUT_FIELD);
     kdtc_avg += kdtc[k];
   }
   kdtc_avg /= 11;
   kdco_avg /= 11;

   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_HF_IC_FIELD, kdco_avg+4); 
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_IC_FIELD, kdtc_avg); 

   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_KDCO_COEF_0_FIELD,0x1f&(kdco[0]-kdco_avg));
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_KDCO_COEF_1_FIELD,0x1f&(kdco[1]-kdco_avg));
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_KDCO_COEF_2_FIELD,0x1f&(kdco[2]-kdco_avg));
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_KDCO_COEF_3_FIELD,0x1f&(kdco[3]-kdco_avg));
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_KDCO_COEF_4_FIELD,0x1f&(kdco[4]-kdco_avg));
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_KDCO_COEF_5_FIELD,0x1f&(kdco[5]-kdco_avg));
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_KDCO_COEF_6_FIELD,0x1f&(kdco[6]-kdco_avg));
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_KDCO_COEF_7_FIELD,0x1f&(kdco[7]-kdco_avg));
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_KDCO_COEF_8_FIELD,0x1f&(kdco[8]-kdco_avg));
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_KDCO_COEF_9_FIELD,0x1f&(kdco[9]-kdco_avg));
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_KDCO_COEF_10_FIELD,0x1f&(kdco[10]-kdco_avg));
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_KDTC_COEF_0_FIELD,0x1f&(kdtc[0]-kdtc_avg));
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_KDTC_COEF_1_FIELD,0x1f&(kdtc[1]-kdtc_avg));
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_KDTC_COEF_2_FIELD,0x1f&(kdtc[2]-kdtc_avg));
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_KDTC_COEF_3_FIELD,0x1f&(kdtc[3]-kdtc_avg));
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_KDTC_COEF_4_FIELD,0x1f&(kdtc[4]-kdtc_avg));
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_KDTC_COEF_5_FIELD,0x1f&(kdtc[5]-kdtc_avg));
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_KDTC_COEF_6_FIELD,0x1f&(kdtc[6]-kdtc_avg));
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_KDTC_COEF_7_FIELD,0x1f&(kdtc[7]-kdtc_avg));
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_KDTC_COEF_8_FIELD,0x1f&(kdtc[8]-kdtc_avg));
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_KDTC_COEF_9_FIELD,0x1f&(kdtc[9]-kdtc_avg));
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_KDTC_COEF_10_FIELD,0x1f&(kdtc[10]-kdtc_avg));

   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_UPDATE_IC_FIELD, 0); 
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_TX_E_FIELD, 0); 
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_RX_E_FIELD, 0); 
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_UPDATE_IC_FIELD, 0); 
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_E_FIELD, 0); 
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_FILTER_MOD_TC_FIELD, 10); 
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_E_FIELD, 1); 
   rf_icydm_set_field((icydm_field)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_E_FIELD, 1); 
   DIS_RRMU_GAL_T22V1A;
}

void rf_calib_dc_branch_v3a(icydm_field cal_p,icydm_field cal_m,icydm_field adc_readout){
    int val_p,val_m,adc,k;
    int16_t val;
    val_p = val_m = 0x40;
    val = 0;
    for(k=0;k<7;k++){
        rf_icydm_set_field(cal_p,val_p);
        rf_icydm_set_field(cal_m,val_m);
        if(k<6) {
            rf_icydm_reg_wr(GALLIFREY_V3A_COMMANDS_ADDR,0xa0);
            rf_icydm_reg_wr(GALLIFREY_V3A_COMMANDS_ADDR,0xa0);
            adc = (int16_t)rf_icydm_read_field(adc_readout);
            if(adc < 0)
              val += (1<<(5-k));
            else
              val -= (1<<(5-k));
            val_p = 0x40+val;
            val_m = 0x40-val;
        }
    }
}


void rf_calib_dc_v3a()
{
    int tee0_dest,op0_dest,sw_i_q;
    tee0_dest = rf_icydm_read_field((icydm_field)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_TEE_0_P0_DEST_ADDR_FIELD);
    op0_dest  = rf_icydm_read_field((icydm_field)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_OBSERVE_POINT_0_DEST_ADDR_FIELD);
    sw_i_q    = rf_icydm_read_field((icydm_field)GALLIFREY_V3A_DIG_RX_TOP_ADC_CTRL_SWITCH_I_Q_FIELD);

    rf_icydm_set_field((icydm_field)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_TEE_0_P0_DEST_ADDR_FIELD, 5);
    rf_icydm_set_field((icydm_field)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_OBSERVE_POINT_0_DEST_ADDR_FIELD,127);
    rf_icydm_set_field((icydm_field)GALLIFREY_V3A_DIG_RX_TOP_ADC_CTRL_SWITCH_I_Q_FIELD,0);

    rf_icydm_set_field((icydm_field)GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_E_FIELD,1);
    rf_icydm_set_field((icydm_field)GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_BUF_E_FIELD,1);
    rf_icydm_set_field((icydm_field)GALLIFREY_V3A_ANA_TRX_BB_ADC_I_E_FIELD,1);
    rf_icydm_set_field((icydm_field)GALLIFREY_V3A_ANA_TRX_BB_ADC_Q_E_FIELD,1);
    rf_icydm_set_field((icydm_field)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_0_SHIFT_AMOUNT_FIELD, 5);
    rf_icydm_set_field((icydm_field)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_0_ENA_FIELD, 1);
    rf_icydm_set_field((icydm_field)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_0_IGNORE_ENA_FIELD, 1);

    rf_calib_dc_branch_v3a(
        (icydm_field)GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_DC_CAL_IP_FIELD,
        (icydm_field)GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_DC_CAL_IM_FIELD,
        (icydm_field)GALLIFREY_V3A_P0_DATA_I_FIELD);
    rf_calib_dc_branch_v3a(
        (icydm_field)GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_DC_CAL_QP_FIELD,
        (icydm_field)GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_DC_CAL_QM_FIELD,
        (icydm_field)GALLIFREY_V3A_P0_DATA_Q_FIELD);

    rf_icydm_set_field((icydm_field)GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_E_FIELD,0);
    rf_icydm_set_field((icydm_field)GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_BUF_E_FIELD,0);
    rf_icydm_set_field((icydm_field)GALLIFREY_V3A_ANA_TRX_BB_ADC_I_E_FIELD,0);
    rf_icydm_set_field((icydm_field)GALLIFREY_V3A_ANA_TRX_BB_ADC_Q_E_FIELD,0);

    rf_icydm_set_field((icydm_field)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_TEE_0_P0_DEST_ADDR_FIELD,tee0_dest );
    rf_icydm_set_field((icydm_field)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_OBSERVE_POINT_0_DEST_ADDR_FIELD,op0_dest);
    rf_icydm_set_field((icydm_field)GALLIFREY_V3A_DIG_RX_TOP_ADC_CTRL_SWITCH_I_Q_FIELD,sw_i_q);
}

void rf_init(struct rwip_rf_api *api)
{
    uint32_t icydm_chip_id = 0;  // Default version is IcyTRXDM
    uint32_t icydm_hash    = 0;  // Default version is IcyTRXDM
    uint8_t length = PARAM_LEN_RSSI_THR;

    volatile uint32_t debug;

    // Initialize the RF driver API structure
    api->reg_rd = rf_icydm_reg_rd;
    api->reg_wr = rf_icydm_reg_wr;
    api->txpwr_dbm_get = rf_txpwr_dbm_get;
    api->txpwr_min = ICYDM_POWER_MIN;
    api->txpwr_max = ICYDM_POWER_MAX;
    api->sleep = rf_sleep;
    api->reset = rf_reset;
    #if defined(CFG_BLE_EMB)
    api->force_agc_enable = rf_force_agc_enable;
    #endif //CFG_BLE_EMB
    api->rssi_convert = rf_rssi_convert;
    api->txpwr_cs_get = rf_txpwr_cs_get;

    #if  defined(CFG_BT_EMB)
        api->txpwr_dec = rf_txpwr_dec;
        api->txpwr_inc = rf_txpwr_inc;
        api->txpwr_max_set = txpwr_max_set;
    #endif //CFG_BT_EMB

    // Initialize the RSSI thresholds (high, low)
    // The upper threshold level is 20 dB above the lower threshold level to an accuracy of +-6 dB
    // These are 'real' signed values in dBm
    if (rwip_param.get(PARAM_ID_RSSI_HIGH_THR, &length, (uint8_t*)&api->rssi_high_thr) != PARAM_OK)
    {
        api->rssi_high_thr = (int8_t)ICYDM_RSSI_40dB_THRHLD;
    }
    if (rwip_param.get(PARAM_ID_RSSI_LOW_THR, &length, (uint8_t*)&api->rssi_low_thr) != PARAM_OK)
    {
        api->rssi_low_thr = (int8_t)ICYDM_RSSI_60dB_THRHLD;
    }
    /// Initialize Exchange Memory
    rf_em_init();


    // --------------- RADIOCNTL0 register ---------------
        // Set pointer SW SPI Drive access Pointer
        ip_radiocntl0_pack( /*uint16_t spiptr*/        (EM_RF_SW_SPI_OFFSET >> 2),
                            /*uint8_t  spicfg*/         0,
                            /*uint8_t  spifreq*/        1,
                            /*uint8_t  spigo*/          0);


    // --------------- RADIOCNTL1 register ---------------
        /* RADIOCNTL1 */
        ip_radiocntl1_pack(/*uint8_t  forceagcen*/      0,
                           /*uint8_t  forceiq*/         0,
                           /*uint8_t  rxdnsl*/          0,
                           /*uint8_t  txdnsl*/          0,
                           /*uint16_t forceagclength*/  0x0,
                           /*uint8_t  syncpulsemode*/   1,
                           /*uint8_t  syncpulsesrc*/    1,
                           /*uint8_t  dpcorren*/        0,
                           /*uint8_t  jefselect*/       0,
                           /*uint8_t  xrfsel*/          0x07,
                           /*uint8_t  subversion*/      0x0);


    #if !defined(RP_HWSIM_BYPASS)
    // Detect the RF version
    icydm_chip_id = rf_icydm_reg_rd(0x3FF);
    icydm_hash    = rf_icydm_reg_rd(0x3F8)|(rf_icydm_reg_rd(0x3F9)<<8)|(rf_icydm_reg_rd(0x3FA)<<16)|(rf_icydm_reg_rd(0x3FB)<<24);//ICYDM_T22_V1A;//
    #else
    // Stick to v0x18 to prevent ASSERT
    icydm_chip_id = 0x18;
    #endif
    
    #if defined(CFG_BLE_EMB)
       /* BLE RADIOCNTL2 */
        ble_radiocntl2_pack(/*uint8_t  lrsynccompmode*/  0x0,
                            /*uint8_t  rxcitermbypass*/  0x0,
                            /*uint8_t  lrvtbflush*/      0x10,
                            /*uint8_t  phymsk*/          0x3, // mark that Coded phy and 2Mbps are supported
                            /*uint8_t  lrsyncerr*/       0,
                            /*uint8_t  syncerr*/         0,
                            /*uint16_t freqtableptr*/    (EM_FT_OFFSET >> 2));

        /* BLE RADIOCNTL3 */
        ble_radiocntl3_pack(/*uint8_t rxrate3cfg*/       0x3,
                            /*uint8_t rxrate2cfg*/       0x2,
                            /*uint8_t rxrate1cfg*/       0x1,
                            /*uint8_t rxrate0cfg*/       0x0,
                            /*uint8_t getrssidelay */ BLE_GETRSSIDELAY_RST,
                            /*uint8_t rxsyncrouting*/    0x0,
                            /*uint8_t rxvalidbeh*/       0x2,
                            /*uint8_t txrate3cfg*/       0x0, // map on 1 Mbps
                            /*uint8_t txrate2cfg*/       0x0, // map on 1 Mbps
                            /*uint8_t txrate1cfg*/       0x1,
                            /*uint8_t txrate0cfg*/       0x0,
                            /*uint8_t txvalidbeh*/       0x2);

  switch(icydm_chip_id)
  {
  // Gallifrey_v3a
  case(0x18):
        /* BLE RADIOPWRUPDN0 */
        ble_radiopwrupdn0_pack(/*uint8_t syncposition0*/  0,
                               /*uint8_t rxpwrup0*/       70,
                               /*uint8_t txpwrdn0*/       5,
                               /*uint8_t txpwrup0*/       29);

        /* BLE RADIOPWRUPDN1 */
        ble_radiopwrupdn1_pack(/*uint8_t syncposition1*/  0,
                               /*uint8_t rxpwrup1*/       50,
                               /*uint8_t txpwrdn1*/       5,
                               /*uint8_t txpwrup1*/       29);

        /* BLE RADIOPWRUPDN2 */
        ble_radiopwrupdn2_pack(/*uint8_t syncposition2*/  0,
                               /*uint8_t rxpwrup2*/       60,
                               /*uint8_t txpwrdn2*/       5,
                               /*uint8_t txpwrup2*/       29);

        /* BLE RADIOPWRUPDN3 */
        ble_radiopwrupdn3_pack(/*uint8_t txpwrdn3*/       5,
                               /*uint8_t txpwrup3*/       29);

        /* BLE RADIOTXRXTIM0 */
        ble_radiotxrxtim0_pack(/*uint8_t rfrxtmda0*/       9,
                               /*uint8_t rxpathdly0*/      10,
                               /*uint8_t txpathdly0*/      38);

        /* BLE RADIOTXRXTIM1 */
        ble_radiotxrxtim1_pack(/*uint8_t rfrxtmda1*/       5,
                               /*uint8_t rxpathdly1*/      5,
                               /*uint8_t txpathdly1*/      37);

        /* BLE RADIOTXRXTIM2 */
        ble_radiotxrxtim2_pack(/*uint8_t rxflushpathdly2*/ 12,
                               /*uint8_t rfrxtmda2*/       165,
                               /*uint8_t rxpathdly2*/      49,
                               /*uint8_t txpathdly2*/      38);

        /* BLE RADIOTXRXTIM3 */
        ble_radiotxrxtim3_pack(/*uint8_t rxflushpathdly3*/ 12,
                               /*uint8_t rfrxtmda3*/       45,
                               /*uint8_t txpathdly3*/      38);
     break;

  // Gallifrey_v3a
  case(0x20):
        /* BLE RADIOPWRUPDN0 */
        ble_radiopwrupdn0_pack(/*uint8_t syncposition0*/  0,
                               /*uint8_t rxpwrup0*/       70,
                               /*uint8_t txpwrdn0*/       5,
                               /*uint8_t txpwrup0*/       29);

        /* BLE RADIOPWRUPDN1 */
        ble_radiopwrupdn1_pack(/*uint8_t syncposition1*/  0,
                               /*uint8_t rxpwrup1*/       50,
                               /*uint8_t txpwrdn1*/       5,
                               /*uint8_t txpwrup1*/       29);

        /* BLE RADIOPWRUPDN2 */
        ble_radiopwrupdn2_pack(/*uint8_t syncposition2*/  0,
                               /*uint8_t rxpwrup2*/       60,
                               /*uint8_t txpwrdn2*/       5,
                               /*uint8_t txpwrup2*/       29);

        /* BLE RADIOPWRUPDN3 */
        ble_radiopwrupdn3_pack(/*uint8_t txpwrdn3*/       5,
                               /*uint8_t txpwrup3*/       29);

        /* BLE RADIOTXRXTIM0 */
        ble_radiotxrxtim0_pack(/*uint8_t rfrxtmda0*/       9,
                               /*uint8_t rxpathdly0*/      13,
                               /*uint8_t txpathdly0*/      38);

        /* BLE RADIOTXRXTIM1 */
        ble_radiotxrxtim1_pack(/*uint8_t rfrxtmda1*/       5,
                               /*uint8_t rxpathdly1*/      7,
                               /*uint8_t txpathdly1*/      37);

        /* BLE RADIOTXRXTIM2 */
        ble_radiotxrxtim2_pack(/*uint8_t rxflushpathdly2*/ 12,
                               /*uint8_t rfrxtmda2*/       165,
                               /*uint8_t rxpathdly2*/      49,
                               /*uint8_t txpathdly2*/      38);

        /* BLE RADIOTXRXTIM3 */
        ble_radiotxrxtim3_pack(/*uint8_t rxflushpathdly3*/ 12,
                               /*uint8_t rfrxtmda3*/       45,
                               /*uint8_t txpathdly3*/      38);
     break;

  default:
   ASSERT_INFO(0,icydm_chip_id,0);
  break;
  }
    #endif // defined CFG_BLE_EMB

    #if  defined(CFG_BT_EMB)

  switch(icydm_chip_id)
  {
  /* Gallifrey_v3a */
  case(0x18):
        /* EDRCNTL */
        bt_rwbtcntl_nwinsize_setf(0);
        bt_edrcntl_rxgrd_timeout_setf(0x23);

        /* BT RADIOPWRUPDN */
        bt_radiopwrupdn_pack(  /*uint8_t rxpwrupct*/       50,
                               /*uint8_t txpwrdnct*/        5,
                               /*uint8_t txpwrupct*/       24);
        
        /* BT RADIOCNTL 2 */
        bt_radiocntl2_pack(    /*uint8_t trailer_gating_value*/ 0x4,
                               /*uint8_t syncerr*/              0x0,
                               /*uint8_t freqtable_ptr*/       ((EM_FT_OFFSET >> 2)));
        
        /* BT RADIOTXRXTIM */
        bt_radiotxrxtim_pack(  /*uint8_t sync_position*/   0,
                               /*uint8_t rxpathdly*/       12,
                               /*uint8_t txpathdly*/       42);
     break;

  /* Gallifrey_t22_v1a */
  case(0x20):
        /* EDRCNTL */
        bt_rwbtcntl_nwinsize_setf(0);
        bt_edrcntl_rxgrd_timeout_setf(0x23);

        /* BT RADIOPWRUPDN */
        bt_radiopwrupdn_pack(  /*uint8_t rxpwrupct*/       50,
                               /*uint8_t txpwrdnct*/        5,
                               /*uint8_t txpwrupct*/       24);
        
        /* BT RADIOCNTL 2 */
        bt_radiocntl2_pack(    /*uint8_t trailer_gating_value*/ 0x4,
                               /*uint8_t syncerr*/              0x0,
                               /*uint8_t freqtable_ptr*/       ((EM_FT_OFFSET >> 2)));
        
        /* BT RADIOTXRXTIM */
        bt_radiotxrxtim_pack(  /*uint8_t sync_position*/   0,
                               /*uint8_t rxpathdly*/       9,
                               /*uint8_t txpathdly*/       42);
     break;
     
  default:
   ASSERT_INFO(0,icydm_chip_id,0);
  break;
  }

#ifdef SET_DATA_IN_PARALLEL_WAY//parallel
        /* BT RADIOCNTL 3*/
        bt_radiocntl3_pack( 
                               /*uint8_t rxrate2cfg*/      3,
                               /*uint8_t rxrate1cfg*/      2,
                               /*uint8_t rxrate0cfg*/      0,
                               /*uint8_t getrssidelay*/    0x0,
                               /*uint8_t rxserparif*/      0,  //1 IN RF TC, 0 in full SoC
                               /*uint8_t rxsyncrouting*/   0,
                               /*uint8_t rxvalidbeh*/      2,
                               /*uint8_t txrate2cfg*/      3,
                               /*uint8_t txrate1cfg*/      2,
                               /*uint8_t txrate0cfg*/      0,
                               /*uint8_t txserparif*/      0,  //1 in RF TC, but 0 in full SoC
                               /*uint8_t txvalidbeh*/      2);
#else//serial
        /* BT RADIOCNTL 3*/
        bt_radiocntl3_pack( 
                               /*uint8_t rxrate2cfg*/      3,
                               /*uint8_t rxrate1cfg*/      2,
                               /*uint8_t rxrate0cfg*/      0,
                               /*uint8_t getrssidelay*/    0x0,
                               /*uint8_t rxserparif*/      1,  //1 IN RF TC, 0 in full SoC
                               /*uint8_t rxsyncrouting*/   0,
                               /*uint8_t rxvalidbeh*/      2,
                               /*uint8_t txrate2cfg*/      3,
                               /*uint8_t txrate1cfg*/      2,
                               /*uint8_t txrate0cfg*/      0,
                               /*uint8_t txserparif*/      1,  //1 in RF TC, but 0 in full SoC
                               /*uint8_t txvalidbeh*/      2);
#endif

    #endif //CFG_BT_EMB

    #if (BLE_CON_CTE_REQ | BLE_CONLESS_CTE_RX)
    // Init the DF CNTL
    ble_dfcntl0_1us_pack(/*uint8_t rxsampstinst01us*/ 0x08, /*uint8_t rxswstinst01us*/ 0x18, /*uint8_t txswstinst01us*/ 0x19);
    ble_dfcntl0_2us_pack(/*uint8_t rxsampstinst02us*/ 0x08, /*uint8_t rxswstinst02us*/ 0x18, /*uint8_t txswstinst02us*/ 0x19);
    ble_dfcntl1_1us_pack(/*uint8_t rxsampstinst11us*/ 0x08, /*uint8_t rxswstinst11us*/ 0x18, /*uint8_t txswstinst11us*/ 0x19);
    ble_dfcntl1_2us_pack(/*uint8_t rxsampstinst12us*/ 0x08, /*uint8_t rxswstinst12us*/ 0x18, /*uint8_t txswstinst12us*/ 0x19);
    ble_dfantcntl_pack(/*uint8_t rxprimidcntlen*/ 1, /*uint8_t rxprimantid*/ 0, /*uint8_t txprimidcntlen*/ 1, /*uint8_t txprimantid*/ 0);
    #endif // (BLE_CON_CTE_REQ | BLE_CONLESS_CTE_RX)

        ble_dfifcntl_pack(     /*uint8_t antswitch_beh*/       0,
                               /*uint8_t sampreq_beh*/         1, // Toggle mode
                               /*uint8_t sampvalid_beh*/       2, // Toggle mode
                               /*uint8_t if_width_setf*/       1, // 1 in RF TC (4bit), but 3 in full SoC (16bit)
                               /*uint8_t msb_lsb_order*/       1, // LSB first
                               /*uint8_t symbol_order*/        0);// I first

        
  // IcyTRx Static Register Initialization
  switch(icydm_chip_id)
  {
  case(0x18):
  // IcyTRxDM Static Register Initialization for TestChip version
    // TX/RX config setting
    {
      unsigned int i,blocks;
      blocks = (sizeof(RF_ICYDM_REG_TBL_18_INIT)-16)>>6;
      for(i=0;i<blocks;i++)
        rf_icydm_reg_burst_wr(i<<6, 64, (uint8_t *) &RF_ICYDM_REG_TBL_18_INIT[i<<6]);
      if(((sizeof(RF_ICYDM_REG_TBL_18_INIT)-16)%64)!=0){
        rf_icydm_reg_burst_wr(i<<6, (sizeof(RF_ICYDM_REG_TBL_18_INIT)-16)%64, (uint8_t *) &RF_ICYDM_REG_TBL_18_INIT[i<<6]);
      }
      rf_icydm_reg_burst_wr(sizeof(RF_ICYDM_REG_TBL_18_INIT)-4,4,(uint8_t *) &RF_ICYDM_REG_TBL_18_INIT[sizeof(RF_ICYDM_REG_TBL_18_INIT)-4]);
    }
       
    // Sequencer
    rf_icydm_reg_burst_wr(0xAD0, sizeof(RF_ICYDM_REG_TBL_18_SEQUENCER), (uint8_t *) &RF_ICYDM_REG_TBL_18_SEQUENCER[0]);

    // TX packet handler
    rf_icydm_reg_burst_wr(0xB50, sizeof(RF_ICYDM_REG_TBL_18_TX_PKT_HANDLER), (uint8_t *) &RF_ICYDM_REG_TBL_18_TX_PKT_HANDLER[0]);

    // RX packet handler
    rf_icydm_reg_burst_wr(0xBD0, sizeof(RF_ICYDM_REG_TBL_18_RX_PKT_HANDLER), (uint8_t *) &RF_ICYDM_REG_TBL_18_RX_PKT_HANDLER[0]);

    // AGC packet handler
    {
      unsigned int i;
      for(i=0;i<(sizeof(RF_ICYDM_REG_TBL_18_AGC)-64);i+=64)
        rf_icydm_reg_burst_wr(0xC50+i, 64, (uint8_t *) &RF_ICYDM_REG_TBL_18_AGC[i]);
      rf_icydm_reg_burst_wr(0xC50+i, sizeof(RF_ICYDM_REG_TBL_18_AGC)%64, (uint8_t *) &RF_ICYDM_REG_TBL_18_AGC[i]);
    }

     // APLL
    rf_icydm_reg_burst_wr(0xE00, sizeof(RF_ICYDM_REG_TBL_18_ADPLL), (uint8_t *) &RF_ICYDM_REG_TBL_18_ADPLL[0]);

    // GPIOS settings
    rf_icydm_reg_burst_wr(0x00A, 10, (uint8_t *) &RF_ICYDM_REG_TBL_18_GPIOS[0]);

    // End Init procedure
    rf_icydm_reg_wr(0x000, 0);
    rf_icydm_reg_wr(0x004, 1);
    rf_calib_adpll_v3a();
    rf_calib_dc_v3a();
    break;

  case(0x20):
    switch(icydm_hash){
      case ICYDM_T22_V1A:
         // IcyTRxDM Static Register Initialization for TestChip version
          // TX/RX config setting
          {
            unsigned int i,blocks;
            blocks = sizeof(RF_ICYDM_REG_TBL_T22V1A_INIT)>>6;
            for(i=0;i<blocks;i++)
              rf_icydm_reg_burst_wr(i<<6, 64, (uint8_t *) &RF_ICYDM_REG_TBL_T22V1A_INIT[i<<6]);
            if((sizeof(RF_ICYDM_REG_TBL_T22V1A_INIT)%64)!=0)
              rf_icydm_reg_burst_wr(i<<6, sizeof(RF_ICYDM_REG_TBL_T22V1A_INIT)%64, (uint8_t *) &RF_ICYDM_REG_TBL_T22V1A_INIT[i<<6]);
          }
             
          // Sequencer
          rf_icydm_reg_burst_wr(0xA00, sizeof(RF_ICYDM_REG_TBL_T22V1A_SEQUENCER), (uint8_t *) &RF_ICYDM_REG_TBL_T22V1A_SEQUENCER[0]);

          // TX packet handler
          rf_icydm_reg_burst_wr(0xA80, sizeof(RF_ICYDM_REG_TBL_T22V1A_TX_PKT_HANDLER), (uint8_t *) &RF_ICYDM_REG_TBL_T22V1A_TX_PKT_HANDLER[0]);

          // RX packet handler
          rf_icydm_reg_burst_wr(0xB40, sizeof(RF_ICYDM_REG_TBL_T22V1A_RX_PKT_HANDLER), (uint8_t *) &RF_ICYDM_REG_TBL_T22V1A_RX_PKT_HANDLER[0]);

          // AGC packet handler
          {
            unsigned int i;
            for(i=0;i<(sizeof(RF_ICYDM_REG_TBL_T22V1A_AGC)-64);i+=64)
              rf_icydm_reg_burst_wr(0xC00+i, 64, (uint8_t *) &RF_ICYDM_REG_TBL_T22V1A_AGC[i]);
            rf_icydm_reg_burst_wr(0xC00+i, sizeof(RF_ICYDM_REG_TBL_T22V1A_AGC)%64, (uint8_t *) &RF_ICYDM_REG_TBL_T22V1A_AGC[i]);
          }

           // APLL
          rf_icydm_reg_burst_wr(0xE00, sizeof(RF_ICYDM_REG_TBL_T22V1A_ADPLL), (uint8_t *) &RF_ICYDM_REG_TBL_T22V1A_ADPLL[0]);

          // GPIOS settings
          rf_icydm_reg_burst_wr(0x00A, 10, (uint8_t *) &RF_ICYDM_REG_TBL_T22V1A_GPIOS[0]);

          // End Init procedure
          rf_icydm_reg_wr(0x001, 0);
          rf_icydm_reg_wr(0x004, 1);
          rf_icydm_reg_wr(0x001, 0);
          rf_icydm_reg_wr(0x004, 1);
          rf_calib_adpll_t22_v1a();
          //rf_calib_dc_v3a();
          break;
      default:
         ASSERT_ERR(0);
         break;
    }
    break;

  default:
    ASSERT_ERR(0);
  break;
  }

   //gpio setting
    /*
    reg name  val     addr  
    gpio_0    0x16     0xA
    gpio_1    0x17     0xB
    gpio_2    0x18     0xC
    gpio_3    0x19     0xD

    gpio_5    0x1A     0xF 
    gpio_6    0x1B     0x10 
    gpio_7    0x1C     0x11
    gpio_8    0x1D     0x12
    gpio_9    0x1E     0x13
    */
    __STATIC  const uint8_t RF_ICYDM_REG_TBL_09_GPIOS[10] =
    {
    #if 1
      0x16,  0x17,  0x1E,  0x19,
      0x00,//no use
      0x1A,  0x1B,  0x1C,  0x1D,
      0x18
    #else      
      0x16,  0x17,  0x18,  0x19,
      0x00,//no use
      0x1A,  0x1B,  0x1C,  0x1D,
      0x1E
    #endif
    };


    rf_icydm_reg_burst_wr(0xA,  sizeof(RF_ICYDM_REG_TBL_09_GPIOS), RF_ICYDM_REG_TBL_09_GPIOS);



#if 0//REGISTER WR/RD TEST

  #define RD_LEN    10

  volatile char test_w_val[RD_LEN] = {0};
  volatile char test_r_val[RD_LEN] = {0};
  
  rf_icydm_reg_burst_rd(0xA, 10, test_r_val);
  
  #if 0//related functions
    rf_icydm_reg_wr(0x300,0);//1 byte per write
    rf_icydm_reg_32bit_wr(0x300,0x162800);//4 bytes per write
    rf_icydm_reg_burst_wr(0x300,  sizeof(ARRAY), (ARRAY));
    rf_icydm_reg_rd(0x300);//1 byte per read
    rf_icydm_reg_burst_rd(0xA, sizeof(ARRAY), (ARRAY));
  #endif

#endif//REGISTER WR/RD TEST

    switch(icydm_chip_id)
    {
		case(0x18):
			rf_icydm_reg_wr(0xe,0x11);//gpio4-> xo clk

			rf_icydm_reg_wr(0x1f2,0x3<<6);
			rf_icydm_reg_wr(0x1f3,0);
			rf_icydm_reg_wr(0x1f4,0x80);
			break;
		case(0x20):
			rf_icydm_reg_wr(0xe,0x11);//gpio4-> xo clk

		  #if 1
            //#define XO_DIV 3//48/3
            #define XO_DIV 4//48/4
            rf_icydm_reg_wr(0x200,XO_DIV&0X3<<6);
			rf_icydm_reg_wr(0x201,XO_DIV>>2);
			rf_icydm_reg_wr(0x202,0x80);
		  #else
			rf_icydm_reg_wr(0x200,0xff);
			rf_icydm_reg_wr(0x201,0xff);
			rf_icydm_reg_wr(0x202,0xff);
		  #endif
			break;
		default:
			break;
    }

    while(0)
    {
        //rf_icydm_reg_wr(0xa, 1);
        //rf_icydm_reg_wr(0xb, 1);
        //rf_icydm_reg_wr(0xc, 1);
        //rf_icydm_reg_wr(0xd, 1);

        rf_icydm_reg_wr(0xe, 1);
        rf_icydm_reg_wr(0xf, 1);
        rf_icydm_reg_wr(0x10, 1);
        rf_icydm_reg_wr(0x11, 1);
        //rf_icydm_reg_wr(0x12, 1);


        uint32_t val = rf_icydm_reg_rd(0xe);

        if (val != 1)
        {
            __asm("break 1,1");
        }

        timer_delayms(100);
        //rf_icydm_reg_wr(0xa, 2);
        //rf_icydm_reg_wr(0xb, 2);
        //rf_icydm_reg_wr(0xc, 2);
        //rf_icydm_reg_wr(0xd, 2);

        rf_icydm_reg_wr(0xe, 2);
        rf_icydm_reg_wr(0xf, 2);
        rf_icydm_reg_wr(0x10, 2);
        rf_icydm_reg_wr(0x11, 2);
        //rf_icydm_reg_wr(0x12, 2);

        val = rf_icydm_reg_rd(0xe);

        if (val != 2)
        {
            __asm("break 1,1");
        }        
        timer_delayms(100);

    }

    //rf_icydm_reg_wr(0xe, 0x12);

    //debug = rf_icydm_reg_rd(0xe);
  /* *************************************************************************************** */
  /* Initialize HW SPI Chains Pointers  */
  /* *************************************************************************************** */
  uint16_t txonptr      = (EM_RF_HW_SPI_OFFSET);
  uint16_t txoffptr     = (0);
#if defined(CFG_BLE_EMB)
  uint16_t ble_rxonptr  = (EM_RF_HW_SPI_OFFSET+0x10);
#endif // defined CFG_BLE_EMB
#if  defined(CFG_BT_EMB)
  uint16_t bt_rxonptr   = (EM_RF_HW_SPI_OFFSET+0x10+0x10);
#endif // defined CFG_BT_EMB
  uint16_t rssiptr      = (EM_RF_HW_SPI_OFFSET+0x10+0x10+0x20);
  uint16_t rxlengthptr  = (EM_RF_HW_SPI_OFFSET+0x10+0x10+0x20+0x10);
#if  defined(CFG_BT_EMB)
  uint16_t rxpkttypptr  = (EM_RF_HW_SPI_OFFSET+0x10+0x10+0x20+0x10+0x10);
#endif
  uint16_t rxoffptr     = (EM_RF_HW_SPI_OFFSET+0x10+0x10+0x20+0x10+0x10+0x10);

#if defined(CFG_BLE_EMB)
  /* TxOn Sequence start pointer */
  ble_spiptrcntl0_txonptr_setf(txonptr >> 2);

  /* TxOff Sequence start pointer */
  ble_spiptrcntl0_txoffptr_setf(txoffptr >> 2);

  /* RxOn Sequence start pointer */
  ble_spiptrcntl1_rxonptr_setf(ble_rxonptr >> 2);

  /* RxOff Sequence start pointer */
  ble_spiptrcntl1_rxoffptr_setf(rxoffptr >> 2);

  /* RSSI Sequence start pointer */
  ble_spiptrcntl2_rssiptr_setf(rssiptr >> 2);

  /* Long Range packet length Sequence start pointer */
  ble_spiptrcntl2_rxlengthptr_setf(rxlengthptr >> 2);
#endif // defined CFG_BLE_EMB

#if  defined(CFG_BT_EMB)
    /* TxOn Sequence start pointer */
    bt_spiptrcntl0_txonptr_setf(txonptr >> 2);

    /* TxOff Sequence start pointer */
    bt_spiptrcntl0_txoffptr_setf(txoffptr >> 2);

    /* RxOn Sequence start pointer */
    bt_spiptrcntl1_rxonptr_setf(bt_rxonptr >> 2);

    /* RxOff Sequence start pointer */
    bt_spiptrcntl1_rxoffptr_setf(rxoffptr >> 2);

    /* RSSI Sequence start pointer */
    bt_spiptrcntl2_rssiptr_setf(rssiptr >> 2);

    /* Packet Type Sequence start pointer */
    bt_spiptrcntl3_rxpkttypptr_setf(rxpkttypptr >> 2);
#endif // defined CFG_BT_EMB

  /* *************************************************************************************** */
  /* Initialize HW SPI Tx On Chained list  -> 1 structure of 12 bytes*/
  /* *************************************************************************************** */
  /*  TxON Sequence -> Write Address @0x800 / 4 bytes
                 -> Next Pointer = 0x0000
                 -> @800 - RATE_TX       - write BT_nBLE mode and CS-TXRATE
                 -> @801 - CHANNEL_TX    - write Bluetooth Channel
                 -> @802 - PA_PWR        - write CS-TXPWR
                 -> @803 - TX_STARTUP    - write 0x10 ( Tx On ) */
  RF_ICTRXDM_EM_WR(txonptr    , 0x0000);
  RF_ICTRXDM_EM_WR(txonptr+0x2, 0x0084);
  RF_ICTRXDM_EM_WR(txonptr+0x4, 0x0008);
  RF_ICTRXDM_EM_WR(txonptr+0x6, 0x0000);
  RF_ICTRXDM_EM_WR(txonptr+0x8, 0x0010); /* set 0x10 to set parallel mode in RF on bit 4 of RATE_TX register */
  RF_ICTRXDM_EM_WR(txonptr+0xA, 0x1000);
  RF_ICTRXDM_EM_WR(txonptr+0xC, 0x0000);

  /* *************************************************************************************** */
  /* Initialize HW SPI Tx Off Chained list -> Nothing here at the moment */
  /* *************************************************************************************** */

  /* *************************************************************************************** */
  /* Initialize HW SPI Rx On Chained list -> 1 structure of 16 bytes or 20 bytes*/
  /* *************************************************************************************** */
  /* Initialize HW SPI Rx On Chained list  */
#if defined(CFG_BLE_EMB)
  /*   RxON Sequence -> Write Address @0x810 / 7 bytes
              -> Next Pointer= 0x0000
              -> @810 - RATE_RX       - write BT_nBLE mode and CS-RXRATE
              -> @811 - CHANNEL_RX    - write Bluetooth Index
              -> @812 - RX_STARTUP    - write 0x12 ( Rx On )
              -> @813 - ACCESS_ADDR   - write CS-SYNCWORD[7:0]
              -> @814 - ACCESS_ADDR   - write CS-SYNCWORD[15:8]
              -> @815 - ACCESS_ADDR   - write CS-SYNCWORD[23:16]
              -> @816 - ACCESS_ADDR   - write CS-SYNCWORD[31:24]    */

  RF_ICTRXDM_EM_WR(ble_rxonptr,     0x0000);
  RF_ICTRXDM_EM_WR(ble_rxonptr+0x2, 0x1087);
  RF_ICTRXDM_EM_WR(ble_rxonptr+0x4, 0x0008);
  RF_ICTRXDM_EM_WR(ble_rxonptr+0x6, 0x0000);
  RF_ICTRXDM_EM_WR(ble_rxonptr+0x8, 0x0000);
  RF_ICTRXDM_EM_WR(ble_rxonptr+0xA, 0x0012);
  RF_ICTRXDM_EM_WR(ble_rxonptr+0xC, 0x0000);
  RF_ICTRXDM_EM_WR(ble_rxonptr+0xE, 0x0000);
 #endif // defined CFG_BLE_EMB

#if  defined(CFG_BT_EMB)
  /*   RxON Sequence -> Write Address @0x810 / 11 bytes
              -> Next Pointer= 0x0000
              -> @810 - RATE_RX       - write BT_nBLE mode and CS-RXRATE
              -> @811 - CHANNEL_RX    - write Bluetooth Index
              -> @812 - RX_STARTUP    - write 0x12 ( Rx On )
              -> @813 - SYNC_WORD_LOW - write CS-SYNCWORD[7:0]
              -> @814 - SYNC_WORD_LOW - write CS-SYNCWORD[15:8]
              -> @815 - SYNC_WORD_LOW - write CS-SYNCWORD[23:16]
              -> @816 - SYNC_WORD_LOW - write CS-SYNCWORD[31:24]  
              -> @817 - SYNC_WORD_HIGH - write CS-SYNCWORD[39:32]
              -> @818 - SYNC_WORD_HIGH - write CS-SYNCWORD[47:40]
              -> @819 - SYNC_WORD_HIGH - write CS-SYNCWORD[55:48]
              -> @81A - SYNC_WORD_HIGH - write CS-SYNCWORD[63:56]    */

  RF_ICTRXDM_EM_WR(bt_rxonptr,     0x0000);
  RF_ICTRXDM_EM_WR(bt_rxonptr+0x2, 0x108B);
  RF_ICTRXDM_EM_WR(bt_rxonptr+0x4, 0x0008);
  RF_ICTRXDM_EM_WR(bt_rxonptr+0x6, 0x0000);
  RF_ICTRXDM_EM_WR(bt_rxonptr+0x8, 0x0010); /* set 0x10 to set parallel mode in RF on bit 4 of RATE_RX register */
  RF_ICTRXDM_EM_WR(bt_rxonptr+0xA, 0x0012);
  RF_ICTRXDM_EM_WR(bt_rxonptr+0xC, 0x0000);
  RF_ICTRXDM_EM_WR(bt_rxonptr+0xE, 0x0000);
  RF_ICTRXDM_EM_WR(bt_rxonptr+0x10,0x0000);
  RF_ICTRXDM_EM_WR(bt_rxonptr+0x12,0x0000);
#endif // defined CFG_BT_EMB

  /* *************************************************************************************** */
  /* Initialize HW SPI Rx Off Chained list -> 1 structure of 9 byte */
  /* *************************************************************************************** */
  /*    RxOFF Access 1  -> Write Address @0x820 / 1 byte
                -> Next Pointer = 0x0000 -> end of Sequence
                -> Write Address @0x820 - RX_POWEROFF
                -> Write data = 0x13 (RX Off)*/

  RF_ICTRXDM_EM_WR(rxoffptr,     0x0000);
  RF_ICTRXDM_EM_WR(rxoffptr+0x2, 0x2081);
  RF_ICTRXDM_EM_WR(rxoffptr+0x4, 0x0008);
  RF_ICTRXDM_EM_WR(rxoffptr+0x6, 0x0000);
  RF_ICTRXDM_EM_WR(rxoffptr+0x8, 0x0013);


  /* *************************************************************************************** */
  /* Initialize HW SPI RSSI Chained list  -> 1 structure of 9 bytes*/
  /* *************************************************************************************** */
  /*    RSSI Access   -> Read Address @0x824 / 1 byte
               -> Next Pointer = 0x0000 -> end of the sequence
               -> Read Address @0x824 - RSSI_READYOUT
               -> Write data = 0x000000 -> Read data to replace this / provided to Packet Controller */
  RF_ICTRXDM_EM_WR(rssiptr,     0x0000);
  RF_ICTRXDM_EM_WR(rssiptr+0x2, 0x2401);
  RF_ICTRXDM_EM_WR(rssiptr+0x4, 0x0008);
  RF_ICTRXDM_EM_WR(rssiptr+0x6, 0x0000);
  RF_ICTRXDM_EM_WR(rssiptr+0x8, 0x0000);

#if defined(CFG_BLE_EMB)
  /* *************************************************************************************** */
  /* Initialize HW SPI Rx Packet Length Chained list  -> 1 structure of 9 bytes*/
  /* *************************************************************************************** */
  /* PACKET_LENGTH Access -> Write Address @0x828 / 1 byte
               -> Next Pointer = 0x0000 -> end of the sequence
               -> Write Address @0x828 - PACKT_LENGTH
               -> Write data = Long Range packet length */
  RF_ICTRXDM_EM_WR(rxlengthptr,     0x0000);
  RF_ICTRXDM_EM_WR(rxlengthptr+0x2, 0x2881);
  RF_ICTRXDM_EM_WR(rxlengthptr+0x4, 0x0008);
  RF_ICTRXDM_EM_WR(rxlengthptr+0x6, 0x0000);
  RF_ICTRXDM_EM_WR(rxlengthptr+0x8, 0x0000);
 #endif // defined CFG_BLE_EMB

#if  defined(CFG_BT_EMB)
  /* *************************************************************************************** */
  /* Initialize HW SPI Rx Packet Rate Chained list  -> 1 structure of 9 bytes*/
  /* *************************************************************************************** */
  /* PACKET_RATE Access -> Write Address @0x810 / 1 byte
               -> Next Pointer = 0x0000 -> end of the sequence
               -> Write Address @0x810 - PACKET_RATE
               -> Write data = EDR Packet Rate */
  RF_ICTRXDM_EM_WR(rxpkttypptr,     0x0000);
  RF_ICTRXDM_EM_WR(rxpkttypptr+0x2, 0x1081);
  RF_ICTRXDM_EM_WR(rxpkttypptr+0x4, 0x0008);
  RF_ICTRXDM_EM_WR(rxpkttypptr+0x6, 0x0000);
  RF_ICTRXDM_EM_WR(rxpkttypptr+0x8, 0x0010); /* set 0x10 to set parallel mode in RF on bit 4 of RATE_RX register */
#endif // defined CFG_BT_EMB
}
///@} RF_ICYTRXDM
 uint32_t read_status(void)
 {
    //rf_icydm_reg_wr(0xe, 0x12);
    return rf_icydm_reg_rd(0xe);     
 }
//#else

#define bool int
 /// API functions of the RF driver that are used by the BLE or BT software
 struct rwip_rf_api
 {
     /// Function called upon HCI reset command reception
     void (*reset)(void);
     /// Function called to enable/disable force AGC mechanism (true: en / false : dis)
     void (*force_agc_enable)(bool);
     /// Function called when TX power has to be decreased for a specific link id
     bool (*txpwr_dec)(uint8_t);
     /// Function called when TX power has to be increased for a specific link id
     bool (*txpwr_inc)(uint8_t);
     /// Function called when TX power has to be set to max for a specific link id
     void (*txpwr_max_set)(uint8_t);
     /// Function called to convert a TX power CS power field into the corresponding value in dBm
     int8_t (*txpwr_dbm_get)(uint8_t, uint8_t);
     /// Function called to convert a power in dBm into a control structure tx power field
     uint8_t (*txpwr_cs_get)(int8_t, uint8_t);
     /// Function called to convert the RSSI read from the control structure into a real RSSI
     int8_t (*rssi_convert)(uint8_t);
     /// Function used to read a RF register
     uint32_t (*reg_rd)(uint32_t);
     /// Function used to write a RF register
     void (*reg_wr)(uint32_t, uint32_t);
     /// Function called to put the RF in deep sleep mode
     void (*sleep)(void);
     /// Index of minimum TX power
     uint8_t txpwr_min;
     /// Index of maximum TX power
     uint8_t txpwr_max;
     /// RSSI high threshold ('real' signed value in dBm)
     int8_t rssi_high_thr;
     /// RSSI low threshold ('real' signed value in dBm)
     int8_t rssi_low_thr;
     /// interferer threshold ('real' signed value in dBm)
     int8_t rssi_interf_thr;
     /// RF wakeup delay (in slots)
     uint8_t wakeup_delay;
 };


 void rf_init(struct rwip_rf_api *api)
 {

 }
#endif
