/* -----------------------------------------------------------------------------
   Copyright (C) CSEM SA. All rights reserved.                               --
   Developed at CSEM SA, Neuchatel, Switzerland                              --
   All rights reserved. Reproduction in whole or part is prohibited without  --
   The written permission of the copyright owner.                            --
  -----------------------------------------------------------------------------
    AUTHOR  : Nicola Scolari                                                 */

/* C header for GALLIFREY_V3A */

#ifndef __GALLIFREY_V3A_REG_MAP__
#define __GALLIFREY_V3A_REG_MAP__

#define GALLIFREY_V3A_BASE                                                                  0x0
#define GALLIFREY_V3A_DIG_OFFSET                                                            0x00
#define GALLIFREY_V3A_DIG_ADDR                                                              (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_OFFSET)
#define GALLIFREY_V3A_DIG_SZ                                                                8
#define GALLIFREY_V3A_DIG                                                                   (volatile uint8_t *)GALLIFREY_V3A_DIG_ADDR
#define GALLIFREY_V3A_DIG_IFACE_OFFSET                                                      0x01
#define GALLIFREY_V3A_DIG_IFACE_ADDR                                                        (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_IFACE_OFFSET)
#define GALLIFREY_V3A_DIG_IFACE_SZ                                                          8
#define GALLIFREY_V3A_DIG_IFACE                                                             (volatile uint8_t *)GALLIFREY_V3A_DIG_IFACE_ADDR
#define GALLIFREY_V3A_DIG_IFACE_PASSWD_OFFSET                                               0x02
#define GALLIFREY_V3A_DIG_IFACE_PASSWD_ADDR                                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_IFACE_PASSWD_OFFSET)
#define GALLIFREY_V3A_DIG_IFACE_PASSWD_SZ                                                   16
#define GALLIFREY_V3A_DIG_IFACE_PASSWD                                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_IFACE_PASSWD_ADDR
#define GALLIFREY_V3A_DIG_IFACE_OCTA_SPI_SLV_OFFSET                                         0x04
#define GALLIFREY_V3A_DIG_IFACE_OCTA_SPI_SLV_ADDR                                           (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_IFACE_OCTA_SPI_SLV_OFFSET)
#define GALLIFREY_V3A_DIG_IFACE_OCTA_SPI_SLV_SZ                                             8
#define GALLIFREY_V3A_DIG_IFACE_OCTA_SPI_SLV                                                (volatile uint8_t *)GALLIFREY_V3A_DIG_IFACE_OCTA_SPI_SLV_ADDR
#define GALLIFREY_V3A_DIG_GPIOS_OFFSET                                                      0x05
#define GALLIFREY_V3A_DIG_GPIOS_ADDR                                                        (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_GPIOS_OFFSET)
#define GALLIFREY_V3A_DIG_GPIOS_SZ                                                          8
#define GALLIFREY_V3A_DIG_GPIOS                                                             (volatile uint8_t *)GALLIFREY_V3A_DIG_GPIOS_ADDR
#define GALLIFREY_V3A_DIG_GPIOS_PD_OFFSET                                                   0x06
#define GALLIFREY_V3A_DIG_GPIOS_PD_ADDR                                                     (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_GPIOS_PD_OFFSET)
#define GALLIFREY_V3A_DIG_GPIOS_PD_SZ                                                       16
#define GALLIFREY_V3A_DIG_GPIOS_PD                                                          (volatile uint8_t *)GALLIFREY_V3A_DIG_GPIOS_PD_ADDR
#define GALLIFREY_V3A_DIG_GPIOS_PU_OFFSET                                                   0x08
#define GALLIFREY_V3A_DIG_GPIOS_PU_ADDR                                                     (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_GPIOS_PU_OFFSET)
#define GALLIFREY_V3A_DIG_GPIOS_PU_SZ                                                       16
#define GALLIFREY_V3A_DIG_GPIOS_PU                                                          (volatile uint8_t *)GALLIFREY_V3A_DIG_GPIOS_PU_ADDR
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_0_OFFSET                                               0x0a
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_0_ADDR                                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_GPIOS_GPIO_0_OFFSET)
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_0_SZ                                                   8
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_0                                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_GPIOS_GPIO_0_ADDR
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_1_OFFSET                                               0x0b
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_1_ADDR                                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_GPIOS_GPIO_1_OFFSET)
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_1_SZ                                                   8
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_1                                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_GPIOS_GPIO_1_ADDR
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_2_OFFSET                                               0x0c
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_2_ADDR                                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_GPIOS_GPIO_2_OFFSET)
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_2_SZ                                                   8
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_2                                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_GPIOS_GPIO_2_ADDR
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_3_OFFSET                                               0x0d
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_3_ADDR                                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_GPIOS_GPIO_3_OFFSET)
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_3_SZ                                                   8
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_3                                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_GPIOS_GPIO_3_ADDR
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_4_OFFSET                                               0x0e
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_4_ADDR                                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_GPIOS_GPIO_4_OFFSET)
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_4_SZ                                                   8
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_4                                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_GPIOS_GPIO_4_ADDR
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_5_OFFSET                                               0x0f
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_5_ADDR                                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_GPIOS_GPIO_5_OFFSET)
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_5_SZ                                                   8
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_5                                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_GPIOS_GPIO_5_ADDR
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_6_OFFSET                                               0x10
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_6_ADDR                                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_GPIOS_GPIO_6_OFFSET)
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_6_SZ                                                   8
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_6                                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_GPIOS_GPIO_6_ADDR
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_7_OFFSET                                               0x11
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_7_ADDR                                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_GPIOS_GPIO_7_OFFSET)
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_7_SZ                                                   8
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_7                                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_GPIOS_GPIO_7_ADDR
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_8_OFFSET                                               0x12
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_8_ADDR                                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_GPIOS_GPIO_8_OFFSET)
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_8_SZ                                                   8
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_8                                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_GPIOS_GPIO_8_ADDR
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_9_OFFSET                                               0x13
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_9_ADDR                                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_GPIOS_GPIO_9_OFFSET)
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_9_SZ                                                   8
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_9                                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_GPIOS_GPIO_9_ADDR
#define GALLIFREY_V3A_DIG_GPIOS_TEST_BUS_OFFSET                                             0x14
#define GALLIFREY_V3A_DIG_GPIOS_TEST_BUS_ADDR                                               (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_GPIOS_TEST_BUS_OFFSET)
#define GALLIFREY_V3A_DIG_GPIOS_TEST_BUS_SZ                                                 8
#define GALLIFREY_V3A_DIG_GPIOS_TEST_BUS                                                    (volatile uint8_t *)GALLIFREY_V3A_DIG_GPIOS_TEST_BUS_ADDR
#define GALLIFREY_V3A_DIG_IRQ_CTRL_OFFSET                                                   0x15
#define GALLIFREY_V3A_DIG_IRQ_CTRL_ADDR                                                     (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_IRQ_CTRL_OFFSET)
#define GALLIFREY_V3A_DIG_IRQ_CTRL_SZ                                                       8
#define GALLIFREY_V3A_DIG_IRQ_CTRL                                                          (volatile uint8_t *)GALLIFREY_V3A_DIG_IRQ_CTRL_ADDR
#define GALLIFREY_V3A_DIG_IRQ_CTRL_TX_OFFSET                                                0x16
#define GALLIFREY_V3A_DIG_IRQ_CTRL_TX_ADDR                                                  (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_IRQ_CTRL_TX_OFFSET)
#define GALLIFREY_V3A_DIG_IRQ_CTRL_TX_SZ                                                    8
#define GALLIFREY_V3A_DIG_IRQ_CTRL_TX                                                       (volatile uint8_t *)GALLIFREY_V3A_DIG_IRQ_CTRL_TX_ADDR
#define GALLIFREY_V3A_DIG_IRQ_CTRL_TX_PH_OFFSET                                             0x17
#define GALLIFREY_V3A_DIG_IRQ_CTRL_TX_PH_ADDR                                               (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_IRQ_CTRL_TX_PH_OFFSET)
#define GALLIFREY_V3A_DIG_IRQ_CTRL_TX_PH_SZ                                                 8
#define GALLIFREY_V3A_DIG_IRQ_CTRL_TX_PH                                                    (volatile uint8_t *)GALLIFREY_V3A_DIG_IRQ_CTRL_TX_PH_ADDR
#define GALLIFREY_V3A_DIG_IRQ_CTRL_RX_OFFSET                                                0x18
#define GALLIFREY_V3A_DIG_IRQ_CTRL_RX_ADDR                                                  (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_IRQ_CTRL_RX_OFFSET)
#define GALLIFREY_V3A_DIG_IRQ_CTRL_RX_SZ                                                    8
#define GALLIFREY_V3A_DIG_IRQ_CTRL_RX                                                       (volatile uint8_t *)GALLIFREY_V3A_DIG_IRQ_CTRL_RX_ADDR
#define GALLIFREY_V3A_DIG_IRQ_CTRL_RX_PH_OFFSET                                             0x19
#define GALLIFREY_V3A_DIG_IRQ_CTRL_RX_PH_ADDR                                               (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_IRQ_CTRL_RX_PH_OFFSET)
#define GALLIFREY_V3A_DIG_IRQ_CTRL_RX_PH_SZ                                                 8
#define GALLIFREY_V3A_DIG_IRQ_CTRL_RX_PH                                                    (volatile uint8_t *)GALLIFREY_V3A_DIG_IRQ_CTRL_RX_PH_ADDR
#define GALLIFREY_V3A_DIG_SEQ_CFG_OFFSET                                                    0x1a
#define GALLIFREY_V3A_DIG_SEQ_CFG_ADDR                                                      (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_SEQ_CFG_OFFSET)
#define GALLIFREY_V3A_DIG_SEQ_CFG_SZ                                                        8
#define GALLIFREY_V3A_DIG_SEQ_CFG                                                           (volatile uint8_t *)GALLIFREY_V3A_DIG_SEQ_CFG_ADDR
#define GALLIFREY_V3A_DIG_SEQ_OFFSET                                                        0x1b
#define GALLIFREY_V3A_DIG_SEQ_ADDR                                                          (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_SEQ_OFFSET)
#define GALLIFREY_V3A_DIG_SEQ_SZ                                                            8
#define GALLIFREY_V3A_DIG_SEQ                                                               (volatile uint8_t *)GALLIFREY_V3A_DIG_SEQ_ADDR
#define GALLIFREY_V3A_DIG_SEQ_SEQS_0_OFFSET                                                 0x1c
#define GALLIFREY_V3A_DIG_SEQ_SEQS_0_ADDR                                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_SEQ_SEQS_0_OFFSET)
#define GALLIFREY_V3A_DIG_SEQ_SEQS_0_SZ                                                     16
#define GALLIFREY_V3A_DIG_SEQ_SEQS_0                                                        (volatile uint8_t *)GALLIFREY_V3A_DIG_SEQ_SEQS_0_ADDR
#define GALLIFREY_V3A_DIG_SEQ_SEQS_1_OFFSET                                                 0x1e
#define GALLIFREY_V3A_DIG_SEQ_SEQS_1_ADDR                                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_SEQ_SEQS_1_OFFSET)
#define GALLIFREY_V3A_DIG_SEQ_SEQS_1_SZ                                                     16
#define GALLIFREY_V3A_DIG_SEQ_SEQS_1                                                        (volatile uint8_t *)GALLIFREY_V3A_DIG_SEQ_SEQS_1_ADDR
#define GALLIFREY_V3A_DIG_SEQ_SEQS_2_OFFSET                                                 0x20
#define GALLIFREY_V3A_DIG_SEQ_SEQS_2_ADDR                                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_SEQ_SEQS_2_OFFSET)
#define GALLIFREY_V3A_DIG_SEQ_SEQS_2_SZ                                                     16
#define GALLIFREY_V3A_DIG_SEQ_SEQS_2                                                        (volatile uint8_t *)GALLIFREY_V3A_DIG_SEQ_SEQS_2_ADDR
#define GALLIFREY_V3A_DIG_SEQ_SEQS_3_OFFSET                                                 0x22
#define GALLIFREY_V3A_DIG_SEQ_SEQS_3_ADDR                                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_SEQ_SEQS_3_OFFSET)
#define GALLIFREY_V3A_DIG_SEQ_SEQS_3_SZ                                                     16
#define GALLIFREY_V3A_DIG_SEQ_SEQS_3                                                        (volatile uint8_t *)GALLIFREY_V3A_DIG_SEQ_SEQS_3_ADDR
#define GALLIFREY_V3A_DIG_SEQ_SEQS_4_OFFSET                                                 0x24
#define GALLIFREY_V3A_DIG_SEQ_SEQS_4_ADDR                                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_SEQ_SEQS_4_OFFSET)
#define GALLIFREY_V3A_DIG_SEQ_SEQS_4_SZ                                                     16
#define GALLIFREY_V3A_DIG_SEQ_SEQS_4                                                        (volatile uint8_t *)GALLIFREY_V3A_DIG_SEQ_SEQS_4_ADDR
#define GALLIFREY_V3A_DIG_SEQ_SEQS_5_OFFSET                                                 0x26
#define GALLIFREY_V3A_DIG_SEQ_SEQS_5_ADDR                                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_SEQ_SEQS_5_OFFSET)
#define GALLIFREY_V3A_DIG_SEQ_SEQS_5_SZ                                                     16
#define GALLIFREY_V3A_DIG_SEQ_SEQS_5                                                        (volatile uint8_t *)GALLIFREY_V3A_DIG_SEQ_SEQS_5_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_OFFSET                                                     0x28
#define GALLIFREY_V3A_DIG_TX_TOP_ADDR                                                       (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_SZ                                                         8
#define GALLIFREY_V3A_DIG_TX_TOP                                                            (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_PH_CFG_OFFSET                                              0x29
#define GALLIFREY_V3A_DIG_TX_TOP_PH_CFG_ADDR                                                (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_PH_CFG_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_PH_CFG_SZ                                                  8
#define GALLIFREY_V3A_DIG_TX_TOP_PH_CFG                                                     (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_PH_CFG_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DEMUX_BIT_OFFSET                                  0x2a
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DEMUX_BIT_ADDR                                    (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DEMUX_BIT_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DEMUX_BIT_SZ                                      16
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DEMUX_BIT                                         (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DEMUX_BIT_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_PH_OFFSET                                                  0x2c
#define GALLIFREY_V3A_DIG_TX_TOP_PH_ADDR                                                    (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_PH_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_PH_SZ                                                      72
#define GALLIFREY_V3A_DIG_TX_TOP_PH                                                         (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_PH_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CHANGE_DT_OFFSET                                  0x35
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CHANGE_DT_ADDR                                    (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CHANGE_DT_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CHANGE_DT_SZ                                      8
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CHANGE_DT                                         (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CHANGE_DT_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MUX_BIT_OFFSET                                    0x36
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MUX_BIT_ADDR                                      (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MUX_BIT_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MUX_BIT_SZ                                        16
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MUX_BIT                                           (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MUX_BIT_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MUX_INT_OFFSET                                    0x38
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MUX_INT_ADDR                                      (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MUX_INT_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MUX_INT_SZ                                        16
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MUX_INT                                           (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MUX_INT_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_BIT_OFFSET                            0x3a
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_BIT_ADDR                              (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_BIT_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_BIT_SZ                                16
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_BIT                                   (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_BIT_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_0_OFFSET                          0x3c
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_0_ADDR                            (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_0_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_0_SZ                              16
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_0                                 (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_0_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_1_OFFSET                          0x3e
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_1_ADDR                            (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_1_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_1_SZ                              16
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_1                                 (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_1_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_INVERT_BIT_OFFSET                                 0x40
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_INVERT_BIT_ADDR                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_INVERT_BIT_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_INVERT_BIT_SZ                                     8
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_INVERT_BIT                                        (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_INVERT_BIT_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_0_OFFSET                             0x41
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_0_ADDR                               (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_0_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_0_SZ                                 8
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_0                                    (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_0_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BIT_REPEATER_OFFSET                               0x42
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BIT_REPEATER_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BIT_REPEATER_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BIT_REPEATER_SZ                                   24
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BIT_REPEATER                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BIT_REPEATER_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_1_OFFSET                             0x45
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_1_ADDR                               (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_1_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_1_SZ                                 8
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_1                                    (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_1_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_IEEE802154_B2C_OFFSET                             0x46
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_IEEE802154_B2C_ADDR                               (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_IEEE802154_B2C_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_IEEE802154_B2C_SZ                                 16
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_IEEE802154_B2C                                    (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_IEEE802154_B2C_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CRC_CUSTOM_OFFSET                                 0x48
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CRC_CUSTOM_ADDR                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CRC_CUSTOM_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CRC_CUSTOM_SZ                                     32
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CRC_CUSTOM                                        (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CRC_CUSTOM_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DATA_WHITENING_OFFSET                             0x4c
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DATA_WHITENING_ADDR                               (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DATA_WHITENING_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DATA_WHITENING_SZ                                 16
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DATA_WHITENING                                    (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DATA_WHITENING_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MANCHESTER_OFFSET                                 0x4e
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MANCHESTER_ADDR                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MANCHESTER_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MANCHESTER_SZ                                     16
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MANCHESTER                                        (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MANCHESTER_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PIO4_4_8PSK_OFFSET                                0x50
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PIO4_4_8PSK_ADDR                                  (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PIO4_4_8PSK_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PIO4_4_8PSK_SZ                                    16
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PIO4_4_8PSK                                       (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PIO4_4_8PSK_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CONV_CODES_OFFSET                                 0x52
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CONV_CODES_ADDR                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CONV_CODES_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CONV_CODES_SZ                                     16
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CONV_CODES                                        (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CONV_CODES_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_0_OFFSET                             0x54
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_0_ADDR                               (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_0_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_0_SZ                                 16
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_0                                    (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_0_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_1_OFFSET                             0x56
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_1_ADDR                               (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_1_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_1_SZ                                 16
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_1                                    (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_1_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_MAN_OFFSET                                   0x58
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_MAN_ADDR                                     (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_MAN_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_MAN_SZ                                       16
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_MAN                                          (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_MAN_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_0_OFFSET                                     0x5a
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_0_ADDR                                       (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_0_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_0_SZ                                         24
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_0                                            (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_0_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DECIMATOR_OFFSET                                  0x5d
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DECIMATOR_ADDR                                    (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DECIMATOR_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DECIMATOR_SZ                                      8
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DECIMATOR                                         (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DECIMATOR_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_1_OFFSET                                     0x5e
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_1_ADDR                                       (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_1_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_1_SZ                                         24
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_1                                            (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_1_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DELAY_PATH_OFFSET                                 0x61
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DELAY_PATH_ADDR                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DELAY_PATH_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DELAY_PATH_SZ                                     8
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DELAY_PATH                                        (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DELAY_PATH_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_2_OFFSET                                     0x62
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_2_ADDR                                       (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_2_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_2_SZ                                         24
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_2                                            (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_2_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DERIVATIVE_OFFSET                                 0x65
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DERIVATIVE_ADDR                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DERIVATIVE_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DERIVATIVE_SZ                                     8
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DERIVATIVE                                        (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DERIVATIVE_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_GENERIC_MAPPER_OFFSET                             0x66
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_GENERIC_MAPPER_ADDR                               (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_GENERIC_MAPPER_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_GENERIC_MAPPER_SZ                                 24
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_GENERIC_MAPPER                                    (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_GENERIC_MAPPER_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_AM2PM_OFFSET                                   0x69
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_AM2PM_ADDR                                     (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_AM2PM_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_AM2PM_SZ                                       8
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_AM2PM                                          (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_AM2PM_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_FRACTIONAL_INTERPOLATOR_OFFSET                    0x6a
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_FRACTIONAL_INTERPOLATOR_ADDR                      (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_FRACTIONAL_INTERPOLATOR_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_FRACTIONAL_INTERPOLATOR_SZ                        16
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_FRACTIONAL_INTERPOLATOR                           (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_FRACTIONAL_INTERPOLATOR_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_OFFSET                               0x6c
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_SZ                                   168
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_SERIAL_FIFO_OFFSET                                         0x81
#define GALLIFREY_V3A_DIG_TX_TOP_SERIAL_FIFO_ADDR                                           (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_SERIAL_FIFO_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_SERIAL_FIFO_SZ                                             8
#define GALLIFREY_V3A_DIG_TX_TOP_SERIAL_FIFO                                                (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_SERIAL_FIFO_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BLOCK_DATA_OFFSET                                 0x82
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BLOCK_DATA_ADDR                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BLOCK_DATA_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BLOCK_DATA_SZ                                     16
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BLOCK_DATA                                        (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BLOCK_DATA_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_FSK_OFFSET                           0x84
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_FSK_ADDR                             (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_FSK_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_FSK_SZ                               152
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_FSK                                  (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_FSK_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_PA_POWER_OFFSET                                     0x97
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_PA_POWER_ADDR                                       (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TX_POW_PA_POWER_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_PA_POWER_SZ                                         8
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_PA_POWER                                            (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TX_POW_PA_POWER_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SATURATE_OFFSET                                   0x98
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SATURATE_ADDR                                     (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SATURATE_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SATURATE_SZ                                       32
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SATURATE                                          (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SATURATE_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_HOLD_DATA_OFFSET                                  0x9c
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_HOLD_DATA_ADDR                                    (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_HOLD_DATA_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_HOLD_DATA_SZ                                      16
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_HOLD_DATA                                         (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_HOLD_DATA_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CONST_DATA_OFFSET                                 0x9e
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CONST_DATA_ADDR                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CONST_DATA_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CONST_DATA_SZ                                     16
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CONST_DATA                                        (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CONST_DATA_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SKIP_DATA_OFFSET                                  0xa0
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SKIP_DATA_ADDR                                    (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SKIP_DATA_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SKIP_DATA_SZ                                      16
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SKIP_DATA                                         (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SKIP_DATA_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_11_OFFSET                              0xa2
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_11_ADDR                                (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_11_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_11_SZ                                  16
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_11                                     (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_11_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_RAMP_OFFSET                                    0xa4
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_RAMP_ADDR                                      (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_RAMP_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_RAMP_SZ                                        96
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_RAMP                                           (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_RAMP_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_AM2PM_LUT_OFFSET                               0xb0
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_AM2PM_LUT_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_AM2PM_LUT_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_AM2PM_LUT_SZ                                   32
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_AM2PM_LUT                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_AM2PM_LUT_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_LINEARIZE_OFFSET                               0xb4
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_LINEARIZE_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_LINEARIZE_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_LINEARIZE_SZ                                   120
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_LINEARIZE                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_LINEARIZE_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_PA_FILT_OFFSET                                             0xc3
#define GALLIFREY_V3A_DIG_TX_TOP_PA_FILT_ADDR                                               (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_PA_FILT_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_PA_FILT_SZ                                                 8
#define GALLIFREY_V3A_DIG_TX_TOP_PA_FILT                                                    (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_PA_FILT_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_TX_BLE_DF_OFFSET                                  0xc4
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_TX_BLE_DF_ADDR                                    (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_TX_BLE_DF_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_TX_BLE_DF_SZ                                      32
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_TX_BLE_DF                                         (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_TX_BLE_DF_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_0_OFFSET                               0xc8
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_0_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_0_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_0_SZ                                   24
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_0                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_0_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_OFFSET                                                     0xcb
#define GALLIFREY_V3A_DIG_RX_TOP_ADDR                                                       (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_SZ                                                         8
#define GALLIFREY_V3A_DIG_RX_TOP                                                            (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_1_OFFSET                               0xcc
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_1_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_1_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_1_SZ                                   24
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_1                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_1_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_INPUT_ADDR_OFFSET                                      0xcf
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_INPUT_ADDR_ADDR                                        (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_INPUT_ADDR_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_INPUT_ADDR_SZ                                          8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_INPUT_ADDR                                             (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_INPUT_ADDR_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_2_OFFSET                               0xd0
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_2_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_2_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_2_SZ                                   24
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_2                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_2_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_CTRL_OFFSET                                            0xd3
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_CTRL_ADDR                                              (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_AGC_CTRL_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_CTRL_SZ                                                8
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_CTRL                                                   (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_AGC_CTRL_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_3_OFFSET                               0xd4
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_3_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_3_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_3_SZ                                   24
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_3                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_3_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_TMR_PREDIV_OFFSET                                      0xd7
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_TMR_PREDIV_ADDR                                        (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_AGC_TMR_PREDIV_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_TMR_PREDIV_SZ                                          8
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_TMR_PREDIV                                             (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_AGC_TMR_PREDIV_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_4_OFFSET                               0xd8
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_4_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_4_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_4_SZ                                   24
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_4                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_4_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_OFFSET                                                 0xdb
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ADDR                                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_AGC_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_SZ                                                     8
#define GALLIFREY_V3A_DIG_RX_TOP_AGC                                                        (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_AGC_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_5_OFFSET                               0xdc
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_5_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_5_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_5_SZ                                   24
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_5                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_5_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_TECHNO_DEPENDANT_OFFSET                                0xdf
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_TECHNO_DEPENDANT_ADDR                                  (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_AGC_TECHNO_DEPENDANT_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_TECHNO_DEPENDANT_SZ                                    8
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_TECHNO_DEPENDANT                                       (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_AGC_TECHNO_DEPENDANT_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_6_OFFSET                               0xe0
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_6_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_6_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_6_SZ                                   24
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_6                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_6_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_RF_OFFSET                                              0xe3
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_RF_ADDR                                                (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_AGC_RF_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_RF_SZ                                                  8
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_RF                                                     (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_AGC_RF_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_7_OFFSET                               0xe4
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_7_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_7_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_7_SZ                                   24
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_7                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_7_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_PA_AMP_OFFSET                                              0xe7
#define GALLIFREY_V3A_DIG_TX_TOP_PA_AMP_ADDR                                                (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_PA_AMP_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_PA_AMP_SZ                                                  8
#define GALLIFREY_V3A_DIG_TX_TOP_PA_AMP                                                     (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_PA_AMP_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_8_OFFSET                               0xe8
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_8_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_8_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_8_SZ                                   24
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_8                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_8_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_BB_OFFSET                                              0xeb
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_BB_ADDR                                                (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_AGC_BB_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_BB_SZ                                                  8
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_BB                                                     (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_AGC_BB_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_9_OFFSET                               0xec
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_9_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_9_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_9_SZ                                   24
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_9                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_9_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_0_OFFSET                            0xef
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_0_ADDR                              (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_0_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_0_SZ                                8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_0                                   (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_0_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_10_OFFSET                              0xf0
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_10_ADDR                                (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_10_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_10_SZ                                  24
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_10                                     (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_10_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_1_OFFSET                            0xf3
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_1_ADDR                              (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_1_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_1_SZ                                8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_1                                   (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_1_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_0_OFFSET                                        0xf4
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_0_ADDR                                          (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_0_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_0_SZ                                            16
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_0                                               (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_0_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_1_OFFSET                                        0xf6
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_1_ADDR                                          (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_1_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_1_SZ                                            16
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_1                                               (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_1_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_2_OFFSET                                        0xf8
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_2_ADDR                                          (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_2_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_2_SZ                                            16
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_2                                               (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_2_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_3_OFFSET                                        0xfa
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_3_ADDR                                          (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_3_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_3_SZ                                            16
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_3                                               (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_3_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_4_OFFSET                                        0xfc
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_4_ADDR                                          (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_4_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_4_SZ                                            16
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_4                                               (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_4_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_5_OFFSET                                        0xfe
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_5_ADDR                                          (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_5_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_5_SZ                                            16
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_5                                               (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_5_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_6_OFFSET                                        0x100
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_6_ADDR                                          (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_6_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_6_SZ                                            16
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_6                                               (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_6_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_7_OFFSET                                        0x102
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_7_ADDR                                          (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_7_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_7_SZ                                            16
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_7                                               (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_7_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_8_OFFSET                                        0x104
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_8_ADDR                                          (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_8_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_8_SZ                                            16
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_8                                               (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_8_ADDR
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_9_OFFSET                                        0x106
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_9_ADDR                                          (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_9_OFFSET)
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_9_SZ                                            16
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_9                                               (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_9_ADDR
#define GALLIFREY_V3A_DIG_PROT_TMR_OFFSET                                                   0x108
#define GALLIFREY_V3A_DIG_PROT_TMR_ADDR                                                     (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_PROT_TMR_OFFSET)
#define GALLIFREY_V3A_DIG_PROT_TMR_SZ                                                       24
#define GALLIFREY_V3A_DIG_PROT_TMR                                                          (volatile uint8_t *)GALLIFREY_V3A_DIG_PROT_TMR_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_OSR_INIT_OFFSET                                        0x10b
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_OSR_INIT_ADDR                                          (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_OSR_INIT_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_OSR_INIT_SZ                                            8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_OSR_INIT                                               (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_OSR_INIT_ADDR
#define GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_0_OFFSET                                          0x10c
#define GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_0_ADDR                                            (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_0_OFFSET)
#define GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_0_SZ                                              16
#define GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_0                                                 (volatile uint8_t *)GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_0_ADDR
#define GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_1_OFFSET                                          0x10e
#define GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_1_ADDR                                            (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_1_OFFSET)
#define GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_1_SZ                                              16
#define GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_1                                                 (volatile uint8_t *)GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_1_ADDR
#define GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_2_OFFSET                                          0x110
#define GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_2_ADDR                                            (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_2_OFFSET)
#define GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_2_SZ                                              16
#define GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_2                                                 (volatile uint8_t *)GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_2_ADDR
#define GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_3_OFFSET                                          0x112
#define GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_3_ADDR                                            (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_3_OFFSET)
#define GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_3_SZ                                              16
#define GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_3                                                 (volatile uint8_t *)GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_3_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_PH_OFFSET                                                  0x114
#define GALLIFREY_V3A_DIG_RX_TOP_PH_ADDR                                                    (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_PH_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_PH_SZ                                                      24
#define GALLIFREY_V3A_DIG_RX_TOP_PH                                                         (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_PH_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0_OFFSET                         0x117
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0_ADDR                           (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0_SZ                             8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0                                (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0_DELAY_OFFSET                   0x118
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0_DELAY_ADDR                     (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0_DELAY_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0_DELAY_SZ                       8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0_DELAY                          (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0_DELAY_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1_OFFSET                         0x119
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1_ADDR                           (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1_SZ                             8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1                                (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1_DELAY_OFFSET                   0x11a
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1_DELAY_ADDR                     (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1_DELAY_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1_DELAY_SZ                       8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1_DELAY                          (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1_DELAY_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_0_OFFSET                           0x11b
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_0_ADDR                             (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_0_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_0_SZ                               8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_0                                  (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_0_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_1_OFFSET                           0x11c
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_1_ADDR                             (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_1_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_1_SZ                               8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_1                                  (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_1_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_2_OFFSET                           0x11d
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_2_ADDR                             (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_2_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_2_SZ                               8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_2                                  (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_2_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_3_OFFSET                           0x11e
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_3_ADDR                             (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_3_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_3_SZ                               8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_3                                  (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_3_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_4_OFFSET                           0x11f
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_4_ADDR                             (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_4_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_4_SZ                               8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_4                                  (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_4_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_5_OFFSET                           0x120
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_5_ADDR                             (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_5_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_5_SZ                               8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_5                                  (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_5_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0_OFFSET                                0x121
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0_ADDR                                  (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0_SZ                                    8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0                                       (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_X4_DEST_OFFSET                          0x122
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_X4_DEST_ADDR                            (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_X4_DEST_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_X4_DEST_SZ                              16
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_X4_DEST                                 (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_X4_DEST_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0_DELAY_OFFSET                          0x124
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0_DELAY_ADDR                            (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0_DELAY_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0_DELAY_SZ                              8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0_DELAY                                 (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0_DELAY_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1_OFFSET                                0x125
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1_ADDR                                  (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1_SZ                                    8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1                                       (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1_DELAY_OFFSET                          0x126
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1_DELAY_ADDR                            (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1_DELAY_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1_DELAY_SZ                              8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1_DELAY                                 (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1_DELAY_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_HP_FILTER_OFFSET                                  0x127
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_HP_FILTER_ADDR                                    (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_HP_FILTER_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_HP_FILTER_SZ                                      8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_HP_FILTER                                         (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_HP_FILTER_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_ADC_LIMIT_DETECT_OFFSET                           0x128
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_ADC_LIMIT_DETECT_ADDR                             (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_ADC_LIMIT_DETECT_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_ADC_LIMIT_DETECT_SZ                               40
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_ADC_LIMIT_DETECT                                  (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_ADC_LIMIT_DETECT_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_0_OFFSET                                0x12d
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_0_ADDR                                  (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_0_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_0_SZ                                    8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_0                                       (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_0_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IQ_BALANCE_OFFSET                                 0x12e
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IQ_BALANCE_ADDR                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IQ_BALANCE_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IQ_BALANCE_SZ                                     24
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IQ_BALANCE                                        (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IQ_BALANCE_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_1_OFFSET                                0x131
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_1_ADDR                                  (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_1_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_1_SZ                                    8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_1                                       (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_1_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_REMOVE_IF_OFFSET                                  0x132
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_REMOVE_IF_ADDR                                    (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_REMOVE_IF_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_REMOVE_IF_SZ                                      16
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_REMOVE_IF                                         (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_REMOVE_IF_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_2_OFFSET                                0x134
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_2_ADDR                                  (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_2_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_2_SZ                                    8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_2                                       (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_2_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_3_OFFSET                                0x135
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_3_ADDR                                  (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_3_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_3_SZ                                    8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_3                                       (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_3_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FRACTIONAL_DECIMATOR_OFFSET                       0x136
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FRACTIONAL_DECIMATOR_ADDR                         (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FRACTIONAL_DECIMATOR_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FRACTIONAL_DECIMATOR_SZ                           8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FRACTIONAL_DECIMATOR                              (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FRACTIONAL_DECIMATOR_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORDIC_LIN2POL_OFFSET                             0x137
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORDIC_LIN2POL_ADDR                               (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORDIC_LIN2POL_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORDIC_LIN2POL_SZ                                 8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORDIC_LIN2POL                                    (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORDIC_LIN2POL_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DERIVATIVE_OFFSET                                 0x138
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DERIVATIVE_ADDR                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DERIVATIVE_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DERIVATIVE_SZ                                     8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DERIVATIVE                                        (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DERIVATIVE_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DIFF_BY_N_OFFSET                                  0x139
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DIFF_BY_N_ADDR                                    (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DIFF_BY_N_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DIFF_BY_N_SZ                                      8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DIFF_BY_N                                         (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DIFF_BY_N_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_CONF_OFFSET                            0x13a
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_CONF_ADDR                              (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_CONF_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_CONF_SZ                                8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_CONF                                   (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_CONF_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_DELAY_OFFSET                           0x13b
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_DELAY_ADDR                             (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_DELAY_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_DELAY_SZ                               8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_DELAY                                  (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_DELAY_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_OPTS_OFFSET                            0x13c
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_OPTS_ADDR                              (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_OPTS_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_OPTS_SZ                                40
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_OPTS                                   (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_OPTS_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0_OFFSET                           0x141
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0_ADDR                             (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0_SZ                               8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0                                  (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_CMDS_OFFSET                            0x142
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_CMDS_ADDR                              (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_CMDS_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_CMDS_SZ                                16
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_CMDS                                   (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_CMDS_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0_PS_COEFS_OFFSET                  0x144
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0_PS_COEFS_ADDR                    (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0_PS_COEFS_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0_PS_COEFS_SZ                      64
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0_PS_COEFS                         (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0_PS_COEFS_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1_PS_COEFS_OFFSET                  0x14c
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1_PS_COEFS_ADDR                    (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1_PS_COEFS_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1_PS_COEFS_SZ                      32
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1_PS_COEFS                         (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1_PS_COEFS_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1_OFFSET                           0x150
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1_ADDR                             (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1_SZ                               8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1                                  (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_CTE_INFO_OFFSET                         0x151
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_CTE_INFO_ADDR                           (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_CTE_INFO_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_CTE_INFO_SZ                             8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_CTE_INFO                                (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_CTE_INFO_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_FAKE_REG_OFFSET                         0x152
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_FAKE_REG_ADDR                           (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_FAKE_REG_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_FAKE_REG_SZ                             8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_FAKE_REG                                (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_FAKE_REG_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_DELAY_OFFSET                            0x153
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_DELAY_ADDR                              (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_DELAY_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_DELAY_SZ                                8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_DELAY                                   (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_DELAY_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_OFFSET                                  0x154
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_ADDR                                    (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_SZ                                      32
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF                                         (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_OFFSET                             0x158
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_ADDR                               (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_SZ                                 8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY                                    (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_DELAY_OFFSET                       0x159
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_DELAY_ADDR                         (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_DELAY_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_DELAY_SZ                           8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_DELAY                              (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_DELAY_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DPSK_TED_OFFSET                                   0x15a
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DPSK_TED_ADDR                                     (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DPSK_TED_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DPSK_TED_SZ                                       16
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DPSK_TED                                          (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DPSK_TED_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_PHASE_INTERP_X2_OFFSET                            0x15c
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_PHASE_INTERP_X2_ADDR                              (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_PHASE_INTERP_X2_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_PHASE_INTERP_X2_SZ                                8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_PHASE_INTERP_X2                                   (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_PHASE_INTERP_X2_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FSK_TED_OFFSET                                    0x15d
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FSK_TED_ADDR                                      (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FSK_TED_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FSK_TED_SZ                                        8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FSK_TED                                           (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FSK_TED_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CARRIER_RECOVERY_OFFSET                           0x15e
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CARRIER_RECOVERY_ADDR                             (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CARRIER_RECOVERY_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CARRIER_RECOVERY_SZ                               16
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CARRIER_RECOVERY                                  (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CARRIER_RECOVERY_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_DEMAPPER_OFFSET                               0x160
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_DEMAPPER_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_DEMAPPER_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_DEMAPPER_SZ                                   8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_DEMAPPER                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_DEMAPPER_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_DELAY_OFFSET                      0x161
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_DELAY_ADDR                        (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_DELAY_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_DELAY_SZ                          8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_DELAY                             (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_DELAY_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_OFFSET                            0x162
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_ADDR                              (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_SZ                                16
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT                                   (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_OFFSET                            0x164
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_ADDR                              (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_SZ                                16
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT                                   (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_DELAY_OFFSET                      0x166
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_DELAY_ADDR                        (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_DELAY_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_DELAY_SZ                          8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_DELAY                             (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_DELAY_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DPSK_DEMOD_OFFSET                                 0x167
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DPSK_DEMOD_ADDR                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DPSK_DEMOD_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DPSK_DEMOD_SZ                                     8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DPSK_DEMOD                                        (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DPSK_DEMOD_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_OFFSET                            0x168
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_ADDR                              (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_SZ                                24
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B                                   (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_DELAY_OFFSET                      0x16b
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_DELAY_ADDR                        (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_DELAY_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_DELAY_SZ                          8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_DELAY                             (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_DELAY_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_OFFSET                               0x16c
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_SZ                                   48
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_DELAY_DELAY_OFFSET                            0x172
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_DELAY_DELAY_ADDR                              (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_DELAY_DELAY_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_DELAY_DELAY_SZ                                8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_DELAY_DELAY                                   (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_DELAY_DELAY_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_SUBSAMPLE_OFFSET                                  0x173
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_SUBSAMPLE_ADDR                                    (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_SUBSAMPLE_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_SUBSAMPLE_SZ                                      8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_SUBSAMPLE                                         (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_SUBSAMPLE_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CRC_CUSTOM_OFFSET                                 0x174
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CRC_CUSTOM_ADDR                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CRC_CUSTOM_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CRC_CUSTOM_SZ                                     8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CRC_CUSTOM                                        (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CRC_CUSTOM_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_OFFSET                               0x175
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_SZ                                   8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_BLR_PACKET_LEN_OFFSET                0x176
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_BLR_PACKET_LEN_ADDR                  (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_BLR_PACKET_LEN_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_BLR_PACKET_LEN_SZ                    8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_BLR_PACKET_LEN                       (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_BLR_PACKET_LEN_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RSSI_READOUT_OFFSET                               0x177
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RSSI_READOUT_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RSSI_READOUT_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RSSI_READOUT_SZ                                   8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RSSI_READOUT                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RSSI_READOUT_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_CMDS_OFFSET                          0x178
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_CMDS_ADDR                            (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_CMDS_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_CMDS_SZ                              24
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_CMDS                                 (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_CMDS_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FLUSH_DELAY_OFFSET                                0x17b
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FLUSH_DELAY_ADDR                                  (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FLUSH_DELAY_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FLUSH_DELAY_SZ                                    8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FLUSH_DELAY                                       (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FLUSH_DELAY_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_LIMIT_RATE_OFFSET                                 0x17c
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_LIMIT_RATE_ADDR                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_LIMIT_RATE_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_LIMIT_RATE_SZ                                     16
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_LIMIT_RATE                                        (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_LIMIT_RATE_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FLUSH_OFFSET                                      0x17e
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FLUSH_ADDR                                        (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FLUSH_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FLUSH_SZ                                          16
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FLUSH                                             (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FLUSH_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_0_OFFSET                                     0x180
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_0_ADDR                                       (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_0_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_0_SZ                                         24
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_0                                            (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_0_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_CTE_OFFSET                                                 0x183
#define GALLIFREY_V3A_DIG_RX_TOP_CTE_ADDR                                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_CTE_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_CTE_SZ                                                     8
#define GALLIFREY_V3A_DIG_RX_TOP_CTE                                                        (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_CTE_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_1_OFFSET                                     0x184
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_1_ADDR                                       (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_1_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_1_SZ                                         24
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_1                                            (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_1_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_ADC_CTRL_OFFSET                                            0x187
#define GALLIFREY_V3A_DIG_RX_TOP_ADC_CTRL_ADDR                                              (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_ADC_CTRL_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_ADC_CTRL_SZ                                                8
#define GALLIFREY_V3A_DIG_RX_TOP_ADC_CTRL                                                   (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_ADC_CTRL_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_2_OFFSET                                     0x188
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_2_ADDR                                       (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_2_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_2_SZ                                         24
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_2                                            (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_2_ADDR
#define GALLIFREY_V3A_DIG_CLK_CTRL_OFFSET                                                   0x18b
#define GALLIFREY_V3A_DIG_CLK_CTRL_ADDR                                                     (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_CLK_CTRL_OFFSET)
#define GALLIFREY_V3A_DIG_CLK_CTRL_SZ                                                       8
#define GALLIFREY_V3A_DIG_CLK_CTRL                                                          (volatile uint8_t *)GALLIFREY_V3A_DIG_CLK_CTRL_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_3_OFFSET                                     0x18c
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_3_ADDR                                       (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_3_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_3_SZ                                         24
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_3                                            (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_3_ADDR
#define GALLIFREY_V3A_DIG_AES_CFG_OFFSET                                                    0x18f
#define GALLIFREY_V3A_DIG_AES_CFG_ADDR                                                      (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_AES_CFG_OFFSET)
#define GALLIFREY_V3A_DIG_AES_CFG_SZ                                                        8
#define GALLIFREY_V3A_DIG_AES_CFG                                                           (volatile uint8_t *)GALLIFREY_V3A_DIG_AES_CFG_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_0_OFFSET                               0x190
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_0_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_0_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_0_SZ                                   32
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_0                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_0_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_1_OFFSET                               0x194
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_1_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_1_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_1_SZ                                   32
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_1                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_1_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_2_OFFSET                               0x198
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_2_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_2_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_2_SZ                                   32
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_2                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_2_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_3_OFFSET                               0x19c
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_3_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_3_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_3_SZ                                   32
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_3                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_3_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_4_OFFSET                               0x1a0
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_4_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_4_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_4_SZ                                   32
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_4                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_4_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_5_OFFSET                               0x1a4
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_5_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_5_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_5_SZ                                   32
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_5                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_5_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_6_OFFSET                               0x1a8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_6_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_6_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_6_SZ                                   32
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_6                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_6_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_7_OFFSET                               0x1ac
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_7_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_7_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_7_SZ                                   32
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_7                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_7_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_8_OFFSET                               0x1b0
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_8_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_8_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_8_SZ                                   32
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_8                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_8_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_9_OFFSET                               0x1b4
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_9_ADDR                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_9_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_9_SZ                                   32
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_9                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_9_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_10_OFFSET                              0x1b8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_10_ADDR                                (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_10_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_10_SZ                                  32
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_10                                     (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_10_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_11_OFFSET                              0x1bc
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_11_ADDR                                (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_11_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_11_SZ                                  32
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_11                                     (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_11_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_12_OFFSET                              0x1c0
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_12_ADDR                                (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_12_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_12_SZ                                  32
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_12                                     (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_12_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_13_OFFSET                              0x1c4
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_13_ADDR                                (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_13_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_13_SZ                                  32
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_13                                     (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_13_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_14_OFFSET                              0x1c8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_14_ADDR                                (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_14_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_14_SZ                                  32
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_14                                     (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_14_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_15_OFFSET                              0x1cc
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_15_ADDR                                (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_15_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_15_SZ                                  32
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_15                                     (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_15_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_16_OFFSET                              0x1d0
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_16_ADDR                                (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_16_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_16_SZ                                  32
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_16                                     (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_16_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_17_OFFSET                              0x1d4
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_17_ADDR                                (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_17_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_17_SZ                                  32
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_17                                     (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_17_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_18_OFFSET                              0x1d8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_18_ADDR                                (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_18_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_18_SZ                                  32
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_18                                     (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_18_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_19_OFFSET                              0x1dc
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_19_ADDR                                (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_19_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_19_SZ                                  32
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_19                                     (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_19_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_20_OFFSET                              0x1e0
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_20_ADDR                                (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_20_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_20_SZ                                  32
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_20                                     (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_20_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_21_OFFSET                              0x1e4
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_21_ADDR                                (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_21_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_21_SZ                                  32
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_21                                     (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_21_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_22_OFFSET                              0x1e8
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_22_ADDR                                (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_22_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_22_SZ                                  32
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_22                                     (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_22_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_23_OFFSET                              0x1ec
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_23_ADDR                                (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_23_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_23_SZ                                  32
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_23                                     (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_23_ADDR
#define GALLIFREY_V3A_DIG_DCO_CAL_OFFSET                                                    0x1f0
#define GALLIFREY_V3A_DIG_DCO_CAL_ADDR                                                      (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_DCO_CAL_OFFSET)
#define GALLIFREY_V3A_DIG_DCO_CAL_SZ                                                        16
#define GALLIFREY_V3A_DIG_DCO_CAL                                                           (volatile uint8_t *)GALLIFREY_V3A_DIG_DCO_CAL_ADDR
#define GALLIFREY_V3A_DIG_CLK_CTRL_CLK_DIV_OFFSET                                           0x1f2
#define GALLIFREY_V3A_DIG_CLK_CTRL_CLK_DIV_ADDR                                             (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_CLK_CTRL_CLK_DIV_OFFSET)
#define GALLIFREY_V3A_DIG_CLK_CTRL_CLK_DIV_SZ                                               24
#define GALLIFREY_V3A_DIG_CLK_CTRL_CLK_DIV                                                  (volatile uint8_t *)GALLIFREY_V3A_DIG_CLK_CTRL_CLK_DIV_ADDR
#define GALLIFREY_V3A_DIG_IQ_FIFO_OFFSET                                                    0x1f5
#define GALLIFREY_V3A_DIG_IQ_FIFO_ADDR                                                      (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_IQ_FIFO_OFFSET)
#define GALLIFREY_V3A_DIG_IQ_FIFO_SZ                                                        8
#define GALLIFREY_V3A_DIG_IQ_FIFO                                                           (volatile uint8_t *)GALLIFREY_V3A_DIG_IQ_FIFO_ADDR
#define GALLIFREY_V3A_DIG_RCCO_COMP_OFFSET                                                  0x1f6
#define GALLIFREY_V3A_DIG_RCCO_COMP_ADDR                                                    (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RCCO_COMP_OFFSET)
#define GALLIFREY_V3A_DIG_RCCO_COMP_SZ                                                      16
#define GALLIFREY_V3A_DIG_RCCO_COMP                                                         (volatile uint8_t *)GALLIFREY_V3A_DIG_RCCO_COMP_ADDR
#define GALLIFREY_V3A_DIG_AES_KEY_OFFSET                                                    0x1f8
#define GALLIFREY_V3A_DIG_AES_KEY_ADDR                                                      (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_AES_KEY_OFFSET)
#define GALLIFREY_V3A_DIG_AES_KEY_SZ                                                        128
#define GALLIFREY_V3A_DIG_AES_KEY                                                           (volatile uint8_t *)GALLIFREY_V3A_DIG_AES_KEY_ADDR
#define GALLIFREY_V3A_DIG_AES_NONCE_OFFSET                                                  0x208
#define GALLIFREY_V3A_DIG_AES_NONCE_ADDR                                                    (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_AES_NONCE_OFFSET)
#define GALLIFREY_V3A_DIG_AES_NONCE_SZ                                                      104
#define GALLIFREY_V3A_DIG_AES_NONCE                                                         (volatile uint8_t *)GALLIFREY_V3A_DIG_AES_NONCE_ADDR
#define GALLIFREY_V3A_DIG_RX_FIFO_OFFSET                                                    0x215
#define GALLIFREY_V3A_DIG_RX_FIFO_ADDR                                                      (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_FIFO_OFFSET)
#define GALLIFREY_V3A_DIG_RX_FIFO_SZ                                                        8
#define GALLIFREY_V3A_DIG_RX_FIFO                                                           (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_FIFO_ADDR
#define GALLIFREY_V3A_DIG_TX_FIFO_OFFSET                                                    0x216
#define GALLIFREY_V3A_DIG_TX_FIFO_ADDR                                                      (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_FIFO_OFFSET)
#define GALLIFREY_V3A_DIG_TX_FIFO_SZ                                                        8
#define GALLIFREY_V3A_DIG_TX_FIFO                                                           (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_FIFO_ADDR
#define GALLIFREY_V3A_DIG_DIR_FIND_OFFSET                                                   0x217
#define GALLIFREY_V3A_DIG_DIR_FIND_ADDR                                                     (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_DIR_FIND_OFFSET)
#define GALLIFREY_V3A_DIG_DIR_FIND_SZ                                                       8
#define GALLIFREY_V3A_DIG_DIR_FIND                                                          (volatile uint8_t *)GALLIFREY_V3A_DIG_DIR_FIND_ADDR
#define GALLIFREY_V3A_DIG_TX_BLE_DTM_OFFSET                                                 0x218
#define GALLIFREY_V3A_DIG_TX_BLE_DTM_ADDR                                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TX_BLE_DTM_OFFSET)
#define GALLIFREY_V3A_DIG_TX_BLE_DTM_SZ                                                     16
#define GALLIFREY_V3A_DIG_TX_BLE_DTM                                                        (volatile uint8_t *)GALLIFREY_V3A_DIG_TX_BLE_DTM_ADDR
#define GALLIFREY_V3A_DIG_CHANNEL_HDL_OFFSET                                                0x21a
#define GALLIFREY_V3A_DIG_CHANNEL_HDL_ADDR                                                  (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_CHANNEL_HDL_OFFSET)
#define GALLIFREY_V3A_DIG_CHANNEL_HDL_SZ                                                    8
#define GALLIFREY_V3A_DIG_CHANNEL_HDL                                                       (volatile uint8_t *)GALLIFREY_V3A_DIG_CHANNEL_HDL_ADDR
#define GALLIFREY_V3A_DIG_MISC_OFFSET                                                       0x21b
#define GALLIFREY_V3A_DIG_MISC_ADDR                                                         (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_MISC_OFFSET)
#define GALLIFREY_V3A_DIG_MISC_SZ                                                           8
#define GALLIFREY_V3A_DIG_MISC                                                              (volatile uint8_t *)GALLIFREY_V3A_DIG_MISC_ADDR
#define GALLIFREY_V3A_DIG_TRX_CRC_POLY_OFFSET                                               0x21c
#define GALLIFREY_V3A_DIG_TRX_CRC_POLY_ADDR                                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TRX_CRC_POLY_OFFSET)
#define GALLIFREY_V3A_DIG_TRX_CRC_POLY_SZ                                                   32
#define GALLIFREY_V3A_DIG_TRX_CRC_POLY                                                      (volatile uint8_t *)GALLIFREY_V3A_DIG_TRX_CRC_POLY_ADDR
#define GALLIFREY_V3A_DIG_TRX_CRC_RST_OFFSET                                                0x220
#define GALLIFREY_V3A_DIG_TRX_CRC_RST_ADDR                                                  (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TRX_CRC_RST_OFFSET)
#define GALLIFREY_V3A_DIG_TRX_CRC_RST_SZ                                                    32
#define GALLIFREY_V3A_DIG_TRX_CRC_RST                                                       (volatile uint8_t *)GALLIFREY_V3A_DIG_TRX_CRC_RST_ADDR
#define GALLIFREY_V3A_DIG_TRX_TRX_DF_LUT_OFFSET                                             0x224
#define GALLIFREY_V3A_DIG_TRX_TRX_DF_LUT_ADDR                                               (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TRX_TRX_DF_LUT_OFFSET)
#define GALLIFREY_V3A_DIG_TRX_TRX_DF_LUT_SZ                                                 64
#define GALLIFREY_V3A_DIG_TRX_TRX_DF_LUT                                                    (volatile uint8_t *)GALLIFREY_V3A_DIG_TRX_TRX_DF_LUT_ADDR
#define GALLIFREY_V3A_DIG_TRX_TRX_DF_CFG_OFFSET                                             0x22c
#define GALLIFREY_V3A_DIG_TRX_TRX_DF_CFG_ADDR                                               (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TRX_TRX_DF_CFG_OFFSET)
#define GALLIFREY_V3A_DIG_TRX_TRX_DF_CFG_SZ                                                 8
#define GALLIFREY_V3A_DIG_TRX_TRX_DF_CFG                                                    (volatile uint8_t *)GALLIFREY_V3A_DIG_TRX_TRX_DF_CFG_ADDR
#define GALLIFREY_V3A_DIG_TRX_MANCHESTER_OFFSET                                             0x22d
#define GALLIFREY_V3A_DIG_TRX_MANCHESTER_ADDR                                               (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TRX_MANCHESTER_OFFSET)
#define GALLIFREY_V3A_DIG_TRX_MANCHESTER_SZ                                                 8
#define GALLIFREY_V3A_DIG_TRX_MANCHESTER                                                    (volatile uint8_t *)GALLIFREY_V3A_DIG_TRX_MANCHESTER_ADDR
#define GALLIFREY_V3A_DIG_TRX_VITERBI_CONV_OFFSET                                           0x22e
#define GALLIFREY_V3A_DIG_TRX_VITERBI_CONV_ADDR                                             (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TRX_VITERBI_CONV_OFFSET)
#define GALLIFREY_V3A_DIG_TRX_VITERBI_CONV_SZ                                               16
#define GALLIFREY_V3A_DIG_TRX_VITERBI_CONV                                                  (volatile uint8_t *)GALLIFREY_V3A_DIG_TRX_VITERBI_CONV_ADDR
#define GALLIFREY_V3A_DIG_TRX_DATA_WHITENING_OFFSET                                         0x230
#define GALLIFREY_V3A_DIG_TRX_DATA_WHITENING_ADDR                                           (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TRX_DATA_WHITENING_OFFSET)
#define GALLIFREY_V3A_DIG_TRX_DATA_WHITENING_SZ                                             8
#define GALLIFREY_V3A_DIG_TRX_DATA_WHITENING                                                (volatile uint8_t *)GALLIFREY_V3A_DIG_TRX_DATA_WHITENING_ADDR
#define GALLIFREY_V3A_DIG_TRX_DPSK_OFFSET                                                   0x231
#define GALLIFREY_V3A_DIG_TRX_DPSK_ADDR                                                     (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TRX_DPSK_OFFSET)
#define GALLIFREY_V3A_DIG_TRX_DPSK_SZ                                                       8
#define GALLIFREY_V3A_DIG_TRX_DPSK                                                          (volatile uint8_t *)GALLIFREY_V3A_DIG_TRX_DPSK_ADDR
#define GALLIFREY_V3A_ANA_TRX_OFFSET                                                        0x232
#define GALLIFREY_V3A_ANA_TRX_ADDR                                                          (GALLIFREY_V3A_BASE + GALLIFREY_V3A_ANA_TRX_OFFSET)
#define GALLIFREY_V3A_ANA_TRX_SZ                                                            8
#define GALLIFREY_V3A_ANA_TRX                                                               (volatile uint8_t *)GALLIFREY_V3A_ANA_TRX_ADDR
#define GALLIFREY_V3A_ANA_TRX_RF_LNA_OFFSET                                                 0x233
#define GALLIFREY_V3A_ANA_TRX_RF_LNA_ADDR                                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_ANA_TRX_RF_LNA_OFFSET)
#define GALLIFREY_V3A_ANA_TRX_RF_LNA_SZ                                                     8
#define GALLIFREY_V3A_ANA_TRX_RF_LNA                                                        (volatile uint8_t *)GALLIFREY_V3A_ANA_TRX_RF_LNA_ADDR
#define GALLIFREY_V3A_DIG_TRX_SYNC_WORD_LOW_OFFSET                                          0x234
#define GALLIFREY_V3A_DIG_TRX_SYNC_WORD_LOW_ADDR                                            (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TRX_SYNC_WORD_LOW_OFFSET)
#define GALLIFREY_V3A_DIG_TRX_SYNC_WORD_LOW_SZ                                              32
#define GALLIFREY_V3A_DIG_TRX_SYNC_WORD_LOW                                                 (volatile uint8_t *)GALLIFREY_V3A_DIG_TRX_SYNC_WORD_LOW_ADDR
#define GALLIFREY_V3A_DIG_TRX_SYNC_WORD_HIGH_OFFSET                                         0x238
#define GALLIFREY_V3A_DIG_TRX_SYNC_WORD_HIGH_ADDR                                           (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_TRX_SYNC_WORD_HIGH_OFFSET)
#define GALLIFREY_V3A_DIG_TRX_SYNC_WORD_HIGH_SZ                                             32
#define GALLIFREY_V3A_DIG_TRX_SYNC_WORD_HIGH                                                (volatile uint8_t *)GALLIFREY_V3A_DIG_TRX_SYNC_WORD_HIGH_ADDR
#define GALLIFREY_V3A_ANA_TRX_RF_PA_OFFSET                                                  0x23c
#define GALLIFREY_V3A_ANA_TRX_RF_PA_ADDR                                                    (GALLIFREY_V3A_BASE + GALLIFREY_V3A_ANA_TRX_RF_PA_OFFSET)
#define GALLIFREY_V3A_ANA_TRX_RF_PA_SZ                                                      16
#define GALLIFREY_V3A_ANA_TRX_RF_PA                                                         (volatile uint8_t *)GALLIFREY_V3A_ANA_TRX_RF_PA_ADDR
#define GALLIFREY_V3A_ANA_TRX_RF_LNA_DET_CONFIG_OFFSET                                      0x23e
#define GALLIFREY_V3A_ANA_TRX_RF_LNA_DET_CONFIG_ADDR                                        (GALLIFREY_V3A_BASE + GALLIFREY_V3A_ANA_TRX_RF_LNA_DET_CONFIG_OFFSET)
#define GALLIFREY_V3A_ANA_TRX_RF_LNA_DET_CONFIG_SZ                                          8
#define GALLIFREY_V3A_ANA_TRX_RF_LNA_DET_CONFIG                                             (volatile uint8_t *)GALLIFREY_V3A_ANA_TRX_RF_LNA_DET_CONFIG_ADDR
#define GALLIFREY_V3A_ANA_TRX_RF_LNA_CONFIG_OFFSET                                          0x23f
#define GALLIFREY_V3A_ANA_TRX_RF_LNA_CONFIG_ADDR                                            (GALLIFREY_V3A_BASE + GALLIFREY_V3A_ANA_TRX_RF_LNA_CONFIG_OFFSET)
#define GALLIFREY_V3A_ANA_TRX_RF_LNA_CONFIG_SZ                                              8
#define GALLIFREY_V3A_ANA_TRX_RF_LNA_CONFIG                                                 (volatile uint8_t *)GALLIFREY_V3A_ANA_TRX_RF_LNA_CONFIG_ADDR
#define GALLIFREY_V3A_ANA_TRX_RF_MXP_OFFSET                                                 0x240
#define GALLIFREY_V3A_ANA_TRX_RF_MXP_ADDR                                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_ANA_TRX_RF_MXP_OFFSET)
#define GALLIFREY_V3A_ANA_TRX_RF_MXP_SZ                                                     8
#define GALLIFREY_V3A_ANA_TRX_RF_MXP                                                        (volatile uint8_t *)GALLIFREY_V3A_ANA_TRX_RF_MXP_ADDR
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_DYN_OFFSET                                         0x241
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_DYN_ADDR                                           (GALLIFREY_V3A_BASE + GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_DYN_OFFSET)
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_DYN_SZ                                             8
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_DYN                                                (volatile uint8_t *)GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_DYN_ADDR
#define GALLIFREY_V3A_ANA_TRX_RF_RFSENSE_OFFSET                                             0x242
#define GALLIFREY_V3A_ANA_TRX_RF_RFSENSE_ADDR                                               (GALLIFREY_V3A_BASE + GALLIFREY_V3A_ANA_TRX_RF_RFSENSE_OFFSET)
#define GALLIFREY_V3A_ANA_TRX_RF_RFSENSE_SZ                                                 16
#define GALLIFREY_V3A_ANA_TRX_RF_RFSENSE                                                    (volatile uint8_t *)GALLIFREY_V3A_ANA_TRX_RF_RFSENSE_ADDR
#define GALLIFREY_V3A_ANA_TRX_RF_DD2_OFFSET                                                 0x244
#define GALLIFREY_V3A_ANA_TRX_RF_DD2_ADDR                                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_ANA_TRX_RF_DD2_OFFSET)
#define GALLIFREY_V3A_ANA_TRX_RF_DD2_SZ                                                     16
#define GALLIFREY_V3A_ANA_TRX_RF_DD2                                                        (volatile uint8_t *)GALLIFREY_V3A_ANA_TRX_RF_DD2_ADDR
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_OFFSET                                             0x246
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_ADDR                                               (GALLIFREY_V3A_BASE + GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_OFFSET)
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_SZ                                                 24
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT                                                    (volatile uint8_t *)GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_ADDR
#define GALLIFREY_V3A_PMU_VREF_PREPA_OFFSET                                                 0x249
#define GALLIFREY_V3A_PMU_VREF_PREPA_ADDR                                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_PMU_VREF_PREPA_OFFSET)
#define GALLIFREY_V3A_PMU_VREF_PREPA_SZ                                                     8
#define GALLIFREY_V3A_PMU_VREF_PREPA                                                        (volatile uint8_t *)GALLIFREY_V3A_PMU_VREF_PREPA_ADDR
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_IBIAS_OFFSET                                       0x24a
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_IBIAS_ADDR                                         (GALLIFREY_V3A_BASE + GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_IBIAS_OFFSET)
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_IBIAS_SZ                                           16
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_IBIAS                                              (volatile uint8_t *)GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_IBIAS_ADDR
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_DC_CAL_OFFSET                                      0x24c
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_DC_CAL_ADDR                                        (GALLIFREY_V3A_BASE + GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_DC_CAL_OFFSET)
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_DC_CAL_SZ                                          32
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_DC_CAL                                             (volatile uint8_t *)GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_DC_CAL_ADDR
#define GALLIFREY_V3A_ANA_TRX_BB_ADC_CONF_OFFSET                                            0x250
#define GALLIFREY_V3A_ANA_TRX_BB_ADC_CONF_ADDR                                              (GALLIFREY_V3A_BASE + GALLIFREY_V3A_ANA_TRX_BB_ADC_CONF_OFFSET)
#define GALLIFREY_V3A_ANA_TRX_BB_ADC_CONF_SZ                                                16
#define GALLIFREY_V3A_ANA_TRX_BB_ADC_CONF                                                   (volatile uint8_t *)GALLIFREY_V3A_ANA_TRX_BB_ADC_CONF_ADDR
#define GALLIFREY_V3A_ANA_TRX_BB_ADC_ENA_OFFSET                                             0x252
#define GALLIFREY_V3A_ANA_TRX_BB_ADC_ENA_ADDR                                               (GALLIFREY_V3A_BASE + GALLIFREY_V3A_ANA_TRX_BB_ADC_ENA_OFFSET)
#define GALLIFREY_V3A_ANA_TRX_BB_ADC_ENA_SZ                                                 16
#define GALLIFREY_V3A_ANA_TRX_BB_ADC_ENA                                                    (volatile uint8_t *)GALLIFREY_V3A_ANA_TRX_BB_ADC_ENA_ADDR
#define GALLIFREY_V3A_PMU_OFFSET                                                            0x254
#define GALLIFREY_V3A_PMU_ADDR                                                              (GALLIFREY_V3A_BASE + GALLIFREY_V3A_PMU_OFFSET)
#define GALLIFREY_V3A_PMU_SZ                                                                16
#define GALLIFREY_V3A_PMU                                                                   (volatile uint8_t *)GALLIFREY_V3A_PMU_ADDR
#define GALLIFREY_V3A_PMU_VREF_PA_OFFSET                                                    0x256
#define GALLIFREY_V3A_PMU_VREF_PA_ADDR                                                      (GALLIFREY_V3A_BASE + GALLIFREY_V3A_PMU_VREF_PA_OFFSET)
#define GALLIFREY_V3A_PMU_VREF_PA_SZ                                                        8
#define GALLIFREY_V3A_PMU_VREF_PA                                                           (volatile uint8_t *)GALLIFREY_V3A_PMU_VREF_PA_ADDR
#define GALLIFREY_V3A_PMU_VREF_LNA_BQF_OFFSET                                               0x257
#define GALLIFREY_V3A_PMU_VREF_LNA_BQF_ADDR                                                 (GALLIFREY_V3A_BASE + GALLIFREY_V3A_PMU_VREF_LNA_BQF_OFFSET)
#define GALLIFREY_V3A_PMU_VREF_LNA_BQF_SZ                                                   8
#define GALLIFREY_V3A_PMU_VREF_LNA_BQF                                                      (volatile uint8_t *)GALLIFREY_V3A_PMU_VREF_LNA_BQF_ADDR
#define GALLIFREY_V3A_PMU_VREF_ADC_OFFSET                                                   0x258
#define GALLIFREY_V3A_PMU_VREF_ADC_ADDR                                                     (GALLIFREY_V3A_BASE + GALLIFREY_V3A_PMU_VREF_ADC_OFFSET)
#define GALLIFREY_V3A_PMU_VREF_ADC_SZ                                                       8
#define GALLIFREY_V3A_PMU_VREF_ADC                                                          (volatile uint8_t *)GALLIFREY_V3A_PMU_VREF_ADC_ADDR
#define GALLIFREY_V3A_PMU_VREF_DIG_OFFSET                                                   0x259
#define GALLIFREY_V3A_PMU_VREF_DIG_ADDR                                                     (GALLIFREY_V3A_BASE + GALLIFREY_V3A_PMU_VREF_DIG_OFFSET)
#define GALLIFREY_V3A_PMU_VREF_DIG_SZ                                                       8
#define GALLIFREY_V3A_PMU_VREF_DIG                                                          (volatile uint8_t *)GALLIFREY_V3A_PMU_VREF_DIG_ADDR
#define GALLIFREY_V3A_PMU_VREF_MISC_OFFSET                                                  0x25a
#define GALLIFREY_V3A_PMU_VREF_MISC_ADDR                                                    (GALLIFREY_V3A_BASE + GALLIFREY_V3A_PMU_VREF_MISC_OFFSET)
#define GALLIFREY_V3A_PMU_VREF_MISC_SZ                                                      8
#define GALLIFREY_V3A_PMU_VREF_MISC                                                         (volatile uint8_t *)GALLIFREY_V3A_PMU_VREF_MISC_ADDR
#define GALLIFREY_V3A_ANA_MISC_OFFSET                                                       0x25b
#define GALLIFREY_V3A_ANA_MISC_ADDR                                                         (GALLIFREY_V3A_BASE + GALLIFREY_V3A_ANA_MISC_OFFSET)
#define GALLIFREY_V3A_ANA_MISC_SZ                                                           8
#define GALLIFREY_V3A_ANA_MISC                                                              (volatile uint8_t *)GALLIFREY_V3A_ANA_MISC_ADDR
#define GALLIFREY_V3A_PMU_LDO_PREPA_OFFSET                                                  0x25c
#define GALLIFREY_V3A_PMU_LDO_PREPA_ADDR                                                    (GALLIFREY_V3A_BASE + GALLIFREY_V3A_PMU_LDO_PREPA_OFFSET)
#define GALLIFREY_V3A_PMU_LDO_PREPA_SZ                                                      16
#define GALLIFREY_V3A_PMU_LDO_PREPA                                                         (volatile uint8_t *)GALLIFREY_V3A_PMU_LDO_PREPA_ADDR
#define GALLIFREY_V3A_PMU_LDO_PALOW_OFFSET                                                  0x25e
#define GALLIFREY_V3A_PMU_LDO_PALOW_ADDR                                                    (GALLIFREY_V3A_BASE + GALLIFREY_V3A_PMU_LDO_PALOW_OFFSET)
#define GALLIFREY_V3A_PMU_LDO_PALOW_SZ                                                      16
#define GALLIFREY_V3A_PMU_LDO_PALOW                                                         (volatile uint8_t *)GALLIFREY_V3A_PMU_LDO_PALOW_ADDR
#define GALLIFREY_V3A_PMU_LDO_PAHIGH_OFFSET                                                 0x260
#define GALLIFREY_V3A_PMU_LDO_PAHIGH_ADDR                                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_PMU_LDO_PAHIGH_OFFSET)
#define GALLIFREY_V3A_PMU_LDO_PAHIGH_SZ                                                     16
#define GALLIFREY_V3A_PMU_LDO_PAHIGH                                                        (volatile uint8_t *)GALLIFREY_V3A_PMU_LDO_PAHIGH_ADDR
#define GALLIFREY_V3A_PMU_LDO_LNA_OFFSET                                                    0x262
#define GALLIFREY_V3A_PMU_LDO_LNA_ADDR                                                      (GALLIFREY_V3A_BASE + GALLIFREY_V3A_PMU_LDO_LNA_OFFSET)
#define GALLIFREY_V3A_PMU_LDO_LNA_SZ                                                        16
#define GALLIFREY_V3A_PMU_LDO_LNA                                                           (volatile uint8_t *)GALLIFREY_V3A_PMU_LDO_LNA_ADDR
#define GALLIFREY_V3A_PMU_LDO_BQF_OFFSET                                                    0x264
#define GALLIFREY_V3A_PMU_LDO_BQF_ADDR                                                      (GALLIFREY_V3A_BASE + GALLIFREY_V3A_PMU_LDO_BQF_OFFSET)
#define GALLIFREY_V3A_PMU_LDO_BQF_SZ                                                        16
#define GALLIFREY_V3A_PMU_LDO_BQF                                                           (volatile uint8_t *)GALLIFREY_V3A_PMU_LDO_BQF_ADDR
#define GALLIFREY_V3A_PMU_LDO_ADCBUF_OFFSET                                                 0x266
#define GALLIFREY_V3A_PMU_LDO_ADCBUF_ADDR                                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_PMU_LDO_ADCBUF_OFFSET)
#define GALLIFREY_V3A_PMU_LDO_ADCBUF_SZ                                                     16
#define GALLIFREY_V3A_PMU_LDO_ADCBUF                                                        (volatile uint8_t *)GALLIFREY_V3A_PMU_LDO_ADCBUF_ADDR
#define GALLIFREY_V3A_PMU_LDO_ADC_OFFSET                                                    0x268
#define GALLIFREY_V3A_PMU_LDO_ADC_ADDR                                                      (GALLIFREY_V3A_BASE + GALLIFREY_V3A_PMU_LDO_ADC_OFFSET)
#define GALLIFREY_V3A_PMU_LDO_ADC_SZ                                                        16
#define GALLIFREY_V3A_PMU_LDO_ADC                                                           (volatile uint8_t *)GALLIFREY_V3A_PMU_LDO_ADC_ADDR
#define GALLIFREY_V3A_PMU_LDO_DIG_OFFSET                                                    0x26a
#define GALLIFREY_V3A_PMU_LDO_DIG_ADDR                                                      (GALLIFREY_V3A_BASE + GALLIFREY_V3A_PMU_LDO_DIG_OFFSET)
#define GALLIFREY_V3A_PMU_LDO_DIG_SZ                                                        16
#define GALLIFREY_V3A_PMU_LDO_DIG                                                           (volatile uint8_t *)GALLIFREY_V3A_PMU_LDO_DIG_ADDR
#define GALLIFREY_V3A_PMU_LDO_MISC_OFFSET                                                   0x26c
#define GALLIFREY_V3A_PMU_LDO_MISC_ADDR                                                     (GALLIFREY_V3A_BASE + GALLIFREY_V3A_PMU_LDO_MISC_OFFSET)
#define GALLIFREY_V3A_PMU_LDO_MISC_SZ                                                       16
#define GALLIFREY_V3A_PMU_LDO_MISC                                                          (volatile uint8_t *)GALLIFREY_V3A_PMU_LDO_MISC_ADDR
#define GALLIFREY_V3A_ANA_MISC_TEMP_SENS_OFFSET                                             0x26e
#define GALLIFREY_V3A_ANA_MISC_TEMP_SENS_ADDR                                               (GALLIFREY_V3A_BASE + GALLIFREY_V3A_ANA_MISC_TEMP_SENS_OFFSET)
#define GALLIFREY_V3A_ANA_MISC_TEMP_SENS_SZ                                                 8
#define GALLIFREY_V3A_ANA_MISC_TEMP_SENS                                                    (volatile uint8_t *)GALLIFREY_V3A_ANA_MISC_TEMP_SENS_ADDR
#define GALLIFREY_V3A_XTAL_FREQ_TRIM_OFFSET                                                 0x26f
#define GALLIFREY_V3A_XTAL_FREQ_TRIM_ADDR                                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_XTAL_FREQ_TRIM_OFFSET)
#define GALLIFREY_V3A_XTAL_FREQ_TRIM_SZ                                                     8
#define GALLIFREY_V3A_XTAL_FREQ_TRIM                                                        (volatile uint8_t *)GALLIFREY_V3A_XTAL_FREQ_TRIM_ADDR
#define GALLIFREY_V3A_ATB_ADPLL_OFFSET                                                      0x270
#define GALLIFREY_V3A_ATB_ADPLL_ADDR                                                        (GALLIFREY_V3A_BASE + GALLIFREY_V3A_ATB_ADPLL_OFFSET)
#define GALLIFREY_V3A_ATB_ADPLL_SZ                                                          16
#define GALLIFREY_V3A_ATB_ADPLL                                                             (volatile uint8_t *)GALLIFREY_V3A_ATB_ADPLL_ADDR
#define GALLIFREY_V3A_ATB_REFE_OFFSET                                                       0x272
#define GALLIFREY_V3A_ATB_REFE_ADDR                                                         (GALLIFREY_V3A_BASE + GALLIFREY_V3A_ATB_REFE_OFFSET)
#define GALLIFREY_V3A_ATB_REFE_SZ                                                           16
#define GALLIFREY_V3A_ATB_REFE                                                              (volatile uint8_t *)GALLIFREY_V3A_ATB_REFE_ADDR
#define GALLIFREY_V3A_ATB_RXTX_OFFSET                                                       0x274
#define GALLIFREY_V3A_ATB_RXTX_ADDR                                                         (GALLIFREY_V3A_BASE + GALLIFREY_V3A_ATB_RXTX_OFFSET)
#define GALLIFREY_V3A_ATB_RXTX_SZ                                                           24
#define GALLIFREY_V3A_ATB_RXTX                                                              (volatile uint8_t *)GALLIFREY_V3A_ATB_RXTX_ADDR
#define GALLIFREY_V3A_CHANNEL_OFFSET                                                        0x277
#define GALLIFREY_V3A_CHANNEL_ADDR                                                          (GALLIFREY_V3A_BASE + GALLIFREY_V3A_CHANNEL_OFFSET)
#define GALLIFREY_V3A_CHANNEL_SZ                                                            8
#define GALLIFREY_V3A_CHANNEL                                                               (volatile uint8_t *)GALLIFREY_V3A_CHANNEL_ADDR
#define GALLIFREY_V3A_ATB_ADC_OFFSET                                                        0x278
#define GALLIFREY_V3A_ATB_ADC_ADDR                                                          (GALLIFREY_V3A_BASE + GALLIFREY_V3A_ATB_ADC_OFFSET)
#define GALLIFREY_V3A_ATB_ADC_SZ                                                            16
#define GALLIFREY_V3A_ATB_ADC                                                               (volatile uint8_t *)GALLIFREY_V3A_ATB_ADC_ADDR
#define GALLIFREY_V3A_COMMANDS_OFFSET                                                       0x27a
#define GALLIFREY_V3A_COMMANDS_ADDR                                                         (GALLIFREY_V3A_BASE + GALLIFREY_V3A_COMMANDS_OFFSET)
#define GALLIFREY_V3A_COMMANDS_SZ                                                           8
#define GALLIFREY_V3A_COMMANDS                                                              (volatile uint8_t *)GALLIFREY_V3A_COMMANDS_ADDR
#define GALLIFREY_V3A_PROT_TIMER_OFFSET                                                     0x27b
#define GALLIFREY_V3A_PROT_TIMER_ADDR                                                       (GALLIFREY_V3A_BASE + GALLIFREY_V3A_PROT_TIMER_OFFSET)
#define GALLIFREY_V3A_PROT_TIMER_SZ                                                         8
#define GALLIFREY_V3A_PROT_TIMER                                                            (volatile uint8_t *)GALLIFREY_V3A_PROT_TIMER_ADDR
#define GALLIFREY_V3A_XTAL_OFFSET                                                           0x27c
#define GALLIFREY_V3A_XTAL_ADDR                                                             (GALLIFREY_V3A_BASE + GALLIFREY_V3A_XTAL_OFFSET)
#define GALLIFREY_V3A_XTAL_SZ                                                               32
#define GALLIFREY_V3A_XTAL                                                                  (volatile uint8_t *)GALLIFREY_V3A_XTAL_ADDR
#define GALLIFREY_V3A_STATUS_OFFSET                                                         0x280
#define GALLIFREY_V3A_STATUS_ADDR                                                           (GALLIFREY_V3A_BASE + GALLIFREY_V3A_STATUS_OFFSET)
#define GALLIFREY_V3A_STATUS_SZ                                                             8
#define GALLIFREY_V3A_STATUS                                                                (volatile uint8_t *)GALLIFREY_V3A_STATUS_ADDR
#define GALLIFREY_V3A_RXPH_STATUS_OFFSET                                                    0x281
#define GALLIFREY_V3A_RXPH_STATUS_ADDR                                                      (GALLIFREY_V3A_BASE + GALLIFREY_V3A_RXPH_STATUS_OFFSET)
#define GALLIFREY_V3A_RXPH_STATUS_SZ                                                        8
#define GALLIFREY_V3A_RXPH_STATUS                                                           (volatile uint8_t *)GALLIFREY_V3A_RXPH_STATUS_ADDR
#define GALLIFREY_V3A_OBSERVE_POINT_0_OFFSET                                                0x284
#define GALLIFREY_V3A_OBSERVE_POINT_0_ADDR                                                  (GALLIFREY_V3A_BASE + GALLIFREY_V3A_OBSERVE_POINT_0_OFFSET)
#define GALLIFREY_V3A_OBSERVE_POINT_0_SZ                                                    32
#define GALLIFREY_V3A_OBSERVE_POINT_0                                                       (volatile uint8_t *)GALLIFREY_V3A_OBSERVE_POINT_0_ADDR
#define GALLIFREY_V3A_OBSERVE_POINT_1_OFFSET                                                0x288
#define GALLIFREY_V3A_OBSERVE_POINT_1_ADDR                                                  (GALLIFREY_V3A_BASE + GALLIFREY_V3A_OBSERVE_POINT_1_OFFSET)
#define GALLIFREY_V3A_OBSERVE_POINT_1_SZ                                                    32
#define GALLIFREY_V3A_OBSERVE_POINT_1                                                       (volatile uint8_t *)GALLIFREY_V3A_OBSERVE_POINT_1_ADDR
#define GALLIFREY_V3A_RX_DATA_OUT_OFFSET                                                    0x28c
#define GALLIFREY_V3A_RX_DATA_OUT_ADDR                                                      (GALLIFREY_V3A_BASE + GALLIFREY_V3A_RX_DATA_OUT_OFFSET)
#define GALLIFREY_V3A_RX_DATA_OUT_SZ                                                        32
#define GALLIFREY_V3A_RX_DATA_OUT                                                           (volatile uint8_t *)GALLIFREY_V3A_RX_DATA_OUT_ADDR
#define GALLIFREY_V3A_TX_FIFO_OFFSET                                                        0x290
#define GALLIFREY_V3A_TX_FIFO_ADDR                                                          (GALLIFREY_V3A_BASE + GALLIFREY_V3A_TX_FIFO_OFFSET)
#define GALLIFREY_V3A_TX_FIFO_SZ                                                            32
#define GALLIFREY_V3A_TX_FIFO                                                               (volatile uint8_t *)GALLIFREY_V3A_TX_FIFO_ADDR
#define GALLIFREY_V3A_TX_FIFO_32_OFFSET                                                     0x294
#define GALLIFREY_V3A_TX_FIFO_32_ADDR                                                       (GALLIFREY_V3A_BASE + GALLIFREY_V3A_TX_FIFO_32_OFFSET)
#define GALLIFREY_V3A_TX_FIFO_32_SZ                                                         32
#define GALLIFREY_V3A_TX_FIFO_32                                                            (volatile uint8_t *)GALLIFREY_V3A_TX_FIFO_32_ADDR
#define GALLIFREY_V3A_TX_FIFO_COUNT_OFFSET                                                  0x298
#define GALLIFREY_V3A_TX_FIFO_COUNT_ADDR                                                    (GALLIFREY_V3A_BASE + GALLIFREY_V3A_TX_FIFO_COUNT_OFFSET)
#define GALLIFREY_V3A_TX_FIFO_COUNT_SZ                                                      8
#define GALLIFREY_V3A_TX_FIFO_COUNT                                                         (volatile uint8_t *)GALLIFREY_V3A_TX_FIFO_COUNT_ADDR
#define GALLIFREY_V3A_TX_FIFO_STATUS_OFFSET                                                 0x29c
#define GALLIFREY_V3A_TX_FIFO_STATUS_ADDR                                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_TX_FIFO_STATUS_OFFSET)
#define GALLIFREY_V3A_TX_FIFO_STATUS_SZ                                                     32
#define GALLIFREY_V3A_TX_FIFO_STATUS                                                        (volatile uint8_t *)GALLIFREY_V3A_TX_FIFO_STATUS_ADDR
#define GALLIFREY_V3A_RX_FIFO_OFFSET                                                        0x2a0
#define GALLIFREY_V3A_RX_FIFO_ADDR                                                          (GALLIFREY_V3A_BASE + GALLIFREY_V3A_RX_FIFO_OFFSET)
#define GALLIFREY_V3A_RX_FIFO_SZ                                                            32
#define GALLIFREY_V3A_RX_FIFO                                                               (volatile uint8_t *)GALLIFREY_V3A_RX_FIFO_ADDR
#define GALLIFREY_V3A_RX_FIFO_32_OFFSET                                                     0x2a4
#define GALLIFREY_V3A_RX_FIFO_32_ADDR                                                       (GALLIFREY_V3A_BASE + GALLIFREY_V3A_RX_FIFO_32_OFFSET)
#define GALLIFREY_V3A_RX_FIFO_32_SZ                                                         32
#define GALLIFREY_V3A_RX_FIFO_32                                                            (volatile uint8_t *)GALLIFREY_V3A_RX_FIFO_32_ADDR
#define GALLIFREY_V3A_RX_FIFO_COUNT_OFFSET                                                  0x2a8
#define GALLIFREY_V3A_RX_FIFO_COUNT_ADDR                                                    (GALLIFREY_V3A_BASE + GALLIFREY_V3A_RX_FIFO_COUNT_OFFSET)
#define GALLIFREY_V3A_RX_FIFO_COUNT_SZ                                                      8
#define GALLIFREY_V3A_RX_FIFO_COUNT                                                         (volatile uint8_t *)GALLIFREY_V3A_RX_FIFO_COUNT_ADDR
#define GALLIFREY_V3A_RX_FIFO_STATUS_OFFSET                                                 0x2ac
#define GALLIFREY_V3A_RX_FIFO_STATUS_ADDR                                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_RX_FIFO_STATUS_OFFSET)
#define GALLIFREY_V3A_RX_FIFO_STATUS_SZ                                                     32
#define GALLIFREY_V3A_RX_FIFO_STATUS                                                        (volatile uint8_t *)GALLIFREY_V3A_RX_FIFO_STATUS_ADDR
#define GALLIFREY_V3A_IQ_FIFO_OFFSET                                                        0x2b0
#define GALLIFREY_V3A_IQ_FIFO_ADDR                                                          (GALLIFREY_V3A_BASE + GALLIFREY_V3A_IQ_FIFO_OFFSET)
#define GALLIFREY_V3A_IQ_FIFO_SZ                                                            32
#define GALLIFREY_V3A_IQ_FIFO                                                               (volatile uint8_t *)GALLIFREY_V3A_IQ_FIFO_ADDR
#define GALLIFREY_V3A_IQ_FIFO_32_OFFSET                                                     0x2b4
#define GALLIFREY_V3A_IQ_FIFO_32_ADDR                                                       (GALLIFREY_V3A_BASE + GALLIFREY_V3A_IQ_FIFO_32_OFFSET)
#define GALLIFREY_V3A_IQ_FIFO_32_SZ                                                         32
#define GALLIFREY_V3A_IQ_FIFO_32                                                            (volatile uint8_t *)GALLIFREY_V3A_IQ_FIFO_32_ADDR
#define GALLIFREY_V3A_IQ_FIFO_COUNT_OFFSET                                                  0x2b8
#define GALLIFREY_V3A_IQ_FIFO_COUNT_ADDR                                                    (GALLIFREY_V3A_BASE + GALLIFREY_V3A_IQ_FIFO_COUNT_OFFSET)
#define GALLIFREY_V3A_IQ_FIFO_COUNT_SZ                                                      8
#define GALLIFREY_V3A_IQ_FIFO_COUNT                                                         (volatile uint8_t *)GALLIFREY_V3A_IQ_FIFO_COUNT_ADDR
#define GALLIFREY_V3A_IQ_FIFO_STATUS_OFFSET                                                 0x2bc
#define GALLIFREY_V3A_IQ_FIFO_STATUS_ADDR                                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_IQ_FIFO_STATUS_OFFSET)
#define GALLIFREY_V3A_IQ_FIFO_STATUS_SZ                                                     32
#define GALLIFREY_V3A_IQ_FIFO_STATUS                                                        (volatile uint8_t *)GALLIFREY_V3A_IQ_FIFO_STATUS_ADDR
#define GALLIFREY_V3A_DCO_CAL_READOUT_OFFSET                                                0x2c0
#define GALLIFREY_V3A_DCO_CAL_READOUT_ADDR                                                  (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DCO_CAL_READOUT_OFFSET)
#define GALLIFREY_V3A_DCO_CAL_READOUT_SZ                                                    8
#define GALLIFREY_V3A_DCO_CAL_READOUT                                                       (volatile uint8_t *)GALLIFREY_V3A_DCO_CAL_READOUT_ADDR
#define GALLIFREY_V3A_ANALOG_INFO_OFFSET                                                    0x2c1
#define GALLIFREY_V3A_ANALOG_INFO_ADDR                                                      (GALLIFREY_V3A_BASE + GALLIFREY_V3A_ANALOG_INFO_OFFSET)
#define GALLIFREY_V3A_ANALOG_INFO_SZ                                                        8
#define GALLIFREY_V3A_ANALOG_INFO                                                           (volatile uint8_t *)GALLIFREY_V3A_ANALOG_INFO_ADDR
#define GALLIFREY_V3A_RC_CALIBRATION_OFFSET                                                 0x2c2
#define GALLIFREY_V3A_RC_CALIBRATION_ADDR                                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_RC_CALIBRATION_OFFSET)
#define GALLIFREY_V3A_RC_CALIBRATION_SZ                                                     8
#define GALLIFREY_V3A_RC_CALIBRATION                                                        (volatile uint8_t *)GALLIFREY_V3A_RC_CALIBRATION_ADDR
#define GALLIFREY_V3A_RSSI_PKT_OFFSET                                                       0x2c3
#define GALLIFREY_V3A_RSSI_PKT_ADDR                                                         (GALLIFREY_V3A_BASE + GALLIFREY_V3A_RSSI_PKT_OFFSET)
#define GALLIFREY_V3A_RSSI_PKT_SZ                                                           8
#define GALLIFREY_V3A_RSSI_PKT                                                              (volatile uint8_t *)GALLIFREY_V3A_RSSI_PKT_ADDR
#define GALLIFREY_V3A_IRQ_STATUS_OFFSET                                                     0x2c4
#define GALLIFREY_V3A_IRQ_STATUS_ADDR                                                       (GALLIFREY_V3A_BASE + GALLIFREY_V3A_IRQ_STATUS_OFFSET)
#define GALLIFREY_V3A_IRQ_STATUS_SZ                                                         32
#define GALLIFREY_V3A_IRQ_STATUS                                                            (volatile uint8_t *)GALLIFREY_V3A_IRQ_STATUS_ADDR
#define GALLIFREY_V3A_RSSI_LIN_OFFSET                                                       0x2c8
#define GALLIFREY_V3A_RSSI_LIN_ADDR                                                         (GALLIFREY_V3A_BASE + GALLIFREY_V3A_RSSI_LIN_OFFSET)
#define GALLIFREY_V3A_RSSI_LIN_SZ                                                           16
#define GALLIFREY_V3A_RSSI_LIN                                                              (volatile uint8_t *)GALLIFREY_V3A_RSSI_LIN_ADDR
#define GALLIFREY_V3A_RSSI_OFFSET                                                           0x2ca
#define GALLIFREY_V3A_RSSI_ADDR                                                             (GALLIFREY_V3A_BASE + GALLIFREY_V3A_RSSI_OFFSET)
#define GALLIFREY_V3A_RSSI_SZ                                                               24
#define GALLIFREY_V3A_RSSI                                                                  (volatile uint8_t *)GALLIFREY_V3A_RSSI_ADDR
#define GALLIFREY_V3A_AGC_ATTENUATION_OFFSET                                                0x2cd
#define GALLIFREY_V3A_AGC_ATTENUATION_ADDR                                                  (GALLIFREY_V3A_BASE + GALLIFREY_V3A_AGC_ATTENUATION_OFFSET)
#define GALLIFREY_V3A_AGC_ATTENUATION_SZ                                                    8
#define GALLIFREY_V3A_AGC_ATTENUATION                                                       (volatile uint8_t *)GALLIFREY_V3A_AGC_ATTENUATION_ADDR
#define GALLIFREY_V3A_AGC_STATUS_OFFSET                                                     0x2ce
#define GALLIFREY_V3A_AGC_STATUS_ADDR                                                       (GALLIFREY_V3A_BASE + GALLIFREY_V3A_AGC_STATUS_OFFSET)
#define GALLIFREY_V3A_AGC_STATUS_SZ                                                         8
#define GALLIFREY_V3A_AGC_STATUS                                                            (volatile uint8_t *)GALLIFREY_V3A_AGC_STATUS_ADDR
#define GALLIFREY_V3A_AES_STATUS_OFFSET                                                     0x2cf
#define GALLIFREY_V3A_AES_STATUS_ADDR                                                       (GALLIFREY_V3A_BASE + GALLIFREY_V3A_AES_STATUS_OFFSET)
#define GALLIFREY_V3A_AES_STATUS_SZ                                                         8
#define GALLIFREY_V3A_AES_STATUS                                                            (volatile uint8_t *)GALLIFREY_V3A_AES_STATUS_ADDR
#define GALLIFREY_V3A_CFO_OFFSET                                                            0x2d0
#define GALLIFREY_V3A_CFO_ADDR                                                              (GALLIFREY_V3A_BASE + GALLIFREY_V3A_CFO_OFFSET)
#define GALLIFREY_V3A_CFO_SZ                                                                16
#define GALLIFREY_V3A_CFO                                                                   (volatile uint8_t *)GALLIFREY_V3A_CFO_ADDR
#define GALLIFREY_V3A_IQ_BALANCE_READOUT_OFFSET                                             0x2d2
#define GALLIFREY_V3A_IQ_BALANCE_READOUT_ADDR                                               (GALLIFREY_V3A_BASE + GALLIFREY_V3A_IQ_BALANCE_READOUT_OFFSET)
#define GALLIFREY_V3A_IQ_BALANCE_READOUT_SZ                                                 16
#define GALLIFREY_V3A_IQ_BALANCE_READOUT                                                    (volatile uint8_t *)GALLIFREY_V3A_IQ_BALANCE_READOUT_ADDR
#define GALLIFREY_V3A_REFE_LDO_MISC_STATUS_OFFSET                                           0x2d4
#define GALLIFREY_V3A_REFE_LDO_MISC_STATUS_ADDR                                             (GALLIFREY_V3A_BASE + GALLIFREY_V3A_REFE_LDO_MISC_STATUS_OFFSET)
#define GALLIFREY_V3A_REFE_LDO_MISC_STATUS_SZ                                               16
#define GALLIFREY_V3A_REFE_LDO_MISC_STATUS                                                  (volatile uint8_t *)GALLIFREY_V3A_REFE_LDO_MISC_STATUS_ADDR
#define GALLIFREY_V3A_TX_POWER_OFFSET                                                       0x2d6
#define GALLIFREY_V3A_TX_POWER_ADDR                                                         (GALLIFREY_V3A_BASE + GALLIFREY_V3A_TX_POWER_OFFSET)
#define GALLIFREY_V3A_TX_POWER_SZ                                                           8
#define GALLIFREY_V3A_TX_POWER                                                              (volatile uint8_t *)GALLIFREY_V3A_TX_POWER_ADDR
#define GALLIFREY_V3A_XO_FREQ_TRIM_STATUS_OFFSET                                            0x2d7
#define GALLIFREY_V3A_XO_FREQ_TRIM_STATUS_ADDR                                              (GALLIFREY_V3A_BASE + GALLIFREY_V3A_XO_FREQ_TRIM_STATUS_OFFSET)
#define GALLIFREY_V3A_XO_FREQ_TRIM_STATUS_SZ                                                8
#define GALLIFREY_V3A_XO_FREQ_TRIM_STATUS                                                   (volatile uint8_t *)GALLIFREY_V3A_XO_FREQ_TRIM_STATUS_ADDR
#define GALLIFREY_V3A_XO_REG_STATUS_OFFSET                                                  0x2d8
#define GALLIFREY_V3A_XO_REG_STATUS_ADDR                                                    (GALLIFREY_V3A_BASE + GALLIFREY_V3A_XO_REG_STATUS_OFFSET)
#define GALLIFREY_V3A_XO_REG_STATUS_SZ                                                      32
#define GALLIFREY_V3A_XO_REG_STATUS                                                         (volatile uint8_t *)GALLIFREY_V3A_XO_REG_STATUS_ADDR
#define GALLIFREY_V3A_ICYTRXDM_FEATURES_OFFSET                                              0x2dc
#define GALLIFREY_V3A_ICYTRXDM_FEATURES_ADDR                                                (GALLIFREY_V3A_BASE + GALLIFREY_V3A_ICYTRXDM_FEATURES_OFFSET)
#define GALLIFREY_V3A_ICYTRXDM_FEATURES_SZ                                                  8
#define GALLIFREY_V3A_ICYTRXDM_FEATURES                                                     (volatile uint8_t *)GALLIFREY_V3A_ICYTRXDM_FEATURES_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_0_OFFSET                                     0x115
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_0_ADDR                                       (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_0_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_0_SZ                                         24
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_0                                            (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_0_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_1_OFFSET                                     0x115
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_1_ADDR                                       (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_1_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_1_SZ                                         24
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_1                                            (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_1_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_2_OFFSET                                     0x115
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_2_ADDR                                       (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_2_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_2_SZ                                         24
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_2                                            (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_2_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_3_OFFSET                                     0x115
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_3_ADDR                                       (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_3_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_3_SZ                                         24
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_3                                            (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_3_ADDR
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_4_OFFSET                                     0x115
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_4_ADDR                                       (GALLIFREY_V3A_BASE + GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_4_OFFSET)
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_4_SZ                                         24
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_4                                            (volatile uint8_t *)GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_4_ADDR
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_CONST_OFFSET                                       0x24b
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_CONST_ADDR                                         (GALLIFREY_V3A_BASE + GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_CONST_OFFSET)
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_CONST_SZ                                           8
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_CONST                                              (volatile uint8_t *)GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_CONST_ADDR
#define GALLIFREY_V3A_CHIP_ID_OFFSET                                                        0x3ff
#define GALLIFREY_V3A_CHIP_ID_ADDR                                                          (GALLIFREY_V3A_BASE + GALLIFREY_V3A_CHIP_ID_OFFSET)
#define GALLIFREY_V3A_CHIP_ID_SZ                                                            8
#define GALLIFREY_V3A_CHIP_ID                                                               (volatile uint8_t *)GALLIFREY_V3A_CHIP_ID_ADDR
#define GALLIFREY_V3A_RATE_TX_OFFSET                                                        0x800
#define GALLIFREY_V3A_RATE_TX_ADDR                                                          (GALLIFREY_V3A_BASE + GALLIFREY_V3A_RATE_TX_OFFSET)
#define GALLIFREY_V3A_RATE_TX_SZ                                                            8
#define GALLIFREY_V3A_RATE_TX                                                               (volatile uint8_t *)GALLIFREY_V3A_RATE_TX_ADDR
#define GALLIFREY_V3A_CHANNEL_TX_OFFSET                                                     0x801
#define GALLIFREY_V3A_CHANNEL_TX_ADDR                                                       (GALLIFREY_V3A_BASE + GALLIFREY_V3A_CHANNEL_TX_OFFSET)
#define GALLIFREY_V3A_CHANNEL_TX_SZ                                                         8
#define GALLIFREY_V3A_CHANNEL_TX                                                            (volatile uint8_t *)GALLIFREY_V3A_CHANNEL_TX_ADDR
#define GALLIFREY_V3A_TX_PA_POWER_OFFSET                                                    0x802
#define GALLIFREY_V3A_TX_PA_POWER_ADDR                                                      (GALLIFREY_V3A_BASE + GALLIFREY_V3A_TX_PA_POWER_OFFSET)
#define GALLIFREY_V3A_TX_PA_POWER_SZ                                                        8
#define GALLIFREY_V3A_TX_PA_POWER                                                           (volatile uint8_t *)GALLIFREY_V3A_TX_PA_POWER_ADDR
#define GALLIFREY_V3A_TX_STARTUP_OFFSET                                                     0x803
#define GALLIFREY_V3A_TX_STARTUP_ADDR                                                       (GALLIFREY_V3A_BASE + GALLIFREY_V3A_TX_STARTUP_OFFSET)
#define GALLIFREY_V3A_TX_STARTUP_SZ                                                         8
#define GALLIFREY_V3A_TX_STARTUP                                                            (volatile uint8_t *)GALLIFREY_V3A_TX_STARTUP_ADDR
#define GALLIFREY_V3A_RATE_RX_OFFSET                                                        0x810
#define GALLIFREY_V3A_RATE_RX_ADDR                                                          (GALLIFREY_V3A_BASE + GALLIFREY_V3A_RATE_RX_OFFSET)
#define GALLIFREY_V3A_RATE_RX_SZ                                                            8
#define GALLIFREY_V3A_RATE_RX                                                               (volatile uint8_t *)GALLIFREY_V3A_RATE_RX_ADDR
#define GALLIFREY_V3A_CHANNEL_RX_OFFSET                                                     0x811
#define GALLIFREY_V3A_CHANNEL_RX_ADDR                                                       (GALLIFREY_V3A_BASE + GALLIFREY_V3A_CHANNEL_RX_OFFSET)
#define GALLIFREY_V3A_CHANNEL_RX_SZ                                                         8
#define GALLIFREY_V3A_CHANNEL_RX                                                            (volatile uint8_t *)GALLIFREY_V3A_CHANNEL_RX_ADDR
#define GALLIFREY_V3A_RX_STARTUP_OFFSET                                                     0x812
#define GALLIFREY_V3A_RX_STARTUP_ADDR                                                       (GALLIFREY_V3A_BASE + GALLIFREY_V3A_RX_STARTUP_OFFSET)
#define GALLIFREY_V3A_RX_STARTUP_SZ                                                         8
#define GALLIFREY_V3A_RX_STARTUP                                                            (volatile uint8_t *)GALLIFREY_V3A_RX_STARTUP_ADDR
#define GALLIFREY_V3A_SYNC_WORD_LOW_OFFSET                                                  0x813
#define GALLIFREY_V3A_SYNC_WORD_LOW_ADDR                                                    (GALLIFREY_V3A_BASE + GALLIFREY_V3A_SYNC_WORD_LOW_OFFSET)
#define GALLIFREY_V3A_SYNC_WORD_LOW_SZ                                                      8
#define GALLIFREY_V3A_SYNC_WORD_LOW                                                         (volatile uint8_t *)GALLIFREY_V3A_SYNC_WORD_LOW_ADDR
#define GALLIFREY_V3A_SYNC_WORD_HIGH_OFFSET                                                 0x817
#define GALLIFREY_V3A_SYNC_WORD_HIGH_ADDR                                                   (GALLIFREY_V3A_BASE + GALLIFREY_V3A_SYNC_WORD_HIGH_OFFSET)
#define GALLIFREY_V3A_SYNC_WORD_HIGH_SZ                                                     8
#define GALLIFREY_V3A_SYNC_WORD_HIGH                                                        (volatile uint8_t *)GALLIFREY_V3A_SYNC_WORD_HIGH_ADDR
#define GALLIFREY_V3A_RX_POWEROFF_OFFSET                                                    0x820
#define GALLIFREY_V3A_RX_POWEROFF_ADDR                                                      (GALLIFREY_V3A_BASE + GALLIFREY_V3A_RX_POWEROFF_OFFSET)
#define GALLIFREY_V3A_RX_POWEROFF_SZ                                                        8
#define GALLIFREY_V3A_RX_POWEROFF                                                           (volatile uint8_t *)GALLIFREY_V3A_RX_POWEROFF_ADDR
#define GALLIFREY_V3A_RSSI_READOUT_OFFSET                                                   0x824
#define GALLIFREY_V3A_RSSI_READOUT_ADDR                                                     (GALLIFREY_V3A_BASE + GALLIFREY_V3A_RSSI_READOUT_OFFSET)
#define GALLIFREY_V3A_RSSI_READOUT_SZ                                                       8
#define GALLIFREY_V3A_RSSI_READOUT                                                          (volatile uint8_t *)GALLIFREY_V3A_RSSI_READOUT_ADDR
#define GALLIFREY_V3A_PACKET_LENGTH_OFFSET                                                  0x828
#define GALLIFREY_V3A_PACKET_LENGTH_ADDR                                                    (GALLIFREY_V3A_BASE + GALLIFREY_V3A_PACKET_LENGTH_OFFSET)
#define GALLIFREY_V3A_PACKET_LENGTH_SZ                                                      8
#define GALLIFREY_V3A_PACKET_LENGTH                                                         (volatile uint8_t *)GALLIFREY_V3A_PACKET_LENGTH_ADDR
#define GALLIFREY_V3A_CFO_READOUT_OFFSET                                                    0x82c
#define GALLIFREY_V3A_CFO_READOUT_ADDR                                                      (GALLIFREY_V3A_BASE + GALLIFREY_V3A_CFO_READOUT_OFFSET)
#define GALLIFREY_V3A_CFO_READOUT_SZ                                                        8
#define GALLIFREY_V3A_CFO_READOUT                                                           (volatile uint8_t *)GALLIFREY_V3A_CFO_READOUT_ADDR


#define GALLIFREY_V3A_DIG_RRMU_SPI_FORWARD_FIELD                                                {0,1,0}
#define GALLIFREY_V3A_DIG_IFACE_APB_BUS_ASYNC_FIELD                                             {9,1,0}
#define GALLIFREY_V3A_DIG_IFACE_APB_SAFE_FIELD                                                  {8,1,0}
#define GALLIFREY_V3A_DIG_IFACE_PASSWD_FIELD                                                    {16,16,0}
#define GALLIFREY_V3A_DIG_IFACE_OCTA_SPI_SLV_MISO_PD_FIELD                                      {35,1,0}
#define GALLIFREY_V3A_DIG_IFACE_OCTA_SPI_SLV_MISO_PU_FIELD                                      {34,1,0}
#define GALLIFREY_V3A_DIG_IFACE_OCTA_SPI_SLV_MISO_HIZ_FIELD                                     {33,1,0}
#define GALLIFREY_V3A_DIG_IFACE_OCTA_SPI_SLV_IS_SYNC_FIELD                                      {32,1,0}
#define GALLIFREY_V3A_DIG_GPIOS_RB_FIELD                                                        {43,1,0}
#define GALLIFREY_V3A_DIG_GPIOS_DRV_FIELD                                                       {41,2,0}
#define GALLIFREY_V3A_DIG_GPIOS_USE_EXT_SIGNALS_FIELD                                           {40,1,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_9_PD_FIELD                                                 {57,1,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_8_PD_FIELD                                                 {56,1,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_7_PD_FIELD                                                 {55,1,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_6_PD_FIELD                                                 {54,1,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_5_PD_FIELD                                                 {53,1,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_4_PD_FIELD                                                 {52,1,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_3_PD_FIELD                                                 {51,1,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_2_PD_FIELD                                                 {50,1,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_1_PD_FIELD                                                 {49,1,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_0_PD_FIELD                                                 {48,1,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_9_PU_FIELD                                                 {73,1,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_8_PU_FIELD                                                 {72,1,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_7_PU_FIELD                                                 {71,1,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_6_PU_FIELD                                                 {70,1,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_5_PU_FIELD                                                 {69,1,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_4_PU_FIELD                                                 {68,1,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_3_PU_FIELD                                                 {67,1,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_2_PU_FIELD                                                 {66,1,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_1_PU_FIELD                                                 {65,1,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_0_PU_FIELD                                                 {64,1,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_0_CONF_FIELD                                               {80,6,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_1_CONF_FIELD                                               {88,6,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_2_CONF_FIELD                                               {96,6,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_3_CONF_FIELD                                               {104,6,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_4_CONF_FIELD                                               {112,6,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_5_CONF_FIELD                                               {120,6,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_6_CONF_FIELD                                               {128,6,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_7_CONF_FIELD                                               {136,6,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_8_CONF_FIELD                                               {144,6,0}
#define GALLIFREY_V3A_DIG_GPIOS_GPIO_9_CONF_FIELD                                               {152,6,0}
#define GALLIFREY_V3A_DIG_GPIOS_TEST_BUS_CONFIG_FIELD                                           {160,4,0}
#define GALLIFREY_V3A_DIG_IRQ_CTRL_HIZ_FIELD                                                    {170,1,0}
#define GALLIFREY_V3A_DIG_IRQ_CTRL_PULSE_FIELD                                                  {169,1,0}
#define GALLIFREY_V3A_DIG_IRQ_CTRL_ACTIVE_LOW_FIELD                                             {168,1,0}
#define GALLIFREY_V3A_DIG_IRQ_CTRL_TX_END_SIG_FIELD                                             {177,1,0}
#define GALLIFREY_V3A_DIG_IRQ_CTRL_TX_START_FIELD                                               {176,1,0}
#define GALLIFREY_V3A_DIG_IRQ_CTRL_TX_PH_END_SIG_FIELD                                          {185,1,0}
#define GALLIFREY_V3A_DIG_IRQ_CTRL_TX_PH_READ_FIELD                                             {184,1,0}
#define GALLIFREY_V3A_DIG_IRQ_CTRL_RX_SYNC_DETECT_FIELD                                         {194,1,0}
#define GALLIFREY_V3A_DIG_IRQ_CTRL_RX_END_SIG_FIELD                                             {193,1,0}
#define GALLIFREY_V3A_DIG_IRQ_CTRL_RX_START_FIELD                                               {192,1,0}
#define GALLIFREY_V3A_DIG_IRQ_CTRL_RX_PH_END_SIG_FIELD                                          {201,1,0}
#define GALLIFREY_V3A_DIG_IRQ_CTRL_RX_PH_DATA_FIELD                                             {200,1,0}
#define GALLIFREY_V3A_DIG_SEQ_CONFIG_FIELD                                                      {208,8,0}
#define GALLIFREY_V3A_DIG_SEQ_SUB_CNT_FIELD                                                     {216,8,0}
#define GALLIFREY_V3A_DIG_SEQ_SEQS_0_TRIG_FIELD                                                 {232,4,0}
#define GALLIFREY_V3A_DIG_SEQ_SEQS_0_ADDR_FIELD                                                 {224,8,0}
#define GALLIFREY_V3A_DIG_SEQ_SEQS_1_TRIG_FIELD                                                 {248,4,0}
#define GALLIFREY_V3A_DIG_SEQ_SEQS_1_ADDR_FIELD                                                 {240,8,0}
#define GALLIFREY_V3A_DIG_SEQ_SEQS_2_TRIG_FIELD                                                 {264,4,0}
#define GALLIFREY_V3A_DIG_SEQ_SEQS_2_ADDR_FIELD                                                 {256,8,0}
#define GALLIFREY_V3A_DIG_SEQ_SEQS_3_TRIG_FIELD                                                 {280,4,0}
#define GALLIFREY_V3A_DIG_SEQ_SEQS_3_ADDR_FIELD                                                 {272,8,0}
#define GALLIFREY_V3A_DIG_SEQ_SEQS_4_TRIG_FIELD                                                 {296,4,0}
#define GALLIFREY_V3A_DIG_SEQ_SEQS_4_ADDR_FIELD                                                 {288,8,0}
#define GALLIFREY_V3A_DIG_SEQ_SEQS_5_TRIG_FIELD                                                 {312,4,0}
#define GALLIFREY_V3A_DIG_SEQ_SEQS_5_ADDR_FIELD                                                 {304,8,0}
#define GALLIFREY_V3A_DIG_TX_TOP_SOFT_STOP_FIELD                                                {325,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_ALT_CLK_FIELD                                                  {324,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_CTE_DATA_TYPE_FIELD                                            {320,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_PH_AES_MULT_ENC_FIELD                                          {328,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DEMUX_BIT_SWITCH_1_TO_0_DT_FIELD                      {341,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DEMUX_BIT_SWITCH_0_TO_1_DT_FIELD                      {337,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DEMUX_BIT_FORWARD_FIELD                               {336,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_PH_DATA_TYPE_IN_FIELD                                          {413,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_PH_USES_SEQ_CONFIG_FIELD                                       {412,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_PH_TYPE_IN_FIELD                                               {408,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_PH_CONFIG_FIELD                                                {400,8,0}
#define GALLIFREY_V3A_DIG_TX_TOP_PH_AES_DATA_TYPE_FIELD                                         {384,16,0}
#define GALLIFREY_V3A_DIG_TX_TOP_PH_DATA_IN_FIELD                                               {352,32,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CHANGE_DT_DEST_TYPE_FIELD                             {428,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CHANGE_DT_SRC_TYPE_FIELD                              {424,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MUX_BIT_SWITCH_1_TO_0_DT_FIELD                        {437,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MUX_BIT_SWITCH_0_TO_1_DT_FIELD                        {433,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MUX_BIT_FORWARD_FIELD                                 {432,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MUX_INT_SWITCH_1_TO_0_DT_FIELD                        {453,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MUX_INT_SWITCH_0_TO_1_DT_FIELD                        {449,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MUX_INT_FORWARD_FIELD                                 {448,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_BIT_TO_DEST_2_FIELD                       {464,16,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_0_TO_DEST_2_FIELD                     {480,16,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_1_TO_DEST_2_FIELD                     {496,16,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_INVERT_BIT_ENABLE_FIELD                               {512,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_0_UPSCALE_FIELD                          {521,3,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_0_ORDER_FIELD                            {520,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BIT_REPEATER_REPEAT_COUNT_FIELD                       {544,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BIT_REPEATER_ENABLE_TYPE_FIELD                        {528,16,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_1_UPSCALE_FIELD                          {553,3,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_1_ORDER_FIELD                            {552,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_IEEE802154_B2C_ENABLE_TYPE_FIELD                      {560,16,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CRC_CUSTOM_OUTPUT_MODE_DT_FIELD                       {592,16,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CRC_CUSTOM_MODE_TYPE_FIELD                            {576,16,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DATA_WHITENING_ENABLE_TYPE_FIELD                      {608,16,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MANCHESTER_ENABLE_TYPE_FIELD                          {624,16,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PIO4_4_8PSK_ENABLE_TYPE_FIELD                         {640,16,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CONV_CODES_ENABLE_TYPE_FIELD                          {656,16,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_0_SGN2_FIELD                             {681,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_0_SGN1_FIELD                             {680,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_0_E2_FIELD                               {676,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_0_E1_FIELD                               {672,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_1_SGN2_FIELD                             {697,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_1_SGN1_FIELD                             {696,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_1_E2_FIELD                               {692,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_1_E1_FIELD                               {688,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_MAN_M2_FIELD                                     {710,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_MAN_M1_FIELD                                     {704,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_0_SGN2_FIELD                                     {741,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_0_SGN1_FIELD                                     {740,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_0_E2_FIELD                                       {736,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_0_E1_FIELD                                       {732,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_0_M2_FIELD                                       {726,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_0_M1_FIELD                                       {720,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DECIMATOR_ORDER_FIELD                                 {747,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DECIMATOR_DOWNSCALE_FIELD                             {744,3,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_1_SGN2_FIELD                                     {773,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_1_SGN1_FIELD                                     {772,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_1_E2_FIELD                                       {768,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_1_E1_FIELD                                       {764,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_1_M2_FIELD                                       {758,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_1_M1_FIELD                                       {752,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DELAY_PATH_DEL_FIELD                                  {777,3,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DELAY_PATH_PATH1_NPATH2_FIELD                         {776,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_2_SGN2_FIELD                                     {805,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_2_SGN1_FIELD                                     {804,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_2_E2_FIELD                                       {800,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_2_E1_FIELD                                       {796,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_2_M2_FIELD                                       {790,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_MULT_2_M1_FIELD                                       {784,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DERIVATIVE_FILT_NON_ACTIVE_FIELD                      {810,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_DERIVATIVE_PORTS_FIELD                                {808,2,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_GENERIC_MAPPER_ENABLE_TYPE_FIELD                      {821,16,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_GENERIC_MAPPER_PI_OVER_4_FIELD                        {820,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_GENERIC_MAPPER_DIFFERENTIAL_FIELD                     {819,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_GENERIC_MAPPER_MAP_TYPE_FIELD                         {816,3,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_AM2PM_GAIN_FIELD                                   {840,3,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_FRACTIONAL_INTERPOLATOR_SKIP_FIRST_FIELD              {859,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_FRACTIONAL_INTERPOLATOR_HIGH_GAIN_FIELD               {858,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_FRACTIONAL_INTERPOLATOR_IS_SIGNED_FIELD               {856,2,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_FRACTIONAL_INTERPOLATOR_DEN_FIELD                     {852,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_FRACTIONAL_INTERPOLATOR_NUM_FIELD                     {848,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_DELAYED_DT_FIELD                         {1027,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_SYM0_2_FIELD                             {1019,8,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_SYM0_1_FIELD                             {1011,8,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_USE_ZERO_FIELD                           {1010,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_ENABLE_TYPE_FIELD                        {994,16,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_OSR_MODE_FIELD                           {992,2,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_PULSE_SHAPE_FIELD                        {864,128,0}
#define GALLIFREY_V3A_DIG_TX_TOP_SERIAL_FIFO_USE_SERIAL_FIFO_FIELD                              {1033,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_SERIAL_FIFO_QUAL_FIELD                                         {1032,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_BLOCK_DATA_ENABLE_TYPE_FIELD                          {1040,16,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_FSK_ENABLE_TYPE_FIELD                    {1186,16,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_FSK_DELAYED_DT_FIELD                     {1185,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_FSK_OSR_FIELD                            {1184,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_FSK_PULSE_SHAPE_FIELD                    {1056,128,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_PA_POWER_FIELD                                          {1208,8,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SATURATE_DELAY_DT_FIELD                               {1247,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SATURATE_PORTS_FIELD                                  {1245,2,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SATURATE_AMP_CORR_FIELD                               {1242,3,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SATURATE_AMP_LIMIT_FIELD                              {1229,13,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SATURATE_LIMIT_FIELD                                  {1216,13,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_HOLD_DATA_ENABLE_TYPE_FIELD                           {1248,16,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CONST_DATA_CHANNEL_FIELD                              {1272,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_CONST_DATA_VALUE_FIELD                                {1264,8,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SKIP_DATA_SKIP_TYPE_FIELD                             {1288,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_SKIP_DATA_N_FIELD                                     {1280,8,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_TX_BLE_DF_DEST_ADDR_FIELD                             {1302,6,1}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_PA_LINEARIZE_DEST_ADDR_FIELD                          {1296,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_RAMP_RD_TYPE_FIELD                                 {1398,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_RAMP_RU_TYPE_FIELD                                 {1394,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_RAMP_RD_WAIT_FIELD                                 {1388,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_RAMP_RU_WAIT_FIELD                                 {1382,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_RAMP_RAMP_DEC_2_FIELD                              {1374,8,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_RAMP_RAMP_INC_2_FIELD                              {1366,8,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_RAMP_RAMP_DEC_1_FIELD                              {1358,8,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_RAMP_RAMP_INC_1_FIELD                              {1350,8,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_RAMP_FINAL_POW_2_FIELD                             {1342,8,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_RAMP_INIT_POW_2_FIELD                              {1334,8,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_RAMP_FINAL_POW_1_FIELD                             {1326,8,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_RAMP_INIT_POW_1_FIELD                              {1318,8,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_RAMP_MULTIPLICATIVE_FIELD                          {1317,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_RAMP_RAMP_DP_FIELD                                 {1315,2,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_RAMP_SIN_RAMP_FIELD                                {1313,2,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_RAMP_ALIGN_RAMPS_FIELD                             {1312,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_AM2PM_LUT_FIELD                                    {1408,32,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_PA_LINEARIZE_LUT_FIELD                                {1440,120,0}
#define GALLIFREY_V3A_DIG_TX_TOP_PA_FILT_DISABLE_CK_GATING_FIELD                                {1565,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_PA_FILT_SEL_CK_FIELD                                           {1564,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_PA_FILT_ON_CK_PA_FIELD                                         {1563,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_PA_FILT_TUNE_FIELD                                             {1561,2,0}
#define GALLIFREY_V3A_DIG_TX_TOP_PA_FILT_E_FIELD                                                {1560,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_TX_BLE_DF_SWITCH_DELAY_FIELD                          {1586,8,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_TX_BLE_DF_CTE_INFO_FIELD                              {1578,8,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_TX_BLE_DF_CTE_DT_FIELD                                {1574,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_CONF_TX_BLE_DF_OSR_FIELD                                   {1568,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_CHANGE_DT_DEST_ADDR_FIELD                             {1618,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DEMUX_BIT_P1_DEST_ADDR_FIELD                          {1612,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DEMUX_BIT_P0_DEST_ADDR_FIELD                          {1606,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_TXPH_DEST_ADDR_FIELD                                  {1600,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_USE_RX_FSM_START_FIELD                                     {1625,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_BQF_DC_CAL_AGC_SEL_FIELD                                       {1624,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_SWITCH_DEST_BIT_P1_DEST_ADDR_FIELD                    {1650,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_SWITCH_DEST_BIT_P0_DEST_ADDR_FIELD                    {1644,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_MUX_INT_DEST_ADDR_FIELD                               {1638,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_MUX_BIT_DEST_ADDR_FIELD                               {1632,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_INPUT_ADDR_FIELD                                           {1656,7,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_SWITCH_DEST_INT_1_P1_DEST_ADDR_FIELD                  {1682,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_SWITCH_DEST_INT_1_P0_DEST_ADDR_FIELD                  {1676,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_SWITCH_DEST_INT_0_P1_DEST_ADDR_FIELD                  {1670,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_SWITCH_DEST_INT_0_P0_DEST_ADDR_FIELD                  {1664,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_CTRL_FIELD                                                 {1688,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_IEEE802154_B2C_DEST_ADDR_FIELD                        {1714,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_BIT_REPEATER_DEST_ADDR_FIELD                          {1708,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_FIFO_4BITS_DEST_ADDR_FIELD                            {1702,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_INVERT_BIT_DEST_ADDR_FIELD                            {1696,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_TMR_PREDIV_FIELD                                           {1720,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_PIO4_4_8PSK_DEST_ADDR_FIELD                           {1746,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_MANCHESTER_DEST_ADDR_FIELD                            {1740,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DATA_WHITENING_DEST_ADDR_FIELD                        {1734,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_CRC_CUSTOM_DEST_ADDR_FIELD                            {1728,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_FAULT_ON_IO_ERR_FIELD                                      {1756,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_TRACE_TO_GPIO_FIELD                                        {1755,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_GP_TMR_IRQ_IS_HARD_FIELD                                   {1754,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_GPIO_IRQ_IS_HARD_FIELD                                     {1753,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_USE_TMR_FOR_BB_FAST_SST_FIELD                              {1752,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_MULT_MAN_DEST_ADDR_FIELD                              {1778,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_BARREL_SHIFT_1_DEST_ADDR_FIELD                        {1772,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_BARREL_SHIFT_0_DEST_ADDR_FIELD                        {1766,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_CONV_CODES_DEST_ADDR_FIELD                            {1760,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_THIRD_STAGE_MASK_FIELD                                     {1785,2,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_THIRD_STAGE_IS_RF_FIELD                                    {1784,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DELAY_PATH_DEST_ADDR_FIELD                            {1810,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_MULT_2_DEST_ADDR_FIELD                                {1804,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_MULT_1_DEST_ADDR_FIELD                                {1798,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_MULT_0_DEST_ADDR_FIELD                                {1792,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_RF_FAST_IN_SST_PRESET_FIELD                                {1817,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_RF_GATE_FIELD                                              {1816,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_INTERPOLATOR_0_DEST_ADDR_FIELD                        {1842,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_PULSE_SHAPER_FSK_DEST_ADDR_FIELD                      {1836,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_PULSE_SHAPER_DEST_ADDR_FIELD                          {1830,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_GENERIC_MAPPER_DEST_ADDR_FIELD                        {1824,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_PA_AMP_DELAY_FIELD                                             {1849,3,0}
#define GALLIFREY_V3A_DIG_TX_TOP_PA_AMP_OFFSET_FIELD                                            {1848,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_CORDIC_ANGLE_DEST_ADDR_FIELD                          {1874,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DECIMATOR_DEST_ADDR_FIELD                             {1868,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_FRACTIONAL_INTERPOLATOR_DEST_ADDR_FIELD               {1862,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_INTERPOLATOR_1_DEST_ADDR_FIELD                        {1856,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_BB_GATE_IQ_1_FIELD                                         {1883,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_BB_GATE_IQ_0_FIELD                                         {1882,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_BB_GATE_DIG_FIELD                                          {1881,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_BB_GATE_ANA_FIELD                                          {1880,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_HOLD_DATA_DEST_ADDR_FIELD                             {1906,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_BLOCK_DATA_DEST_ADDR_FIELD                            {1900,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_SATURATE_DEST_ADDR_FIELD                              {1894,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_DERIVATIVE_DEST_ADDR_FIELD                            {1888,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_0_IGNORE_ENA_FIELD                      {1916,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_0_ENA_FIELD                             {1915,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_0_SHIFT_AMOUNT_FIELD                    {1912,3,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_PA_AM2PM_DEST_ADDR_FIELD                              {1938,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_PA_RAMP_DEST_ADDR_FIELD                               {1932,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_SKIP_DATA_DEST_ADDR_FIELD                             {1926,6,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TDP_MAPS_CONST_DATA_DEST_ADDR_FIELD                            {1920,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_1_IGNORE_ENA_FIELD                      {1948,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_1_ENA_FIELD                             {1947,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_1_SHIFT_AMOUNT_FIELD                    {1944,3,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_0_IND_FIELD                                         {1964,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_0_VREF_TUNE_FIELD                                   {1960,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_0_LP_FIELD                                          {1959,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_0_RAMP_MAX_FIELD                                    {1952,7,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_1_IND_FIELD                                         {1980,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_1_VREF_TUNE_FIELD                                   {1976,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_1_LP_FIELD                                          {1975,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_1_RAMP_MAX_FIELD                                    {1968,7,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_2_IND_FIELD                                         {1996,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_2_VREF_TUNE_FIELD                                   {1992,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_2_LP_FIELD                                          {1991,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_2_RAMP_MAX_FIELD                                    {1984,7,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_3_IND_FIELD                                         {2012,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_3_VREF_TUNE_FIELD                                   {2008,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_3_LP_FIELD                                          {2007,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_3_RAMP_MAX_FIELD                                    {2000,7,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_4_IND_FIELD                                         {2028,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_4_VREF_TUNE_FIELD                                   {2024,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_4_LP_FIELD                                          {2023,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_4_RAMP_MAX_FIELD                                    {2016,7,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_5_IND_FIELD                                         {2044,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_5_VREF_TUNE_FIELD                                   {2040,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_5_LP_FIELD                                          {2039,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_5_RAMP_MAX_FIELD                                    {2032,7,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_6_IND_FIELD                                         {2060,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_6_VREF_TUNE_FIELD                                   {2056,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_6_LP_FIELD                                          {2055,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_6_RAMP_MAX_FIELD                                    {2048,7,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_7_IND_FIELD                                         {2076,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_7_VREF_TUNE_FIELD                                   {2072,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_7_LP_FIELD                                          {2071,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_7_RAMP_MAX_FIELD                                    {2064,7,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_8_IND_FIELD                                         {2092,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_8_VREF_TUNE_FIELD                                   {2088,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_8_LP_FIELD                                          {2087,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_8_RAMP_MAX_FIELD                                    {2080,7,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_9_IND_FIELD                                         {2108,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_9_VREF_TUNE_FIELD                                   {2104,4,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_9_LP_FIELD                                          {2103,1,0}
#define GALLIFREY_V3A_DIG_TX_TOP_TX_POW_LUT_9_RAMP_MAX_FIELD                                    {2096,7,0}
#define GALLIFREY_V3A_DIG_PROT_TMR_T_STP_3_FIELD                                                {2128,3,0}
#define GALLIFREY_V3A_DIG_PROT_TMR_T_STP_2_FIELD                                                {2125,3,0}
#define GALLIFREY_V3A_DIG_PROT_TMR_T_STP_1_FIELD                                                {2122,3,0}
#define GALLIFREY_V3A_DIG_PROT_TMR_T_STP_0_FIELD                                                {2119,3,0}
#define GALLIFREY_V3A_DIG_PROT_TMR_ENA_FIELD                                                    {2118,1,0}
#define GALLIFREY_V3A_DIG_PROT_TMR_US_TICK_FIELD                                                {2112,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_OSR_INIT_FIELD                                             {2136,8,0}
#define GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_0_MULT_FIELD                                          {2158,2,0}
#define GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_0_VAL_FIELD                                           {2144,14,0}
#define GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_1_MULT_FIELD                                          {2174,2,0}
#define GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_1_VAL_FIELD                                           {2160,14,0}
#define GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_2_MULT_FIELD                                          {2190,2,0}
#define GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_2_VAL_FIELD                                           {2176,14,0}
#define GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_3_MULT_FIELD                                          {2206,2,0}
#define GALLIFREY_V3A_DIG_PROT_TMR_DELTAS_3_VAL_FIELD                                           {2192,14,0}
#define GALLIFREY_V3A_DIG_RX_TOP_PH_CHAIN_DELAY_FIELD                                           {2217,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_PH_USES_SEQ_CONFIG_FIELD                                       {2216,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_PH_CONFIG_FIELD                                                {2208,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0_INIT_STATE_FIELD                   {2232,2,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0_CHAIN_DELAY_FIELD                  {2240,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1_INIT_STATE_FIELD                   {2248,2,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1_CHAIN_DELAY_FIELD                  {2256,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_0_PORT_1_DT_FIELD                      {2264,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_1_PORT_1_DT_FIELD                      {2272,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_2_PORT_1_DT_FIELD                      {2280,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_3_PORT_1_DT_FIELD                      {2288,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_4_PORT_1_DT_FIELD                      {2296,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_5_PORT_1_DT_FIELD                      {2304,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0_INIT_DT_FIELD                             {2312,3,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_X4_DEST_LVL1_PORT_1_DT_FIELD                {2328,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_X4_DEST_LVL0_PORT_1_DT_FIELD                {2320,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0_CHAIN_DELAY_FIELD                         {2336,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1_INIT_DT_FIELD                             {2344,3,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1_CHAIN_DELAY_FIELD                         {2352,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_HP_FILTER_KEEP_CAL_FIELD                              {2362,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_HP_FILTER_TAU_FIELD                                   {2360,2,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_ADC_LIMIT_DETECT_SIG_LOW_TIME_DI_FIELD                {2394,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_ADC_LIMIT_DETECT_SIG_LOW_THRESH_DI_FIELD              {2386,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_ADC_LIMIT_DETECT_SOFT_CLIP_THRESH_DI_FIELD            {2378,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_ADC_LIMIT_DETECT_USE_ASYM_SI_FIELD                    {2377,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_ADC_LIMIT_DETECT_USE_BITMASK_SI_FIELD                 {2376,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_ADC_LIMIT_DETECT_SOFT_CLIP_DI_FIELD                   {2372,4,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_ADC_LIMIT_DETECT_HARD_CLIP_DI_FIELD                   {2368,4,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_0_DOWNSCALE_FIELD                           {2410,4,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_0_ORDER_FIELD                               {2408,2,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IQ_BALANCE_CAL_PHA_INIT_FIELD                         {2432,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IQ_BALANCE_CAL_AMP_INIT_FIELD                         {2424,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IQ_BALANCE_KEEP_CAL_FIELD                             {2423,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IQ_BALANCE_TAU_FIELD                                  {2418,5,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IQ_BALANCE_DYNAMIC_POST_SYNC_FIELD                    {2417,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IQ_BALANCE_ENABLE_DYNAMIC_FIELD                       {2416,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_1_DOWNSCALE_FIELD                           {2442,4,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_1_ORDER_FIELD                               {2440,2,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_REMOVE_IF_FLUSH_FIELD                                 {2460,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_REMOVE_IF_ANGLE_INC_FIELD                             {2448,12,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_2_DOWNSCALE_FIELD                           {2466,4,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_2_ORDER_FIELD                               {2464,2,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_3_DOWNSCALE_FIELD                           {2474,4,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DECIMATOR_3_ORDER_FIELD                               {2472,2,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FRACTIONAL_DECIMATOR_DEN_FIELD                        {2484,4,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FRACTIONAL_DECIMATOR_NUM_FIELD                        {2480,4,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORDIC_LIN2POL_FLUSH_FIELD                            {2488,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DERIVATIVE_FILT_NON_ACTIVE_FIELD                      {2498,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DERIVATIVE_PORTS_FIELD                                {2496,2,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DIFF_BY_N_N_FIELD                                     {2504,4,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_MAX_ERRORS_FIELD                           {2513,4,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_MODE_FIELD                                 {2512,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_CHAIN_DELAY_FIELD                          {2520,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_FULL_ISI_FIELD                             {2561,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_DO_FILTER_FIELD                            {2560,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_USE_SYNC_DETECT_FIELD                      {2559,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_DELAY_MODE_1_FIELD                         {2553,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_RESYNC_DELAY_FIELD                         {2547,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_PATTERN_BIAS_FIELD                         {2541,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_SINGLE_PEAK_FIELD                          {2540,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_ISI_THR_FIELD                              {2536,4,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_GAIN_FIELD                                 {2534,2,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_PATTERN_LEN_FIELD                          {2528,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0_GAIN_EXP_FIELD                       {2568,4,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_AVG_CMD_FIELD                              {2584,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CORRELATOR_SYNC_CMD_FIELD                             {2576,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0_PS_COEFS_FIELD                       {2592,64,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1_PS_COEFS_FIELD                       {2656,32,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1_GAIN_EXP_FIELD                       {2688,4,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_CTE_INFO_FIELD                              {2696,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_EXT_SAMPLE_FIELD                            {2705,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_EXT_IS_CTE_FIELD                            {2704,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_CHAIN_DELAY_FIELD                           {2712,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_EXT_SAMPLE_TYPE_FIELD                       {2745,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_USE_EXT_SAMPLE_FIELD                        {2744,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_IS_CTE_FIELD                                {2743,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_AOA_SLOT_TIME_FIELD                         {2742,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_SWITCH_DELAY_FIELD                          {2734,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_RX_PATH_DELAY_FIELD                         {2726,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_OSR_FIELD                                   {2720,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_SEND_INTERSYMB_FIELD                   {2759,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_PRESAMPLE_FIELD                        {2758,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_SAMPLE_BASED_FIELD                     {2757,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_OSR_FIELD                              {2752,5,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_CHAIN_DELAY_FIELD                      {2760,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DPSK_TED_ERR_INC_FIELD                                {2776,4,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DPSK_TED_TC_FIELD                                     {2768,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_PHASE_INTERP_X2_DELAY_DATA_FIELD                      {2784,3,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FSK_TED_TC_FIELD                                      {2792,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CARRIER_RECOVERY_LP_FILTER_TAU_FIELD                  {2813,2,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CARRIER_RECOVERY_TAU_PHASE_ERR_FIELD                  {2807,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CARRIER_RECOVERY_TAU_FREQ_ERR_FIELD                   {2801,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CARRIER_RECOVERY_PHASE_MODE_FIELD                     {2800,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_DEMAPPER_ENABLE_TYPE_FIELD                        {2816,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_CHAIN_DELAY_FIELD                     {2824,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_RESYNC_DELAY_FIELD                    {2842,5,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_MARGIN_PREAMBLE_FIELD                 {2837,5,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_MARGIN_FIELD                          {2832,5,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_ACCEPTED_ERRS_FIELD                   {2855,2,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_RESYNC_DELAY_FIELD                    {2850,5,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_USE_SYNC_DETECT_FIELD                 {2849,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_SEND_PHASE_CORR_FIELD                 {2848,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_CHAIN_DELAY_FIELD                     {2864,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DPSK_DEMOD_FILTER_SAMPLES_FIELD                       {2873,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_DPSK_DEMOD_COHERENT_FIELD                             {2872,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_COUNTER_OSR_FIELD                     {2897,4,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_USE_SYNC_DETECT_FIELD                 {2896,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_CMD_SYNC_FOUND_FIELD                  {2888,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_PATTERN_FIELD                         {2880,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_CHAIN_DELAY_FIELD                     {2904,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_FREQ_ERR_CMD_FIELD                       {2945,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_SEND_FREQ_ERR_FIELD                      {2944,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_SOFT_GAIN_FIELD                          {2942,2,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_USE_SOFT_DEC_FIELD                       {2941,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_USE_LAST_BIT_FIELD                       {2940,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_LAST_BIT_FIELD                           {2939,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_DEPTH_FIELD                              {2936,3,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_AMP_111_FIELD                            {2928,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_AMP_011_FIELD                            {2920,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_AMP_010_FIELD                            {2912,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_BLR_DELAY_CHAIN_DELAY_FIELD                           {2960,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_SUBSAMPLE_OSR_FIELD                                   {2968,4,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_CRC_CUSTOM_MODE_INIT_FIELD                            {2976,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_USE_SYNC_DETECT_FIELD                    {2988,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_BLR_DONT_FLUSH_FIELD                     {2987,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_MEM_OUT_FIELD                            {2985,2,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_BLR_E_FIELD                              {2984,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_BLR_PACKET_LEN_FIELD                     {2992,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RSSI_READOUT_NRST_MIN_MAX_FIELD                       {3004,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_RSSI_READOUT_TAU_FIELD                                {3000,4,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_BLR_CMD_ERR1_FIELD                       {3024,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_BLR_CMD_OK2_FIELD                        {3016,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_BLR_CMD_OK1_FIELD                        {3008,8,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FLUSH_CHAIN_DELAY_FIELD                               {3032,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_LIMIT_RATE_DATA_SIZE_FIELD                            {3048,2,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_LIMIT_RATE_CLOCK_MODE_FIELD                           {3046,2,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_LIMIT_RATE_OSR_FIELD                                  {3040,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FLUSH_LEN_FIELD                                       {3062,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_FLUSH_OSR_FIELD                                       {3056,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_0_SGN2_FIELD                                     {3093,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_0_SGN1_FIELD                                     {3092,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_0_E2_FIELD                                       {3088,4,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_0_E1_FIELD                                       {3084,4,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_0_M2_FIELD                                       {3078,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_0_M1_FIELD                                       {3072,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_CTE_QUAL_FIELD                                                 {3102,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_CTE_AUTO_PULL_FIELD                                            {3101,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_CTE_EXT_IQ_SMP_TYPE_FIELD                                      {3100,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_CTE_Q_NI_FIRST_FIELD                                           {3099,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_CTE_IQ_MSB_FIRST_FIELD                                         {3098,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_CTE_IQ_DATA_BUS_SIZE_FIELD                                     {3096,2,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_1_SGN2_FIELD                                     {3125,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_1_SGN1_FIELD                                     {3124,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_1_E2_FIELD                                       {3120,4,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_1_E1_FIELD                                       {3116,4,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_1_M2_FIELD                                       {3110,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_1_M1_FIELD                                       {3104,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_ADC_CTRL_SWITCH_I_Q_FIELD                                      {3134,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_ADC_CTRL_INVERT_DUTY_CYCLE_FIELD                               {3133,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_ADC_CTRL_INVERT_CLK_FIELD                                      {3132,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_ADC_CTRL_CLK_DIV_FIELD                                         {3128,4,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_2_SGN2_FIELD                                     {3157,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_2_SGN1_FIELD                                     {3156,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_2_E2_FIELD                                       {3152,4,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_2_E1_FIELD                                       {3148,4,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_2_M2_FIELD                                       {3142,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_2_M1_FIELD                                       {3136,6,0}
#define GALLIFREY_V3A_DIG_CLK_CTRL_TX_CK_ALT_FIELD                                              {3163,1,0}
#define GALLIFREY_V3A_DIG_CLK_CTRL_ACCEPT_CK_PAD_FIELD                                          {3162,1,0}
#define GALLIFREY_V3A_DIG_CLK_CTRL_SEL_CK_DIG_NXO_FIELD                                         {3161,1,0}
#define GALLIFREY_V3A_DIG_CLK_CTRL_SYS_CK_E_FIELD                                               {3160,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_3_SGN2_FIELD                                     {3189,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_3_SGN1_FIELD                                     {3188,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_3_E2_FIELD                                       {3184,4,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_3_E1_FIELD                                       {3180,4,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_3_M2_FIELD                                       {3174,6,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_CONF_MULT_3_M1_FIELD                                       {3168,6,0}
#define GALLIFREY_V3A_DIG_AES_BYTE_ORDER_FIELD                                                  {3193,1,0}
#define GALLIFREY_V3A_DIG_AES_RX_MULTIPLE_ENCODING_FIELD                                        {3192,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_MEM_EL_1_DEST_ADDR_FIELD                              {3221,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_MEM_EL_0_DEST_ADDR_FIELD                              {3214,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_RXPH_DEST_ADDR_FIELD                                  {3207,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_INPUT_DEST_ADDR_FIELD                                 {3200,7,1}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_TEE_0_P0_DEST_ADDR_FIELD                              {3253,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_OBSERVE_POINT_1_DEST_ADDR_FIELD                       {3246,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_OBSERVE_POINT_0_DEST_ADDR_FIELD                       {3239,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_MEM_EL_2_DEST_ADDR_FIELD                              {3232,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_TEE_2_P0_DEST_ADDR_FIELD                              {3285,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_TEE_1_P1_DEST_ADDR_FIELD                              {3278,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_TEE_1_P0_DEST_ADDR_FIELD                              {3271,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_TEE_0_P1_DEST_ADDR_FIELD                              {3264,7,1}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_TEE4_P0_DEST_ADDR_FIELD                               {3317,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_TEE_3_P1_DEST_ADDR_FIELD                              {3310,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_TEE_3_P0_DEST_ADDR_FIELD                              {3303,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_TEE_2_P1_DEST_ADDR_FIELD                              {3296,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_SPLIT_DATA_P0_DEST_ADDR_FIELD                         {3349,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_TEE4_P3_DEST_ADDR_FIELD                               {3342,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_TEE4_P2_DEST_ADDR_FIELD                               {3335,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_TEE4_P1_DEST_ADDR_FIELD                               {3328,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_CTRL_SWITCH_DEST_1_P0_DEST_ADDR_FIELD                 {3381,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_CTRL_SWITCH_DEST_0_P1_DEST_ADDR_FIELD                 {3374,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_CTRL_SWITCH_DEST_0_P0_DEST_ADDR_FIELD                 {3367,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_SPLIT_DATA_P1_DEST_ADDR_FIELD                         {3360,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_DEST_1_P0_DEST_ADDR_FIELD                   {3413,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_DEST_0_P1_DEST_ADDR_FIELD                   {3406,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_DEST_0_P0_DEST_ADDR_FIELD                   {3399,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_CTRL_SWITCH_DEST_1_P1_DEST_ADDR_FIELD                 {3392,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_DEST_3_P0_DEST_ADDR_FIELD                   {3445,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_DEST_2_P1_DEST_ADDR_FIELD                   {3438,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_DEST_2_P0_DEST_ADDR_FIELD                   {3431,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_DEST_1_P1_DEST_ADDR_FIELD                   {3424,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_DEST_5_P0_DEST_ADDR_FIELD                   {3477,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_DEST_4_P1_DEST_ADDR_FIELD                   {3470,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_DEST_4_P0_DEST_ADDR_FIELD                   {3463,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_DEST_3_P1_DEST_ADDR_FIELD                   {3456,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_X4_DEST_P2_DEST_ADDR_FIELD                  {3509,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_X4_DEST_P1_DEST_ADDR_FIELD                  {3502,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_X4_DEST_P0_DEST_ADDR_FIELD                  {3495,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_DEST_5_P1_DEST_ADDR_FIELD                   {3488,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_IQ_BALANCE_DEST_ADDR_FIELD                            {3541,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_CHANGE_DT_1_DEST_ADDR_FIELD                           {3534,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_CHANGE_DT_0_DEST_ADDR_FIELD                           {3527,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_X4_DEST_P3_DEST_ADDR_FIELD                  {3520,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DECIMATOR_1_DEST_ADDR_FIELD                           {3573,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DECIMATOR_0_DEST_ADDR_FIELD                           {3566,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_REMOVE_IF_DEST_ADDR_FIELD                             {3559,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_HP_FILTER_DEST_ADDR_FIELD                             {3552,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_CHANNEL_FILTER_FIR_DEST_ADDR_FIELD                    {3605,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_FRACTIONAL_DECIMATOR_DEST_ADDR_FIELD                  {3598,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DECIMATOR_3_DEST_ADDR_FIELD                           {3591,7,1}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DECIMATOR_2_DEST_ADDR_FIELD                           {3584,7,1}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_CORRELATOR_P0_DEST_ADDR_FIELD                         {3637,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DIFF_BY_N_DEST_ADDR_FIELD                             {3630,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DERIVATIVE_DEST_ADDR_FIELD                            {3623,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_CORDIC_LIN2POL_DEST_ADDR_FIELD                        {3616,7,1}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_CLOCK_RECOVERY_P0_DEST_ADDR_FIELD                     {3669,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_MATCHED_FILTER_1_DEST_ADDR_FIELD                      {3662,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_MATCHED_FILTER_0_DEST_ADDR_FIELD                      {3655,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_CORRELATOR_P1_DEST_ADDR_FIELD                         {3648,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_FSK_TED_DEST_ADDR_FIELD                               {3701,7,1}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_PHASE_INTERP_X2_DEST_ADDR_FIELD                       {3694,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DPSK_TED_DEST_ADDR_FIELD                              {3687,7,1}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_CLOCK_RECOVERY_P1_DEST_ADDR_FIELD                     {3680,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_BLR_SYNC_DETECT_P1_DEST_ADDR_FIELD                    {3733,7,1}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_BLR_SYNC_DETECT_P0_DEST_ADDR_FIELD                    {3726,7,1}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_BLR_DEMAPPER_DEST_ADDR_FIELD                          {3719,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_CARRIER_RECOVERY_DEST_ADDR_FIELD                      {3712,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_EDR_SYNC_DETECT_P2_DEST_ADDR_FIELD                    {3765,7,1}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_EDR_SYNC_DETECT_P1_DEST_ADDR_FIELD                    {3758,7,1}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_EDR_SYNC_DETECT_P0_DEST_ADDR_FIELD                    {3751,7,1}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_BLR_SYNC_DETECT_P2_DEST_ADDR_FIELD                    {3744,7,1}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_IEEE_802154_C2B_P0_DEST_ADDR_FIELD                    {3797,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DPSK_DEMOD_P2_DEST_ADDR_FIELD                         {3790,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DPSK_DEMOD_P1_DEST_ADDR_FIELD                         {3783,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DPSK_DEMOD_P0_DEST_ADDR_FIELD                         {3776,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_HARD_DECISION_DEST_ADDR_FIELD                         {3829,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_VITERBI_GFSK_P1_DEST_ADDR_FIELD                       {3822,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_VITERBI_GFSK_P0_DEST_ADDR_FIELD                       {3815,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_IEEE_802154_C2B_P1_DEST_ADDR_FIELD                    {3808,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_DATA_WHITENING_DEST_ADDR_FIELD                        {3861,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_MANCHESTER_DEST_ADDR_FIELD                            {3854,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_SUBSAMPLE_DEST_ADDR_FIELD                             {3847,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_BLR_DELAY_DEST_ADDR_FIELD                             {3840,7,1}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_VITERBI_CONV_P1_DEST_ADDR_FIELD                       {3893,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_VITERBI_CONV_P0_DEST_ADDR_FIELD                       {3886,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_CRC_CUSTOM_DEST_ADDR_FIELD                            {3879,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_IEEE_802154_F2L_DEST_ADDR_FIELD                       {3872,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_FLUSH_DEST_ADDR_FIELD                                 {3925,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_LIMIT_RATE_DEST_ADDR_FIELD                            {3918,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_VITERBI_CONV_P3_DEST_ADDR_FIELD                       {3911,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_VITERBI_CONV_P2_DEST_ADDR_FIELD                       {3904,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_MULT_3_DEST_ADDR_FIELD                                {3957,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_MULT_2_DEST_ADDR_FIELD                                {3950,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_MULT_1_DEST_ADDR_FIELD                                {3943,7,0}
#define GALLIFREY_V3A_DIG_RX_TOP_RDP_MAPS_MULT_0_DEST_ADDR_FIELD                                {3936,7,0}
#define GALLIFREY_V3A_DIG_DCO_CAL_BIAS_LEVEL_CTRL_RST_FIELD                                     {3981,2,0}
#define GALLIFREY_V3A_DIG_DCO_CAL_ADD_BIAS_CTRL_FIELD                                           {3979,2,0}
#define GALLIFREY_V3A_DIG_DCO_CAL_T_WAIT_FIELD                                                  {3976,3,0}
#define GALLIFREY_V3A_DIG_DCO_CAL_DET_BITS_FIELD                                                {3974,2,0}
#define GALLIFREY_V3A_DIG_DCO_CAL_T_BASE_FIELD                                                  {3968,6,0}
#define GALLIFREY_V3A_DIG_CLK_CTRL_DIV_CK_1_6_FIELD                                             {3999,3,0}
#define GALLIFREY_V3A_DIG_CLK_CTRL_DIV_CK_RCCO_FIELD                                            {3996,3,0}
#define GALLIFREY_V3A_DIG_CLK_CTRL_DIV_CK_TX_FIELD                                              {3993,3,0}
#define GALLIFREY_V3A_DIG_CLK_CTRL_DIV_CK_XO_FIELD                                              {3990,3,0}
#define GALLIFREY_V3A_DIG_CLK_CTRL_DIV_CK_DIG_FIELD                                             {3987,3,0}
#define GALLIFREY_V3A_DIG_CLK_CTRL_DIV_CK_SYS_FIELD                                             {3984,3,0}
#define GALLIFREY_V3A_DIG_IQ_FIFO_FLUSH_ON_COND_FIELD                                           {4009,1,0}
#define GALLIFREY_V3A_DIG_IQ_FIFO_USE_IT_FIELD                                                  {4008,1,0}
#define GALLIFREY_V3A_DIG_RCCO_COMP_E_FIELD                                                     {4029,1,0}
#define GALLIFREY_V3A_DIG_RCCO_COMP_BYPASS_FIELD                                                {4028,1,0}
#define GALLIFREY_V3A_DIG_RCCO_COMP_TAU_FIELD                                                   {4026,2,0}
#define GALLIFREY_V3A_DIG_RCCO_COMP_CNT_RC_LEN_L2_FIELD                                         {4024,2,0}
#define GALLIFREY_V3A_DIG_RCCO_COMP_TARGET_FREQ_FIELD                                           {4016,8,0}
#define GALLIFREY_V3A_DIG_AES_KEY_FIELD                                                         {4032,128,0}
#define GALLIFREY_V3A_DIG_AES_NONCE_FIELD                                                       {4160,104,0}
#define GALLIFREY_V3A_DIG_RX_FIFO_FLUSH_ON_COND_FIELD                                           {4265,1,0}
#define GALLIFREY_V3A_DIG_RX_FIFO_USE_IT_FIELD                                                  {4264,1,0}
#define GALLIFREY_V3A_DIG_TX_FIFO_FLUSH_ON_COND_FIELD                                           {4273,1,0}
#define GALLIFREY_V3A_DIG_TX_FIFO_USE_IT_FIELD                                                  {4272,1,0}
#define GALLIFREY_V3A_DIG_DIR_FIND_EXT_IQ_SMP_TYPE_FIELD                                        {4286,1,0}
#define GALLIFREY_V3A_DIG_DIR_FIND_Q_NI_FIRST_FIELD                                             {4285,1,0}
#define GALLIFREY_V3A_DIG_DIR_FIND_IQ_MSB_FIRST_FIELD                                           {4284,1,0}
#define GALLIFREY_V3A_DIG_DIR_FIND_IQ_DATA_BUS_SIZE_FIELD                                       {4282,2,0}
#define GALLIFREY_V3A_DIG_DIR_FIND_CTE_QUAL_FIELD                                               {4281,1,0}
#define GALLIFREY_V3A_DIG_DIR_FIND_CTE_AUTO_PULL_FIELD                                          {4280,1,0}
#define GALLIFREY_V3A_DIG_TX_BLE_DTM_E_FIELD                                                    {4301,1,0}
#define GALLIFREY_V3A_DIG_TX_BLE_DTM_CUSTOM_SW_FIELD                                            {4300,1,0}
#define GALLIFREY_V3A_DIG_TX_BLE_DTM_PKT_TYPE_FIELD                                             {4296,4,0}
#define GALLIFREY_V3A_DIG_TX_BLE_DTM_PKT_LEN_FIELD                                              {4288,8,0}
#define GALLIFREY_V3A_DIG_CHANNEL_HDL_SPACE_X0P5_FIELD                                          {4309,1,0}
#define GALLIFREY_V3A_DIG_CHANNEL_HDL_FW_TO_ADPLL_FIELD                                         {4308,1,0}
#define GALLIFREY_V3A_DIG_CHANNEL_HDL_ADPLL_CONF_FIELD                                          {4306,2,0}
#define GALLIFREY_V3A_DIG_CHANNEL_HDL_BLE_DW_CONF_FIELD                                         {4304,2,0}
#define GALLIFREY_V3A_DIG_MISC_TX_RF_E_FIELD                                                    {4312,1,0}
#define GALLIFREY_V3A_DIG_TRX_CRC_POLY_FIELD                                                    {4320,32,0}
#define GALLIFREY_V3A_DIG_TRX_CRC_RST_FIELD                                                     {4352,32,0}
#define GALLIFREY_V3A_DIG_TRX_DF_LUT_FIELD                                                      {4384,64,0}
#define GALLIFREY_V3A_DIG_TRX_DF_NO_REF_ANT_IN_SW_FIELD                                         {4453,1,0}
#define GALLIFREY_V3A_DIG_TRX_DF_REF_NEW_ANTENNA_FIELD                                          {4452,1,0}
#define GALLIFREY_V3A_DIG_TRX_DF_LUT_ELEM_LEN_FIELD                                             {4448,4,0}
#define GALLIFREY_V3A_DIG_TRX_MANCHESTER_IEEE_CODING_FIELD                                      {4456,1,0}
#define GALLIFREY_V3A_DIG_TRX_VITERBI_CONV_PUNCT_FIELD                                          {4472,8,0}
#define GALLIFREY_V3A_DIG_TRX_VITERBI_CONV_CODES_FIELD                                          {4464,8,0}
#define GALLIFREY_V3A_DIG_TRX_DATA_WHITENING_USE_CHANNEL_DW_FIELD                               {4487,1,0}
#define GALLIFREY_V3A_DIG_TRX_DATA_WHITENING_BLE_DW_FIELD                                       {4486,1,0}
#define GALLIFREY_V3A_DIG_TRX_DATA_WHITENING_BLE_CHN_FIELD                                      {4480,6,0}
#define GALLIFREY_V3A_DIG_TRX_DPSK_MOD_TYPE_FIELD                                               {4488,1,0}
#define GALLIFREY_V3A_ANA_TRX_DD_IF_E_FIELD                                                     {4501,1,0}
#define GALLIFREY_V3A_ANA_TRX_DD2IQ25_E_FIELD                                                   {4500,1,0}
#define GALLIFREY_V3A_ANA_TRX_DD2_E_FIELD                                                       {4499,1,0}
#define GALLIFREY_V3A_ANA_TRX_PA_E_FIELD                                                        {4498,1,0}
#define GALLIFREY_V3A_ANA_TRX_LNA_E_FIELD                                                       {4497,1,0}
#define GALLIFREY_V3A_ANA_TRX_ADPLL_DIG_RSTB_FIELD                                              {4496,1,0}
#define GALLIFREY_V3A_ANA_TRX_RF_LNA_CMPRDET_TH_FIELD                                           {4504,2,0}
#define GALLIFREY_V3A_DIG_TRX_SYNC_WORD_LOW_FIELD                                               {4512,32,0}
#define GALLIFREY_V3A_DIG_TRX_SYNC_WORD_HIGH_FIELD                                              {4544,32,0}
#define GALLIFREY_V3A_ANA_TRX_RF_PA_LAST_SLICE_E_FIELD                                          {4587,1,0}
#define GALLIFREY_V3A_ANA_TRX_RF_PA_POW_USE_LUT_FIELD                                           {4586,1,0}
#define GALLIFREY_V3A_ANA_TRX_RF_PA_CTUNE_SINGLE_FIELD                                          {4584,2,0}
#define GALLIFREY_V3A_ANA_TRX_RF_PA_CTUNE_DIFF_FIELD                                            {4582,2,0}
#define GALLIFREY_V3A_ANA_TRX_RF_PA_RFIN_VCM_CTRL_FIELD                                         {4577,5,0}
#define GALLIFREY_V3A_ANA_TRX_RF_PA_LP_E_FIELD                                                  {4576,1,0}
#define GALLIFREY_V3A_ANA_TRX_RF_LNA_PKDET_TH_FIELD                                             {4592,2,0}
#define GALLIFREY_V3A_ANA_TRX_RF_LNA_STG2_IBIAS_FIELD                                           {4606,2,0}
#define GALLIFREY_V3A_ANA_TRX_RF_LNA_STG1_IBIAS_FIELD                                           {4603,3,0}
#define GALLIFREY_V3A_ANA_TRX_RF_LNA_STG1_CTRIM_FIELD                                           {4600,3,0}
#define GALLIFREY_V3A_ANA_TRX_RF_MXP_CL_FIELD                                                   {4610,3,0}
#define GALLIFREY_V3A_ANA_TRX_RF_MXP_RB_FIELD                                                   {4608,2,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_PKDET_BIAS_E_FIELD                                     {4622,1,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_DC_FSU_FIELD                                           {4621,1,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_BUF_E_FIELD                                            {4620,1,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_E_FIELD                                                {4619,1,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_HBW_FIELD                                              {4618,1,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_FAST_IN_SST_FIELD                                      {4617,1,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_BYPASS_F_STRTUP_FIELD                                  {4616,1,0}
#define GALLIFREY_V3A_ANA_TRX_RF_RFSENSE_GAIN_FIELD                                             {4631,2,0}
#define GALLIFREY_V3A_ANA_TRX_RF_RFSENSE_CTRL_BIAS_SCAMP_FIELD                                  {4628,3,0}
#define GALLIFREY_V3A_ANA_TRX_RF_RFSENSE_CTRL_BIAS_FIELD                                        {4625,3,0}
#define GALLIFREY_V3A_ANA_TRX_RF_RFSENSE_E_FIELD                                                {4624,1,0}
#define GALLIFREY_V3A_ANA_TRX_RF_DD2_IF_DIV4B_8_FIELD                                           {4654,1,0}
#define GALLIFREY_V3A_ANA_TRX_RF_DD2_DELAY_E_FIELD                                              {4653,1,0}
#define GALLIFREY_V3A_ANA_TRX_RF_DD2_INV_CLK_FIELD                                              {4652,1,0}
#define GALLIFREY_V3A_ANA_TRX_RF_DD2_DELAY_SEL_FIELD                                            {4648,4,0}
#define GALLIFREY_V3A_ANA_TRX_RF_DD2_IQ25_IBIAS_FIELD                                           {4644,4,0}
#define GALLIFREY_V3A_ANA_TRX_RF_DD2_IBIAS_FIELD                                                {4640,4,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_BUF_HP_FIELD                                           {4677,1,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_BUF_OD_LP_FIELD                                        {4676,1,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_HP2_FIELD                                              {4675,1,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_HP1_FIELD                                              {4674,1,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_OD_LP2_FIELD                                           {4673,1,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_OD_LP1_FIELD                                           {4672,1,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_PKDET_IB_CTRL_FIELD                                    {4668,4,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_IB_CTRL_FIELD                                          {4664,4,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_PP_OFF_FIELD                                           {4663,1,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_R2_FIELD                                               {4659,4,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_R1_FIELD                                               {4656,3,0}
#define GALLIFREY_V3A_PMU_VREF_PREPA_E_FIELD                                                    {4683,1,0}
#define GALLIFREY_V3A_PMU_VREF_PREPA_TUNE_FIELD                                                 {4680,3,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_BQF2_PKDET_REF_FIELD                                   {4696,4,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_BQF1_PKDET_REF_FIELD                                   {4692,4,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_BUF_PKDET_REF_FIELD                                    {4688,4,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_DC_CAL_QM_FIELD                                        {4725,7,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_DC_CAL_QP_FIELD                                        {4718,7,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_DC_CAL_IM_FIELD                                        {4711,7,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_DC_CAL_IP_FIELD                                        {4704,7,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ADC_BIAS_COMP_FIELD                                            {4744,3,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ADC_BIAS_REF_FIELD                                             {4741,3,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ADC_VREF_TUNE_FIELD                                            {4739,2,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ADC_Q_ATB_AMP_GAIN_FIELD                                       {4737,2,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ADC_Q_ATB_AMP_RST_FIELD                                        {4736,1,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ADC_Q_ATB_AMP_CK_FIELD                                         {4762,1,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ADC_Q_ATB_E_FIELD                                              {4761,1,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ADC_I_ATB_E_FIELD                                              {4760,1,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ADC_VREF_I_E_FIELD                                             {4759,1,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ADC_VREF_V_E_FIELD                                             {4758,1,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ADC_BUFFER_TIE_DIRECT_E_FIELD                                  {4757,1,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ADC_BUFFER_E_FIELD                                             {4756,1,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ADC_Q_RST_B_FIELD                                              {4755,1,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ADC_I_RST_B_FIELD                                              {4754,1,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ADC_Q_E_FIELD                                                  {4753,1,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ADC_I_E_FIELD                                                  {4752,1,0}
#define GALLIFREY_V3A_PMU_LDO_PA_E_FIELD                                                        {4776,1,0}
#define GALLIFREY_V3A_PMU_LDO_PA_TUNE_DYN_NSTAT_FIELD                                           {4775,1,0}
#define GALLIFREY_V3A_PMU_IEXT_E_FIELD                                                          {4774,1,0}
#define GALLIFREY_V3A_PMU_PTAT_THIN_E_FIELD                                                     {4773,1,0}
#define GALLIFREY_V3A_PMU_PTAT_THICK_E_FIELD                                                    {4772,1,0}
#define GALLIFREY_V3A_PMU_ISOLATE_MISC_B_FIELD                                                  {4771,1,0}
#define GALLIFREY_V3A_PMU_ISOLATE_TX_B_FIELD                                                    {4770,1,0}
#define GALLIFREY_V3A_PMU_ISOLATE_RX_B_FIELD                                                    {4769,1,0}
#define GALLIFREY_V3A_PMU_USE_REFE_REG_FIELD                                                    {4768,1,0}
#define GALLIFREY_V3A_PMU_VREF_PA_E_FIELD                                                       {4788,1,0}
#define GALLIFREY_V3A_PMU_VREF_PA_TUNE_FIELD                                                    {4784,4,0}
#define GALLIFREY_V3A_PMU_VREF_LNA_BQF_E_FIELD                                                  {4795,1,0}
#define GALLIFREY_V3A_PMU_VREF_LNA_BQF_TUNE_FIELD                                               {4792,3,0}
#define GALLIFREY_V3A_PMU_VREF_ADC_E_FIELD                                                      {4803,1,0}
#define GALLIFREY_V3A_PMU_VREF_ADC_TUNE_FIELD                                                   {4800,3,0}
#define GALLIFREY_V3A_PMU_VREF_DIG_E_FIELD                                                      {4811,1,0}
#define GALLIFREY_V3A_PMU_VREF_DIG_TUNE_FIELD                                                   {4808,3,0}
#define GALLIFREY_V3A_PMU_VREF_MISC_E_FIELD                                                     {4819,1,0}
#define GALLIFREY_V3A_PMU_VREF_MISC_TUNE_FIELD                                                  {4816,3,0}
#define GALLIFREY_V3A_ANA_MISC_RCCO_E_FIELD                                                     {4824,1,0}
#define GALLIFREY_V3A_PMU_LDO_PREPA_E_FIELD                                                     {4840,1,0}
#define GALLIFREY_V3A_PMU_LDO_PREPA_ISTAB_FIELD                                                 {4838,2,0}
#define GALLIFREY_V3A_PMU_LDO_PREPA_IBIAS_FIELD                                                 {4836,2,0}
#define GALLIFREY_V3A_PMU_LDO_PREPA_CM_FIELD                                                    {4834,2,0}
#define GALLIFREY_V3A_PMU_LDO_PREPA_RM_FIELD                                                    {4832,2,0}
#define GALLIFREY_V3A_PMU_LDO_PALOW_ISTAB_FIELD                                                 {4860,4,0}
#define GALLIFREY_V3A_PMU_LDO_PALOW_IBIAS_FIELD                                                 {4856,4,0}
#define GALLIFREY_V3A_PMU_LDO_PALOW_CM_FIELD                                                    {4852,4,0}
#define GALLIFREY_V3A_PMU_LDO_PALOW_RM_FIELD                                                    {4848,4,0}
#define GALLIFREY_V3A_PMU_LDO_PAHIGH_ISTAB_FIELD                                                {4876,4,0}
#define GALLIFREY_V3A_PMU_LDO_PAHIGH_IBIAS_FIELD                                                {4872,4,0}
#define GALLIFREY_V3A_PMU_LDO_PAHIGH_CM_FIELD                                                   {4868,4,0}
#define GALLIFREY_V3A_PMU_LDO_PAHIGH_RM_FIELD                                                   {4864,4,0}
#define GALLIFREY_V3A_PMU_LDO_LNA_E_FIELD                                                       {4888,1,0}
#define GALLIFREY_V3A_PMU_LDO_LNA_ISTAB_FIELD                                                   {4886,2,0}
#define GALLIFREY_V3A_PMU_LDO_LNA_IBIAS_FIELD                                                   {4884,2,0}
#define GALLIFREY_V3A_PMU_LDO_LNA_CM_FIELD                                                      {4882,2,0}
#define GALLIFREY_V3A_PMU_LDO_LNA_RM_FIELD                                                      {4880,2,0}
#define GALLIFREY_V3A_PMU_LDO_BQF_E_FIELD                                                       {4904,1,0}
#define GALLIFREY_V3A_PMU_LDO_BQF_ISTAB_FIELD                                                   {4902,2,0}
#define GALLIFREY_V3A_PMU_LDO_BQF_IBIAS_FIELD                                                   {4900,2,0}
#define GALLIFREY_V3A_PMU_LDO_BQF_CM_FIELD                                                      {4898,2,0}
#define GALLIFREY_V3A_PMU_LDO_BQF_RM_FIELD                                                      {4896,2,0}
#define GALLIFREY_V3A_PMU_LDO_ADCBUF_E_FIELD                                                    {4920,1,0}
#define GALLIFREY_V3A_PMU_LDO_ADCBUF_ISTAB_FIELD                                                {4918,2,0}
#define GALLIFREY_V3A_PMU_LDO_ADCBUF_IBIAS_FIELD                                                {4916,2,0}
#define GALLIFREY_V3A_PMU_LDO_ADCBUF_CM_FIELD                                                   {4914,2,0}
#define GALLIFREY_V3A_PMU_LDO_ADCBUF_RM_FIELD                                                   {4912,2,0}
#define GALLIFREY_V3A_PMU_LDO_ADC_E_FIELD                                                       {4936,1,0}
#define GALLIFREY_V3A_PMU_LDO_ADC_ISTAB_FIELD                                                   {4934,2,0}
#define GALLIFREY_V3A_PMU_LDO_ADC_IBIAS_FIELD                                                   {4932,2,0}
#define GALLIFREY_V3A_PMU_LDO_ADC_CM_FIELD                                                      {4930,2,0}
#define GALLIFREY_V3A_PMU_LDO_ADC_RM_FIELD                                                      {4928,2,0}
#define GALLIFREY_V3A_PMU_LDO_DIG_E_FIELD                                                       {4952,1,0}
#define GALLIFREY_V3A_PMU_LDO_DIG_ISTAB_FIELD                                                   {4950,2,0}
#define GALLIFREY_V3A_PMU_LDO_DIG_IBIAS_FIELD                                                   {4948,2,0}
#define GALLIFREY_V3A_PMU_LDO_DIG_CM_FIELD                                                      {4946,2,0}
#define GALLIFREY_V3A_PMU_LDO_DIG_RM_FIELD                                                      {4944,2,0}
#define GALLIFREY_V3A_PMU_LDO_MISC_E_FIELD                                                      {4968,1,0}
#define GALLIFREY_V3A_PMU_LDO_MISC_ISTAB_FIELD                                                  {4966,2,0}
#define GALLIFREY_V3A_PMU_LDO_MISC_IBIAS_FIELD                                                  {4964,2,0}
#define GALLIFREY_V3A_PMU_LDO_MISC_CM_FIELD                                                     {4962,2,0}
#define GALLIFREY_V3A_PMU_LDO_MISC_RM_FIELD                                                     {4960,2,0}
#define GALLIFREY_V3A_ANA_MISC_TEMP_SENS_E_FIELD                                                {4981,1,0}
#define GALLIFREY_V3A_ANA_MISC_TEMP_SENS_FORCE_PTAT_OK_FIELD                                    {4980,1,0}
#define GALLIFREY_V3A_ANA_MISC_TEMP_SENS_PTAT_COMP_E_FIELD                                      {4979,1,0}
#define GALLIFREY_V3A_ANA_MISC_TEMP_SENS_PTAT_CTRL_FIELD                                        {4976,3,0}
#define GALLIFREY_V3A_XTAL_FREQ_TRIM_FIELD                                                      {4984,8,0}
#define GALLIFREY_V3A_ATB_ADPLL_FIELD                                                           {4992,16,0}
#define GALLIFREY_V3A_ATB_REFE_FIELD                                                            {5008,16,0}
#define GALLIFREY_V3A_ATB_RXTX_FIELD                                                            {5024,20,0}
#define GALLIFREY_V3A_CHANNEL_FIELD                                                             {5048,7,0}
#define GALLIFREY_V3A_ATB_ADC_FIELD                                                             {5056,16,0}
#define GALLIFREY_V3A_COMMAND_FIELD                                                             {5072,8,0}
#define GALLIFREY_V3A_PT_CMD_FIELD                                                              {5080,8,0}
#define GALLIFREY_V3A_XTAL_SEL_XO_CTRL_FIELD                                                    {5113,1,0}
#define GALLIFREY_V3A_XTAL_XO_E_B_FIELD                                                         {5112,1,0}
#define GALLIFREY_V3A_XTAL_OSC_PTAT_START_FIELD                                                 {5111,1,0}
#define GALLIFREY_V3A_XTAL_PTAT_LP_FIELD                                                        {5109,2,0}
#define GALLIFREY_V3A_XTAL_BUFFER_GAIN_FIELD                                                    {5107,2,0}
#define GALLIFREY_V3A_XTAL_XTAL_AMP_FIELD                                                       {5103,4,0}
#define GALLIFREY_V3A_XTAL_LOW_CLK_READY_TH_FIELD                                               {5102,1,0}
#define GALLIFREY_V3A_XTAL_AFTERSTARTUP_CURR_SEL_FIELD                                          {5100,2,0}
#define GALLIFREY_V3A_XTAL_STARTUP_CURR_SEL_FIELD                                               {5098,2,0}
#define GALLIFREY_V3A_XTAL_INV_CLK_DIG_FIELD                                                    {5097,1,0}
#define GALLIFREY_V3A_XTAL_INV_CLK_PLL_FIELD                                                    {5096,1,0}
#define GALLIFREY_V3A_XTAL_FORCE_CLK_READY_FIELD                                                {5095,1,0}
#define GALLIFREY_V3A_XTAL_CLK_OUT_E_B_FIELD                                                    {5094,1,0}
#define GALLIFREY_V3A_XTAL_CLK_PLL_E_B_FIELD                                                    {5093,1,0}
#define GALLIFREY_V3A_XTAL_CLK_DIG_E_B_FIELD                                                    {5092,1,0}
#define GALLIFREY_V3A_XTAL_BUFF_E_B_FIELD                                                       {5091,1,0}
#define GALLIFREY_V3A_XTAL_FAST_H_MODE_FIELD                                                    {5090,1,0}
#define GALLIFREY_V3A_XTAL_LP_MODE_FIELD                                                        {5089,1,0}
#define GALLIFREY_V3A_XTAL_EXT_CLK_MODE_FIELD                                                   {5088,1,0}
#define GALLIFREY_V3A_SYNC_DETECTED_FIELD                                                       {5123,1,0}
#define GALLIFREY_V3A_RX_STATUS_FIELD                                                           {5122,1,0}
#define GALLIFREY_V3A_TX_SERIAL_OVERFLOW_FIELD                                                  {5121,1,0}
#define GALLIFREY_V3A_TX_STATUS_FIELD                                                           {5120,1,0}
#define GALLIFREY_V3A_RXPH_STATUS_FIELD                                                         {5128,4,0}
#define GALLIFREY_V3A_P0_DATA_Q_FIELD                                                           {5168,16,0}
#define GALLIFREY_V3A_P0_DATA_I_FIELD                                                           {5152,16,0}
#define GALLIFREY_V3A_P1_DATA_Q_FIELD                                                           {5200,16,0}
#define GALLIFREY_V3A_P1_DATA_I_FIELD                                                           {5184,16,0}
#define GALLIFREY_V3A_RX_DATA_OUT_FIELD                                                         {5216,32,0}
#define GALLIFREY_V3A_TX_FIFO_DATA_FIELD                                                        {5248,8,0}
#define GALLIFREY_V3A_TX_FIFO_DATA_32_FIELD                                                     {5280,32,0}
#define GALLIFREY_V3A_TX_FIFO_COUNT_FIELD                                                       {5312,8,0}
#define GALLIFREY_V3A_TX_FIFO_ACCESS_ERR_FIELD                                                  {5348,3,0}
#define GALLIFREY_V3A_TX_UNDERFLOW_FIELD                                                        {5347,1,0}
#define GALLIFREY_V3A_TX_OVERFLOW_FIELD                                                         {5346,1,0}
#define GALLIFREY_V3A_TX_FULL_FIELD                                                             {5345,1,0}
#define GALLIFREY_V3A_TX_EMPTY_FIELD                                                            {5344,1,0}
#define GALLIFREY_V3A_TX_FIFO_FLUSH_FIELD                                                       {5344,1,0}
#define GALLIFREY_V3A_RX_FIFO_DATA_FIELD                                                        {5376,8,0}
#define GALLIFREY_V3A_RX_FIFO_DATA_32_FIELD                                                     {5408,32,0}
#define GALLIFREY_V3A_RX_FIFO_COUNT_FIELD                                                       {5440,8,0}
#define GALLIFREY_V3A_RX_FIFO_ACCESS_ERR_FIELD                                                  {5476,3,0}
#define GALLIFREY_V3A_RX_UNDERFLOW_FIELD                                                        {5475,1,0}
#define GALLIFREY_V3A_RX_OVERFLOW_FIELD                                                         {5474,1,0}
#define GALLIFREY_V3A_RX_FULL_FIELD                                                             {5473,1,0}
#define GALLIFREY_V3A_RX_EMPTY_FIELD                                                            {5472,1,0}
#define GALLIFREY_V3A_RX_FIFO_FLUSH_FIELD                                                       {5472,1,0}
#define GALLIFREY_V3A_IQ_FIFO_DATA_FIELD                                                        {5504,8,0}
#define GALLIFREY_V3A_IQ_FIFO_DATA_32_FIELD                                                     {5536,32,0}
#define GALLIFREY_V3A_IQ_FIFO_COUNT_FIELD                                                       {5568,8,0}
#define GALLIFREY_V3A_IQ_FIFO_ACCESS_ERR_FIELD                                                  {5604,3,0}
#define GALLIFREY_V3A_IQ_UNDERFLOW_FIELD                                                        {5603,1,0}
#define GALLIFREY_V3A_IQ_OVERFLOW_FIELD                                                         {5602,1,0}
#define GALLIFREY_V3A_IQ_FULL_FIELD                                                             {5601,1,0}
#define GALLIFREY_V3A_IQ_EMPTY_FIELD                                                            {5600,1,0}
#define GALLIFREY_V3A_IQ_FIFO_FLUSH_FIELD                                                       {5600,1,0}
#define GALLIFREY_V3A_DCO_AMP_CAL_BIAS_CTRL_FIELD                                               {5636,4,0}
#define GALLIFREY_V3A_DCO_AMP_CAL_BIAS_LEVEL_FIELD                                              {5634,2,0}
#define GALLIFREY_V3A_DCO_AMP_CAL_ERR_FIELD                                                     {5633,1,0}
#define GALLIFREY_V3A_DCO_AMP_CAL_DONE_FIELD                                                    {5632,1,0}
#define GALLIFREY_V3A_TS_O_FIELD                                                                {5644,2,0}
#define GALLIFREY_V3A_TS_PTAT_OK_FIELD                                                          {5643,1,0}
#define GALLIFREY_V3A_BANDGAP_OK_FIELD                                                          {5642,1,0}
#define GALLIFREY_V3A_OK_CK_DIG_FIELD                                                           {5641,1,0}
#define GALLIFREY_V3A_OK_CK_PLL_FIELD                                                           {5640,1,0}
#define GALLIFREY_V3A_RC_CAL_FIELD                                                              {5648,4,0}
#define GALLIFREY_V3A_RSSI_PKT_FIELD                                                            {5656,8,0}
#define GALLIFREY_V3A_SYNC_DETECT_FLAG_FIELD                                                    {5672,1,0}
#define GALLIFREY_V3A_RXPH_END_FLAG_FIELD                                                       {5671,1,0}
#define GALLIFREY_V3A_RXPH_DATA_FLAG_FIELD                                                      {5670,1,0}
#define GALLIFREY_V3A_TXPH_END_FLAG_FIELD                                                       {5669,1,0}
#define GALLIFREY_V3A_TXPH_READ_FLAG_FIELD                                                      {5668,1,0}
#define GALLIFREY_V3A_RX_END_FLAG_FIELD                                                         {5667,1,0}
#define GALLIFREY_V3A_RX_START_FLAG_FIELD                                                       {5666,1,0}
#define GALLIFREY_V3A_TX_END_FLAG_FIELD                                                         {5665,1,0}
#define GALLIFREY_V3A_TX_START_FLAG_FIELD                                                       {5664,1,0}
#define GALLIFREY_V3A_RSSI_LIN_FIELD                                                            {5696,12,0}
#define GALLIFREY_V3A_RSSI_MAX_FIELD                                                            {5728,8,0}
#define GALLIFREY_V3A_RSSI_MIN_FIELD                                                            {5720,8,0}
#define GALLIFREY_V3A_RSSI_FIELD                                                                {5712,8,0}
#define GALLIFREY_V3A_ATTENUATION_PKT_FIELD                                                     {5736,7,0}
#define GALLIFREY_V3A_AGC_CFG_ERR_FIELD                                                         {5745,1,0}
#define GALLIFREY_V3A_AGC_INSTR_ERR_FIELD                                                       {5744,1,0}
#define GALLIFREY_V3A_AES_LOCK_STATUS_FIELD                                                     {5752,2,0}
#define GALLIFREY_V3A_CFO_PKT_FIELD                                                             {5760,16,0}
#define GALLIFREY_V3A_IQ_CAL_PHA_FIELD                                                          {5784,8,0}
#define GALLIFREY_V3A_IQ_CAL_AMP_FIELD                                                          {5776,8,0}
#define GALLIFREY_V3A_REFE_LDO_VREF_TUNE_ST_FIELD                                               {5800,3,0}
#define GALLIFREY_V3A_REFE_LDO_MISC_TUNE_RM_ST_FIELD                                            {5798,2,0}
#define GALLIFREY_V3A_REFE_LDO_MISC_TUNE_CM_ST_FIELD                                            {5796,2,0}
#define GALLIFREY_V3A_REFE_LDO_MISC_TUNE_IBIAS_ST_FIELD                                         {5794,2,0}
#define GALLIFREY_V3A_REFE_LDO_MISC_TUNE_ISTAB_ST_FIELD                                         {5792,2,0}
#define GALLIFREY_V3A_TX_POWER_FIELD                                                            {5808,4,0}
#define GALLIFREY_V3A_XO_FREQ_TRIM_ST_FIELD                                                     {5816,8,0}
#define GALLIFREY_V3A_XO_E_B_ST_FIELD                                                           {5848,1,0}
#define GALLIFREY_V3A_XO_REG_ST_FIELD                                                           {5824,24,0}
#define GALLIFREY_V3A_HAS_PROT_TIMER_FIELD                                                      {5863,1,0}
#define GALLIFREY_V3A_HAS_32BIT_FIFO_FIELD                                                      {5862,1,0}
#define GALLIFREY_V3A_HAS_AES_FIELD                                                             {5861,1,0}
#define GALLIFREY_V3A_HAS_DF_FIELD                                                              {5860,1,0}
#define GALLIFREY_V3A_HAS_RX_FIFO_FIELD                                                         {5859,1,0}
#define GALLIFREY_V3A_HAS_TX_FIFO_FIELD                                                         {5858,1,0}
#define GALLIFREY_V3A_HAS_APB_FIELD                                                             {5857,1,0}
#define GALLIFREY_V3A_HAS_XO_FIELD                                                              {5856,1,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_0_ATTEN_3_FIELD                                  {2231,5,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_0_ATTEN_2_FIELD                                  {2226,5,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_0_ATTEN_1_FIELD                                  {2221,5,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_0_ATTEN_0_FIELD                                  {2216,5,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_1_ATTEN_3_FIELD                                  {2231,5,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_1_ATTEN_2_FIELD                                  {2226,5,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_1_ATTEN_1_FIELD                                  {2221,5,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_1_ATTEN_0_FIELD                                  {2216,5,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_2_ATTEN_3_FIELD                                  {2231,5,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_2_ATTEN_2_FIELD                                  {2226,5,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_2_ATTEN_1_FIELD                                  {2221,5,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_2_ATTEN_0_FIELD                                  {2216,5,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_3_ATTEN_3_FIELD                                  {2231,5,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_3_ATTEN_2_FIELD                                  {2226,5,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_3_ATTEN_1_FIELD                                  {2221,5,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_3_ATTEN_0_FIELD                                  {2216,5,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_4_ATTEN_3_FIELD                                  {2231,5,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_4_ATTEN_2_FIELD                                  {2226,5,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_4_ATTEN_1_FIELD                                  {2221,5,0}
#define GALLIFREY_V3A_DIG_RX_TOP_AGC_ATTEN_LUT_4_ATTEN_0_FIELD                                  {2216,5,0}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_PKDET_BIAS_E_B_FIELD                                   {4698,1,1}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_FAST_IN_SST_B_FIELD                                    {4697,1,1}
#define GALLIFREY_V3A_ANA_TRX_BB_ANAFILT_DC_FSU_B_FIELD                                         {4696,1,1}
#define GALLIFREY_V3A_DIG_CHIP_ID_FIELD                                                         {8184,8,0}

#endif
/* C header for GALLIFREY_V3A_ADPLLDIG */

#ifndef __GALLIFREY_V3A_ADPLLDIG__
#define __GALLIFREY_V3A_ADPLLDIG__

#define GALLIFREY_V3A_ADPLLDIG_BASE                                         0xe00
#define GALLIFREY_V3A_ADPLLDIG_FREQUENCY_OFFSET                             0x00
#define GALLIFREY_V3A_ADPLLDIG_FREQUENCY_ADDR                               (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_FREQUENCY_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_FREQUENCY_SZ                                 24
#define GALLIFREY_V3A_ADPLLDIG_FREQUENCY                                    (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_FREQUENCY_ADDR
#define GALLIFREY_V3A_ADPLLDIG_CHANNEL_SPACING_OFFSET                       0x03
#define GALLIFREY_V3A_ADPLLDIG_CHANNEL_SPACING_ADDR                         (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_CHANNEL_SPACING_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_CHANNEL_SPACING_SZ                           16
#define GALLIFREY_V3A_ADPLLDIG_CHANNEL_SPACING                              (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_CHANNEL_SPACING_ADDR
#define GALLIFREY_V3A_ADPLLDIG_INTERMEDIATE_FREQUENCY_OFFSET                0x05
#define GALLIFREY_V3A_ADPLLDIG_INTERMEDIATE_FREQUENCY_ADDR                  (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_INTERMEDIATE_FREQUENCY_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_INTERMEDIATE_FREQUENCY_SZ                    32
#define GALLIFREY_V3A_ADPLLDIG_INTERMEDIATE_FREQUENCY                       (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_INTERMEDIATE_FREQUENCY_ADDR
#define GALLIFREY_V3A_ADPLLDIG_FREQ_CONFIG_OFFSET                           0x09
#define GALLIFREY_V3A_ADPLLDIG_FREQ_CONFIG_ADDR                             (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_FREQ_CONFIG_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_FREQ_CONFIG_SZ                               8
#define GALLIFREY_V3A_ADPLLDIG_FREQ_CONFIG                                  (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_FREQ_CONFIG_ADDR
#define GALLIFREY_V3A_ADPLLDIG_PARAMETERS_1_OFFSET                          0x0a
#define GALLIFREY_V3A_ADPLLDIG_PARAMETERS_1_ADDR                            (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_PARAMETERS_1_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_PARAMETERS_1_SZ                              32
#define GALLIFREY_V3A_ADPLLDIG_PARAMETERS_1                                 (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_PARAMETERS_1_ADDR
#define GALLIFREY_V3A_ADPLLDIG_PARAMETERS_2_OFFSET                          0x0e
#define GALLIFREY_V3A_ADPLLDIG_PARAMETERS_2_ADDR                            (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_PARAMETERS_2_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_PARAMETERS_2_SZ                              24
#define GALLIFREY_V3A_ADPLLDIG_PARAMETERS_2                                 (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_PARAMETERS_2_ADDR
#define GALLIFREY_V3A_ADPLLDIG_KDCO_PARAMS_OFFSET                           0x11
#define GALLIFREY_V3A_ADPLLDIG_KDCO_PARAMS_ADDR                             (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_KDCO_PARAMS_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_KDCO_PARAMS_SZ                               24
#define GALLIFREY_V3A_ADPLLDIG_KDCO_PARAMS                                  (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_KDCO_PARAMS_ADDR
#define GALLIFREY_V3A_ADPLLDIG_KDCOHF_OFFSET                                0x14
#define GALLIFREY_V3A_ADPLLDIG_KDCOHF_ADDR                                  (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_KDCOHF_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_KDCOHF_SZ                                    16
#define GALLIFREY_V3A_ADPLLDIG_KDCOHF                                       (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_KDCOHF_ADDR
#define GALLIFREY_V3A_ADPLLDIG_DTCOFFSET_OFFSET                             0x16
#define GALLIFREY_V3A_ADPLLDIG_DTCOFFSET_ADDR                               (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_DTCOFFSET_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_DTCOFFSET_SZ                                 8
#define GALLIFREY_V3A_ADPLLDIG_DTCOFFSET                                    (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_DTCOFFSET_ADDR
#define GALLIFREY_V3A_ADPLLDIG_KDTC_OFFSET                                  0x17
#define GALLIFREY_V3A_ADPLLDIG_KDTC_ADDR                                    (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_KDTC_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_KDTC_SZ                                      16
#define GALLIFREY_V3A_ADPLLDIG_KDTC                                         (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_KDTC_ADDR
#define GALLIFREY_V3A_ADPLLDIG_DTC_CONFIG_OFFSET                            0x19
#define GALLIFREY_V3A_ADPLLDIG_DTC_CONFIG_ADDR                              (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_DTC_CONFIG_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_DTC_CONFIG_SZ                                8
#define GALLIFREY_V3A_ADPLLDIG_DTC_CONFIG                                   (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_DTC_CONFIG_ADDR
#define GALLIFREY_V3A_ADPLLDIG_KTDC_CONFIG_OFFSET                           0x1a
#define GALLIFREY_V3A_ADPLLDIG_KTDC_CONFIG_ADDR                             (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_KTDC_CONFIG_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_KTDC_CONFIG_SZ                               16
#define GALLIFREY_V3A_ADPLLDIG_KTDC_CONFIG                                  (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_KTDC_CONFIG_ADDR
#define GALLIFREY_V3A_ADPLLDIG_DCO_CTRL_OFFSET                              0x1c
#define GALLIFREY_V3A_ADPLLDIG_DCO_CTRL_ADDR                                (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_DCO_CTRL_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_DCO_CTRL_SZ                                  16
#define GALLIFREY_V3A_ADPLLDIG_DCO_CTRL                                     (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_DCO_CTRL_ADDR
#define GALLIFREY_V3A_ADPLLDIG_DCO_AMP_OFFSET                               0x1e
#define GALLIFREY_V3A_ADPLLDIG_DCO_AMP_ADDR                                 (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_DCO_AMP_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_DCO_AMP_SZ                                   8
#define GALLIFREY_V3A_ADPLLDIG_DCO_AMP                                      (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_DCO_AMP_ADDR
#define GALLIFREY_V3A_ADPLLDIG_DCO_AMP_2_OFFSET                             0x1f
#define GALLIFREY_V3A_ADPLLDIG_DCO_AMP_2_ADDR                               (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_DCO_AMP_2_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_DCO_AMP_2_SZ                                 8
#define GALLIFREY_V3A_ADPLLDIG_DCO_AMP_2                                    (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_DCO_AMP_2_ADDR
#define GALLIFREY_V3A_ADPLLDIG_DCO_CAP_OFFSET                               0x20
#define GALLIFREY_V3A_ADPLLDIG_DCO_CAP_ADDR                                 (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_DCO_CAP_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_DCO_CAP_SZ                                   24
#define GALLIFREY_V3A_ADPLLDIG_DCO_CAP                                      (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_DCO_CAP_ADDR
#define GALLIFREY_V3A_ADPLLDIG_DCO_CAP_MOD_OFFSET                           0x23
#define GALLIFREY_V3A_ADPLLDIG_DCO_CAP_MOD_ADDR                             (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_DCO_CAP_MOD_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_DCO_CAP_MOD_SZ                               16
#define GALLIFREY_V3A_ADPLLDIG_DCO_CAP_MOD                                  (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_DCO_CAP_MOD_ADDR
#define GALLIFREY_V3A_ADPLLDIG_MEASUREMENT_OFFSET                           0x25
#define GALLIFREY_V3A_ADPLLDIG_MEASUREMENT_ADDR                             (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_MEASUREMENT_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_MEASUREMENT_SZ                               8
#define GALLIFREY_V3A_ADPLLDIG_MEASUREMENT                                  (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_MEASUREMENT_ADDR
#define GALLIFREY_V3A_ADPLLDIG_TIMING_OFFSET                                0x26
#define GALLIFREY_V3A_ADPLLDIG_TIMING_ADDR                                  (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_TIMING_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_TIMING_SZ                                    32
#define GALLIFREY_V3A_ADPLLDIG_TIMING                                       (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_TIMING_ADDR
#define GALLIFREY_V3A_ADPLLDIG_ANATSTEN_OFFSET                              0x2a
#define GALLIFREY_V3A_ADPLLDIG_ANATSTEN_ADDR                                (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_ANATSTEN_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_ANATSTEN_SZ                                  32
#define GALLIFREY_V3A_ADPLLDIG_ANATSTEN                                     (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_ANATSTEN_ADDR
#define GALLIFREY_V3A_ADPLLDIG_MODULATION_OFFSET                            0x2e
#define GALLIFREY_V3A_ADPLLDIG_MODULATION_ADDR                              (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_MODULATION_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_MODULATION_SZ                                16
#define GALLIFREY_V3A_ADPLLDIG_MODULATION                                   (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_MODULATION_ADDR
#define GALLIFREY_V3A_ADPLLDIG_DEBUG_OFFSET                                 0x30
#define GALLIFREY_V3A_ADPLLDIG_DEBUG_ADDR                                   (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_DEBUG_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_DEBUG_SZ                                     8
#define GALLIFREY_V3A_ADPLLDIG_DEBUG                                        (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_DEBUG_ADDR
#define GALLIFREY_V3A_ADPLLDIG_REGVALUES_OFFSET                             0x31
#define GALLIFREY_V3A_ADPLLDIG_REGVALUES_ADDR                               (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_REGVALUES_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_REGVALUES_SZ                                 16
#define GALLIFREY_V3A_ADPLLDIG_REGVALUES                                    (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_REGVALUES_ADDR
#define GALLIFREY_V3A_ADPLLDIG_CHANNEL_REG_OFFSET                           0x33
#define GALLIFREY_V3A_ADPLLDIG_CHANNEL_REG_ADDR                             (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_CHANNEL_REG_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_CHANNEL_REG_SZ                               8
#define GALLIFREY_V3A_ADPLLDIG_CHANNEL_REG                                  (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_CHANNEL_REG_ADDR
#define GALLIFREY_V3A_ADPLLDIG_CLOCKS_OFFSET                                0x34
#define GALLIFREY_V3A_ADPLLDIG_CLOCKS_ADDR                                  (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_CLOCKS_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_CLOCKS_SZ                                    16
#define GALLIFREY_V3A_ADPLLDIG_CLOCKS                                       (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_CLOCKS_ADDR
#define GALLIFREY_V3A_ADPLLDIG_TUNING_OFFSET                                0x36
#define GALLIFREY_V3A_ADPLLDIG_TUNING_ADDR                                  (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_TUNING_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_TUNING_SZ                                    8
#define GALLIFREY_V3A_ADPLLDIG_TUNING                                       (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_TUNING_ADDR
#define GALLIFREY_V3A_ADPLLDIG_OVERRIDES_OFFSET                             0x37
#define GALLIFREY_V3A_ADPLLDIG_OVERRIDES_ADDR                               (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_OVERRIDES_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_OVERRIDES_SZ                                 8
#define GALLIFREY_V3A_ADPLLDIG_OVERRIDES                                    (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_OVERRIDES_ADDR
#define GALLIFREY_V3A_ADPLLDIG_DTC_TDC_CONFIG_EXTRA_OFFSET                  0x38
#define GALLIFREY_V3A_ADPLLDIG_DTC_TDC_CONFIG_EXTRA_ADDR                    (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_DTC_TDC_CONFIG_EXTRA_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_DTC_TDC_CONFIG_EXTRA_SZ                      16
#define GALLIFREY_V3A_ADPLLDIG_DTC_TDC_CONFIG_EXTRA                         (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_DTC_TDC_CONFIG_EXTRA_ADDR
#define GALLIFREY_V3A_ADPLLDIG_TVPASETTLE_OFFSET                            0x3a
#define GALLIFREY_V3A_ADPLLDIG_TVPASETTLE_ADDR                              (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_TVPASETTLE_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_TVPASETTLE_SZ                                8
#define GALLIFREY_V3A_ADPLLDIG_TVPASETTLE                                   (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_TVPASETTLE_ADDR
#define GALLIFREY_V3A_ADPLLDIG_SYSTEM_OFFSET                                0x3b
#define GALLIFREY_V3A_ADPLLDIG_SYSTEM_ADDR                                  (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_SYSTEM_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_SYSTEM_SZ                                    8
#define GALLIFREY_V3A_ADPLLDIG_SYSTEM                                       (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_SYSTEM_ADDR
#define GALLIFREY_V3A_ADPLLDIG_DCO_COMP_LUT_0_OFFSET                        0x3c
#define GALLIFREY_V3A_ADPLLDIG_DCO_COMP_LUT_0_ADDR                          (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_DCO_COMP_LUT_0_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_DCO_COMP_LUT_0_SZ                            32
#define GALLIFREY_V3A_ADPLLDIG_DCO_COMP_LUT_0                               (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_DCO_COMP_LUT_0_ADDR
#define GALLIFREY_V3A_ADPLLDIG_DCO_COMP_LUT_1_OFFSET                        0x40
#define GALLIFREY_V3A_ADPLLDIG_DCO_COMP_LUT_1_ADDR                          (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_DCO_COMP_LUT_1_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_DCO_COMP_LUT_1_SZ                            32
#define GALLIFREY_V3A_ADPLLDIG_DCO_COMP_LUT_1                               (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_DCO_COMP_LUT_1_ADDR
#define GALLIFREY_V3A_ADPLLDIG_DCO_COMP_LUT_2_OFFSET                        0x44
#define GALLIFREY_V3A_ADPLLDIG_DCO_COMP_LUT_2_ADDR                          (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_DCO_COMP_LUT_2_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_DCO_COMP_LUT_2_SZ                            8
#define GALLIFREY_V3A_ADPLLDIG_DCO_COMP_LUT_2                               (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_DCO_COMP_LUT_2_ADDR
#define GALLIFREY_V3A_ADPLLDIG_KDCO_CALC_OFFSET                             0x45
#define GALLIFREY_V3A_ADPLLDIG_KDCO_CALC_ADDR                               (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_KDCO_CALC_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_KDCO_CALC_SZ                                 8
#define GALLIFREY_V3A_ADPLLDIG_KDCO_CALC                                    (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_KDCO_CALC_ADDR
#define GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_LUT_0_OFFSET                       0x46
#define GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_LUT_0_ADDR                         (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_LUT_0_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_LUT_0_SZ                           32
#define GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_LUT_0                              (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_LUT_0_ADDR
#define GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_LUT_1_OFFSET                       0x4a
#define GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_LUT_1_ADDR                         (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_LUT_1_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_LUT_1_SZ                           32
#define GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_LUT_1                              (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_LUT_1_ADDR
#define GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_LUT_2_OFFSET                       0x4e
#define GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_LUT_2_ADDR                         (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_LUT_2_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_LUT_2_SZ                           24
#define GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_LUT_2                              (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_LUT_2_ADDR
#define GALLIFREY_V3A_ADPLLDIG_KDTC_CALC_OFFSET                             0x51
#define GALLIFREY_V3A_ADPLLDIG_KDTC_CALC_ADDR                               (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_KDTC_CALC_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_KDTC_CALC_SZ                                 8
#define GALLIFREY_V3A_ADPLLDIG_KDTC_CALC                                    (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_KDTC_CALC_ADDR
#define GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_LUT_0_OFFSET                       0x52
#define GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_LUT_0_ADDR                         (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_LUT_0_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_LUT_0_SZ                           32
#define GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_LUT_0                              (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_LUT_0_ADDR
#define GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_LUT_1_OFFSET                       0x56
#define GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_LUT_1_ADDR                         (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_LUT_1_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_LUT_1_SZ                           32
#define GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_LUT_1                              (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_LUT_1_ADDR
#define GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_LUT_2_OFFSET                       0x5a
#define GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_LUT_2_ADDR                         (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_LUT_2_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_LUT_2_SZ                           24
#define GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_LUT_2                              (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_LUT_2_ADDR
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_DCO_OFFSET                         0x5d
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_DCO_ADDR                           (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_DCO_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_DCO_SZ                             8
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_DCO                                (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_DCO_ADDR
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_TDC_OFFSET                         0x5e
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_TDC_ADDR                           (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_TDC_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_TDC_SZ                             8
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_TDC                                (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_TDC_ADDR
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_DTC_OFFSET                         0x5f
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_DTC_ADDR                           (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_DTC_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_DTC_SZ                             8
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_DTC                                (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_DTC_ADDR
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_PMU_MISC_OFFSET                        0x60
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_PMU_MISC_ADDR                          (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_ADPLL_PMU_MISC_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_PMU_MISC_SZ                            8
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_PMU_MISC                               (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_ADPLL_PMU_MISC_ADDR
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_ENABLES_OFFSET                         0x61
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_ENABLES_ADDR                           (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_ADPLL_ENABLES_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_ENABLES_SZ                             16
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_ENABLES                                (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_ADPLL_ENABLES_ADDR
#define GALLIFREY_V3A_ADPLLDIG_KDCOHF_OUT_OFFSET                            0x63
#define GALLIFREY_V3A_ADPLLDIG_KDCOHF_OUT_ADDR                              (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_KDCOHF_OUT_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_KDCOHF_OUT_SZ                                16
#define GALLIFREY_V3A_ADPLLDIG_KDCOHF_OUT                                   (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_KDCOHF_OUT_ADDR
#define GALLIFREY_V3A_ADPLLDIG_KDTC_OUT_OFFSET                              0x65
#define GALLIFREY_V3A_ADPLLDIG_KDTC_OUT_ADDR                                (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_KDTC_OUT_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_KDTC_OUT_SZ                                  16
#define GALLIFREY_V3A_ADPLLDIG_KDTC_OUT                                     (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_KDTC_OUT_ADDR
#define GALLIFREY_V3A_ADPLLDIG_DCO_READOUT_OFFSET                           0x67
#define GALLIFREY_V3A_ADPLLDIG_DCO_READOUT_ADDR                             (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_DCO_READOUT_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_DCO_READOUT_SZ                               8
#define GALLIFREY_V3A_ADPLLDIG_DCO_READOUT                                  (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_DCO_READOUT_ADDR
#define GALLIFREY_V3A_ADPLLDIG_DCO_CAP_READOUT_OFFSET                       0x68
#define GALLIFREY_V3A_ADPLLDIG_DCO_CAP_READOUT_ADDR                         (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_DCO_CAP_READOUT_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_DCO_CAP_READOUT_SZ                           24
#define GALLIFREY_V3A_ADPLLDIG_DCO_CAP_READOUT                              (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_DCO_CAP_READOUT_ADDR
#define GALLIFREY_V3A_ADPLLDIG_DCO_MOD_CAP_READOUT_OFFSET                   0x6b
#define GALLIFREY_V3A_ADPLLDIG_DCO_MOD_CAP_READOUT_ADDR                     (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_DCO_MOD_CAP_READOUT_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_DCO_MOD_CAP_READOUT_SZ                       16
#define GALLIFREY_V3A_ADPLLDIG_DCO_MOD_CAP_READOUT                          (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_DCO_MOD_CAP_READOUT_ADDR
#define GALLIFREY_V3A_ADPLLDIG_FREQDIFF_OFFSET                              0x6d
#define GALLIFREY_V3A_ADPLLDIG_FREQDIFF_ADDR                                (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_FREQDIFF_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_FREQDIFF_SZ                                  24
#define GALLIFREY_V3A_ADPLLDIG_FREQDIFF                                     (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_FREQDIFF_ADDR
#define GALLIFREY_V3A_ADPLLDIG_STATUS_OFFSET                                0x70
#define GALLIFREY_V3A_ADPLLDIG_STATUS_ADDR                                  (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_STATUS_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_STATUS_SZ                                    16
#define GALLIFREY_V3A_ADPLLDIG_STATUS                                       (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_STATUS_ADDR
#define GALLIFREY_V3A_ADPLLDIG_VARIOUS_READOUT_OFFSET                       0x72
#define GALLIFREY_V3A_ADPLLDIG_VARIOUS_READOUT_ADDR                         (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_VARIOUS_READOUT_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_VARIOUS_READOUT_SZ                           8
#define GALLIFREY_V3A_ADPLLDIG_VARIOUS_READOUT                              (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_VARIOUS_READOUT_ADDR
#define GALLIFREY_V3A_ADPLLDIG_DEBUG_READOUT_OFFSET                         0x73
#define GALLIFREY_V3A_ADPLLDIG_DEBUG_READOUT_ADDR                           (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_DEBUG_READOUT_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_DEBUG_READOUT_SZ                             8
#define GALLIFREY_V3A_ADPLLDIG_DEBUG_READOUT                                (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_DEBUG_READOUT_ADDR
#define GALLIFREY_V3A_ADPLLDIG_PLLFCWDT_OFFSET                              0x74
#define GALLIFREY_V3A_ADPLLDIG_PLLFCWDT_ADDR                                (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_PLLFCWDT_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_PLLFCWDT_SZ                                  24
#define GALLIFREY_V3A_ADPLLDIG_PLLFCWDT                                     (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_PLLFCWDT_ADDR
#define GALLIFREY_V3A_ADPLLDIG_ANATSTEN_IN_OFFSET                           0x77
#define GALLIFREY_V3A_ADPLLDIG_ANATSTEN_IN_ADDR                             (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_ANATSTEN_IN_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_ANATSTEN_IN_SZ                               16
#define GALLIFREY_V3A_ADPLLDIG_ANATSTEN_IN                                  (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_ANATSTEN_IN_ADDR
#define GALLIFREY_V3A_ADPLLDIG_QUALMON_OFFSET                               0x25
#define GALLIFREY_V3A_ADPLLDIG_QUALMON_ADDR                                 (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_QUALMON_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_QUALMON_SZ                                   16
#define GALLIFREY_V3A_ADPLLDIG_QUALMON                                      (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_QUALMON_ADDR
#define GALLIFREY_V3A_ADPLLDIG_CHIP_ID_OFFSET                               0x7f
#define GALLIFREY_V3A_ADPLLDIG_CHIP_ID_ADDR                                 (GALLIFREY_V3A_ADPLLDIG_BASE + GALLIFREY_V3A_ADPLLDIG_CHIP_ID_OFFSET)
#define GALLIFREY_V3A_ADPLLDIG_CHIP_ID_SZ                                   8
#define GALLIFREY_V3A_ADPLLDIG_CHIP_ID                                      (volatile uint8_t *)GALLIFREY_V3A_ADPLLDIG_CHIP_ID_ADDR


#define GALLIFREY_V3A_ADPLLDIG_CH0_FIELD                                 {28672,23,0}
#define GALLIFREY_V3A_ADPLLDIG_CHANNEL_SPACE_FIELD                       {28696,16,0}
#define GALLIFREY_V3A_ADPLLDIG_FIFTX_FIELD                               {28728,12,0}
#define GALLIFREY_V3A_ADPLLDIG_FIFRX_FIELD                               {28712,12,0}
#define GALLIFREY_V3A_ADPLLDIG_DBL_CS_FIELD                              {28745,1,0}
#define GALLIFREY_V3A_ADPLLDIG_DBL_FIF_FIELD                             {28744,1,0}
#define GALLIFREY_V3A_ADPLLDIG_MODTAU_TUNE_FIELD                         {28776,6,0}
#define GALLIFREY_V3A_ADPLLDIG_MODK_FIELD                                {28770,6,0}
#define GALLIFREY_V3A_ADPLLDIG_FINEKZ_FIELD                              {28764,6,0}
#define GALLIFREY_V3A_ADPLLDIG_MODTAU_FIELD                              {28758,6,0}
#define GALLIFREY_V3A_ADPLLDIG_MODKZ_FIELD                               {28752,6,0}
#define GALLIFREY_V3A_ADPLLDIG_KMEDIUM_FIELD                             {28803,3,0}
#define GALLIFREY_V3A_ADPLLDIG_KCOARSE_FIELD                             {28800,3,0}
#define GALLIFREY_V3A_ADPLLDIG_FINEK_FIELD                               {28795,5,0}
#define GALLIFREY_V3A_ADPLLDIG_FINETAU_FIELD                             {28790,5,0}
#define GALLIFREY_V3A_ADPLLDIG_MODK_TUNE_FIELD                           {28784,6,0}
#define GALLIFREY_V3A_ADPLLDIG_MASK_DCOMOD_LSB_FIELD                     {28830,2,0}
#define GALLIFREY_V3A_ADPLLDIG_UPDATE_KDCOHF_IC_FIELD                    {28829,1,0}
#define GALLIFREY_V3A_ADPLLDIG_KDCODEV_DTC_FIELD                         {28828,1,0}
#define GALLIFREY_V3A_ADPLLDIG_KDCODEV_TUNE_FIELD                        {28827,1,0}
#define GALLIFREY_V3A_ADPLLDIG_KDCOCAL_POS_NEG_FIELD                     {28826,1,0}
#define GALLIFREY_V3A_ADPLLDIG_KDCOESTDEV_FIELD                          {28824,2,0}
#define GALLIFREY_V3A_ADPLLDIG_KDCOLFCALEN_FIELD                         {28822,1,0}
#define GALLIFREY_V3A_ADPLLDIG_KDCOCALRX_FIELD                           {28821,1,0}
#define GALLIFREY_V3A_ADPLLDIG_KDCOCALTX_FIELD                           {28820,1,0}
#define GALLIFREY_V3A_ADPLLDIG_TKDCOCAL_FIELD                            {28817,3,0}
#define GALLIFREY_V3A_ADPLLDIG_KDCOLF_IN_FIELD                           {28808,9,0}
#define GALLIFREY_V3A_ADPLLDIG_KDCOHFIC_FIELD                            {28832,9,0}
#define GALLIFREY_V3A_ADPLLDIG_DTCOFFSET_FIELD                           {28848,7,0}
#define GALLIFREY_V3A_ADPLLDIG_KDTCIC_FIELD                              {28856,9,0}
#define GALLIFREY_V3A_ADPLLDIG_UPDATE_KDTC_IC_FIELD                      {28878,1,0}
#define GALLIFREY_V3A_ADPLLDIG_KDTCCAL_INV_FIELD                         {28877,1,0}
#define GALLIFREY_V3A_ADPLLDIG_KDTCCALMOD_FIELD                          {28876,1,0}
#define GALLIFREY_V3A_ADPLLDIG_KDTCCALEN_FIELD                           {28875,1,0}
#define GALLIFREY_V3A_ADPLLDIG_KDTCCALLG_FIELD                           {28872,3,0}
#define GALLIFREY_V3A_ADPLLDIG_MODDLY_FIELD                              {28894,2,0}
#define GALLIFREY_V3A_ADPLLDIG_ENFCWMOD_FIELD                            {28893,1,0}
#define GALLIFREY_V3A_ADPLLDIG_RESDLY_FIELD                              {28891,2,0}
#define GALLIFREY_V3A_ADPLLDIG_ENRESIDUE_FIELD                           {28890,1,0}
#define GALLIFREY_V3A_ADPLLDIG_KTDCCALEN_FIELD                           {28889,1,0}
#define GALLIFREY_V3A_ADPLLDIG_KTDC_IN_FIELD                             {28880,9,0}
#define GALLIFREY_V3A_ADPLLDIG_DCO_PTAT_OSC_SEL_FIELD                    {28907,2,0}
#define GALLIFREY_V3A_ADPLLDIG_DCO_FAST_START_EB_FIELD                   {28906,1,0}
#define GALLIFREY_V3A_ADPLLDIG_DCO_BIAS_PTAT_E_FIELD                     {28905,1,0}
#define GALLIFREY_V3A_ADPLLDIG_DCO_CORE_CM_EB_FIELD                      {28904,1,0}
#define GALLIFREY_V3A_ADPLLDIG_DCOAMPTM_FIELD                            {28903,1,0}
#define GALLIFREY_V3A_ADPLLDIG_AMPCALEN_FIELD                            {28902,1,0}
#define GALLIFREY_V3A_ADPLLDIG_SDMMODETX_FIELD                           {28899,3,0}
#define GALLIFREY_V3A_ADPLLDIG_SDMMODERX_FIELD                           {28896,3,0}
#define GALLIFREY_V3A_ADPLLDIG_DCO_BIAS_LEVEL_CTRL_FIELD                 {28916,2,0}
#define GALLIFREY_V3A_ADPLLDIG_DCO_AMP_FIELD                             {28912,4,0}
#define GALLIFREY_V3A_ADPLLDIG_DCO_AMP_DET_INT_ALGO_FIELD                {28927,1,0}
#define GALLIFREY_V3A_ADPLLDIG_DCO_AMP_DET_CHG_LSB_FIELD                 {28926,1,0}
#define GALLIFREY_V3A_ADPLLDIG_DCO_AMP_DET_ANA_CTRL_E_FIELD              {28925,1,0}
#define GALLIFREY_V3A_ADPLLDIG_DCO_AMP_DET_E_FIELD                       {28924,1,0}
#define GALLIFREY_V3A_ADPLLDIG_DCO_AMP_DET_SEL_FIELD                     {28920,4,0}
#define GALLIFREY_V3A_ADPLLDIG_DCOCOARSEIC_FIELD                         {28944,5,0}
#define GALLIFREY_V3A_ADPLLDIG_DCOMEDIUMIC_FIELD                         {28936,3,0}
#define GALLIFREY_V3A_ADPLLDIG_DCOFINEIC_FIELD                           {28928,6,0}
#define GALLIFREY_V3A_ADPLLDIG_DCOMODIC_FIELD                            {28952,14,0}
#define GALLIFREY_V3A_ADPLLDIG_TMREN_FIELD                               {28974,1,0}
#define GALLIFREY_V3A_ADPLLDIG_ENRFMEAS_FIELD                            {28973,1,0}
#define GALLIFREY_V3A_ADPLLDIG_TFREQMEAS_FIELD                           {28968,5,0}
#define GALLIFREY_V3A_ADPLLDIG_TMOD_FIELD                                {29004,4,0}
#define GALLIFREY_V3A_ADPLLDIG_TFINE_FIELD                               {29000,4,0}
#define GALLIFREY_V3A_ADPLLDIG_TMEDIUM_FIELD                             {28996,4,0}
#define GALLIFREY_V3A_ADPLLDIG_TCOARSE_FIELD                             {28992,4,0}
#define GALLIFREY_V3A_ADPLLDIG_TSETTLE_FIELD                             {28988,4,0}
#define GALLIFREY_V3A_ADPLLDIG_TKDTCCAL_FIELD                            {28984,4,0}
#define GALLIFREY_V3A_ADPLLDIG_TPASETTLE_FIELD                           {28980,4,0}
#define GALLIFREY_V3A_ADPLLDIG_HOLD_STATE_FIELD                          {28976,4,0}
#define GALLIFREY_V3A_ADPLLDIG_ANATSTSPARE_FIELD                         {29024,16,0}
#define GALLIFREY_V3A_ADPLLDIG_ANATSTEN_FIELD                            {29008,16,0}
#define GALLIFREY_V3A_ADPLLDIG_MOD_RFP_4_EL_LUT_FIELD                    {29053,1,0}
#define GALLIFREY_V3A_ADPLLDIG_MOD_RFP_COMP_DEL_FIELD                    {29050,3,0}
#define GALLIFREY_V3A_ADPLLDIG_MOD_COMPENSATE_RFP_FIELD                  {29049,1,0}
#define GALLIFREY_V3A_ADPLLDIG_MOD_START_W_TUNED_FIELD                   {29048,1,0}
#define GALLIFREY_V3A_ADPLLDIG_MOD_BACK_IDLE_FIELD                       {29047,1,0}
#define GALLIFREY_V3A_ADPLLDIG_MOD_ZERO_POST_FIELD                       {29046,1,0}
#define GALLIFREY_V3A_ADPLLDIG_MOD_ZERO_PRE_FIELD                        {29045,1,0}
#define GALLIFREY_V3A_ADPLLDIG_MOD_PULL_CNT_FIELD                        {29040,5,0}
#define GALLIFREY_V3A_ADPLLDIG_TGLDETEN_FIELD                            {29057,1,0}
#define GALLIFREY_V3A_ADPLLDIG_FBDIVEN_FIELD                             {29056,1,0}
#define GALLIFREY_V3A_ADPLLDIG_RXBIT_REG_FIELD                           {29072,1,0}
#define GALLIFREY_V3A_ADPLLDIG_RXDIVEN_REG_FIELD                         {29071,1,0}
#define GALLIFREY_V3A_ADPLLDIG_TXDIVEN_REG_FIELD                         {29070,1,0}
#define GALLIFREY_V3A_ADPLLDIG_VPAEN_REG_FIELD                           {29069,1,0}
#define GALLIFREY_V3A_ADPLLDIG_ACTIVE_REG_FIELD                          {29068,1,0}
#define GALLIFREY_V3A_ADPLLDIG_DCOAMPHOLD_REG_FIELD                      {29067,1,0}
#define GALLIFREY_V3A_ADPLLDIG_DCOAMPDETEN_REG_FIELD                     {29066,1,0}
#define GALLIFREY_V3A_ADPLLDIG_ENPAIN_REG_FIELD                          {29065,1,0}
#define GALLIFREY_V3A_ADPLLDIG_SRESET_GAUSS_N_REG_FIELD                  {29064,1,0}
#define GALLIFREY_V3A_ADPLLDIG_CHANNEL_REG_FIELD                         {29080,7,0}
#define GALLIFREY_V3A_ADPLLDIG_TDC_INV_FIELD                             {29103,1,0}
#define GALLIFREY_V3A_ADPLLDIG_INV_CKTDC_FIELD                           {29102,1,0}
#define GALLIFREY_V3A_ADPLLDIG_INV_CKPHV_FIELD                           {29101,1,0}
#define GALLIFREY_V3A_ADPLLDIG_EN_CKDCOMOD_FIELD                         {29100,1,0}
#define GALLIFREY_V3A_ADPLLDIG_INV_CKDCOMOD_FIELD                        {29099,1,0}
#define GALLIFREY_V3A_ADPLLDIG_DLYFCWDT_FIELD                            {29097,2,0}
#define GALLIFREY_V3A_ADPLLDIG_INV_CKDMA_FIELD                           {29096,1,0}
#define GALLIFREY_V3A_ADPLLDIG_DMA_ASYNC_FIELD                           {29095,1,0}
#define GALLIFREY_V3A_ADPLLDIG_DMA_RATE_FIELD                            {29093,2,0}
#define GALLIFREY_V3A_ADPLLDIG_DMA_MUX_FIELD                             {29088,5,0}
#define GALLIFREY_V3A_ADPLLDIG_SKIP_PA_STATE_FIELD                       {29106,1,0}
#define GALLIFREY_V3A_ADPLLDIG_PHRDLY_FIELD                              {29104,2,0}
#define GALLIFREY_V3A_ADPLLDIG_OVRDEF_SRESET_GAUSS_N_FIELD               {29119,1,0}
#define GALLIFREY_V3A_ADPLLDIG_OVRDEF_VPAEN_FIELD                        {29118,1,0}
#define GALLIFREY_V3A_ADPLLDIG_OVRDEF_AMPHOLD_FIELD                      {29117,1,0}
#define GALLIFREY_V3A_ADPLLDIG_OVRDEF_ENPAIN_FIELD                       {29116,1,0}
#define GALLIFREY_V3A_ADPLLDIG_OVRDEF_DCOAMPDETEN_FIELD                  {29115,1,0}
#define GALLIFREY_V3A_ADPLLDIG_OVRDEF_TXDIVEN_FIELD                      {29114,1,0}
#define GALLIFREY_V3A_ADPLLDIG_OVRDEF_RXDIVEN_FIELD                      {29113,1,0}
#define GALLIFREY_V3A_ADPLLDIG_OVRDEF_FIELD                              {29112,1,0}
#define GALLIFREY_V3A_ADPLLDIG_RST_TAU_EN_FIELD                          {29134,1,0}
#define GALLIFREY_V3A_ADPLLDIG_TDC_OFFSET_FIELD                          {29128,6,0}
#define GALLIFREY_V3A_ADPLLDIG_KDTCCALMOD1PH_FIELD                       {29124,1,0}
#define GALLIFREY_V3A_ADPLLDIG_VPASETTLE_FIELD                           {29122,2,0}
#define GALLIFREY_V3A_ADPLLDIG_DTC_LDO_DMY_FIELD                         {29120,2,0}
#define GALLIFREY_V3A_ADPLLDIG_TVPASETTLE_FIELD                          {29136,6,0}
#define GALLIFREY_V3A_ADPLLDIG_SINGLE_CAL_FIELD                          {29144,1,0}
#define GALLIFREY_V3A_ADPLLDIG_DCO_COMP_3_FIELD                          {29176,8,0}
#define GALLIFREY_V3A_ADPLLDIG_DCO_COMP_2_FIELD                          {29168,8,0}
#define GALLIFREY_V3A_ADPLLDIG_DCO_COMP_1_FIELD                          {29160,8,0}
#define GALLIFREY_V3A_ADPLLDIG_DCO_COMP_0_FIELD                          {29152,8,0}
#define GALLIFREY_V3A_ADPLLDIG_DCO_COMP_7_FIELD                          {29208,8,0}
#define GALLIFREY_V3A_ADPLLDIG_DCO_COMP_6_FIELD                          {29200,8,0}
#define GALLIFREY_V3A_ADPLLDIG_DCO_COMP_5_FIELD                          {29192,8,0}
#define GALLIFREY_V3A_ADPLLDIG_DCO_COMP_4_FIELD                          {29184,8,0}
#define GALLIFREY_V3A_ADPLLDIG_DCO_COMP_8_FIELD                          {29216,8,0}
#define GALLIFREY_V3A_ADPLLDIG_KDCO_LUT_DBL_FIELD                        {29225,1,0}
#define GALLIFREY_V3A_ADPLLDIG_KDCO_LUT_E_FIELD                          {29224,1,0}
#define GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_3_FIELD                         {29256,5,0}
#define GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_2_FIELD                         {29248,5,0}
#define GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_1_FIELD                         {29240,5,0}
#define GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_0_FIELD                         {29232,5,0}
#define GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_7_FIELD                         {29288,5,0}
#define GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_6_FIELD                         {29280,5,0}
#define GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_5_FIELD                         {29272,5,0}
#define GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_4_FIELD                         {29264,5,0}
#define GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_10_FIELD                        {29312,5,0}
#define GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_9_FIELD                         {29304,5,0}
#define GALLIFREY_V3A_ADPLLDIG_KDCO_COMP_8_FIELD                         {29296,5,0}
#define GALLIFREY_V3A_ADPLLDIG_KDTC_LUT_DBL_FIELD                        {29321,1,0}
#define GALLIFREY_V3A_ADPLLDIG_KDTC_LUT_E_FIELD                          {29320,1,0}
#define GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_3_FIELD                         {29352,5,0}
#define GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_2_FIELD                         {29344,5,0}
#define GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_1_FIELD                         {29336,5,0}
#define GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_0_FIELD                         {29328,5,0}
#define GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_7_FIELD                         {29384,5,0}
#define GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_6_FIELD                         {29376,5,0}
#define GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_5_FIELD                         {29368,5,0}
#define GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_4_FIELD                         {29360,5,0}
#define GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_10_FIELD                        {29408,5,0}
#define GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_9_FIELD                         {29400,5,0}
#define GALLIFREY_V3A_ADPLLDIG_KDTC_COMP_8_FIELD                         {29392,5,0}
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_DCO_TUNE_RM_FIELD               {29422,2,0}
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_DCO_TUNE_CM_FIELD               {29420,2,0}
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_DCO_TUNE_IBIAS_FIELD            {29418,2,0}
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_DCO_TUNE_ISTAB_FIELD            {29416,2,0}
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_TDC_TUNE_RM_FIELD               {29430,2,0}
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_TDC_TUNE_CM_FIELD               {29428,2,0}
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_TDC_TUNE_IBIAS_FIELD            {29426,2,0}
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_TDC_TUNE_ISTAB_FIELD            {29424,2,0}
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_DTC_TUNE_RM_FIELD               {29438,2,0}
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_DTC_TUNE_CM_FIELD               {29436,2,0}
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_DTC_TUNE_IBIAS_FIELD            {29434,2,0}
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_DTC_TUNE_ISTAB_FIELD            {29432,2,0}
#define GALLIFREY_V3A_ADPLLDIG_DCO_DD4_TUNE_IBIAS_FIELD                  {29444,4,0}
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_VREF_TUNE_FIELD                 {29440,3,0}
#define GALLIFREY_V3A_ADPLLDIG_TDC_CKVIN_EN_FIELD                        {29457,1,0}
#define GALLIFREY_V3A_ADPLLDIG_TDC_DTCIN_EN_FIELD                        {29456,1,0}
#define GALLIFREY_V3A_ADPLLDIG_ISOLATE_B_FIELD                           {29455,1,0}
#define GALLIFREY_V3A_ADPLLDIG_DTC_EN_FIELD                              {29454,1,0}
#define GALLIFREY_V3A_ADPLLDIG_DCO_BIAS_E_FIELD                          {29453,1,0}
#define GALLIFREY_V3A_ADPLLDIG_DCO_CORE_E_FIELD                          {29452,1,0}
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_VREF_E_FIELD                    {29451,1,0}
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_DTC_E_FIELD                     {29450,1,0}
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_TDC_E_FIELD                     {29449,1,0}
#define GALLIFREY_V3A_ADPLLDIG_ADPLL_LDO_DCO_E_FIELD                     {29448,1,0}
#define GALLIFREY_V3A_ADPLLDIG_KDCO_HF_FIELD                             {29464,9,0}
#define GALLIFREY_V3A_ADPLLDIG_KDTC_OUT_FIELD                            {29480,9,0}
#define GALLIFREY_V3A_ADPLLDIG_DCOAMPDET_ASYNC_FIELD                     {29500,1,0}
#define GALLIFREY_V3A_ADPLLDIG_DCOAMP_FIELD                              {29496,4,0}
#define GALLIFREY_V3A_ADPLLDIG_DCOCOARSE_FIELD                           {29520,5,0}
#define GALLIFREY_V3A_ADPLLDIG_DCOMEDIUM_FIELD                           {29512,3,0}
#define GALLIFREY_V3A_ADPLLDIG_DCOFINE_FIELD                             {29504,6,0}
#define GALLIFREY_V3A_ADPLLDIG_DCOMOD_FIELD                              {29528,14,0}
#define GALLIFREY_V3A_ADPLLDIG_MEASDONE_OUT_FIELD                        {29567,1,0}
#define GALLIFREY_V3A_ADPLLDIG_FREQDIFF_FIELD                            {29544,23,0}
#define GALLIFREY_V3A_ADPLLDIG_TUNE_STATE_FIELD                          {29580,4,0}
#define GALLIFREY_V3A_ADPLLDIG_CAL_KDCOCAL_FIELD                         {29579,1,0}
#define GALLIFREY_V3A_ADPLLDIG_CAL_KDTCCAL_FIELD                         {29578,1,0}
#define GALLIFREY_V3A_ADPLLDIG_TMRVAL_FIELD                              {29568,10,0}
#define GALLIFREY_V3A_ADPLLDIG_QUALMONDET_FIELD                          {29584,1,0}
#define GALLIFREY_V3A_ADPLLDIG_DCO_STARTED_FIELD                         {29598,1,0}
#define GALLIFREY_V3A_ADPLLDIG_DCO_AMPL_FIELD                            {29595,3,0}
#define GALLIFREY_V3A_ADPLLDIG_TDCBUB_FIELD                              {29594,1,0}
#define GALLIFREY_V3A_ADPLLDIG_PHVSA0_FIELD                              {29593,1,0}
#define GALLIFREY_V3A_ADPLLDIG_PHVSA1_FIELD                              {29592,1,0}
#define GALLIFREY_V3A_ADPLLDIG_PLLFCWDT_FIELD                            {29600,23,0}
#define GALLIFREY_V3A_ADPLLDIG_ANATSTSPARE_IN_FIELD                      {29624,16,0}
#define GALLIFREY_V3A_ADPLLDIG_QUALMONMOD_FIELD                          {28981,2,0}
#define GALLIFREY_V3A_ADPLLDIG_QUALMONTRHLD_FIELD                        {28975,6,0}
#define GALLIFREY_V3A_ADPLLDIG_QUALMONFRCEN_FIELD                        {28974,1,1}
#define GALLIFREY_V3A_ADPLLDIG_QUALMONWND_FIELD                          {28968,6,0}
#define GALLIFREY_V3A_ADPLLDIG_CHIP_ID_FIELD                             {29688,8,0}

#endif
