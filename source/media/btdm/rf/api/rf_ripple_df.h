/**
 ****************************************************************************************
 *
 * @file rf_ripple_df.h
 *
 * @brief Radio-specific header file
 *
 * Copyright (C) RivieraWaves 2021
 *
 *
 ****************************************************************************************
 */

#ifndef RF_RIPPLE_DF_H_
#define RF_RIPPLE_DF_H_

/**
 ****************************************************************************************
 * @addtogroup RF
 * @{
 ****************************************************************************************
 */

/**
 * DEFINES
 ****************************************************************************************
 */

/// SUPPORTED RADIO PHY
#define RF_BLE_PHY_1MBPS_SUPPORT                    1
#define RF_BLE_PHY_2MBPS_SUPPORT                    0
#define RF_BLE_PHY_CODED_SUPPORT                    1
#define RF_STABLE_MOD_IDX_TX_SUPPORT                0
#define RF_STABLE_MOD_IDX_RX_SUPPORT                0
#define RF_PWR_CLASS_1_SUPPORT                      0

/// Radio power-up duration
#define RF_POWERUP_TIME_US                          60
/// Radio power-down duration
#define RF_POWERDOWN_TIME_US                        26

/// RF SW-Driven SPI transfers area definition
#define RF_SW_SPI_SIZE_MAX                          8
/// RF HW-Driven SPI transfers area definition
#define RF_HW_SPI_SIZE_MAX                          0

/// Frequency table area definition - Word size in bytes i.e. 8-bit words
#define RF_FREQ_TABLE_ELT_SIZE                      sizeof(uint8_t)
/// Frequency table area definition - No VCO sub-band table
#define RF_VCO_TABLE_ELT_SIZE                       0

/// Display information for radio
#define RF_BOARD_STR                               "Ripple DF #"
#define RF_BOARD_ID_READ                            1

#endif // RF_RIPPLE_DF_H_
