/* C header for GALLIFREY_T22_V1A */

#ifndef __GALLIFREY_T22_V1A__
#define __GALLIFREY_T22_V1A__

#define GALLIFREY_T22_V1A_BASE                                                                               0x0
#define GALLIFREY_T22_V1A_DIG_CUSTOM_IFACE_OFFSET                                                            0x00
#define GALLIFREY_T22_V1A_DIG_CUSTOM_IFACE_ADDR                                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_CUSTOM_IFACE_OFFSET)
#define GALLIFREY_T22_V1A_DIG_CUSTOM_IFACE_SZ                                                                8
#define GALLIFREY_T22_V1A_DIG_CUSTOM_IFACE                                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_CUSTOM_IFACE_ADDR
#define GALLIFREY_T22_V1A_DIG_IFACE_OFFSET                                                                   0x01
#define GALLIFREY_T22_V1A_DIG_IFACE_ADDR                                                                     (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_IFACE_OFFSET)
#define GALLIFREY_T22_V1A_DIG_IFACE_SZ                                                                       8
#define GALLIFREY_T22_V1A_DIG_IFACE                                                                          (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_IFACE_ADDR
#define GALLIFREY_T22_V1A_DIG_IFACE_PASSWD_OFFSET                                                            0x02
#define GALLIFREY_T22_V1A_DIG_IFACE_PASSWD_ADDR                                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_IFACE_PASSWD_OFFSET)
#define GALLIFREY_T22_V1A_DIG_IFACE_PASSWD_SZ                                                                16
#define GALLIFREY_T22_V1A_DIG_IFACE_PASSWD                                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_IFACE_PASSWD_ADDR
#define GALLIFREY_T22_V1A_DIG_IFACE_OCTA_SPI_SLV_OFFSET                                                      0x04
#define GALLIFREY_T22_V1A_DIG_IFACE_OCTA_SPI_SLV_ADDR                                                        (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_IFACE_OCTA_SPI_SLV_OFFSET)
#define GALLIFREY_T22_V1A_DIG_IFACE_OCTA_SPI_SLV_SZ                                                          8
#define GALLIFREY_T22_V1A_DIG_IFACE_OCTA_SPI_SLV                                                             (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_IFACE_OCTA_SPI_SLV_ADDR
#define GALLIFREY_T22_V1A_DIG_GPIOS_OFFSET                                                                   0x05
#define GALLIFREY_T22_V1A_DIG_GPIOS_ADDR                                                                     (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_GPIOS_OFFSET)
#define GALLIFREY_T22_V1A_DIG_GPIOS_SZ                                                                       8
#define GALLIFREY_T22_V1A_DIG_GPIOS                                                                          (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_GPIOS_ADDR
#define GALLIFREY_T22_V1A_DIG_GPIOS_PD_OFFSET                                                                0x06
#define GALLIFREY_T22_V1A_DIG_GPIOS_PD_ADDR                                                                  (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_GPIOS_PD_OFFSET)
#define GALLIFREY_T22_V1A_DIG_GPIOS_PD_SZ                                                                    16
#define GALLIFREY_T22_V1A_DIG_GPIOS_PD                                                                       (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_GPIOS_PD_ADDR
#define GALLIFREY_T22_V1A_DIG_GPIOS_PU_OFFSET                                                                0x08
#define GALLIFREY_T22_V1A_DIG_GPIOS_PU_ADDR                                                                  (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_GPIOS_PU_OFFSET)
#define GALLIFREY_T22_V1A_DIG_GPIOS_PU_SZ                                                                    16
#define GALLIFREY_T22_V1A_DIG_GPIOS_PU                                                                       (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_GPIOS_PU_ADDR
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_0_OFFSET                                                            0x0a
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_0_ADDR                                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_0_OFFSET)
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_0_SZ                                                                8
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_0                                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_0_ADDR
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_1_OFFSET                                                            0x0b
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_1_ADDR                                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_1_OFFSET)
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_1_SZ                                                                8
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_1                                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_1_ADDR
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_2_OFFSET                                                            0x0c
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_2_ADDR                                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_2_OFFSET)
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_2_SZ                                                                8
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_2                                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_2_ADDR
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_3_OFFSET                                                            0x0d
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_3_ADDR                                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_3_OFFSET)
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_3_SZ                                                                8
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_3                                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_3_ADDR
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_4_OFFSET                                                            0x0e
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_4_ADDR                                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_4_OFFSET)
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_4_SZ                                                                8
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_4                                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_4_ADDR
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_5_OFFSET                                                            0x0f
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_5_ADDR                                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_5_OFFSET)
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_5_SZ                                                                8
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_5                                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_5_ADDR
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_6_OFFSET                                                            0x10
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_6_ADDR                                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_6_OFFSET)
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_6_SZ                                                                8
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_6                                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_6_ADDR
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_7_OFFSET                                                            0x11
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_7_ADDR                                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_7_OFFSET)
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_7_SZ                                                                8
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_7                                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_7_ADDR
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_8_OFFSET                                                            0x12
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_8_ADDR                                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_8_OFFSET)
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_8_SZ                                                                8
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_8                                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_8_ADDR
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_9_OFFSET                                                            0x13
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_9_ADDR                                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_9_OFFSET)
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_9_SZ                                                                8
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_9                                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_9_ADDR
#define GALLIFREY_T22_V1A_DIG_GPIOS_TEST_BUS_OFFSET                                                          0x14
#define GALLIFREY_T22_V1A_DIG_GPIOS_TEST_BUS_ADDR                                                            (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_GPIOS_TEST_BUS_OFFSET)
#define GALLIFREY_T22_V1A_DIG_GPIOS_TEST_BUS_SZ                                                              8
#define GALLIFREY_T22_V1A_DIG_GPIOS_TEST_BUS                                                                 (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_GPIOS_TEST_BUS_ADDR
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_OFFSET                                                                0x15
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_ADDR                                                                  (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_IRQ_CTRL_OFFSET)
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_SZ                                                                    8
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL                                                                       (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_IRQ_CTRL_ADDR
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_TX_OFFSET                                                             0x16
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_TX_ADDR                                                               (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_IRQ_CTRL_TX_OFFSET)
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_TX_SZ                                                                 8
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_TX                                                                    (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_IRQ_CTRL_TX_ADDR
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_TX_PH_OFFSET                                                          0x17
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_TX_PH_ADDR                                                            (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_IRQ_CTRL_TX_PH_OFFSET)
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_TX_PH_SZ                                                              8
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_TX_PH                                                                 (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_IRQ_CTRL_TX_PH_ADDR
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_RX_OFFSET                                                             0x18
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_RX_ADDR                                                               (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_IRQ_CTRL_RX_OFFSET)
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_RX_SZ                                                                 8
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_RX                                                                    (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_IRQ_CTRL_RX_ADDR
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_RX_PH_OFFSET                                                          0x19
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_RX_PH_ADDR                                                            (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_IRQ_CTRL_RX_PH_OFFSET)
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_RX_PH_SZ                                                              8
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_RX_PH                                                                 (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_IRQ_CTRL_RX_PH_ADDR
#define GALLIFREY_T22_V1A_DIG_SEQ_CFG_OFFSET                                                                 0x1a
#define GALLIFREY_T22_V1A_DIG_SEQ_CFG_ADDR                                                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_SEQ_CFG_OFFSET)
#define GALLIFREY_T22_V1A_DIG_SEQ_CFG_SZ                                                                     8
#define GALLIFREY_T22_V1A_DIG_SEQ_CFG                                                                        (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_SEQ_CFG_ADDR
#define GALLIFREY_T22_V1A_DIG_SEQ_OFFSET                                                                     0x1b
#define GALLIFREY_T22_V1A_DIG_SEQ_ADDR                                                                       (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_SEQ_OFFSET)
#define GALLIFREY_T22_V1A_DIG_SEQ_SZ                                                                         8
#define GALLIFREY_T22_V1A_DIG_SEQ                                                                            (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_SEQ_ADDR
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_0_OFFSET                                                              0x1c
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_0_ADDR                                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_SEQ_SEQS_0_OFFSET)
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_0_SZ                                                                  16
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_0                                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_SEQ_SEQS_0_ADDR
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_1_OFFSET                                                              0x1e
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_1_ADDR                                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_SEQ_SEQS_1_OFFSET)
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_1_SZ                                                                  16
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_1                                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_SEQ_SEQS_1_ADDR
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_2_OFFSET                                                              0x20
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_2_ADDR                                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_SEQ_SEQS_2_OFFSET)
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_2_SZ                                                                  16
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_2                                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_SEQ_SEQS_2_ADDR
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_3_OFFSET                                                              0x22
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_3_ADDR                                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_SEQ_SEQS_3_OFFSET)
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_3_SZ                                                                  16
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_3                                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_SEQ_SEQS_3_ADDR
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_4_OFFSET                                                              0x24
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_4_ADDR                                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_SEQ_SEQS_4_OFFSET)
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_4_SZ                                                                  16
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_4                                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_SEQ_SEQS_4_ADDR
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_5_OFFSET                                                              0x26
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_5_ADDR                                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_SEQ_SEQS_5_OFFSET)
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_5_SZ                                                                  16
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_5                                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_SEQ_SEQS_5_ADDR
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_6_OFFSET                                                              0x28
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_6_ADDR                                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_SEQ_SEQS_6_OFFSET)
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_6_SZ                                                                  16
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_6                                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_SEQ_SEQS_6_ADDR
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_7_OFFSET                                                              0x2a
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_7_ADDR                                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_SEQ_SEQS_7_OFFSET)
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_7_SZ                                                                  16
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_7                                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_SEQ_SEQS_7_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_OFFSET                                                                  0x2c
#define GALLIFREY_T22_V1A_DIG_TX_TOP_ADDR                                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_SZ                                                                      8
#define GALLIFREY_T22_V1A_DIG_TX_TOP                                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CHANGE_DT_OFFSET                                               0x2d
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CHANGE_DT_ADDR                                                 (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CHANGE_DT_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CHANGE_DT_SZ                                                   8
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CHANGE_DT                                                      (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CHANGE_DT_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DEMUX_BIT_OFFSET                                               0x2e
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DEMUX_BIT_ADDR                                                 (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DEMUX_BIT_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DEMUX_BIT_SZ                                                   16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DEMUX_BIT                                                      (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DEMUX_BIT_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_PH_OFFSET                                                               0x30
#define GALLIFREY_T22_V1A_DIG_TX_TOP_PH_ADDR                                                                 (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_PH_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_PH_SZ                                                                   72
#define GALLIFREY_T22_V1A_DIG_TX_TOP_PH                                                                      (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_PH_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_INVERT_BIT_OFFSET                                              0x39
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_INVERT_BIT_ADDR                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_INVERT_BIT_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_INVERT_BIT_SZ                                                  8
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_INVERT_BIT                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_INVERT_BIT_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MUX_BIT_OFFSET                                                 0x3a
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MUX_BIT_ADDR                                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MUX_BIT_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MUX_BIT_SZ                                                     16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MUX_BIT                                                        (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MUX_BIT_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MUX_INT_OFFSET                                                 0x3c
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MUX_INT_ADDR                                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MUX_INT_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MUX_INT_SZ                                                     16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MUX_INT                                                        (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MUX_INT_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_BIT_OFFSET                                         0x3e
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_BIT_ADDR                                           (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_BIT_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_BIT_SZ                                             16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_BIT                                                (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_BIT_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_0_OFFSET                                       0x40
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_0_ADDR                                         (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_0_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_0_SZ                                           16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_0                                              (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_0_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_1_OFFSET                                       0x42
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_1_ADDR                                         (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_1_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_1_SZ                                           16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_1                                              (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_1_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BIT_REPEATER_OFFSET                                            0x44
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BIT_REPEATER_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BIT_REPEATER_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BIT_REPEATER_SZ                                                24
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BIT_REPEATER                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BIT_REPEATER_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DELAY_PATH_OFFSET                                              0x47
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DELAY_PATH_ADDR                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DELAY_PATH_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DELAY_PATH_SZ                                                  8
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DELAY_PATH                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DELAY_PATH_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_IEEE802154_B2C_OFFSET                                          0x48
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_IEEE802154_B2C_ADDR                                            (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_IEEE802154_B2C_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_IEEE802154_B2C_SZ                                              16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_IEEE802154_B2C                                                 (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_IEEE802154_B2C_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DATA_WHITENING_OFFSET                                          0x4a
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DATA_WHITENING_ADDR                                            (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DATA_WHITENING_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DATA_WHITENING_SZ                                              16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DATA_WHITENING                                                 (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DATA_WHITENING_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CRC_CUSTOM_OFFSET                                              0x4c
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CRC_CUSTOM_ADDR                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CRC_CUSTOM_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CRC_CUSTOM_SZ                                                  32
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CRC_CUSTOM                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CRC_CUSTOM_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MANCHESTER_OFFSET                                              0x50
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MANCHESTER_ADDR                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MANCHESTER_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MANCHESTER_SZ                                                  16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MANCHESTER                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MANCHESTER_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PIO4_4_8PSK_OFFSET                                             0x52
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PIO4_4_8PSK_ADDR                                               (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PIO4_4_8PSK_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PIO4_4_8PSK_SZ                                                 16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PIO4_4_8PSK                                                    (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PIO4_4_8PSK_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CONV_CODES_OFFSET                                              0x54
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CONV_CODES_ADDR                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CONV_CODES_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CONV_CODES_SZ                                                  16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CONV_CODES                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CONV_CODES_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_0_OFFSET                                          0x56
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_0_ADDR                                            (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_0_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_0_SZ                                              16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_0                                                 (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_0_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_1_OFFSET                                          0x58
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_1_ADDR                                            (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_1_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_1_SZ                                              16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_1                                                 (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_1_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_MAN_OFFSET                                                0x5a
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_MAN_ADDR                                                  (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_MAN_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_MAN_SZ                                                    16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_MAN                                                       (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_MAN_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_0_OFFSET                                                  0x5c
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_0_ADDR                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_0_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_0_SZ                                                      24
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_0                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_0_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_0_OFFSET                                          0x5f
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_0_ADDR                                            (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_0_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_0_SZ                                              8
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_0                                                 (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_0_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_1_OFFSET                                                  0x60
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_1_ADDR                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_1_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_1_SZ                                                      24
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_1                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_1_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_1_OFFSET                                          0x63
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_1_ADDR                                            (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_1_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_1_SZ                                              8
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_1                                                 (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_1_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_2_OFFSET                                                  0x64
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_2_ADDR                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_2_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_2_SZ                                                      24
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_2                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_2_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DECIMATOR_OFFSET                                               0x67
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DECIMATOR_ADDR                                                 (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DECIMATOR_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DECIMATOR_SZ                                                   8
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DECIMATOR                                                      (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DECIMATOR_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_GENERIC_MAPPER_OFFSET                                          0x68
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_GENERIC_MAPPER_ADDR                                            (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_GENERIC_MAPPER_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_GENERIC_MAPPER_SZ                                              24
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_GENERIC_MAPPER                                                 (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_GENERIC_MAPPER_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DERIVATIVE_OFFSET                                              0x6b
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DERIVATIVE_ADDR                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DERIVATIVE_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DERIVATIVE_SZ                                                  8
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DERIVATIVE                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DERIVATIVE_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_OFFSET                                            0x6c
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_SZ                                                168
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_AM2PM_OFFSET                                                0x81
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_AM2PM_ADDR                                                  (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_AM2PM_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_AM2PM_SZ                                                    8
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_AM2PM                                                       (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_AM2PM_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_FRACTIONAL_INTERPOLATOR_OFFSET                                 0x82
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_FRACTIONAL_INTERPOLATOR_ADDR                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_FRACTIONAL_INTERPOLATOR_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_FRACTIONAL_INTERPOLATOR_SZ                                     16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_FRACTIONAL_INTERPOLATOR                                        (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_FRACTIONAL_INTERPOLATOR_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_FSK_OFFSET                                        0x84
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_FSK_ADDR                                          (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_FSK_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_FSK_SZ                                            152
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_FSK                                               (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_FSK_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_SERIAL_FIFO_OFFSET                                                      0x97
#define GALLIFREY_T22_V1A_DIG_TX_TOP_SERIAL_FIFO_ADDR                                                        (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_SERIAL_FIFO_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_SERIAL_FIFO_SZ                                                          8
#define GALLIFREY_T22_V1A_DIG_TX_TOP_SERIAL_FIFO                                                             (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_SERIAL_FIFO_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SATURATE_OFFSET                                                0x98
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SATURATE_ADDR                                                  (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SATURATE_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SATURATE_SZ                                                    32
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SATURATE                                                       (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SATURATE_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BLOCK_DATA_OFFSET                                              0x9c
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BLOCK_DATA_ADDR                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BLOCK_DATA_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BLOCK_DATA_SZ                                                  16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BLOCK_DATA                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BLOCK_DATA_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_HOLD_DATA_OFFSET                                               0x9e
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_HOLD_DATA_ADDR                                                 (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_HOLD_DATA_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_HOLD_DATA_SZ                                                   16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_HOLD_DATA                                                      (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_HOLD_DATA_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CONST_DATA_OFFSET                                              0xa0
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CONST_DATA_ADDR                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CONST_DATA_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CONST_DATA_SZ                                                  16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CONST_DATA                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CONST_DATA_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SKIP_DATA_OFFSET                                               0xa2
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SKIP_DATA_ADDR                                                 (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SKIP_DATA_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SKIP_DATA_SZ                                                   16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SKIP_DATA                                                      (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SKIP_DATA_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_RAMP_OFFSET                                                 0xa4
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_RAMP_ADDR                                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_RAMP_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_RAMP_SZ                                                     96
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_RAMP                                                        (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_RAMP_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_AM2PM_LUT_OFFSET                                            0xb0
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_AM2PM_LUT_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_AM2PM_LUT_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_AM2PM_LUT_SZ                                                32
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_AM2PM_LUT                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_AM2PM_LUT_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_LINEARIZE_OFFSET                                            0xb4
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_LINEARIZE_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_LINEARIZE_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_LINEARIZE_SZ                                                120
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_LINEARIZE                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_LINEARIZE_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_PA_AMP_OFFSET                                                           0xc3
#define GALLIFREY_T22_V1A_DIG_TX_TOP_PA_AMP_ADDR                                                             (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_PA_AMP_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_PA_AMP_SZ                                                               8
#define GALLIFREY_T22_V1A_DIG_TX_TOP_PA_AMP                                                                  (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_PA_AMP_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_TX_BLE_DF_OFFSET                                               0xc4
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_TX_BLE_DF_ADDR                                                 (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_TX_BLE_DF_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_TX_BLE_DF_SZ                                                   32
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_TX_BLE_DF                                                      (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_TX_BLE_DF_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_0_OFFSET                                            0xc8
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_0_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_0_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_0_SZ                                                24
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_0                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_0_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_OFFSET                                                           0xcb
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_ADDR                                                             (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_SZ                                                               8
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW                                                                  (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_1_OFFSET                                            0xcc
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_1_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_1_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_1_SZ                                                24
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_1                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_1_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_PA_FILT_OFFSET                                                          0xcf
#define GALLIFREY_T22_V1A_DIG_TX_TOP_PA_FILT_ADDR                                                            (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_PA_FILT_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_PA_FILT_SZ                                                              8
#define GALLIFREY_T22_V1A_DIG_TX_TOP_PA_FILT                                                                 (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_PA_FILT_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_2_OFFSET                                            0xd0
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_2_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_2_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_2_SZ                                                24
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_2                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_2_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_OFFSET                                                                  0xd3
#define GALLIFREY_T22_V1A_DIG_RX_TOP_ADDR                                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_SZ                                                                      8
#define GALLIFREY_T22_V1A_DIG_RX_TOP                                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_3_OFFSET                                            0xd4
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_3_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_3_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_3_SZ                                                24
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_3                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_3_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_INPUT_ADDR_OFFSET                                                   0xd7
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_INPUT_ADDR_ADDR                                                     (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_INPUT_ADDR_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_INPUT_ADDR_SZ                                                       8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_INPUT_ADDR                                                          (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_INPUT_ADDR_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_4_OFFSET                                            0xd8
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_4_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_4_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_4_SZ                                                24
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_4                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_4_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_CTRL_OFFSET                                                         0xdb
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_CTRL_ADDR                                                           (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_CTRL_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_CTRL_SZ                                                             8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_CTRL                                                                (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_CTRL_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_5_OFFSET                                            0xdc
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_5_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_5_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_5_SZ                                                24
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_5                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_5_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_TMR_PREDIV_OFFSET                                                   0xdf
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_TMR_PREDIV_ADDR                                                     (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_TMR_PREDIV_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_TMR_PREDIV_SZ                                                       8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_TMR_PREDIV                                                          (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_TMR_PREDIV_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_6_OFFSET                                            0xe0
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_6_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_6_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_6_SZ                                                24
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_6                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_6_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_OFFSET                                                              0xe3
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ADDR                                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_SZ                                                                  8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC                                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_7_OFFSET                                            0xe4
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_7_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_7_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_7_SZ                                                24
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_7                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_7_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_0_OFFSET                                         0xe7
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_0_ADDR                                           (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_0_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_0_SZ                                             8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_0                                                (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_0_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_8_OFFSET                                            0xe8
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_8_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_8_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_8_SZ                                                24
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_8                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_8_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_PA_POWER_OFFSET                                                  0xeb
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_PA_POWER_ADDR                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_PA_POWER_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_PA_POWER_SZ                                                      8
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_PA_POWER                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_PA_POWER_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_9_OFFSET                                            0xec
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_9_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_9_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_9_SZ                                                24
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_9                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_9_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_1_OFFSET                                         0xef
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_1_ADDR                                           (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_1_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_1_SZ                                             8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_1                                                (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_1_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_10_OFFSET                                           0xf0
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_10_ADDR                                             (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_10_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_10_SZ                                               24
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_10                                                  (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_10_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0_OFFSET                                      0xf3
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0_ADDR                                        (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0_SZ                                          8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0                                             (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_11_OFFSET                                           0xf4
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_11_ADDR                                             (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_11_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_11_SZ                                               16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_11                                                  (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DP_MAPPING_11_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_0_OFFSET                                                     0xf6
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_0_ADDR                                                       (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_0_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_0_SZ                                                         16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_0                                                            (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_0_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_1_OFFSET                                                     0xf8
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_1_ADDR                                                       (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_1_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_1_SZ                                                         16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_1                                                            (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_1_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_2_OFFSET                                                     0xfa
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_2_ADDR                                                       (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_2_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_2_SZ                                                         16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_2                                                            (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_2_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_3_OFFSET                                                     0xfc
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_3_ADDR                                                       (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_3_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_3_SZ                                                         16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_3                                                            (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_3_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_4_OFFSET                                                     0xfe
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_4_ADDR                                                       (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_4_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_4_SZ                                                         16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_4                                                            (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_4_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_5_OFFSET                                                     0x100
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_5_ADDR                                                       (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_5_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_5_SZ                                                         16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_5                                                            (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_5_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_6_OFFSET                                                     0x102
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_6_ADDR                                                       (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_6_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_6_SZ                                                         16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_6                                                            (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_6_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_7_OFFSET                                                     0x104
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_7_ADDR                                                       (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_7_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_7_SZ                                                         16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_7                                                            (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_7_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_8_OFFSET                                                     0x106
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_8_ADDR                                                       (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_8_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_8_SZ                                                         16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_8                                                            (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_8_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_9_OFFSET                                                     0x108
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_9_ADDR                                                       (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_9_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_9_SZ                                                         16
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_9                                                            (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_9_ADDR
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_0_OFFSET                                                       0x10a
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_0_ADDR                                                         (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_0_OFFSET)
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_0_SZ                                                           16
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_0                                                              (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_0_ADDR
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_OFFSET                                                                0x10c
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_ADDR                                                                  (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_PROT_TMR_OFFSET)
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_SZ                                                                    24
#define GALLIFREY_T22_V1A_DIG_PROT_TMR                                                                       (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_PROT_TMR_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_OSR_INIT_OFFSET                                                     0x10f
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_OSR_INIT_ADDR                                                       (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_OSR_INIT_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_OSR_INIT_SZ                                                         8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_OSR_INIT                                                            (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_OSR_INIT_ADDR
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_1_OFFSET                                                       0x110
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_1_ADDR                                                         (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_1_OFFSET)
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_1_SZ                                                           16
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_1                                                              (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_1_ADDR
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_2_OFFSET                                                       0x112
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_2_ADDR                                                         (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_2_OFFSET)
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_2_SZ                                                           16
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_2                                                              (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_2_ADDR
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_3_OFFSET                                                       0x114
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_3_ADDR                                                         (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_3_OFFSET)
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_3_SZ                                                           16
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_3                                                              (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_3_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_RF_OFFSET                                                           0x116
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_RF_ADDR                                                             (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_RF_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_RF_SZ                                                               8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_RF                                                                  (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_RF_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_BB_OFFSET                                                           0x117
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_BB_ADDR                                                             (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_BB_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_BB_SZ                                                               8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_BB                                                                  (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_BB_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_PH_OFFSET                                                               0x118
#define GALLIFREY_T22_V1A_DIG_RX_TOP_PH_ADDR                                                                 (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_PH_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_PH_SZ                                                                   24
#define GALLIFREY_T22_V1A_DIG_RX_TOP_PH                                                                      (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_PH_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0_DELAY_OFFSET                                0x11b
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0_DELAY_ADDR                                  (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0_DELAY_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0_DELAY_SZ                                    8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0_DELAY                                       (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0_DELAY_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_0_OFFSET                                                  0x11c
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_0_ADDR                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_0_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_0_SZ                                                      32
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_0                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_0_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_1_OFFSET                                                  0x120
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_1_ADDR                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_1_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_1_SZ                                                      24
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_1                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_1_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1_OFFSET                                      0x123
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1_ADDR                                        (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1_SZ                                          8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1                                             (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1_DELAY_OFFSET                                0x124
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1_DELAY_ADDR                                  (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1_DELAY_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1_DELAY_SZ                                    8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1_DELAY                                       (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1_DELAY_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_0_OFFSET                                        0x125
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_0_ADDR                                          (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_0_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_0_SZ                                            8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_0                                               (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_0_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_1_OFFSET                                        0x126
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_1_ADDR                                          (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_1_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_1_SZ                                            8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_1                                               (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_1_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_2_OFFSET                                        0x127
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_2_ADDR                                          (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_2_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_2_SZ                                            8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_2                                               (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_2_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_3_OFFSET                                        0x128
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_3_ADDR                                          (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_3_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_3_SZ                                            8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_3                                               (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_3_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_4_OFFSET                                        0x129
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_4_ADDR                                          (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_4_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_4_SZ                                            8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_4                                               (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_4_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_5_OFFSET                                        0x12a
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_5_ADDR                                          (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_5_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_5_SZ                                            8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_5                                               (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_5_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0_OFFSET                                             0x12b
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0_ADDR                                               (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0_SZ                                                 8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0                                                    (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_X4_DEST_OFFSET                                       0x12c
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_X4_DEST_ADDR                                         (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_X4_DEST_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_X4_DEST_SZ                                           16
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_X4_DEST                                              (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_X4_DEST_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0_DELAY_OFFSET                                       0x12e
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0_DELAY_ADDR                                         (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0_DELAY_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0_DELAY_SZ                                           8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0_DELAY                                              (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0_DELAY_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1_OFFSET                                             0x12f
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1_ADDR                                               (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1_SZ                                                 8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1                                                    (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1_DELAY_OFFSET                                       0x130
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1_DELAY_ADDR                                         (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1_DELAY_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1_DELAY_SZ                                           8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1_DELAY                                              (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1_DELAY_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_0_OFFSET                                             0x131
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_0_ADDR                                               (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_0_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_0_SZ                                                 8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_0                                                    (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_0_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_1_OFFSET                                             0x132
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_1_ADDR                                               (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_1_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_1_SZ                                                 8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_1                                                    (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_1_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_2_OFFSET                                             0x133
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_2_ADDR                                               (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_2_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_2_SZ                                                 8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_2                                                    (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_2_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_ADC_LIMIT_DETECT_OFFSET                                        0x134
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_ADC_LIMIT_DETECT_ADDR                                          (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_ADC_LIMIT_DETECT_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_ADC_LIMIT_DETECT_SZ                                            40
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_ADC_LIMIT_DETECT                                               (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_ADC_LIMIT_DETECT_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_HP_FILTER_OFFSET                                               0x139
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_HP_FILTER_ADDR                                                 (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_HP_FILTER_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_HP_FILTER_SZ                                                   8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_HP_FILTER                                                      (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_HP_FILTER_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_REMOVE_IF_OFFSET                                               0x13a
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_REMOVE_IF_ADDR                                                 (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_REMOVE_IF_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_REMOVE_IF_SZ                                                   16
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_REMOVE_IF                                                      (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_REMOVE_IF_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IQ_BALANCE_OFFSET                                              0x13c
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IQ_BALANCE_ADDR                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IQ_BALANCE_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IQ_BALANCE_SZ                                                  24
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IQ_BALANCE                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IQ_BALANCE_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_3_OFFSET                                             0x13f
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_3_ADDR                                               (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_3_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_3_SZ                                                 8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_3                                                    (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_3_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FRACTIONAL_DECIMATOR_OFFSET                                    0x140
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FRACTIONAL_DECIMATOR_ADDR                                      (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FRACTIONAL_DECIMATOR_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FRACTIONAL_DECIMATOR_SZ                                        8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FRACTIONAL_DECIMATOR                                           (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FRACTIONAL_DECIMATOR_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORDIC_LIN2POL_OFFSET                                          0x141
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORDIC_LIN2POL_ADDR                                            (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORDIC_LIN2POL_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORDIC_LIN2POL_SZ                                              8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORDIC_LIN2POL                                                 (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORDIC_LIN2POL_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DERIVATIVE_OFFSET                                              0x142
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DERIVATIVE_ADDR                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DERIVATIVE_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DERIVATIVE_SZ                                                  8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DERIVATIVE                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DERIVATIVE_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DIFF_BY_N_OFFSET                                               0x143
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DIFF_BY_N_ADDR                                                 (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DIFF_BY_N_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DIFF_BY_N_SZ                                                   8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DIFF_BY_N                                                      (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DIFF_BY_N_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_CONF_OFFSET                                         0x144
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_CONF_ADDR                                           (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_CONF_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_CONF_SZ                                             8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_CONF                                                (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_CONF_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_DELAY_OFFSET                                        0x145
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_DELAY_ADDR                                          (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_DELAY_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_DELAY_SZ                                            8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_DELAY                                               (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_DELAY_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_CMDS_OFFSET                                         0x146
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_CMDS_ADDR                                           (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_CMDS_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_CMDS_SZ                                             16
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_CMDS                                                (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_CMDS_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_OPTS_OFFSET                                         0x148
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_OPTS_ADDR                                           (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_OPTS_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_OPTS_SZ                                             40
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_OPTS                                                (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_OPTS_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0_OFFSET                                        0x14d
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0_ADDR                                          (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0_SZ                                            8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0                                               (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1_OFFSET                                        0x14e
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1_ADDR                                          (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1_SZ                                            8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1                                               (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_CTE_INFO_OFFSET                                      0x14f
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_CTE_INFO_ADDR                                        (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_CTE_INFO_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_CTE_INFO_SZ                                          8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_CTE_INFO                                             (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_CTE_INFO_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0_PS_COEFS_OFFSET                               0x150
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0_PS_COEFS_ADDR                                 (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0_PS_COEFS_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0_PS_COEFS_SZ                                   64
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0_PS_COEFS                                      (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0_PS_COEFS_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1_PS_COEFS_OFFSET                               0x158
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1_PS_COEFS_ADDR                                 (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1_PS_COEFS_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1_PS_COEFS_SZ                                   32
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1_PS_COEFS                                      (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1_PS_COEFS_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_OFFSET                                               0x15c
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_ADDR                                                 (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_SZ                                                   16
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF                                                      (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_RX_PATH_DELAY_OFFSET                                 0x15e
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_RX_PATH_DELAY_ADDR                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_RX_PATH_DELAY_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_RX_PATH_DELAY_SZ                                     8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_RX_PATH_DELAY                                        (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_RX_PATH_DELAY_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_FAKE_REG_OFFSET                                      0x15f
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_FAKE_REG_ADDR                                        (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_FAKE_REG_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_FAKE_REG_SZ                                          8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_FAKE_REG                                             (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_FAKE_REG_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_DELAY_OFFSET                                         0x160
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_DELAY_ADDR                                           (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_DELAY_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_DELAY_SZ                                             8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_DELAY                                                (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_DELAY_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_OFFSET                                          0x161
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_ADDR                                            (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_SZ                                              8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY                                                 (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_DELAY_OFFSET                                    0x162
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_DELAY_ADDR                                      (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_DELAY_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_DELAY_SZ                                        8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_DELAY                                           (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_DELAY_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_PHASE_INTERP_X2_OFFSET                                         0x163
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_PHASE_INTERP_X2_ADDR                                           (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_PHASE_INTERP_X2_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_PHASE_INTERP_X2_SZ                                             8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_PHASE_INTERP_X2                                                (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_PHASE_INTERP_X2_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DPSK_TED_OFFSET                                                0x164
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DPSK_TED_ADDR                                                  (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DPSK_TED_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DPSK_TED_SZ                                                    16
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DPSK_TED                                                       (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DPSK_TED_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FSK_TED_OFFSET                                                 0x166
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FSK_TED_ADDR                                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FSK_TED_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FSK_TED_SZ                                                     8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FSK_TED                                                        (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FSK_TED_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_DEMAPPER_OFFSET                                            0x167
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_DEMAPPER_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_DEMAPPER_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_DEMAPPER_SZ                                                8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_DEMAPPER                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_DEMAPPER_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CARRIER_RECOVERY_OFFSET                                        0x168
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CARRIER_RECOVERY_ADDR                                          (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CARRIER_RECOVERY_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CARRIER_RECOVERY_SZ                                            16
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CARRIER_RECOVERY                                               (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CARRIER_RECOVERY_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_OFFSET                                         0x16a
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_ADDR                                           (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_SZ                                             16
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT                                                (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_DELAY_OFFSET                                   0x16c
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_DELAY_ADDR                                     (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_DELAY_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_DELAY_SZ                                       8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_DELAY                                          (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_DELAY_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_DELAY_OFFSET                                   0x16d
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_DELAY_ADDR                                     (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_DELAY_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_DELAY_SZ                                       8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_DELAY                                          (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_DELAY_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_OFFSET                                         0x16e
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_ADDR                                           (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_SZ                                             16
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT                                                (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DPSK_DEMOD_OFFSET                                              0x170
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DPSK_DEMOD_ADDR                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DPSK_DEMOD_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DPSK_DEMOD_SZ                                                  8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DPSK_DEMOD                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DPSK_DEMOD_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_DELAY_OFFSET                                   0x171
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_DELAY_ADDR                                     (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_DELAY_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_DELAY_SZ                                       8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_DELAY                                          (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_DELAY_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_DELAY_DELAY_OFFSET                                         0x172
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_DELAY_DELAY_ADDR                                           (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_DELAY_DELAY_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_DELAY_DELAY_SZ                                             8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_DELAY_DELAY                                                (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_DELAY_DELAY_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_SUBSAMPLE_OFFSET                                               0x173
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_SUBSAMPLE_ADDR                                                 (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_SUBSAMPLE_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_SUBSAMPLE_SZ                                                   8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_SUBSAMPLE                                                      (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_SUBSAMPLE_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_OFFSET                                         0x174
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_ADDR                                           (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_SZ                                             24
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B                                                (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CRC_CUSTOM_OFFSET                                              0x177
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CRC_CUSTOM_ADDR                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CRC_CUSTOM_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CRC_CUSTOM_SZ                                                  8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CRC_CUSTOM                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CRC_CUSTOM_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_OFFSET                                            0x178
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_SZ                                                48
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_OFFSET                                            0x17e
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_SZ                                                8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_BLR_PACKET_LEN_OFFSET                             0x17f
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_BLR_PACKET_LEN_ADDR                               (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_BLR_PACKET_LEN_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_BLR_PACKET_LEN_SZ                                 8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_BLR_PACKET_LEN                                    (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_BLR_PACKET_LEN_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_CMDS_OFFSET                                       0x180
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_CMDS_ADDR                                         (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_CMDS_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_CMDS_SZ                                           24
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_CMDS                                              (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_CMDS_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RSSI_READOUT_OFFSET                                            0x183
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RSSI_READOUT_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RSSI_READOUT_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RSSI_READOUT_SZ                                                8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RSSI_READOUT                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RSSI_READOUT_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_LIMIT_RATE_OFFSET                                              0x184
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_LIMIT_RATE_ADDR                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_LIMIT_RATE_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_LIMIT_RATE_SZ                                                  16
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_LIMIT_RATE                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_LIMIT_RATE_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FLUSH_OFFSET                                                   0x186
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FLUSH_ADDR                                                     (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FLUSH_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FLUSH_SZ                                                       16
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FLUSH                                                          (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FLUSH_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FLUSH_DELAY_OFFSET                                             0x188
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FLUSH_DELAY_ADDR                                               (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FLUSH_DELAY_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FLUSH_DELAY_SZ                                                 8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FLUSH_DELAY                                                    (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FLUSH_DELAY_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_CTE_OFFSET                                                              0x189
#define GALLIFREY_T22_V1A_DIG_RX_TOP_CTE_ADDR                                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_CTE_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_CTE_SZ                                                                  8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_CTE                                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_CTE_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_ADC_CTRL_OFFSET                                                         0x18a
#define GALLIFREY_T22_V1A_DIG_RX_TOP_ADC_CTRL_ADDR                                                           (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_ADC_CTRL_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_ADC_CTRL_SZ                                                             8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_ADC_CTRL                                                                (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_ADC_CTRL_ADDR
#define GALLIFREY_T22_V1A_DIG_CLK_CTRL_OFFSET                                                                0x18b
#define GALLIFREY_T22_V1A_DIG_CLK_CTRL_ADDR                                                                  (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_CLK_CTRL_OFFSET)
#define GALLIFREY_T22_V1A_DIG_CLK_CTRL_SZ                                                                    8
#define GALLIFREY_T22_V1A_DIG_CLK_CTRL                                                                       (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_CLK_CTRL_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_0_OFFSET                                                  0x18c
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_0_ADDR                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_0_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_0_SZ                                                      24
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_0                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_0_ADDR
#define GALLIFREY_T22_V1A_DIG_AES_CFG_OFFSET                                                                 0x18f
#define GALLIFREY_T22_V1A_DIG_AES_CFG_ADDR                                                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_AES_CFG_OFFSET)
#define GALLIFREY_T22_V1A_DIG_AES_CFG_SZ                                                                     8
#define GALLIFREY_T22_V1A_DIG_AES_CFG                                                                        (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_AES_CFG_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_1_OFFSET                                                  0x190
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_1_ADDR                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_1_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_1_SZ                                                      24
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_1                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_1_ADDR
#define GALLIFREY_T22_V1A_DIG_IQ_FIFO_OFFSET                                                                 0x193
#define GALLIFREY_T22_V1A_DIG_IQ_FIFO_ADDR                                                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_IQ_FIFO_OFFSET)
#define GALLIFREY_T22_V1A_DIG_IQ_FIFO_SZ                                                                     8
#define GALLIFREY_T22_V1A_DIG_IQ_FIFO                                                                        (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_IQ_FIFO_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_2_OFFSET                                                  0x194
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_2_ADDR                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_2_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_2_SZ                                                      24
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_2                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_2_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_FIFO_OFFSET                                                                 0x197
#define GALLIFREY_T22_V1A_DIG_RX_FIFO_ADDR                                                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_FIFO_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_FIFO_SZ                                                                     8
#define GALLIFREY_T22_V1A_DIG_RX_FIFO                                                                        (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_FIFO_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_3_OFFSET                                                  0x198
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_3_ADDR                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_3_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_3_SZ                                                      24
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_3                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_3_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_FIFO_OFFSET                                                                 0x19b
#define GALLIFREY_T22_V1A_DIG_TX_FIFO_ADDR                                                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_FIFO_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_FIFO_SZ                                                                     8
#define GALLIFREY_T22_V1A_DIG_TX_FIFO                                                                        (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_FIFO_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_0_OFFSET                                            0x19c
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_0_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_0_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_0_SZ                                                32
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_0                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_0_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_1_OFFSET                                            0x1a0
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_1_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_1_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_1_SZ                                                32
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_1                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_1_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_2_OFFSET                                            0x1a4
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_2_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_2_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_2_SZ                                                32
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_2                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_2_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_3_OFFSET                                            0x1a8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_3_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_3_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_3_SZ                                                32
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_3                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_3_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_4_OFFSET                                            0x1ac
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_4_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_4_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_4_SZ                                                32
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_4                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_4_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_5_OFFSET                                            0x1b0
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_5_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_5_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_5_SZ                                                32
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_5                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_5_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_6_OFFSET                                            0x1b4
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_6_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_6_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_6_SZ                                                32
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_6                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_6_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_7_OFFSET                                            0x1b8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_7_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_7_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_7_SZ                                                32
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_7                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_7_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_8_OFFSET                                            0x1bc
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_8_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_8_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_8_SZ                                                32
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_8                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_8_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_9_OFFSET                                            0x1c0
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_9_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_9_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_9_SZ                                                32
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_9                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_9_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_10_OFFSET                                           0x1c4
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_10_ADDR                                             (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_10_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_10_SZ                                               32
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_10                                                  (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_10_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_11_OFFSET                                           0x1c8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_11_ADDR                                             (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_11_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_11_SZ                                               32
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_11                                                  (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_11_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_12_OFFSET                                           0x1cc
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_12_ADDR                                             (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_12_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_12_SZ                                               32
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_12                                                  (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_12_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_13_OFFSET                                           0x1d0
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_13_ADDR                                             (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_13_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_13_SZ                                               32
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_13                                                  (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_13_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_14_OFFSET                                           0x1d4
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_14_ADDR                                             (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_14_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_14_SZ                                               32
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_14                                                  (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_14_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_15_OFFSET                                           0x1d8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_15_ADDR                                             (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_15_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_15_SZ                                               32
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_15                                                  (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_15_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_16_OFFSET                                           0x1dc
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_16_ADDR                                             (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_16_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_16_SZ                                               32
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_16                                                  (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_16_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_17_OFFSET                                           0x1e0
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_17_ADDR                                             (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_17_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_17_SZ                                               32
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_17                                                  (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_17_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_18_OFFSET                                           0x1e4
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_18_ADDR                                             (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_18_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_18_SZ                                               32
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_18                                                  (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_18_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_19_OFFSET                                           0x1e8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_19_ADDR                                             (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_19_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_19_SZ                                               32
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_19                                                  (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_19_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_20_OFFSET                                           0x1ec
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_20_ADDR                                             (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_20_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_20_SZ                                               32
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_20                                                  (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_20_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_21_OFFSET                                           0x1f0
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_21_ADDR                                             (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_21_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_21_SZ                                               32
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_21                                                  (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_21_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_22_OFFSET                                           0x1f4
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_22_ADDR                                             (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_22_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_22_SZ                                               32
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_22                                                  (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_22_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_23_OFFSET                                           0x1f8
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_23_ADDR                                             (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_23_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_23_SZ                                               32
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_23                                                  (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DP_MAPPING_23_ADDR
#define GALLIFREY_T22_V1A_DIG_DCO_CAL_OFFSET                                                                 0x1fc
#define GALLIFREY_T22_V1A_DIG_DCO_CAL_ADDR                                                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_DCO_CAL_OFFSET)
#define GALLIFREY_T22_V1A_DIG_DCO_CAL_SZ                                                                     16
#define GALLIFREY_T22_V1A_DIG_DCO_CAL                                                                        (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_DCO_CAL_ADDR
#define GALLIFREY_T22_V1A_DIG_TX_BLE_DTM_OFFSET                                                              0x1fe
#define GALLIFREY_T22_V1A_DIG_TX_BLE_DTM_ADDR                                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TX_BLE_DTM_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TX_BLE_DTM_SZ                                                                  16
#define GALLIFREY_T22_V1A_DIG_TX_BLE_DTM                                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TX_BLE_DTM_ADDR
#define GALLIFREY_T22_V1A_DIG_CLK_CTRL_CLK_DIV_OFFSET                                                        0x200
#define GALLIFREY_T22_V1A_DIG_CLK_CTRL_CLK_DIV_ADDR                                                          (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_CLK_CTRL_CLK_DIV_OFFSET)
#define GALLIFREY_T22_V1A_DIG_CLK_CTRL_CLK_DIV_SZ                                                            24
#define GALLIFREY_T22_V1A_DIG_CLK_CTRL_CLK_DIV                                                               (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_CLK_CTRL_CLK_DIV_ADDR
#define GALLIFREY_T22_V1A_DIG_DIR_FIND_OFFSET                                                                0x203
#define GALLIFREY_T22_V1A_DIG_DIR_FIND_ADDR                                                                  (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_DIR_FIND_OFFSET)
#define GALLIFREY_T22_V1A_DIG_DIR_FIND_SZ                                                                    8
#define GALLIFREY_T22_V1A_DIG_DIR_FIND                                                                       (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_DIR_FIND_ADDR
#define GALLIFREY_T22_V1A_DIG_RCCO_COMP_OFFSET                                                               0x204
#define GALLIFREY_T22_V1A_DIG_RCCO_COMP_ADDR                                                                 (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RCCO_COMP_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RCCO_COMP_SZ                                                                   24
#define GALLIFREY_T22_V1A_DIG_RCCO_COMP                                                                      (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RCCO_COMP_ADDR
#define GALLIFREY_T22_V1A_DIG_CHANNEL_HDL_OFFSET                                                             0x207
#define GALLIFREY_T22_V1A_DIG_CHANNEL_HDL_ADDR                                                               (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_CHANNEL_HDL_OFFSET)
#define GALLIFREY_T22_V1A_DIG_CHANNEL_HDL_SZ                                                                 8
#define GALLIFREY_T22_V1A_DIG_CHANNEL_HDL                                                                    (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_CHANNEL_HDL_ADDR
#define GALLIFREY_T22_V1A_DIG_AES_KEY_OFFSET                                                                 0x208
#define GALLIFREY_T22_V1A_DIG_AES_KEY_ADDR                                                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_AES_KEY_OFFSET)
#define GALLIFREY_T22_V1A_DIG_AES_KEY_SZ                                                                     128
#define GALLIFREY_T22_V1A_DIG_AES_KEY                                                                        (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_AES_KEY_ADDR
#define GALLIFREY_T22_V1A_DIG_AES_NONCE_OFFSET                                                               0x218
#define GALLIFREY_T22_V1A_DIG_AES_NONCE_ADDR                                                                 (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_AES_NONCE_OFFSET)
#define GALLIFREY_T22_V1A_DIG_AES_NONCE_SZ                                                                   104
#define GALLIFREY_T22_V1A_DIG_AES_NONCE                                                                      (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_AES_NONCE_ADDR
#define GALLIFREY_T22_V1A_DIG_MISC_OFFSET                                                                    0x225
#define GALLIFREY_T22_V1A_DIG_MISC_ADDR                                                                      (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_MISC_OFFSET)
#define GALLIFREY_T22_V1A_DIG_MISC_SZ                                                                        8
#define GALLIFREY_T22_V1A_DIG_MISC                                                                           (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_MISC_ADDR
#define GALLIFREY_T22_V1A_DIG_TRX_TRX_DF_CFG_OFFSET                                                          0x226
#define GALLIFREY_T22_V1A_DIG_TRX_TRX_DF_CFG_ADDR                                                            (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TRX_TRX_DF_CFG_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TRX_TRX_DF_CFG_SZ                                                              8
#define GALLIFREY_T22_V1A_DIG_TRX_TRX_DF_CFG                                                                 (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TRX_TRX_DF_CFG_ADDR
#define GALLIFREY_T22_V1A_DIG_TRX_MANCHESTER_OFFSET                                                          0x227
#define GALLIFREY_T22_V1A_DIG_TRX_MANCHESTER_ADDR                                                            (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TRX_MANCHESTER_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TRX_MANCHESTER_SZ                                                              8
#define GALLIFREY_T22_V1A_DIG_TRX_MANCHESTER                                                                 (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TRX_MANCHESTER_ADDR
#define GALLIFREY_T22_V1A_DIG_TRX_CRC_POLY_OFFSET                                                            0x228
#define GALLIFREY_T22_V1A_DIG_TRX_CRC_POLY_ADDR                                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TRX_CRC_POLY_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TRX_CRC_POLY_SZ                                                                32
#define GALLIFREY_T22_V1A_DIG_TRX_CRC_POLY                                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TRX_CRC_POLY_ADDR
#define GALLIFREY_T22_V1A_DIG_TRX_CRC_RST_OFFSET                                                             0x22c
#define GALLIFREY_T22_V1A_DIG_TRX_CRC_RST_ADDR                                                               (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TRX_CRC_RST_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TRX_CRC_RST_SZ                                                                 32
#define GALLIFREY_T22_V1A_DIG_TRX_CRC_RST                                                                    (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TRX_CRC_RST_ADDR
#define GALLIFREY_T22_V1A_DIG_TRX_TRX_DF_LUT_OFFSET                                                          0x230
#define GALLIFREY_T22_V1A_DIG_TRX_TRX_DF_LUT_ADDR                                                            (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TRX_TRX_DF_LUT_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TRX_TRX_DF_LUT_SZ                                                              64
#define GALLIFREY_T22_V1A_DIG_TRX_TRX_DF_LUT                                                                 (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TRX_TRX_DF_LUT_ADDR
#define GALLIFREY_T22_V1A_DIG_TRX_VITERBI_CONV_OFFSET                                                        0x238
#define GALLIFREY_T22_V1A_DIG_TRX_VITERBI_CONV_ADDR                                                          (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TRX_VITERBI_CONV_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TRX_VITERBI_CONV_SZ                                                            16
#define GALLIFREY_T22_V1A_DIG_TRX_VITERBI_CONV                                                               (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TRX_VITERBI_CONV_ADDR
#define GALLIFREY_T22_V1A_DIG_TRX_DATA_WHITENING_OFFSET                                                      0x23a
#define GALLIFREY_T22_V1A_DIG_TRX_DATA_WHITENING_ADDR                                                        (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TRX_DATA_WHITENING_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TRX_DATA_WHITENING_SZ                                                          8
#define GALLIFREY_T22_V1A_DIG_TRX_DATA_WHITENING                                                             (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TRX_DATA_WHITENING_ADDR
#define GALLIFREY_T22_V1A_DIG_TRX_DPSK_OFFSET                                                                0x23b
#define GALLIFREY_T22_V1A_DIG_TRX_DPSK_ADDR                                                                  (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TRX_DPSK_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TRX_DPSK_SZ                                                                    8
#define GALLIFREY_T22_V1A_DIG_TRX_DPSK                                                                       (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TRX_DPSK_ADDR
#define GALLIFREY_T22_V1A_DIG_TRX_SYNC_WORD_LOW_OFFSET                                                       0x23c
#define GALLIFREY_T22_V1A_DIG_TRX_SYNC_WORD_LOW_ADDR                                                         (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TRX_SYNC_WORD_LOW_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TRX_SYNC_WORD_LOW_SZ                                                           32
#define GALLIFREY_T22_V1A_DIG_TRX_SYNC_WORD_LOW                                                              (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TRX_SYNC_WORD_LOW_ADDR
#define GALLIFREY_T22_V1A_DIG_TRX_SYNC_WORD_HIGH_OFFSET                                                      0x240
#define GALLIFREY_T22_V1A_DIG_TRX_SYNC_WORD_HIGH_ADDR                                                        (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_TRX_SYNC_WORD_HIGH_OFFSET)
#define GALLIFREY_T22_V1A_DIG_TRX_SYNC_WORD_HIGH_SZ                                                          32
#define GALLIFREY_T22_V1A_DIG_TRX_SYNC_WORD_HIGH                                                             (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_TRX_SYNC_WORD_HIGH_ADDR
#define GALLIFREY_T22_V1A_LDO_DIG_VREF_CONSTS_OFFSET                                                         0x244
#define GALLIFREY_T22_V1A_LDO_DIG_VREF_CONSTS_ADDR                                                           (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_LDO_DIG_VREF_CONSTS_OFFSET)
#define GALLIFREY_T22_V1A_LDO_DIG_VREF_CONSTS_SZ                                                             8
#define GALLIFREY_T22_V1A_LDO_DIG_VREF_CONSTS                                                                (volatile uint8_t *)GALLIFREY_T22_V1A_LDO_DIG_VREF_CONSTS_ADDR
#define GALLIFREY_T22_V1A_LDO_DIG_VREF_OFFSET                                                                0x245
#define GALLIFREY_T22_V1A_LDO_DIG_VREF_ADDR                                                                  (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_LDO_DIG_VREF_OFFSET)
#define GALLIFREY_T22_V1A_LDO_DIG_VREF_SZ                                                                    8
#define GALLIFREY_T22_V1A_LDO_DIG_VREF                                                                       (volatile uint8_t *)GALLIFREY_T22_V1A_LDO_DIG_VREF_ADDR
#define GALLIFREY_T22_V1A_LDO_DIG_LDO_CONSTS_OFFSET                                                          0x246
#define GALLIFREY_T22_V1A_LDO_DIG_LDO_CONSTS_ADDR                                                            (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_LDO_DIG_LDO_CONSTS_OFFSET)
#define GALLIFREY_T22_V1A_LDO_DIG_LDO_CONSTS_SZ                                                              8
#define GALLIFREY_T22_V1A_LDO_DIG_LDO_CONSTS                                                                 (volatile uint8_t *)GALLIFREY_T22_V1A_LDO_DIG_LDO_CONSTS_ADDR
#define GALLIFREY_T22_V1A_LDO_DIG_LDO_TSMC_CONFIG_OFFSET                                                     0x247
#define GALLIFREY_T22_V1A_LDO_DIG_LDO_TSMC_CONFIG_ADDR                                                       (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_LDO_DIG_LDO_TSMC_CONFIG_OFFSET)
#define GALLIFREY_T22_V1A_LDO_DIG_LDO_TSMC_CONFIG_SZ                                                         8
#define GALLIFREY_T22_V1A_LDO_DIG_LDO_TSMC_CONFIG                                                            (volatile uint8_t *)GALLIFREY_T22_V1A_LDO_DIG_LDO_TSMC_CONFIG_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_OFFSET                                                                     0x248
#define GALLIFREY_T22_V1A_ANA_TRX_ADDR                                                                       (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_SZ                                                                         8
#define GALLIFREY_T22_V1A_ANA_TRX                                                                            (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_OFFSET                                                                  0x249
#define GALLIFREY_T22_V1A_ANA_TRX_RF_ADDR                                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_SZ                                                                      8
#define GALLIFREY_T22_V1A_ANA_TRX_RF                                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_CONFIG_OFFSET                                                           0x24a
#define GALLIFREY_T22_V1A_ANA_TRX_RF_CONFIG_ADDR                                                             (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_CONFIG_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_CONFIG_SZ                                                               16
#define GALLIFREY_T22_V1A_ANA_TRX_RF_CONFIG                                                                  (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_CONFIG_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_BLOCK_ENA_OFFSET                                                        0x24c
#define GALLIFREY_T22_V1A_ANA_TRX_RF_BLOCK_ENA_ADDR                                                          (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_BLOCK_ENA_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_BLOCK_ENA_SZ                                                            8
#define GALLIFREY_T22_V1A_ANA_TRX_RF_BLOCK_ENA                                                               (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_BLOCK_ENA_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_POW_ENA_OFFSET                                                          0x24d
#define GALLIFREY_T22_V1A_ANA_TRX_RF_POW_ENA_ADDR                                                            (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_POW_ENA_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_POW_ENA_SZ                                                              8
#define GALLIFREY_T22_V1A_ANA_TRX_RF_POW_ENA                                                                 (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_POW_ENA_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_CONFIG_OFFSET                                                        0x24e
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_CONFIG_ADDR                                                          (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_PA_CONFIG_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_CONFIG_SZ                                                            8
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_CONFIG                                                               (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_PA_CONFIG_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_IBIAS_OFFSET                                                         0x24f
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_IBIAS_ADDR                                                           (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_PA_IBIAS_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_IBIAS_SZ                                                             8
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_IBIAS                                                                (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_PA_IBIAS_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_PHASE_SHIFT_DYN_OFFSET                                               0x250
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_PHASE_SHIFT_DYN_ADDR                                                 (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_PA_PHASE_SHIFT_DYN_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_PHASE_SHIFT_DYN_SZ                                                   8
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_PHASE_SHIFT_DYN                                                      (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_PA_PHASE_SHIFT_DYN_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_IBIAS_OFFSET                                                        0x251
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_IBIAS_ADDR                                                          (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_IBIAS_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_IBIAS_SZ                                                            8
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_IBIAS                                                               (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_IBIAS_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_CONFIG_OFFSET                                                       0x252
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_CONFIG_ADDR                                                         (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_CONFIG_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_CONFIG_SZ                                                           8
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_CONFIG                                                              (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_CONFIG_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_AGC_OFFSET                                                          0x253
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_AGC_ADDR                                                            (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_AGC_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_AGC_SZ                                                              8
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_AGC                                                                 (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_AGC_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_CMPRDET_OFFSET                                                      0x254
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_CMPRDET_ADDR                                                        (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_CMPRDET_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_CMPRDET_SZ                                                          8
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_CMPRDET                                                             (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_CMPRDET_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_CMPRDET_IBIAS_OFFSET                                                0x255
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_CMPRDET_IBIAS_ADDR                                                  (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_CMPRDET_IBIAS_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_CMPRDET_IBIAS_SZ                                                    8
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_CMPRDET_IBIAS                                                       (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_CMPRDET_IBIAS_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_PKDET_DYN_OFFSET                                                    0x256
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_PKDET_DYN_ADDR                                                      (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_PKDET_DYN_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_PKDET_DYN_SZ                                                        8
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_PKDET_DYN                                                           (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_PKDET_DYN_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_PKDET_CONFIG_OFFSET                                                 0x257
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_PKDET_CONFIG_ADDR                                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_PKDET_CONFIG_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_PKDET_CONFIG_SZ                                                     8
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_PKDET_CONFIG                                                        (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_PKDET_CONFIG_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_PKDET_IBIAS_OFFSET                                                  0x258
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_PKDET_IBIAS_ADDR                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_PKDET_IBIAS_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_PKDET_IBIAS_SZ                                                      8
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_PKDET_IBIAS                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_PKDET_IBIAS_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_DD2_IQ_IBIAS_OFFSET                                                     0x259
#define GALLIFREY_T22_V1A_ANA_TRX_RF_DD2_IQ_IBIAS_ADDR                                                       (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_DD2_IQ_IBIAS_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_DD2_IQ_IBIAS_SZ                                                         8
#define GALLIFREY_T22_V1A_ANA_TRX_RF_DD2_IQ_IBIAS                                                            (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_DD2_IQ_IBIAS_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_RFSENSE_OFFSET                                                          0x25a
#define GALLIFREY_T22_V1A_ANA_TRX_RF_RFSENSE_ADDR                                                            (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_RFSENSE_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_RFSENSE_SZ                                                              16
#define GALLIFREY_T22_V1A_ANA_TRX_RF_RFSENSE                                                                 (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_RFSENSE_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_BQF_TSMC_CONFIG_OFFSET                                              0x25c
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_BQF_TSMC_CONFIG_ADDR                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_BQF_TSMC_CONFIG_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_BQF_TSMC_CONFIG_SZ                                                  8
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_BQF_TSMC_CONFIG                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_BQF_TSMC_CONFIG_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_VREF_PA_OFFSET                                                          0x25d
#define GALLIFREY_T22_V1A_ANA_TRX_RF_VREF_PA_ADDR                                                            (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_VREF_PA_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_VREF_PA_SZ                                                              8
#define GALLIFREY_T22_V1A_ANA_TRX_RF_VREF_PA                                                                 (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_VREF_PA_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_DD_IF_OFFSET                                                            0x25e
#define GALLIFREY_T22_V1A_ANA_TRX_RF_DD_IF_ADDR                                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_DD_IF_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_DD_IF_SZ                                                                8
#define GALLIFREY_T22_V1A_ANA_TRX_RF_DD_IF                                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_DD_IF_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_CONFIG_OFFSET                                                    0x25f
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_CONFIG_ADDR                                                      (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_CONFIG_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_CONFIG_SZ                                                        8
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_CONFIG                                                           (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_CONFIG_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_DYN_OFFSET                                                       0x260
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_DYN_ADDR                                                         (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_DYN_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_DYN_SZ                                                           8
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_DYN                                                              (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_DYN_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_LNA_PREPA_TSMC_CONFIG_OFFSET                                        0x261
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_LNA_PREPA_TSMC_CONFIG_ADDR                                          (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_LNA_PREPA_TSMC_CONFIG_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_LNA_PREPA_TSMC_CONFIG_SZ                                            8
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_LNA_PREPA_TSMC_CONFIG                                               (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_LNA_PREPA_TSMC_CONFIG_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_VREF_LNA_PREPA_BQF_OFFSET                                               0x262
#define GALLIFREY_T22_V1A_ANA_TRX_RF_VREF_LNA_PREPA_BQF_ADDR                                                 (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_VREF_LNA_PREPA_BQF_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_VREF_LNA_PREPA_BQF_SZ                                                   8
#define GALLIFREY_T22_V1A_ANA_TRX_RF_VREF_LNA_PREPA_BQF                                                      (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_VREF_LNA_PREPA_BQF_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_OFFSET                                                                  0x263
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADDR                                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_SZ                                                                      8
#define GALLIFREY_T22_V1A_ANA_TRX_BB                                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ATB_CONFIG_OFFSET                                                       0x264
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ATB_CONFIG_ADDR                                                         (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_ATB_CONFIG_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ATB_CONFIG_SZ                                                           16
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ATB_CONFIG                                                              (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_ATB_CONFIG_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_BLOCK_ENA_OFFSET                                                        0x266
#define GALLIFREY_T22_V1A_ANA_TRX_BB_BLOCK_ENA_ADDR                                                          (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_BLOCK_ENA_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_BLOCK_ENA_SZ                                                            8
#define GALLIFREY_T22_V1A_ANA_TRX_BB_BLOCK_ENA                                                               (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_BLOCK_ENA_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_IBIAS_OFFSET                                                       0x267
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_IBIAS_ADDR                                                         (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_IBIAS_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_IBIAS_SZ                                                           8
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_IBIAS                                                              (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_IBIAS_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_LOWER_LEVEL_OFFSET                                                      0x268
#define GALLIFREY_T22_V1A_ANA_TRX_BB_LOWER_LEVEL_ADDR                                                        (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_LOWER_LEVEL_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_LOWER_LEVEL_SZ                                                          32
#define GALLIFREY_T22_V1A_ANA_TRX_BB_LOWER_LEVEL                                                             (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_LOWER_LEVEL_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_BB_PKDET_OFFSET                                                         0x26c
#define GALLIFREY_T22_V1A_ANA_TRX_BB_BB_PKDET_ADDR                                                           (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_BB_PKDET_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_BB_PKDET_SZ                                                             16
#define GALLIFREY_T22_V1A_ANA_TRX_BB_BB_PKDET                                                                (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_BB_PKDET_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_CONFIG_OFFSET                                                      0x26e
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_CONFIG_ADDR                                                        (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_CONFIG_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_CONFIG_SZ                                                          8
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_CONFIG                                                             (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_CONFIG_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_OFFSET                                                             0x26f
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_ADDR                                                               (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_SZ                                                                 8
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS                                                                    (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_ATB_OFFSET                                                         0x270
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_ATB_ADDR                                                           (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_ATB_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_ATB_SZ                                                             16
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_ATB                                                                (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_ATB_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_POW_ENA_OFFSET                                                     0x272
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_POW_ENA_ADDR                                                       (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_POW_ENA_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_POW_ENA_SZ                                                         8
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_POW_ENA                                                            (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_POW_ENA_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_CORE_0_DYN_OFFSET                                                  0x273
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_CORE_0_DYN_ADDR                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_CORE_0_DYN_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_CORE_0_DYN_SZ                                                      8
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_CORE_0_DYN                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_CORE_0_DYN_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_CORE_1_DYN_OFFSET                                                  0x274
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_CORE_1_DYN_ADDR                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_CORE_1_DYN_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_CORE_1_DYN_SZ                                                      8
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_CORE_1_DYN                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_CORE_1_DYN_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_BUFF_CONFIG_OFFSET                                                 0x275
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_BUFF_CONFIG_ADDR                                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_BUFF_CONFIG_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_BUFF_CONFIG_SZ                                                     8
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_BUFF_CONFIG                                                        (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_BUFF_CONFIG_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_AMP_DET_DYN_OFFSET                                                 0x276
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_AMP_DET_DYN_ADDR                                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_AMP_DET_DYN_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_AMP_DET_DYN_SZ                                                     8
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_AMP_DET_DYN                                                        (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_AMP_DET_DYN_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_AMP_DET_CONFIG_OFFSET                                              0x277
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_AMP_DET_CONFIG_ADDR                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_AMP_DET_CONFIG_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_AMP_DET_CONFIG_SZ                                                  8
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_AMP_DET_CONFIG                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_AMP_DET_CONFIG_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_VREF_OFFSET                                                    0x278
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_VREF_ADDR                                                      (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_VREF_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_VREF_SZ                                                        8
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_VREF                                                           (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_VREF_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_ADC_TSMC_CONFIG_OFFSET                                         0x279
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_ADC_TSMC_CONFIG_ADDR                                           (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_ADC_TSMC_CONFIG_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_ADC_TSMC_CONFIG_SZ                                             8
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_ADC_TSMC_CONFIG                                                (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_ADC_TSMC_CONFIG_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_BUFFER_TSMC_CONFIG_OFFSET                                      0x27a
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_BUFFER_TSMC_CONFIG_ADDR                                        (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_BUFFER_TSMC_CONFIG_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_BUFFER_TSMC_CONFIG_SZ                                          8
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_BUFFER_TSMC_CONFIG                                             (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_BUFFER_TSMC_CONFIG_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BQF_DC_COMP_TSMC_0_CAL_OFFSET                                 0x27b
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BQF_DC_COMP_TSMC_0_CAL_ADDR                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BQF_DC_COMP_TSMC_0_CAL_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BQF_DC_COMP_TSMC_0_CAL_SZ                                     8
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BQF_DC_COMP_TSMC_0_CAL                                        (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BQF_DC_COMP_TSMC_0_CAL_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BQF_DC_COMP_TSMC_1_CAL_OFFSET                                 0x27c
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BQF_DC_COMP_TSMC_1_CAL_ADDR                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BQF_DC_COMP_TSMC_1_CAL_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BQF_DC_COMP_TSMC_1_CAL_SZ                                     8
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BQF_DC_COMP_TSMC_1_CAL                                        (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BQF_DC_COMP_TSMC_1_CAL_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BQF_DC_COMP_TSMC_0_CAL_OFFSET                                 0x27d
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BQF_DC_COMP_TSMC_0_CAL_ADDR                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BQF_DC_COMP_TSMC_0_CAL_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BQF_DC_COMP_TSMC_0_CAL_SZ                                     8
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BQF_DC_COMP_TSMC_0_CAL                                        (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BQF_DC_COMP_TSMC_0_CAL_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BQF_DC_COMP_TSMC_1_CAL_OFFSET                                 0x27e
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BQF_DC_COMP_TSMC_1_CAL_ADDR                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BQF_DC_COMP_TSMC_1_CAL_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BQF_DC_COMP_TSMC_1_CAL_SZ                                     8
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BQF_DC_COMP_TSMC_1_CAL                                        (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BQF_DC_COMP_TSMC_1_CAL_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_VREF_LNA_PREPA_BQF_OFFSET                                                  0x27f
#define GALLIFREY_T22_V1A_ANA_TRX_VREF_LNA_PREPA_BQF_ADDR                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_VREF_LNA_PREPA_BQF_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_VREF_LNA_PREPA_BQF_SZ                                                      8
#define GALLIFREY_T22_V1A_ANA_TRX_VREF_LNA_PREPA_BQF                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_VREF_LNA_PREPA_BQF_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_ADPLL_DYN_OFFSET                                                           0x280
#define GALLIFREY_T22_V1A_ANA_TRX_ADPLL_DYN_ADDR                                                             (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_ADPLL_DYN_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_ADPLL_DYN_SZ                                                               8
#define GALLIFREY_T22_V1A_ANA_TRX_ADPLL_DYN                                                                  (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_ADPLL_DYN_ADDR
#define GALLIFREY_T22_V1A_REFE_CONFIG_OFFSET                                                                 0x281
#define GALLIFREY_T22_V1A_REFE_CONFIG_ADDR                                                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_REFE_CONFIG_OFFSET)
#define GALLIFREY_T22_V1A_REFE_CONFIG_SZ                                                                     8
#define GALLIFREY_T22_V1A_REFE_CONFIG                                                                        (volatile uint8_t *)GALLIFREY_T22_V1A_REFE_CONFIG_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_ADPLL_ATB_OFFSET                                                           0x282
#define GALLIFREY_T22_V1A_ANA_TRX_ADPLL_ATB_ADDR                                                             (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_ADPLL_ATB_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_ADPLL_ATB_SZ                                                               16
#define GALLIFREY_T22_V1A_ANA_TRX_ADPLL_ATB                                                                  (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_ADPLL_ATB_ADDR
#define GALLIFREY_T22_V1A_REFE_ATB_OFFSET                                                                    0x284
#define GALLIFREY_T22_V1A_REFE_ATB_ADDR                                                                      (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_REFE_ATB_OFFSET)
#define GALLIFREY_T22_V1A_REFE_ATB_SZ                                                                        16
#define GALLIFREY_T22_V1A_REFE_ATB                                                                           (volatile uint8_t *)GALLIFREY_T22_V1A_REFE_ATB_ADDR
#define GALLIFREY_T22_V1A_REFE_BLOCK_ENA_OFFSET                                                              0x286
#define GALLIFREY_T22_V1A_REFE_BLOCK_ENA_ADDR                                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_REFE_BLOCK_ENA_OFFSET)
#define GALLIFREY_T22_V1A_REFE_BLOCK_ENA_SZ                                                                  8
#define GALLIFREY_T22_V1A_REFE_BLOCK_ENA                                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_REFE_BLOCK_ENA_ADDR
#define GALLIFREY_T22_V1A_REFE_XTAL_FREQ_TRIM_OFFSET                                                         0x287
#define GALLIFREY_T22_V1A_REFE_XTAL_FREQ_TRIM_ADDR                                                           (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_REFE_XTAL_FREQ_TRIM_OFFSET)
#define GALLIFREY_T22_V1A_REFE_XTAL_FREQ_TRIM_SZ                                                             8
#define GALLIFREY_T22_V1A_REFE_XTAL_FREQ_TRIM                                                                (volatile uint8_t *)GALLIFREY_T22_V1A_REFE_XTAL_FREQ_TRIM_ADDR
#define GALLIFREY_T22_V1A_REFE_XTAL_OFFSET                                                                   0x288
#define GALLIFREY_T22_V1A_REFE_XTAL_ADDR                                                                     (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_REFE_XTAL_OFFSET)
#define GALLIFREY_T22_V1A_REFE_XTAL_SZ                                                                       32
#define GALLIFREY_T22_V1A_REFE_XTAL                                                                          (volatile uint8_t *)GALLIFREY_T22_V1A_REFE_XTAL_ADDR
#define GALLIFREY_T22_V1A_REFE_TS_CONFIG_OFFSET                                                              0x28c
#define GALLIFREY_T22_V1A_REFE_TS_CONFIG_ADDR                                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_REFE_TS_CONFIG_OFFSET)
#define GALLIFREY_T22_V1A_REFE_TS_CONFIG_SZ                                                                  8
#define GALLIFREY_T22_V1A_REFE_TS_CONFIG                                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_REFE_TS_CONFIG_ADDR
#define GALLIFREY_T22_V1A_COMMANDS_OFFSET                                                                    0x2a0
#define GALLIFREY_T22_V1A_COMMANDS_ADDR                                                                      (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_COMMANDS_OFFSET)
#define GALLIFREY_T22_V1A_COMMANDS_SZ                                                                        32
#define GALLIFREY_T22_V1A_COMMANDS                                                                           (volatile uint8_t *)GALLIFREY_T22_V1A_COMMANDS_ADDR
#define GALLIFREY_T22_V1A_CHANNEL_OFFSET                                                                     0x2a4
#define GALLIFREY_T22_V1A_CHANNEL_ADDR                                                                       (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_CHANNEL_OFFSET)
#define GALLIFREY_T22_V1A_CHANNEL_SZ                                                                         32
#define GALLIFREY_T22_V1A_CHANNEL                                                                            (volatile uint8_t *)GALLIFREY_T22_V1A_CHANNEL_ADDR
#define GALLIFREY_T22_V1A_PROT_TIMER_OFFSET                                                                  0x2a8
#define GALLIFREY_T22_V1A_PROT_TIMER_ADDR                                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_PROT_TIMER_OFFSET)
#define GALLIFREY_T22_V1A_PROT_TIMER_SZ                                                                      32
#define GALLIFREY_T22_V1A_PROT_TIMER                                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_PROT_TIMER_ADDR
#define GALLIFREY_T22_V1A_STATUS_OFFSET                                                                      0x2ac
#define GALLIFREY_T22_V1A_STATUS_ADDR                                                                        (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_STATUS_OFFSET)
#define GALLIFREY_T22_V1A_STATUS_SZ                                                                          8
#define GALLIFREY_T22_V1A_STATUS                                                                             (volatile uint8_t *)GALLIFREY_T22_V1A_STATUS_ADDR
#define GALLIFREY_T22_V1A_RXPH_STATUS_OFFSET                                                                 0x2ad
#define GALLIFREY_T22_V1A_RXPH_STATUS_ADDR                                                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_RXPH_STATUS_OFFSET)
#define GALLIFREY_T22_V1A_RXPH_STATUS_SZ                                                                     8
#define GALLIFREY_T22_V1A_RXPH_STATUS                                                                        (volatile uint8_t *)GALLIFREY_T22_V1A_RXPH_STATUS_ADDR
#define GALLIFREY_T22_V1A_OBSERVE_POINT_0_OFFSET                                                             0x2b0
#define GALLIFREY_T22_V1A_OBSERVE_POINT_0_ADDR                                                               (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_OBSERVE_POINT_0_OFFSET)
#define GALLIFREY_T22_V1A_OBSERVE_POINT_0_SZ                                                                 32
#define GALLIFREY_T22_V1A_OBSERVE_POINT_0                                                                    (volatile uint8_t *)GALLIFREY_T22_V1A_OBSERVE_POINT_0_ADDR
#define GALLIFREY_T22_V1A_OBSERVE_POINT_1_OFFSET                                                             0x2b4
#define GALLIFREY_T22_V1A_OBSERVE_POINT_1_ADDR                                                               (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_OBSERVE_POINT_1_OFFSET)
#define GALLIFREY_T22_V1A_OBSERVE_POINT_1_SZ                                                                 32
#define GALLIFREY_T22_V1A_OBSERVE_POINT_1                                                                    (volatile uint8_t *)GALLIFREY_T22_V1A_OBSERVE_POINT_1_ADDR
#define GALLIFREY_T22_V1A_RX_DATA_OUT_OFFSET                                                                 0x2b8
#define GALLIFREY_T22_V1A_RX_DATA_OUT_ADDR                                                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_RX_DATA_OUT_OFFSET)
#define GALLIFREY_T22_V1A_RX_DATA_OUT_SZ                                                                     32
#define GALLIFREY_T22_V1A_RX_DATA_OUT                                                                        (volatile uint8_t *)GALLIFREY_T22_V1A_RX_DATA_OUT_ADDR
#define GALLIFREY_T22_V1A_TX_FIFO_OFFSET                                                                     0x2bc
#define GALLIFREY_T22_V1A_TX_FIFO_ADDR                                                                       (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_TX_FIFO_OFFSET)
#define GALLIFREY_T22_V1A_TX_FIFO_SZ                                                                         32
#define GALLIFREY_T22_V1A_TX_FIFO                                                                            (volatile uint8_t *)GALLIFREY_T22_V1A_TX_FIFO_ADDR
#define GALLIFREY_T22_V1A_TX_FIFO_32_OFFSET                                                                  0x2c0
#define GALLIFREY_T22_V1A_TX_FIFO_32_ADDR                                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_TX_FIFO_32_OFFSET)
#define GALLIFREY_T22_V1A_TX_FIFO_32_SZ                                                                      32
#define GALLIFREY_T22_V1A_TX_FIFO_32                                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_TX_FIFO_32_ADDR
#define GALLIFREY_T22_V1A_TX_FIFO_COUNT_OFFSET                                                               0x2c4
#define GALLIFREY_T22_V1A_TX_FIFO_COUNT_ADDR                                                                 (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_TX_FIFO_COUNT_OFFSET)
#define GALLIFREY_T22_V1A_TX_FIFO_COUNT_SZ                                                                   8
#define GALLIFREY_T22_V1A_TX_FIFO_COUNT                                                                      (volatile uint8_t *)GALLIFREY_T22_V1A_TX_FIFO_COUNT_ADDR
#define GALLIFREY_T22_V1A_TX_FIFO_STATUS_OFFSET                                                              0x2c8
#define GALLIFREY_T22_V1A_TX_FIFO_STATUS_ADDR                                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_TX_FIFO_STATUS_OFFSET)
#define GALLIFREY_T22_V1A_TX_FIFO_STATUS_SZ                                                                  32
#define GALLIFREY_T22_V1A_TX_FIFO_STATUS                                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_TX_FIFO_STATUS_ADDR
#define GALLIFREY_T22_V1A_RX_FIFO_OFFSET                                                                     0x2cc
#define GALLIFREY_T22_V1A_RX_FIFO_ADDR                                                                       (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_RX_FIFO_OFFSET)
#define GALLIFREY_T22_V1A_RX_FIFO_SZ                                                                         32
#define GALLIFREY_T22_V1A_RX_FIFO                                                                            (volatile uint8_t *)GALLIFREY_T22_V1A_RX_FIFO_ADDR
#define GALLIFREY_T22_V1A_RX_FIFO_32_OFFSET                                                                  0x2d0
#define GALLIFREY_T22_V1A_RX_FIFO_32_ADDR                                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_RX_FIFO_32_OFFSET)
#define GALLIFREY_T22_V1A_RX_FIFO_32_SZ                                                                      32
#define GALLIFREY_T22_V1A_RX_FIFO_32                                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_RX_FIFO_32_ADDR
#define GALLIFREY_T22_V1A_RX_FIFO_COUNT_OFFSET                                                               0x2d4
#define GALLIFREY_T22_V1A_RX_FIFO_COUNT_ADDR                                                                 (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_RX_FIFO_COUNT_OFFSET)
#define GALLIFREY_T22_V1A_RX_FIFO_COUNT_SZ                                                                   8
#define GALLIFREY_T22_V1A_RX_FIFO_COUNT                                                                      (volatile uint8_t *)GALLIFREY_T22_V1A_RX_FIFO_COUNT_ADDR
#define GALLIFREY_T22_V1A_RX_FIFO_STATUS_OFFSET                                                              0x2d8
#define GALLIFREY_T22_V1A_RX_FIFO_STATUS_ADDR                                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_RX_FIFO_STATUS_OFFSET)
#define GALLIFREY_T22_V1A_RX_FIFO_STATUS_SZ                                                                  32
#define GALLIFREY_T22_V1A_RX_FIFO_STATUS                                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_RX_FIFO_STATUS_ADDR
#define GALLIFREY_T22_V1A_IQ_FIFO_OFFSET                                                                     0x2dc
#define GALLIFREY_T22_V1A_IQ_FIFO_ADDR                                                                       (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_IQ_FIFO_OFFSET)
#define GALLIFREY_T22_V1A_IQ_FIFO_SZ                                                                         32
#define GALLIFREY_T22_V1A_IQ_FIFO                                                                            (volatile uint8_t *)GALLIFREY_T22_V1A_IQ_FIFO_ADDR
#define GALLIFREY_T22_V1A_IQ_FIFO_32_OFFSET                                                                  0x2e0
#define GALLIFREY_T22_V1A_IQ_FIFO_32_ADDR                                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_IQ_FIFO_32_OFFSET)
#define GALLIFREY_T22_V1A_IQ_FIFO_32_SZ                                                                      32
#define GALLIFREY_T22_V1A_IQ_FIFO_32                                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_IQ_FIFO_32_ADDR
#define GALLIFREY_T22_V1A_IQ_FIFO_COUNT_OFFSET                                                               0x2e4
#define GALLIFREY_T22_V1A_IQ_FIFO_COUNT_ADDR                                                                 (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_IQ_FIFO_COUNT_OFFSET)
#define GALLIFREY_T22_V1A_IQ_FIFO_COUNT_SZ                                                                   8
#define GALLIFREY_T22_V1A_IQ_FIFO_COUNT                                                                      (volatile uint8_t *)GALLIFREY_T22_V1A_IQ_FIFO_COUNT_ADDR
#define GALLIFREY_T22_V1A_IQ_FIFO_STATUS_OFFSET                                                              0x2e8
#define GALLIFREY_T22_V1A_IQ_FIFO_STATUS_ADDR                                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_IQ_FIFO_STATUS_OFFSET)
#define GALLIFREY_T22_V1A_IQ_FIFO_STATUS_SZ                                                                  32
#define GALLIFREY_T22_V1A_IQ_FIFO_STATUS                                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_IQ_FIFO_STATUS_ADDR
#define GALLIFREY_T22_V1A_DCO_CAL_READOUT_OFFSET                                                             0x2ec
#define GALLIFREY_T22_V1A_DCO_CAL_READOUT_ADDR                                                               (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DCO_CAL_READOUT_OFFSET)
#define GALLIFREY_T22_V1A_DCO_CAL_READOUT_SZ                                                                 8
#define GALLIFREY_T22_V1A_DCO_CAL_READOUT                                                                    (volatile uint8_t *)GALLIFREY_T22_V1A_DCO_CAL_READOUT_ADDR
#define GALLIFREY_T22_V1A_ANALOG_INFO_OFFSET                                                                 0x2ed
#define GALLIFREY_T22_V1A_ANALOG_INFO_ADDR                                                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANALOG_INFO_OFFSET)
#define GALLIFREY_T22_V1A_ANALOG_INFO_SZ                                                                     8
#define GALLIFREY_T22_V1A_ANALOG_INFO                                                                        (volatile uint8_t *)GALLIFREY_T22_V1A_ANALOG_INFO_ADDR
#define GALLIFREY_T22_V1A_RC_CALIBRATION_OFFSET                                                              0x2ee
#define GALLIFREY_T22_V1A_RC_CALIBRATION_ADDR                                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_RC_CALIBRATION_OFFSET)
#define GALLIFREY_T22_V1A_RC_CALIBRATION_SZ                                                                  8
#define GALLIFREY_T22_V1A_RC_CALIBRATION                                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_RC_CALIBRATION_ADDR
#define GALLIFREY_T22_V1A_RSSI_PKT_OFFSET                                                                    0x2ef
#define GALLIFREY_T22_V1A_RSSI_PKT_ADDR                                                                      (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_RSSI_PKT_OFFSET)
#define GALLIFREY_T22_V1A_RSSI_PKT_SZ                                                                        8
#define GALLIFREY_T22_V1A_RSSI_PKT                                                                           (volatile uint8_t *)GALLIFREY_T22_V1A_RSSI_PKT_ADDR
#define GALLIFREY_T22_V1A_IRQ_STATUS_OFFSET                                                                  0x2f0
#define GALLIFREY_T22_V1A_IRQ_STATUS_ADDR                                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_IRQ_STATUS_OFFSET)
#define GALLIFREY_T22_V1A_IRQ_STATUS_SZ                                                                      32
#define GALLIFREY_T22_V1A_IRQ_STATUS                                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_IRQ_STATUS_ADDR
#define GALLIFREY_T22_V1A_RSSI_LIN_OFFSET                                                                    0x2f4
#define GALLIFREY_T22_V1A_RSSI_LIN_ADDR                                                                      (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_RSSI_LIN_OFFSET)
#define GALLIFREY_T22_V1A_RSSI_LIN_SZ                                                                        16
#define GALLIFREY_T22_V1A_RSSI_LIN                                                                           (volatile uint8_t *)GALLIFREY_T22_V1A_RSSI_LIN_ADDR
#define GALLIFREY_T22_V1A_AGC_ATTENUATION_OFFSET                                                             0x2f6
#define GALLIFREY_T22_V1A_AGC_ATTENUATION_ADDR                                                               (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_AGC_ATTENUATION_OFFSET)
#define GALLIFREY_T22_V1A_AGC_ATTENUATION_SZ                                                                 8
#define GALLIFREY_T22_V1A_AGC_ATTENUATION                                                                    (volatile uint8_t *)GALLIFREY_T22_V1A_AGC_ATTENUATION_ADDR
#define GALLIFREY_T22_V1A_AGC_STATUS_OFFSET                                                                  0x2f7
#define GALLIFREY_T22_V1A_AGC_STATUS_ADDR                                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_AGC_STATUS_OFFSET)
#define GALLIFREY_T22_V1A_AGC_STATUS_SZ                                                                      8
#define GALLIFREY_T22_V1A_AGC_STATUS                                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_AGC_STATUS_ADDR
#define GALLIFREY_T22_V1A_RSSI_OFFSET                                                                        0x2f8
#define GALLIFREY_T22_V1A_RSSI_ADDR                                                                          (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_RSSI_OFFSET)
#define GALLIFREY_T22_V1A_RSSI_SZ                                                                            24
#define GALLIFREY_T22_V1A_RSSI                                                                               (volatile uint8_t *)GALLIFREY_T22_V1A_RSSI_ADDR
#define GALLIFREY_T22_V1A_AES_STATUS_OFFSET                                                                  0x2fb
#define GALLIFREY_T22_V1A_AES_STATUS_ADDR                                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_AES_STATUS_OFFSET)
#define GALLIFREY_T22_V1A_AES_STATUS_SZ                                                                      8
#define GALLIFREY_T22_V1A_AES_STATUS                                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_AES_STATUS_ADDR
#define GALLIFREY_T22_V1A_CFO_OFFSET                                                                         0x2fc
#define GALLIFREY_T22_V1A_CFO_ADDR                                                                           (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_CFO_OFFSET)
#define GALLIFREY_T22_V1A_CFO_SZ                                                                             16
#define GALLIFREY_T22_V1A_CFO                                                                                (volatile uint8_t *)GALLIFREY_T22_V1A_CFO_ADDR
#define GALLIFREY_T22_V1A_IQ_BALANCE_READOUT_OFFSET                                                          0x2fe
#define GALLIFREY_T22_V1A_IQ_BALANCE_READOUT_ADDR                                                            (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_IQ_BALANCE_READOUT_OFFSET)
#define GALLIFREY_T22_V1A_IQ_BALANCE_READOUT_SZ                                                              16
#define GALLIFREY_T22_V1A_IQ_BALANCE_READOUT                                                                 (volatile uint8_t *)GALLIFREY_T22_V1A_IQ_BALANCE_READOUT_ADDR
#define GALLIFREY_T22_V1A_TX_POWER_OFFSET                                                                    0x300
#define GALLIFREY_T22_V1A_TX_POWER_ADDR                                                                      (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_TX_POWER_OFFSET)
#define GALLIFREY_T22_V1A_TX_POWER_SZ                                                                        8
#define GALLIFREY_T22_V1A_TX_POWER                                                                           (volatile uint8_t *)GALLIFREY_T22_V1A_TX_POWER_ADDR
#define GALLIFREY_T22_V1A_ICYTRXDM_FEATURES_OFFSET                                                           0x301
#define GALLIFREY_T22_V1A_ICYTRXDM_FEATURES_ADDR                                                             (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ICYTRXDM_FEATURES_OFFSET)
#define GALLIFREY_T22_V1A_ICYTRXDM_FEATURES_SZ                                                               8
#define GALLIFREY_T22_V1A_ICYTRXDM_FEATURES                                                                  (volatile uint8_t *)GALLIFREY_T22_V1A_ICYTRXDM_FEATURES_ADDR
#define GALLIFREY_T22_V1A_REGISTERS_HASH_OFFSET                                                              0x3f8
#define GALLIFREY_T22_V1A_REGISTERS_HASH_ADDR                                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_REGISTERS_HASH_OFFSET)
#define GALLIFREY_T22_V1A_REGISTERS_HASH_SZ                                                                  32
#define GALLIFREY_T22_V1A_REGISTERS_HASH                                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_REGISTERS_HASH_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_TECHNO_DEPENDANT_OFFSET                                             0x116
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_TECHNO_DEPENDANT_ADDR                                               (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_TECHNO_DEPENDANT_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_TECHNO_DEPENDANT_SZ                                                 16
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_TECHNO_DEPENDANT                                                    (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_TECHNO_DEPENDANT_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_0_AGC_ATT_NOT_USED_OFFSET                                 0x11c
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_0_AGC_ATT_NOT_USED_ADDR                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_0_AGC_ATT_NOT_USED_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_0_AGC_ATT_NOT_USED_SZ                                     16
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_0_AGC_ATT_NOT_USED                                        (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_0_AGC_ATT_NOT_USED_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_1_AGC_ATT_NOT_USED_OFFSET                                 0x11f
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_1_AGC_ATT_NOT_USED_ADDR                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_1_AGC_ATT_NOT_USED_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_1_AGC_ATT_NOT_USED_SZ                                     24
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_1_AGC_ATT_NOT_USED                                        (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_1_AGC_ATT_NOT_USED_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_2_OFFSET                                                  0x11f
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_2_ADDR                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_2_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_2_SZ                                                      40
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_2                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_2_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_3_OFFSET                                                  0x11f
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_3_ADDR                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_3_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_3_SZ                                                      40
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_3                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_3_ADDR
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_4_OFFSET                                                  0x11f
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_4_ADDR                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_4_OFFSET)
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_4_SZ                                                      40
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_4                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_4_ADDR
#define GALLIFREY_T22_V1A_LDO_DIG_LDO_PARAM_NOT_USED_IN_TECHNO_OFFSET                                        0x248
#define GALLIFREY_T22_V1A_LDO_DIG_LDO_PARAM_NOT_USED_IN_TECHNO_ADDR                                          (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_LDO_DIG_LDO_PARAM_NOT_USED_IN_TECHNO_OFFSET)
#define GALLIFREY_T22_V1A_LDO_DIG_LDO_PARAM_NOT_USED_IN_TECHNO_SZ                                            8
#define GALLIFREY_T22_V1A_LDO_DIG_LDO_PARAM_NOT_USED_IN_TECHNO                                               (volatile uint8_t *)GALLIFREY_T22_V1A_LDO_DIG_LDO_PARAM_NOT_USED_IN_TECHNO_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_PARAM_NOT_USED_IN_TECHNO_OFFSET                                            0x249
#define GALLIFREY_T22_V1A_ANA_TRX_PARAM_NOT_USED_IN_TECHNO_ADDR                                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_PARAM_NOT_USED_IN_TECHNO_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_PARAM_NOT_USED_IN_TECHNO_SZ                                                24
#define GALLIFREY_T22_V1A_ANA_TRX_PARAM_NOT_USED_IN_TECHNO                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_PARAM_NOT_USED_IN_TECHNO_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_PARAM_NOT_USED_IN_TECHNO_OFFSET                                      0x250
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_PARAM_NOT_USED_IN_TECHNO_ADDR                                        (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_PA_PARAM_NOT_USED_IN_TECHNO_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_PARAM_NOT_USED_IN_TECHNO_SZ                                          16
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_PARAM_NOT_USED_IN_TECHNO                                             (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_PA_PARAM_NOT_USED_IN_TECHNO_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_BQF_PARAM_NOT_USED_IN_TECHNO_OFFSET                                 0x25d
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_BQF_PARAM_NOT_USED_IN_TECHNO_ADDR                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_BQF_PARAM_NOT_USED_IN_TECHNO_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_BQF_PARAM_NOT_USED_IN_TECHNO_SZ                                     8
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_BQF_PARAM_NOT_USED_IN_TECHNO                                        (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_BQF_PARAM_NOT_USED_IN_TECHNO_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_PALOW_PARAM_NOT_USED_IN_TECHNO_OFFSET                               0x25d
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_PALOW_PARAM_NOT_USED_IN_TECHNO_ADDR                                 (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_PALOW_PARAM_NOT_USED_IN_TECHNO_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_PALOW_PARAM_NOT_USED_IN_TECHNO_SZ                                   16
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_PALOW_PARAM_NOT_USED_IN_TECHNO                                      (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_PALOW_PARAM_NOT_USED_IN_TECHNO_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_PAHIGH_PARAM_NOT_USED_IN_TECHNO_OFFSET                              0x25d
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_PAHIGH_PARAM_NOT_USED_IN_TECHNO_ADDR                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_PAHIGH_PARAM_NOT_USED_IN_TECHNO_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_PAHIGH_PARAM_NOT_USED_IN_TECHNO_SZ                                  16
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_PAHIGH_PARAM_NOT_USED_IN_TECHNO                                     (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_PAHIGH_PARAM_NOT_USED_IN_TECHNO_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_VREF_LNA_BQF_PARAM_NOT_USED_IN_TECHNO_OFFSET                            0x25e
#define GALLIFREY_T22_V1A_ANA_TRX_RF_VREF_LNA_BQF_PARAM_NOT_USED_IN_TECHNO_ADDR                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_VREF_LNA_BQF_PARAM_NOT_USED_IN_TECHNO_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_VREF_LNA_BQF_PARAM_NOT_USED_IN_TECHNO_SZ                                8
#define GALLIFREY_T22_V1A_ANA_TRX_RF_VREF_LNA_BQF_PARAM_NOT_USED_IN_TECHNO                                   (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_VREF_LNA_BQF_PARAM_NOT_USED_IN_TECHNO_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_MXP_PARAM_NOT_USED_IN_TECHNO_OFFSET                                     0x25e
#define GALLIFREY_T22_V1A_ANA_TRX_RF_MXP_PARAM_NOT_USED_IN_TECHNO_ADDR                                       (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_MXP_PARAM_NOT_USED_IN_TECHNO_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_MXP_PARAM_NOT_USED_IN_TECHNO_SZ                                         8
#define GALLIFREY_T22_V1A_ANA_TRX_RF_MXP_PARAM_NOT_USED_IN_TECHNO                                            (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_MXP_PARAM_NOT_USED_IN_TECHNO_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_DD2_GF_PARAM_NOT_USED_IN_TECHNO_OFFSET                                  0x25e
#define GALLIFREY_T22_V1A_ANA_TRX_RF_DD2_GF_PARAM_NOT_USED_IN_TECHNO_ADDR                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_DD2_GF_PARAM_NOT_USED_IN_TECHNO_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_DD2_GF_PARAM_NOT_USED_IN_TECHNO_SZ                                      16
#define GALLIFREY_T22_V1A_ANA_TRX_RF_DD2_GF_PARAM_NOT_USED_IN_TECHNO                                         (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_DD2_GF_PARAM_NOT_USED_IN_TECHNO_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_CONFIG_CONST_OFFSET                                              0x25f
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_CONFIG_CONST_ADDR                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_CONFIG_CONST_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_CONFIG_CONST_SZ                                                  8
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_CONFIG_CONST                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_CONFIG_CONST_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_DUMMY_ONLY_HERE_TO_HAVE_TB_COMPILE_OFFSET                        0x261
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_DUMMY_ONLY_HERE_TO_HAVE_TB_COMPILE_ADDR                          (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_DUMMY_ONLY_HERE_TO_HAVE_TB_COMPILE_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_DUMMY_ONLY_HERE_TO_HAVE_TB_COMPILE_SZ                            8
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_DUMMY_ONLY_HERE_TO_HAVE_TB_COMPILE                               (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_DUMMY_ONLY_HERE_TO_HAVE_TB_COMPILE_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_LNA_PREPA_PARAM_NOT_USED_IN_TECHNO_OFFSET                           0x262
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_LNA_PREPA_PARAM_NOT_USED_IN_TECHNO_ADDR                             (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_LNA_PREPA_PARAM_NOT_USED_IN_TECHNO_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_LNA_PREPA_PARAM_NOT_USED_IN_TECHNO_SZ                               8
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_LNA_PREPA_PARAM_NOT_USED_IN_TECHNO                                  (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_LNA_PREPA_PARAM_NOT_USED_IN_TECHNO_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_ADC_PARAM_NOT_USED_IN_TECHNO_OFFSET                            0x27a
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_ADC_PARAM_NOT_USED_IN_TECHNO_ADDR                              (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_ADC_PARAM_NOT_USED_IN_TECHNO_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_ADC_PARAM_NOT_USED_IN_TECHNO_SZ                                8
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_ADC_PARAM_NOT_USED_IN_TECHNO                                   (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_ADC_PARAM_NOT_USED_IN_TECHNO_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_BUFFER_PARAM_NOT_USED_IN_TECHNO_OFFSET                         0x27b
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_BUFFER_PARAM_NOT_USED_IN_TECHNO_ADDR                           (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_BUFFER_PARAM_NOT_USED_IN_TECHNO_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_BUFFER_PARAM_NOT_USED_IN_TECHNO_SZ                             8
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_BUFFER_PARAM_NOT_USED_IN_TECHNO                                (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_BUFFER_PARAM_NOT_USED_IN_TECHNO_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BUFF_PARAM_NOT_USED_IN_TECHNO_OFFSET                          0x27b
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BUFF_PARAM_NOT_USED_IN_TECHNO_ADDR                            (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BUFF_PARAM_NOT_USED_IN_TECHNO_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BUFF_PARAM_NOT_USED_IN_TECHNO_SZ                              8
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BUFF_PARAM_NOT_USED_IN_TECHNO                                 (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BUFF_PARAM_NOT_USED_IN_TECHNO_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BQF_DC_COMP_GF_PARAM_NOT_USED_IN_TECHNO_OFFSET                0x27d
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BQF_DC_COMP_GF_PARAM_NOT_USED_IN_TECHNO_ADDR                  (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BQF_DC_COMP_GF_PARAM_NOT_USED_IN_TECHNO_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BQF_DC_COMP_GF_PARAM_NOT_USED_IN_TECHNO_SZ                    16
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BQF_DC_COMP_GF_PARAM_NOT_USED_IN_TECHNO                       (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BQF_DC_COMP_GF_PARAM_NOT_USED_IN_TECHNO_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BUFF_PARAM_NOT_USED_IN_TECHNO_OFFSET                          0x27d
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BUFF_PARAM_NOT_USED_IN_TECHNO_ADDR                            (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BUFF_PARAM_NOT_USED_IN_TECHNO_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BUFF_PARAM_NOT_USED_IN_TECHNO_SZ                              8
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BUFF_PARAM_NOT_USED_IN_TECHNO                                 (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BUFF_PARAM_NOT_USED_IN_TECHNO_ADDR
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BQF_DC_COMP_GF_PARAM_NOT_USED_IN_TECHNO_OFFSET                0x27f
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BQF_DC_COMP_GF_PARAM_NOT_USED_IN_TECHNO_ADDR                  (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BQF_DC_COMP_GF_PARAM_NOT_USED_IN_TECHNO_OFFSET)
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BQF_DC_COMP_GF_PARAM_NOT_USED_IN_TECHNO_SZ                    16
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BQF_DC_COMP_GF_PARAM_NOT_USED_IN_TECHNO                       (volatile uint8_t *)GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BQF_DC_COMP_GF_PARAM_NOT_USED_IN_TECHNO_ADDR
#define GALLIFREY_T22_V1A_REFE_LDO_MISC_LDO_PARAM_NOT_USED_IN_TECHNO_OFFSET                                  0x287
#define GALLIFREY_T22_V1A_REFE_LDO_MISC_LDO_PARAM_NOT_USED_IN_TECHNO_ADDR                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_REFE_LDO_MISC_LDO_PARAM_NOT_USED_IN_TECHNO_OFFSET)
#define GALLIFREY_T22_V1A_REFE_LDO_MISC_LDO_PARAM_NOT_USED_IN_TECHNO_SZ                                      8
#define GALLIFREY_T22_V1A_REFE_LDO_MISC_LDO_PARAM_NOT_USED_IN_TECHNO                                         (volatile uint8_t *)GALLIFREY_T22_V1A_REFE_LDO_MISC_LDO_PARAM_NOT_USED_IN_TECHNO_ADDR
#define GALLIFREY_T22_V1A_CHIP_ID_OFFSET                                                                     0x3ff
#define GALLIFREY_T22_V1A_CHIP_ID_ADDR                                                                       (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_CHIP_ID_OFFSET)
#define GALLIFREY_T22_V1A_CHIP_ID_SZ                                                                         8
#define GALLIFREY_T22_V1A_CHIP_ID                                                                            (volatile uint8_t *)GALLIFREY_T22_V1A_CHIP_ID_ADDR
#define GALLIFREY_T22_V1A_RATE_TX_OFFSET                                                                     0x800
#define GALLIFREY_T22_V1A_RATE_TX_ADDR                                                                       (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_RATE_TX_OFFSET)
#define GALLIFREY_T22_V1A_RATE_TX_SZ                                                                         8
#define GALLIFREY_T22_V1A_RATE_TX                                                                            (volatile uint8_t *)GALLIFREY_T22_V1A_RATE_TX_ADDR
#define GALLIFREY_T22_V1A_CHANNEL_TX_OFFSET                                                                  0x801
#define GALLIFREY_T22_V1A_CHANNEL_TX_ADDR                                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_CHANNEL_TX_OFFSET)
#define GALLIFREY_T22_V1A_CHANNEL_TX_SZ                                                                      8
#define GALLIFREY_T22_V1A_CHANNEL_TX                                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_CHANNEL_TX_ADDR
#define GALLIFREY_T22_V1A_TX_PA_POWER_OFFSET                                                                 0x802
#define GALLIFREY_T22_V1A_TX_PA_POWER_ADDR                                                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_TX_PA_POWER_OFFSET)
#define GALLIFREY_T22_V1A_TX_PA_POWER_SZ                                                                     8
#define GALLIFREY_T22_V1A_TX_PA_POWER                                                                        (volatile uint8_t *)GALLIFREY_T22_V1A_TX_PA_POWER_ADDR
#define GALLIFREY_T22_V1A_TX_STARTUP_OFFSET                                                                  0x803
#define GALLIFREY_T22_V1A_TX_STARTUP_ADDR                                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_TX_STARTUP_OFFSET)
#define GALLIFREY_T22_V1A_TX_STARTUP_SZ                                                                      8
#define GALLIFREY_T22_V1A_TX_STARTUP                                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_TX_STARTUP_ADDR
#define GALLIFREY_T22_V1A_RATE_RX_OFFSET                                                                     0x810
#define GALLIFREY_T22_V1A_RATE_RX_ADDR                                                                       (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_RATE_RX_OFFSET)
#define GALLIFREY_T22_V1A_RATE_RX_SZ                                                                         8
#define GALLIFREY_T22_V1A_RATE_RX                                                                            (volatile uint8_t *)GALLIFREY_T22_V1A_RATE_RX_ADDR
#define GALLIFREY_T22_V1A_CHANNEL_RX_OFFSET                                                                  0x811
#define GALLIFREY_T22_V1A_CHANNEL_RX_ADDR                                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_CHANNEL_RX_OFFSET)
#define GALLIFREY_T22_V1A_CHANNEL_RX_SZ                                                                      8
#define GALLIFREY_T22_V1A_CHANNEL_RX                                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_CHANNEL_RX_ADDR
#define GALLIFREY_T22_V1A_RX_STARTUP_OFFSET                                                                  0x812
#define GALLIFREY_T22_V1A_RX_STARTUP_ADDR                                                                    (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_RX_STARTUP_OFFSET)
#define GALLIFREY_T22_V1A_RX_STARTUP_SZ                                                                      8
#define GALLIFREY_T22_V1A_RX_STARTUP                                                                         (volatile uint8_t *)GALLIFREY_T22_V1A_RX_STARTUP_ADDR
#define GALLIFREY_T22_V1A_SYNC_WORD_LOW_OFFSET                                                               0x813
#define GALLIFREY_T22_V1A_SYNC_WORD_LOW_ADDR                                                                 (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_SYNC_WORD_LOW_OFFSET)
#define GALLIFREY_T22_V1A_SYNC_WORD_LOW_SZ                                                                   8
#define GALLIFREY_T22_V1A_SYNC_WORD_LOW                                                                      (volatile uint8_t *)GALLIFREY_T22_V1A_SYNC_WORD_LOW_ADDR
#define GALLIFREY_T22_V1A_SYNC_WORD_HIGH_OFFSET                                                              0x817
#define GALLIFREY_T22_V1A_SYNC_WORD_HIGH_ADDR                                                                (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_SYNC_WORD_HIGH_OFFSET)
#define GALLIFREY_T22_V1A_SYNC_WORD_HIGH_SZ                                                                  8
#define GALLIFREY_T22_V1A_SYNC_WORD_HIGH                                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_SYNC_WORD_HIGH_ADDR
#define GALLIFREY_T22_V1A_RX_POWEROFF_OFFSET                                                                 0x820
#define GALLIFREY_T22_V1A_RX_POWEROFF_ADDR                                                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_RX_POWEROFF_OFFSET)
#define GALLIFREY_T22_V1A_RX_POWEROFF_SZ                                                                     8
#define GALLIFREY_T22_V1A_RX_POWEROFF                                                                        (volatile uint8_t *)GALLIFREY_T22_V1A_RX_POWEROFF_ADDR
#define GALLIFREY_T22_V1A_RSSI_READOUT_OFFSET                                                                0x824
#define GALLIFREY_T22_V1A_RSSI_READOUT_ADDR                                                                  (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_RSSI_READOUT_OFFSET)
#define GALLIFREY_T22_V1A_RSSI_READOUT_SZ                                                                    8
#define GALLIFREY_T22_V1A_RSSI_READOUT                                                                       (volatile uint8_t *)GALLIFREY_T22_V1A_RSSI_READOUT_ADDR
#define GALLIFREY_T22_V1A_PACKET_LENGTH_OFFSET                                                               0x828
#define GALLIFREY_T22_V1A_PACKET_LENGTH_ADDR                                                                 (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_PACKET_LENGTH_OFFSET)
#define GALLIFREY_T22_V1A_PACKET_LENGTH_SZ                                                                   8
#define GALLIFREY_T22_V1A_PACKET_LENGTH                                                                      (volatile uint8_t *)GALLIFREY_T22_V1A_PACKET_LENGTH_ADDR
#define GALLIFREY_T22_V1A_CFO_READOUT_OFFSET                                                                 0x82c
#define GALLIFREY_T22_V1A_CFO_READOUT_ADDR                                                                   (GALLIFREY_T22_V1A_BASE + GALLIFREY_T22_V1A_CFO_READOUT_OFFSET)
#define GALLIFREY_T22_V1A_CFO_READOUT_SZ                                                                     8
#define GALLIFREY_T22_V1A_CFO_READOUT                                                                        (volatile uint8_t *)GALLIFREY_T22_V1A_CFO_READOUT_ADDR


#define GALLIFREY_T22_V1A_DIG_USE_IQ_FIELD                                                          {1,1,0}
#define GALLIFREY_T22_V1A_DIG_USE_CUSTOM_IFACE_FIELD                                                {0,1,0}
#define GALLIFREY_T22_V1A_DIG_IFACE_APB_SAFE_FIELD                                                  {15,1,0}
#define GALLIFREY_T22_V1A_DIG_IFACE_APB_BUS_ASYNC_FIELD                                             {14,1,0}
#define GALLIFREY_T22_V1A_DIG_IFACE_APB_TIMEOUT_FIELD                                               {9,5,0}
#define GALLIFREY_T22_V1A_DIG_RRMU_SPI_FORWARD_FIELD                                                {8,1,0}
#define GALLIFREY_T22_V1A_DIG_IFACE_PASSWD_FIELD                                                    {16,16,0}
#define GALLIFREY_T22_V1A_DIG_IFACE_OCTA_SPI_SLV_MISO_PD_FIELD                                      {35,1,0}
#define GALLIFREY_T22_V1A_DIG_IFACE_OCTA_SPI_SLV_MISO_PU_FIELD                                      {34,1,0}
#define GALLIFREY_T22_V1A_DIG_IFACE_OCTA_SPI_SLV_MISO_HIZ_FIELD                                     {33,1,0}
#define GALLIFREY_T22_V1A_DIG_IFACE_OCTA_SPI_SLV_IS_SYNC_FIELD                                      {32,1,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_RB_FIELD                                                        {43,1,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_DRV_FIELD                                                       {41,2,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_USE_EXT_SIGNALS_FIELD                                           {40,1,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_9_PD_FIELD                                                 {57,1,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_8_PD_FIELD                                                 {56,1,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_7_PD_FIELD                                                 {55,1,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_6_PD_FIELD                                                 {54,1,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_5_PD_FIELD                                                 {53,1,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_4_PD_FIELD                                                 {52,1,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_3_PD_FIELD                                                 {51,1,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_2_PD_FIELD                                                 {50,1,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_1_PD_FIELD                                                 {49,1,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_0_PD_FIELD                                                 {48,1,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_9_PU_FIELD                                                 {73,1,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_8_PU_FIELD                                                 {72,1,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_7_PU_FIELD                                                 {71,1,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_6_PU_FIELD                                                 {70,1,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_5_PU_FIELD                                                 {69,1,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_4_PU_FIELD                                                 {68,1,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_3_PU_FIELD                                                 {67,1,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_2_PU_FIELD                                                 {66,1,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_1_PU_FIELD                                                 {65,1,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_0_PU_FIELD                                                 {64,1,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_0_CONF_FIELD                                               {80,6,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_1_CONF_FIELD                                               {88,6,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_2_CONF_FIELD                                               {96,6,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_3_CONF_FIELD                                               {104,6,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_4_CONF_FIELD                                               {112,6,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_5_CONF_FIELD                                               {120,6,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_6_CONF_FIELD                                               {128,6,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_7_CONF_FIELD                                               {136,6,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_8_CONF_FIELD                                               {144,6,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_GPIO_9_CONF_FIELD                                               {152,6,0}
#define GALLIFREY_T22_V1A_DIG_GPIOS_TEST_BUS_CONFIG_FIELD                                           {160,4,0}
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_HIZ_FIELD                                                    {170,1,0}
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_PULSE_FIELD                                                  {169,1,0}
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_ACTIVE_LOW_FIELD                                             {168,1,0}
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_TX_END_SIG_FIELD                                             {177,1,0}
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_TX_START_FIELD                                               {176,1,0}
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_TX_PH_END_SIG_FIELD                                          {185,1,0}
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_TX_PH_READ_FIELD                                             {184,1,0}
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_RX_SYNC_DETECT_FIELD                                         {194,1,0}
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_RX_END_SIG_FIELD                                             {193,1,0}
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_RX_START_FIELD                                               {192,1,0}
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_RX_PH_END_SIG_FIELD                                          {201,1,0}
#define GALLIFREY_T22_V1A_DIG_IRQ_CTRL_RX_PH_DATA_FIELD                                             {200,1,0}
#define GALLIFREY_T22_V1A_DIG_SEQ_CONFIG_FIELD                                                      {208,8,0}
#define GALLIFREY_T22_V1A_DIG_SEQ_SUB_CNT_FIELD                                                     {216,8,0}
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_0_TRIG_FIELD                                                 {232,4,0}
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_0_ADDR_FIELD                                                 {224,8,0}
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_1_TRIG_FIELD                                                 {248,4,0}
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_1_ADDR_FIELD                                                 {240,8,0}
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_2_TRIG_FIELD                                                 {264,4,0}
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_2_ADDR_FIELD                                                 {256,8,0}
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_3_TRIG_FIELD                                                 {280,4,0}
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_3_ADDR_FIELD                                                 {272,8,0}
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_4_TRIG_FIELD                                                 {296,4,0}
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_4_ADDR_FIELD                                                 {288,8,0}
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_5_TRIG_FIELD                                                 {312,4,0}
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_5_ADDR_FIELD                                                 {304,8,0}
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_6_TRIG_FIELD                                                 {328,4,0}
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_6_ADDR_FIELD                                                 {320,8,0}
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_7_TRIG_FIELD                                                 {344,4,0}
#define GALLIFREY_T22_V1A_DIG_SEQ_SEQS_7_ADDR_FIELD                                                 {336,8,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_SOFT_STOP_FIELD                                                {357,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_ALT_CLK_FIELD                                                  {356,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_CTE_DATA_TYPE_FIELD                                            {352,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CHANGE_DT_DEST_TYPE_FIELD                             {364,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CHANGE_DT_SRC_TYPE_FIELD                              {360,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DEMUX_BIT_SWITCH_1_TO_0_DT_FIELD                      {373,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DEMUX_BIT_SWITCH_0_TO_1_DT_FIELD                      {369,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DEMUX_BIT_FORWARD_FIELD                               {368,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_PH_DATA_TYPE_IN_FIELD                                          {446,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_PH_AES_MULT_ENC_FIELD                                          {445,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_PH_USES_SEQ_CONFIG_FIELD                                       {444,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_PH_TYPE_IN_FIELD                                               {440,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_PH_CONFIG_FIELD                                                {432,8,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_PH_AES_DATA_TYPE_FIELD                                         {416,16,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_PH_DATA_IN_FIELD                                               {384,32,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_INVERT_BIT_ENABLE_FIELD                               {456,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MUX_BIT_SWITCH_1_TO_0_DT_FIELD                        {469,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MUX_BIT_SWITCH_0_TO_1_DT_FIELD                        {465,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MUX_BIT_FORWARD_FIELD                                 {464,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MUX_INT_SWITCH_1_TO_0_DT_FIELD                        {485,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MUX_INT_SWITCH_0_TO_1_DT_FIELD                        {481,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MUX_INT_FORWARD_FIELD                                 {480,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_BIT_TO_DEST_2_FIELD                       {496,16,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_0_TO_DEST_2_FIELD                     {512,16,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SWITCH_DEST_INT_1_TO_DEST_2_FIELD                     {528,16,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BIT_REPEATER_REPEAT_COUNT_FIELD                       {560,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BIT_REPEATER_ENABLE_TYPE_FIELD                        {544,16,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DELAY_PATH_DEL_FIELD                                  {569,3,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DELAY_PATH_PATH1_NPATH2_FIELD                         {568,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_IEEE802154_B2C_ENABLE_TYPE_FIELD                      {576,16,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DATA_WHITENING_ENABLE_TYPE_FIELD                      {592,16,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CRC_CUSTOM_OUTPUT_MODE_DT_FIELD                       {624,16,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CRC_CUSTOM_MODE_TYPE_FIELD                            {608,16,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MANCHESTER_ENABLE_TYPE_FIELD                          {640,16,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PIO4_4_8PSK_ENABLE_TYPE_FIELD                         {656,16,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CONV_CODES_ENABLE_TYPE_FIELD                          {672,16,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_0_SGN2_FIELD                             {697,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_0_SGN1_FIELD                             {696,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_0_E2_FIELD                               {692,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_0_E1_FIELD                               {688,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_1_SGN2_FIELD                             {713,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_1_SGN1_FIELD                             {712,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_1_E2_FIELD                               {708,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BARREL_SHIFT_1_E1_FIELD                               {704,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_MAN_M2_FIELD                                     {726,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_MAN_M1_FIELD                                     {720,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_0_SGN2_FIELD                                     {757,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_0_SGN1_FIELD                                     {756,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_0_E2_FIELD                                       {752,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_0_E1_FIELD                                       {748,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_0_M2_FIELD                                       {742,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_0_M1_FIELD                                       {736,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_0_UPSCALE_FIELD                          {761,3,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_0_ORDER_FIELD                            {760,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_1_SGN2_FIELD                                     {789,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_1_SGN1_FIELD                                     {788,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_1_E2_FIELD                                       {784,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_1_E1_FIELD                                       {780,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_1_M2_FIELD                                       {774,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_1_M1_FIELD                                       {768,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_1_UPSCALE_FIELD                          {793,3,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_INTERPOLATOR_1_ORDER_FIELD                            {792,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_2_SGN2_FIELD                                     {821,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_2_SGN1_FIELD                                     {820,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_2_E2_FIELD                                       {816,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_2_E1_FIELD                                       {812,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_2_M2_FIELD                                       {806,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_MULT_2_M1_FIELD                                       {800,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DECIMATOR_ORDER_FIELD                                 {827,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DECIMATOR_DOWNSCALE_FIELD                             {824,3,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_GENERIC_MAPPER_ENABLE_TYPE_FIELD                      {837,16,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_GENERIC_MAPPER_PI_OVER_4_FIELD                        {836,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_GENERIC_MAPPER_DIFFERENTIAL_FIELD                     {835,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_GENERIC_MAPPER_MAP_TYPE_FIELD                         {832,3,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DERIVATIVE_FILT_NON_ACTIVE_FIELD                      {858,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_DERIVATIVE_PORTS_FIELD                                {856,2,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_DELAYED_DT_FIELD                         {1027,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_SYM0_2_FIELD                             {1019,8,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_SYM0_1_FIELD                             {1011,8,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_USE_ZERO_FIELD                           {1010,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_ENABLE_TYPE_FIELD                        {994,16,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_OSR_MODE_FIELD                           {992,2,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_PULSE_SHAPE_FIELD                        {864,128,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_AM2PM_GAIN_FIELD                                   {1032,3,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_FRACTIONAL_INTERPOLATOR_SKIP_FIRST_FIELD              {1051,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_FRACTIONAL_INTERPOLATOR_HIGH_GAIN_FIELD               {1050,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_FRACTIONAL_INTERPOLATOR_IS_SIGNED_FIELD               {1048,2,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_FRACTIONAL_INTERPOLATOR_DEN_FIELD                     {1044,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_FRACTIONAL_INTERPOLATOR_NUM_FIELD                     {1040,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_FSK_ENABLE_TYPE_FIELD                    {1186,16,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_FSK_DELAYED_DT_FIELD                     {1185,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_FSK_OSR_FIELD                            {1184,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PULSE_SHAPER_FSK_PULSE_SHAPE_FIELD                    {1056,128,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_SERIAL_FIFO_USE_SERIAL_FIFO_FIELD                              {1209,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_SERIAL_FIFO_QUAL_FIELD                                         {1208,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SATURATE_DELAY_DT_FIELD                               {1247,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SATURATE_PORTS_FIELD                                  {1245,2,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SATURATE_AMP_CORR_FIELD                               {1242,3,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SATURATE_AMP_LIMIT_FIELD                              {1229,13,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SATURATE_LIMIT_FIELD                                  {1216,13,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_BLOCK_DATA_ENABLE_TYPE_FIELD                          {1248,16,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_HOLD_DATA_ENABLE_TYPE_FIELD                           {1264,16,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CONST_DATA_CHANNEL_FIELD                              {1288,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_CONST_DATA_VALUE_FIELD                                {1280,8,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SKIP_DATA_SKIP_TYPE_FIELD                             {1304,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_SKIP_DATA_N_FIELD                                     {1296,8,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_RAMP_RD_TYPE_FIELD                                 {1398,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_RAMP_RU_TYPE_FIELD                                 {1394,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_RAMP_RD_WAIT_FIELD                                 {1388,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_RAMP_RU_WAIT_FIELD                                 {1382,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_RAMP_RAMP_DEC_2_FIELD                              {1374,8,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_RAMP_RAMP_INC_2_FIELD                              {1366,8,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_RAMP_RAMP_DEC_1_FIELD                              {1358,8,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_RAMP_RAMP_INC_1_FIELD                              {1350,8,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_RAMP_FINAL_POW_2_FIELD                             {1342,8,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_RAMP_INIT_POW_2_FIELD                              {1334,8,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_RAMP_FINAL_POW_1_FIELD                             {1326,8,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_RAMP_INIT_POW_1_FIELD                              {1318,8,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_RAMP_MULTIPLICATIVE_FIELD                          {1317,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_RAMP_RAMP_DP_FIELD                                 {1315,2,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_RAMP_SIN_RAMP_FIELD                                {1313,2,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_RAMP_ALIGN_RAMPS_FIELD                             {1312,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_AM2PM_LUT_FIELD                                    {1408,32,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_PA_LINEARIZE_LUT_FIELD                                {1440,120,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_PA_AMP_DELAY_FIELD                                             {1561,3,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_PA_AMP_OFFSET_FIELD                                            {1560,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_TX_BLE_DF_SWITCH_DELAY_FIELD                          {1586,8,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_TX_BLE_DF_CTE_INFO_FIELD                              {1578,8,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_TX_BLE_DF_CTE_DT_FIELD                                {1574,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_CONF_TX_BLE_DF_OSR_FIELD                                   {1568,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_CHANGE_DT_DEST_ADDR_FIELD                             {1618,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DEMUX_BIT_P1_DEST_ADDR_FIELD                          {1612,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DEMUX_BIT_P0_DEST_ADDR_FIELD                          {1606,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_TXPH_DEST_ADDR_FIELD                                  {1600,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_USE_LUT_FIELD                                           {1624,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_SWITCH_DEST_BIT_P1_DEST_ADDR_FIELD                    {1650,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_SWITCH_DEST_BIT_P0_DEST_ADDR_FIELD                    {1644,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_MUX_INT_DEST_ADDR_FIELD                               {1638,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_MUX_BIT_DEST_ADDR_FIELD                               {1632,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_PA_FILT_DISABLE_CK_GATING_FIELD                                {1661,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_PA_FILT_SEL_CK_FIELD                                           {1660,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_PA_FILT_ON_CK_PA_FIELD                                         {1659,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_PA_FILT_TUNE_FIELD                                             {1657,2,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_PA_FILT_E_FIELD                                                {1656,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_SWITCH_DEST_INT_1_P1_DEST_ADDR_FIELD                  {1682,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_SWITCH_DEST_INT_1_P0_DEST_ADDR_FIELD                  {1676,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_SWITCH_DEST_INT_0_P1_DEST_ADDR_FIELD                  {1670,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_SWITCH_DEST_INT_0_P0_DEST_ADDR_FIELD                  {1664,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_USE_RX_FSM_START_FIELD                                     {1689,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_BQF_DC_CAL_AGC_SEL_FIELD                                       {1688,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_IEEE802154_B2C_DEST_ADDR_FIELD                        {1714,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_BIT_REPEATER_DEST_ADDR_FIELD                          {1708,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_FIFO_4BITS_DEST_ADDR_FIELD                            {1702,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_INVERT_BIT_DEST_ADDR_FIELD                            {1696,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_INPUT_ADDR_FIELD                                           {1720,7,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_PIO4_4_8PSK_DEST_ADDR_FIELD                           {1746,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_MANCHESTER_DEST_ADDR_FIELD                            {1740,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DATA_WHITENING_DEST_ADDR_FIELD                        {1734,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_CRC_CUSTOM_DEST_ADDR_FIELD                            {1728,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_CTRL_FIELD                                                 {1752,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_MULT_MAN_DEST_ADDR_FIELD                              {1778,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_BARREL_SHIFT_1_DEST_ADDR_FIELD                        {1772,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_BARREL_SHIFT_0_DEST_ADDR_FIELD                        {1766,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_CONV_CODES_DEST_ADDR_FIELD                            {1760,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_TMR_PREDIV_FIELD                                           {1784,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DELAY_PATH_DEST_ADDR_FIELD                            {1810,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_MULT_2_DEST_ADDR_FIELD                                {1804,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_MULT_1_DEST_ADDR_FIELD                                {1798,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_MULT_0_DEST_ADDR_FIELD                                {1792,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_FAULT_ON_IO_ERR_FIELD                                      {1820,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_TRACE_TO_GPIO_FIELD                                        {1819,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_GP_TMR_IRQ_IS_HARD_FIELD                                   {1818,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_GPIO_IRQ_IS_HARD_FIELD                                     {1817,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_USE_TMR_FOR_BB_FAST_SST_FIELD                              {1816,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_INTERPOLATOR_0_DEST_ADDR_FIELD                        {1842,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_PULSE_SHAPER_FSK_DEST_ADDR_FIELD                      {1836,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_PULSE_SHAPER_DEST_ADDR_FIELD                          {1830,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_GENERIC_MAPPER_DEST_ADDR_FIELD                        {1824,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_0_IGNORE_ENA_FIELD                      {1852,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_0_ENA_FIELD                             {1851,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_0_SHIFT_AMOUNT_FIELD                    {1848,3,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_CORDIC_ANGLE_DEST_ADDR_FIELD                          {1874,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DECIMATOR_DEST_ADDR_FIELD                             {1868,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_FRACTIONAL_INTERPOLATOR_DEST_ADDR_FIELD               {1862,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_INTERPOLATOR_1_DEST_ADDR_FIELD                        {1856,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_PA_POWER_FIELD                                          {1880,8,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_HOLD_DATA_DEST_ADDR_FIELD                             {1906,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_BLOCK_DATA_DEST_ADDR_FIELD                            {1900,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_SATURATE_DEST_ADDR_FIELD                              {1894,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_DERIVATIVE_DEST_ADDR_FIELD                            {1888,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_1_IGNORE_ENA_FIELD                      {1916,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_1_ENA_FIELD                             {1915,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_OBSERVE_POINT_1_SHIFT_AMOUNT_FIELD                    {1912,3,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_PA_AM2PM_DEST_ADDR_FIELD                              {1938,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_PA_RAMP_DEST_ADDR_FIELD                               {1932,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_SKIP_DATA_DEST_ADDR_FIELD                             {1926,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_CONST_DATA_DEST_ADDR_FIELD                            {1920,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0_INIT_STATE_FIELD                   {1944,2,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_TX_BLE_DF_DEST_ADDR_FIELD                             {1958,6,1}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TDP_MAPS_PA_LINEARIZE_DEST_ADDR_FIELD                          {1952,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_0_IND_FIELD                                         {1980,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_0_VREF_TUNE_FIELD                                   {1976,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_0_LP_FIELD                                          {1975,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_0_RAMP_MAX_FIELD                                    {1968,7,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_1_IND_FIELD                                         {1996,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_1_VREF_TUNE_FIELD                                   {1992,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_1_LP_FIELD                                          {1991,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_1_RAMP_MAX_FIELD                                    {1984,7,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_2_IND_FIELD                                         {2012,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_2_VREF_TUNE_FIELD                                   {2008,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_2_LP_FIELD                                          {2007,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_2_RAMP_MAX_FIELD                                    {2000,7,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_3_IND_FIELD                                         {2028,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_3_VREF_TUNE_FIELD                                   {2024,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_3_LP_FIELD                                          {2023,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_3_RAMP_MAX_FIELD                                    {2016,7,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_4_IND_FIELD                                         {2044,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_4_VREF_TUNE_FIELD                                   {2040,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_4_LP_FIELD                                          {2039,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_4_RAMP_MAX_FIELD                                    {2032,7,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_5_IND_FIELD                                         {2060,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_5_VREF_TUNE_FIELD                                   {2056,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_5_LP_FIELD                                          {2055,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_5_RAMP_MAX_FIELD                                    {2048,7,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_6_IND_FIELD                                         {2076,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_6_VREF_TUNE_FIELD                                   {2072,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_6_LP_FIELD                                          {2071,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_6_RAMP_MAX_FIELD                                    {2064,7,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_7_IND_FIELD                                         {2092,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_7_VREF_TUNE_FIELD                                   {2088,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_7_LP_FIELD                                          {2087,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_7_RAMP_MAX_FIELD                                    {2080,7,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_8_IND_FIELD                                         {2108,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_8_VREF_TUNE_FIELD                                   {2104,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_8_LP_FIELD                                          {2103,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_8_RAMP_MAX_FIELD                                    {2096,7,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_9_IND_FIELD                                         {2124,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_9_VREF_TUNE_FIELD                                   {2120,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_9_LP_FIELD                                          {2119,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_TOP_TX_POW_LUT_9_RAMP_MAX_FIELD                                    {2112,7,0}
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_0_MULT_FIELD                                          {2142,2,0}
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_0_VAL_FIELD                                           {2128,14,0}
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_T_STP_3_FIELD                                                {2160,3,0}
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_T_STP_2_FIELD                                                {2157,3,0}
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_T_STP_1_FIELD                                                {2154,3,0}
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_T_STP_0_FIELD                                                {2151,3,0}
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_ENA_FIELD                                                    {2150,1,0}
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_US_TICK_FIELD                                                {2144,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_OSR_INIT_FIELD                                             {2168,8,0}
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_1_MULT_FIELD                                          {2190,2,0}
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_1_VAL_FIELD                                           {2176,14,0}
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_2_MULT_FIELD                                          {2206,2,0}
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_2_VAL_FIELD                                           {2192,14,0}
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_3_MULT_FIELD                                          {2222,2,0}
#define GALLIFREY_T22_V1A_DIG_PROT_TMR_DELTAS_3_VAL_FIELD                                           {2208,14,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_RF_FAST_IN_SST_PRESET_FIELD                                {2225,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_RF_GATE_FIELD                                              {2224,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_BB_GATE_IQ_1_FIELD                                         {2235,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_BB_GATE_IQ_0_FIELD                                         {2234,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_BB_GATE_DIG_FIELD                                          {2233,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_BB_GATE_ANA_FIELD                                          {2232,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_PH_CHAIN_DELAY_FIELD                                           {2249,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_PH_USES_SEQ_CONFIG_FIELD                                       {2248,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_PH_CONFIG_FIELD                                                {2240,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_0_CHAIN_DELAY_FIELD                  {2264,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_0_ATTEN_4_FIELD                                  {2292,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_0_ATTEN_3_FIELD                                  {2287,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_0_ATTEN_2_FIELD                                  {2282,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_0_ATTEN_1_FIELD                                  {2277,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_0_ATTEN_0_FIELD                                  {2272,5,1}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_1_ATTEN_3_FIELD                                  {2319,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_1_ATTEN_2_FIELD                                  {2314,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_1_ATTEN_1_FIELD                                  {2309,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_1_ATTEN_0_FIELD                                  {2304,5,1}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1_INIT_STATE_FIELD                   {2328,2,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CTRL_SWITCH_DEST_1_CHAIN_DELAY_FIELD                  {2336,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_0_PORT_1_DT_FIELD                      {2344,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_1_PORT_1_DT_FIELD                      {2352,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_2_PORT_1_DT_FIELD                      {2360,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_3_PORT_1_DT_FIELD                      {2368,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_4_PORT_1_DT_FIELD                      {2376,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_DEST_5_PORT_1_DT_FIELD                      {2384,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0_INIT_DT_FIELD                             {2392,3,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_X4_DEST_LVL1_PORT_1_DT_FIELD                {2408,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DT_SWITCH_X4_DEST_LVL0_PORT_1_DT_FIELD                {2400,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_0_CHAIN_DELAY_FIELD                         {2416,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1_INIT_DT_FIELD                             {2424,3,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CHANGE_DT_1_CHAIN_DELAY_FIELD                         {2432,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_0_DOWNSCALE_FIELD                           {2442,4,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_0_ORDER_FIELD                               {2440,2,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_1_DOWNSCALE_FIELD                           {2450,4,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_1_ORDER_FIELD                               {2448,2,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_2_DOWNSCALE_FIELD                           {2458,4,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_2_ORDER_FIELD                               {2456,2,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_ADC_LIMIT_DETECT_SIG_LOW_TIME_DI_FIELD                {2490,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_ADC_LIMIT_DETECT_SIG_LOW_THRESH_DI_FIELD              {2482,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_ADC_LIMIT_DETECT_SOFT_CLIP_THRESH_DI_FIELD            {2474,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_ADC_LIMIT_DETECT_USE_ASYM_SI_FIELD                    {2473,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_ADC_LIMIT_DETECT_USE_BITMASK_SI_FIELD                 {2472,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_ADC_LIMIT_DETECT_SOFT_CLIP_DI_FIELD                   {2468,4,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_ADC_LIMIT_DETECT_HARD_CLIP_DI_FIELD                   {2464,4,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_HP_FILTER_KEEP_CAL_FIELD                              {2506,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_HP_FILTER_TAU_FIELD                                   {2504,2,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_REMOVE_IF_FLUSH_FIELD                                 {2524,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_REMOVE_IF_ANGLE_INC_FIELD                             {2512,12,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IQ_BALANCE_CAL_PHA_INIT_FIELD                         {2544,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IQ_BALANCE_CAL_AMP_INIT_FIELD                         {2536,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IQ_BALANCE_KEEP_CAL_FIELD                             {2535,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IQ_BALANCE_TAU_FIELD                                  {2530,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IQ_BALANCE_DYNAMIC_POST_SYNC_FIELD                    {2529,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IQ_BALANCE_ENABLE_DYNAMIC_FIELD                       {2528,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_3_DOWNSCALE_FIELD                           {2554,4,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DECIMATOR_3_ORDER_FIELD                               {2552,2,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FRACTIONAL_DECIMATOR_DEN_FIELD                        {2564,4,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FRACTIONAL_DECIMATOR_NUM_FIELD                        {2560,4,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORDIC_LIN2POL_FLUSH_FIELD                            {2568,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DERIVATIVE_FILT_NON_ACTIVE_FIELD                      {2578,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DERIVATIVE_PORTS_FIELD                                {2576,2,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DIFF_BY_N_N_FIELD                                     {2584,4,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_MAX_ERRORS_FIELD                           {2593,4,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_MODE_FIELD                                 {2592,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_CHAIN_DELAY_FIELD                          {2600,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_AVG_CMD_FIELD                              {2616,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_SYNC_CMD_FIELD                             {2608,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_FULL_ISI_FIELD                             {2657,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_DO_FILTER_FIELD                            {2656,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_USE_SYNC_DETECT_FIELD                      {2655,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_DELAY_MODE_1_FIELD                         {2649,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_RESYNC_DELAY_FIELD                         {2643,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_PATTERN_BIAS_FIELD                         {2637,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_SINGLE_PEAK_FIELD                          {2636,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_ISI_THR_FIELD                              {2632,4,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_GAIN_FIELD                                 {2630,2,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CORRELATOR_PATTERN_LEN_FIELD                          {2624,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0_GAIN_EXP_FIELD                       {2664,4,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1_GAIN_EXP_FIELD                       {2672,4,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_CTE_INFO_FIELD                              {2680,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_0_PS_COEFS_FIELD                       {2688,64,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MATCHED_FILTER_1_PS_COEFS_FIELD                       {2752,32,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_EXT_SAMPLE_TYPE_FIELD                       {2793,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_USE_EXT_SAMPLE_FIELD                        {2792,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_IS_CTE_FIELD                                {2791,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_AOA_SLOT_TIME_FIELD                         {2790,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_OSR_FIELD                                   {2784,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_RX_PATH_DELAY_FIELD                         {2800,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_EXT_SAMPLE_FIELD                            {2809,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_EXT_IS_CTE_FIELD                            {2808,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RX_BLE_DF_CHAIN_DELAY_FIELD                           {2816,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_SEND_INTERSYMB_FIELD                   {2831,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_PRESAMPLE_FIELD                        {2830,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_SAMPLE_BASED_FIELD                     {2829,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_OSR_FIELD                              {2824,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CLOCK_RECOVERY_CHAIN_DELAY_FIELD                      {2832,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_PHASE_INTERP_X2_DELAY_DATA_FIELD                      {2840,3,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DPSK_TED_ERR_INC_FIELD                                {2856,4,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DPSK_TED_TC_FIELD                                     {2848,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FSK_TED_TC_FIELD                                      {2864,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_DEMAPPER_ENABLE_TYPE_FIELD                        {2872,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CARRIER_RECOVERY_LP_FILTER_TAU_FIELD                  {2893,2,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CARRIER_RECOVERY_TAU_PHASE_ERR_FIELD                  {2887,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CARRIER_RECOVERY_TAU_FREQ_ERR_FIELD                   {2881,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CARRIER_RECOVERY_PHASE_MODE_FIELD                     {2880,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_RESYNC_DELAY_FIELD                    {2906,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_MARGIN_PREAMBLE_FIELD                 {2901,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_MARGIN_FIELD                          {2896,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_SYNC_DETECT_CHAIN_DELAY_FIELD                     {2912,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_CHAIN_DELAY_FIELD                     {2920,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_ACCEPTED_ERRS_FIELD                   {2935,2,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_RESYNC_DELAY_FIELD                    {2930,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_USE_SYNC_DETECT_FIELD                 {2929,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_EDR_SYNC_DETECT_SEND_PHASE_CORR_FIELD                 {2928,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DPSK_DEMOD_FILTER_SAMPLES_FIELD                       {2945,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_DPSK_DEMOD_COHERENT_FIELD                             {2944,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_CHAIN_DELAY_FIELD                     {2952,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_BLR_DELAY_CHAIN_DELAY_FIELD                           {2960,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_SUBSAMPLE_OSR_FIELD                                   {2968,4,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_COUNTER_OSR_FIELD                     {2993,4,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_USE_SYNC_DETECT_FIELD                 {2992,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_CMD_SYNC_FOUND_FIELD                  {2984,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_IEEE_802154_C2B_PATTERN_FIELD                         {2976,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_CRC_CUSTOM_MODE_INIT_FIELD                            {3000,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_FREQ_ERR_CMD_FIELD                       {3041,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_SEND_FREQ_ERR_FIELD                      {3040,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_SOFT_GAIN_FIELD                          {3038,2,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_USE_SOFT_DEC_FIELD                       {3037,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_USE_LAST_BIT_FIELD                       {3036,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_LAST_BIT_FIELD                           {3035,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_DEPTH_FIELD                              {3032,3,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_AMP_111_FIELD                            {3024,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_AMP_011_FIELD                            {3016,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_GFSK_AMP_010_FIELD                            {3008,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_USE_SYNC_DETECT_FIELD                    {3060,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_BLR_DONT_FLUSH_FIELD                     {3059,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_MEM_OUT_FIELD                            {3057,2,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_BLR_E_FIELD                              {3056,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_BLR_PACKET_LEN_FIELD                     {3064,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_BLR_CMD_ERR1_FIELD                       {3088,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_BLR_CMD_OK2_FIELD                        {3080,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_VITERBI_CONV_BLR_CMD_OK1_FIELD                        {3072,8,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RSSI_READOUT_NRST_MIN_MAX_FIELD                       {3100,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_RSSI_READOUT_TAU_FIELD                                {3096,4,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_LIMIT_RATE_DATA_SIZE_FIELD                            {3112,2,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_LIMIT_RATE_CLOCK_MODE_FIELD                           {3110,2,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_LIMIT_RATE_OSR_FIELD                                  {3104,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FLUSH_LEN_FIELD                                       {3126,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FLUSH_OSR_FIELD                                       {3120,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_FLUSH_CHAIN_DELAY_FIELD                               {3136,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_CTE_QUAL_FIELD                                                 {3150,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_CTE_AUTO_PULL_FIELD                                            {3149,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_CTE_EXT_IQ_SMP_TYPE_FIELD                                      {3148,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_CTE_Q_NI_FIRST_FIELD                                           {3147,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_CTE_IQ_MSB_FIRST_FIELD                                         {3146,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_CTE_IQ_DATA_BUS_SIZE_FIELD                                     {3144,2,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_ADC_CTRL_SWITCH_I_Q_FIELD                                      {3158,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_ADC_CTRL_INVERT_DUTY_CYCLE_FIELD                               {3157,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_ADC_CTRL_INVERT_CLK_FIELD                                      {3156,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_ADC_CTRL_CLK_DIV_FIELD                                         {3152,4,0}
#define GALLIFREY_T22_V1A_DIG_CLK_CTRL_TX_CK_ALT_FIELD                                              {3163,1,0}
#define GALLIFREY_T22_V1A_DIG_CLK_CTRL_ACCEPT_CK_PAD_FIELD                                          {3162,1,0}
#define GALLIFREY_T22_V1A_DIG_CLK_CTRL_SEL_CK_DIG_NXO_FIELD                                         {3161,1,0}
#define GALLIFREY_T22_V1A_DIG_CLK_CTRL_SYS_CK_E_FIELD                                               {3160,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_0_SGN2_FIELD                                     {3189,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_0_SGN1_FIELD                                     {3188,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_0_E2_FIELD                                       {3184,4,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_0_E1_FIELD                                       {3180,4,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_0_M2_FIELD                                       {3174,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_0_M1_FIELD                                       {3168,6,0}
#define GALLIFREY_T22_V1A_DIG_AES_BYTE_ORDER_FIELD                                                  {3193,1,0}
#define GALLIFREY_T22_V1A_DIG_AES_RX_MULTIPLE_ENCODING_FIELD                                        {3192,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_1_SGN2_FIELD                                     {3221,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_1_SGN1_FIELD                                     {3220,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_1_E2_FIELD                                       {3216,4,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_1_E1_FIELD                                       {3212,4,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_1_M2_FIELD                                       {3206,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_1_M1_FIELD                                       {3200,6,0}
#define GALLIFREY_T22_V1A_DIG_IQ_FIFO_FLUSH_ON_COND_FIELD                                           {3225,1,0}
#define GALLIFREY_T22_V1A_DIG_IQ_FIFO_USE_IT_FIELD                                                  {3224,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_2_SGN2_FIELD                                     {3253,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_2_SGN1_FIELD                                     {3252,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_2_E2_FIELD                                       {3248,4,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_2_E1_FIELD                                       {3244,4,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_2_M2_FIELD                                       {3238,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_2_M1_FIELD                                       {3232,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_FIFO_FLUSH_ON_COND_FIELD                                           {3257,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_FIFO_USE_IT_FIELD                                                  {3256,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_3_SGN2_FIELD                                     {3285,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_3_SGN1_FIELD                                     {3284,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_3_E2_FIELD                                       {3280,4,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_3_E1_FIELD                                       {3276,4,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_3_M2_FIELD                                       {3270,6,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_CONF_MULT_3_M1_FIELD                                       {3264,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_FIFO_FLUSH_ON_COND_FIELD                                           {3289,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_FIFO_USE_IT_FIELD                                                  {3288,1,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_MEM_EL_1_DEST_ADDR_FIELD                              {3317,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_MEM_EL_0_DEST_ADDR_FIELD                              {3310,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_RXPH_DEST_ADDR_FIELD                                  {3303,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_INPUT_DEST_ADDR_FIELD                                 {3296,7,1}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_TEE_0_P0_DEST_ADDR_FIELD                              {3349,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_OBSERVE_POINT_1_DEST_ADDR_FIELD                       {3342,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_OBSERVE_POINT_0_DEST_ADDR_FIELD                       {3335,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_MEM_EL_2_DEST_ADDR_FIELD                              {3328,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_TEE_2_P0_DEST_ADDR_FIELD                              {3381,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_TEE_1_P1_DEST_ADDR_FIELD                              {3374,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_TEE_1_P0_DEST_ADDR_FIELD                              {3367,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_TEE_0_P1_DEST_ADDR_FIELD                              {3360,7,1}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_TEE4_P0_DEST_ADDR_FIELD                               {3413,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_TEE_3_P1_DEST_ADDR_FIELD                              {3406,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_TEE_3_P0_DEST_ADDR_FIELD                              {3399,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_TEE_2_P1_DEST_ADDR_FIELD                              {3392,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_SPLIT_DATA_P0_DEST_ADDR_FIELD                         {3445,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_TEE4_P3_DEST_ADDR_FIELD                               {3438,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_TEE4_P2_DEST_ADDR_FIELD                               {3431,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_TEE4_P1_DEST_ADDR_FIELD                               {3424,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_CTRL_SWITCH_DEST_1_P0_DEST_ADDR_FIELD                 {3477,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_CTRL_SWITCH_DEST_0_P1_DEST_ADDR_FIELD                 {3470,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_CTRL_SWITCH_DEST_0_P0_DEST_ADDR_FIELD                 {3463,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_SPLIT_DATA_P1_DEST_ADDR_FIELD                         {3456,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_DEST_1_P0_DEST_ADDR_FIELD                   {3509,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_DEST_0_P1_DEST_ADDR_FIELD                   {3502,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_DEST_0_P0_DEST_ADDR_FIELD                   {3495,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_CTRL_SWITCH_DEST_1_P1_DEST_ADDR_FIELD                 {3488,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_DEST_3_P0_DEST_ADDR_FIELD                   {3541,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_DEST_2_P1_DEST_ADDR_FIELD                   {3534,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_DEST_2_P0_DEST_ADDR_FIELD                   {3527,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_DEST_1_P1_DEST_ADDR_FIELD                   {3520,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_DEST_5_P0_DEST_ADDR_FIELD                   {3573,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_DEST_4_P1_DEST_ADDR_FIELD                   {3566,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_DEST_4_P0_DEST_ADDR_FIELD                   {3559,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_DEST_3_P1_DEST_ADDR_FIELD                   {3552,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_X4_DEST_P2_DEST_ADDR_FIELD                  {3605,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_X4_DEST_P1_DEST_ADDR_FIELD                  {3598,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_X4_DEST_P0_DEST_ADDR_FIELD                  {3591,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_DEST_5_P1_DEST_ADDR_FIELD                   {3584,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_IQ_BALANCE_DEST_ADDR_FIELD                            {3637,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_CHANGE_DT_1_DEST_ADDR_FIELD                           {3630,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_CHANGE_DT_0_DEST_ADDR_FIELD                           {3623,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DT_SWITCH_X4_DEST_P3_DEST_ADDR_FIELD                  {3616,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DECIMATOR_1_DEST_ADDR_FIELD                           {3669,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DECIMATOR_0_DEST_ADDR_FIELD                           {3662,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_REMOVE_IF_DEST_ADDR_FIELD                             {3655,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_HP_FILTER_DEST_ADDR_FIELD                             {3648,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_CHANNEL_FILTER_FIR_DEST_ADDR_FIELD                    {3701,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_FRACTIONAL_DECIMATOR_DEST_ADDR_FIELD                  {3694,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DECIMATOR_3_DEST_ADDR_FIELD                           {3687,7,1}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DECIMATOR_2_DEST_ADDR_FIELD                           {3680,7,1}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_CORRELATOR_P0_DEST_ADDR_FIELD                         {3733,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DIFF_BY_N_DEST_ADDR_FIELD                             {3726,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DERIVATIVE_DEST_ADDR_FIELD                            {3719,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_CORDIC_LIN2POL_DEST_ADDR_FIELD                        {3712,7,1}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_CLOCK_RECOVERY_P0_DEST_ADDR_FIELD                     {3765,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_MATCHED_FILTER_1_DEST_ADDR_FIELD                      {3758,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_MATCHED_FILTER_0_DEST_ADDR_FIELD                      {3751,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_CORRELATOR_P1_DEST_ADDR_FIELD                         {3744,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_FSK_TED_DEST_ADDR_FIELD                               {3797,7,1}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_PHASE_INTERP_X2_DEST_ADDR_FIELD                       {3790,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DPSK_TED_DEST_ADDR_FIELD                              {3783,7,1}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_CLOCK_RECOVERY_P1_DEST_ADDR_FIELD                     {3776,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_BLR_SYNC_DETECT_P1_DEST_ADDR_FIELD                    {3829,7,1}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_BLR_SYNC_DETECT_P0_DEST_ADDR_FIELD                    {3822,7,1}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_BLR_DEMAPPER_DEST_ADDR_FIELD                          {3815,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_CARRIER_RECOVERY_DEST_ADDR_FIELD                      {3808,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_EDR_SYNC_DETECT_P2_DEST_ADDR_FIELD                    {3861,7,1}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_EDR_SYNC_DETECT_P1_DEST_ADDR_FIELD                    {3854,7,1}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_EDR_SYNC_DETECT_P0_DEST_ADDR_FIELD                    {3847,7,1}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_BLR_SYNC_DETECT_P2_DEST_ADDR_FIELD                    {3840,7,1}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_IEEE_802154_C2B_P0_DEST_ADDR_FIELD                    {3893,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DPSK_DEMOD_P2_DEST_ADDR_FIELD                         {3886,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DPSK_DEMOD_P1_DEST_ADDR_FIELD                         {3879,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DPSK_DEMOD_P0_DEST_ADDR_FIELD                         {3872,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_HARD_DECISION_DEST_ADDR_FIELD                         {3925,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_VITERBI_GFSK_P1_DEST_ADDR_FIELD                       {3918,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_VITERBI_GFSK_P0_DEST_ADDR_FIELD                       {3911,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_IEEE_802154_C2B_P1_DEST_ADDR_FIELD                    {3904,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_DATA_WHITENING_DEST_ADDR_FIELD                        {3957,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_MANCHESTER_DEST_ADDR_FIELD                            {3950,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_SUBSAMPLE_DEST_ADDR_FIELD                             {3943,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_BLR_DELAY_DEST_ADDR_FIELD                             {3936,7,1}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_VITERBI_CONV_P1_DEST_ADDR_FIELD                       {3989,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_VITERBI_CONV_P0_DEST_ADDR_FIELD                       {3982,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_CRC_CUSTOM_DEST_ADDR_FIELD                            {3975,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_IEEE_802154_F2L_DEST_ADDR_FIELD                       {3968,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_FLUSH_DEST_ADDR_FIELD                                 {4021,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_LIMIT_RATE_DEST_ADDR_FIELD                            {4014,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_VITERBI_CONV_P3_DEST_ADDR_FIELD                       {4007,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_VITERBI_CONV_P2_DEST_ADDR_FIELD                       {4000,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_MULT_3_DEST_ADDR_FIELD                                {4053,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_MULT_2_DEST_ADDR_FIELD                                {4046,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_MULT_1_DEST_ADDR_FIELD                                {4039,7,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_RDP_MAPS_MULT_0_DEST_ADDR_FIELD                                {4032,7,0}
#define GALLIFREY_T22_V1A_DIG_DCO_CAL_BIAS_LEVEL_CTRL_RST_FIELD                                     {4077,2,0}
#define GALLIFREY_T22_V1A_DIG_DCO_CAL_ADD_BIAS_CTRL_FIELD                                           {4075,2,0}
#define GALLIFREY_T22_V1A_DIG_DCO_CAL_T_WAIT_FIELD                                                  {4072,3,0}
#define GALLIFREY_T22_V1A_DIG_DCO_CAL_DET_BITS_FIELD                                                {4070,2,0}
#define GALLIFREY_T22_V1A_DIG_DCO_CAL_T_BASE_FIELD                                                  {4064,6,0}
#define GALLIFREY_T22_V1A_DIG_TX_BLE_DTM_E_FIELD                                                    {4093,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_BLE_DTM_CUSTOM_SW_FIELD                                            {4092,1,0}
#define GALLIFREY_T22_V1A_DIG_TX_BLE_DTM_PKT_TYPE_FIELD                                             {4088,4,0}
#define GALLIFREY_T22_V1A_DIG_TX_BLE_DTM_PKT_LEN_FIELD                                              {4080,8,0}
#define GALLIFREY_T22_V1A_DIG_CLK_CTRL_DIV_CK_1_6_FIELD                                             {4111,3,0}
#define GALLIFREY_T22_V1A_DIG_CLK_CTRL_DIV_CK_RCCO_FIELD                                            {4108,3,0}
#define GALLIFREY_T22_V1A_DIG_CLK_CTRL_DIV_CK_TX_FIELD                                              {4105,3,0}
#define GALLIFREY_T22_V1A_DIG_CLK_CTRL_DIV_CK_XO_FIELD                                              {4102,3,0}
#define GALLIFREY_T22_V1A_DIG_CLK_CTRL_DIV_CK_DIG_FIELD                                             {4099,3,0}
#define GALLIFREY_T22_V1A_DIG_CLK_CTRL_DIV_CK_SYS_FIELD                                             {4096,3,0}
#define GALLIFREY_T22_V1A_DIG_DIR_FIND_EXT_IQ_SMP_TYPE_FIELD                                        {4126,1,0}
#define GALLIFREY_T22_V1A_DIG_DIR_FIND_Q_NI_FIRST_FIELD                                             {4125,1,0}
#define GALLIFREY_T22_V1A_DIG_DIR_FIND_IQ_MSB_FIRST_FIELD                                           {4124,1,0}
#define GALLIFREY_T22_V1A_DIG_DIR_FIND_IQ_DATA_BUS_SIZE_FIELD                                       {4122,2,0}
#define GALLIFREY_T22_V1A_DIG_DIR_FIND_CTE_QUAL_FIELD                                               {4121,1,0}
#define GALLIFREY_T22_V1A_DIG_DIR_FIND_CTE_AUTO_PULL_FIELD                                          {4120,1,0}
#define GALLIFREY_T22_V1A_DIG_RCCO_COMP_BYPASS_FIELD                                                {4144,1,0}
#define GALLIFREY_T22_V1A_DIG_RCCO_COMP_TAU_FIELD                                                   {4142,2,0}
#define GALLIFREY_T22_V1A_DIG_RCCO_COMP_CNT_RC_LEN_L2_FIELD                                         {4140,2,0}
#define GALLIFREY_T22_V1A_DIG_RCCO_COMP_RC_CAL_REG_FIELD                                            {4136,4,0}
#define GALLIFREY_T22_V1A_DIG_RCCO_COMP_TARGET_FREQ_FIELD                                           {4128,8,0}
#define GALLIFREY_T22_V1A_DIG_CHANNEL_HDL_SPACE_X0P5_FIELD                                          {4157,1,0}
#define GALLIFREY_T22_V1A_DIG_CHANNEL_HDL_FW_TO_ADPLL_FIELD                                         {4156,1,0}
#define GALLIFREY_T22_V1A_DIG_CHANNEL_HDL_ADPLL_CONF_FIELD                                          {4154,2,0}
#define GALLIFREY_T22_V1A_DIG_CHANNEL_HDL_BLE_DW_CONF_FIELD                                         {4152,2,0}
#define GALLIFREY_T22_V1A_DIG_AES_KEY_FIELD                                                         {4160,128,0}
#define GALLIFREY_T22_V1A_DIG_AES_NONCE_FIELD                                                       {4288,104,0}
#define GALLIFREY_T22_V1A_DIG_MISC_TX_RF_E_FIELD                                                    {4392,1,0}
#define GALLIFREY_T22_V1A_DIG_TRX_DF_NO_REF_ANT_IN_SW_FIELD                                         {4405,1,0}
#define GALLIFREY_T22_V1A_DIG_TRX_DF_REF_NEW_ANTENNA_FIELD                                          {4404,1,0}
#define GALLIFREY_T22_V1A_DIG_TRX_DF_LUT_ELEM_LEN_FIELD                                             {4400,4,0}
#define GALLIFREY_T22_V1A_DIG_TRX_MANCHESTER_IEEE_CODING_FIELD                                      {4408,1,0}
#define GALLIFREY_T22_V1A_DIG_TRX_CRC_POLY_FIELD                                                    {4416,32,0}
#define GALLIFREY_T22_V1A_DIG_TRX_CRC_RST_FIELD                                                     {4448,32,0}
#define GALLIFREY_T22_V1A_DIG_TRX_DF_LUT_FIELD                                                      {4480,64,0}
#define GALLIFREY_T22_V1A_DIG_TRX_VITERBI_CONV_PUNCT_FIELD                                          {4552,8,0}
#define GALLIFREY_T22_V1A_DIG_TRX_VITERBI_CONV_CODES_FIELD                                          {4544,8,0}
#define GALLIFREY_T22_V1A_DIG_TRX_DATA_WHITENING_USE_CHANNEL_DW_FIELD                               {4567,1,0}
#define GALLIFREY_T22_V1A_DIG_TRX_DATA_WHITENING_BLE_DW_FIELD                                       {4566,1,0}
#define GALLIFREY_T22_V1A_DIG_TRX_DATA_WHITENING_BLE_CHN_FIELD                                      {4560,6,0}
#define GALLIFREY_T22_V1A_DIG_TRX_DPSK_MOD_TYPE_FIELD                                               {4568,1,0}
#define GALLIFREY_T22_V1A_DIG_TRX_SYNC_WORD_LOW_FIELD                                               {4576,32,0}
#define GALLIFREY_T22_V1A_DIG_TRX_SYNC_WORD_HIGH_FIELD                                              {4608,32,0}
#define GALLIFREY_T22_V1A_LDO_DIG_VREF_E_FIELD                                                      {4640,1,0}
#define GALLIFREY_T22_V1A_LDO_DIG_VREF_TUNE_FIELD                                                   {4648,3,0}
#define GALLIFREY_T22_V1A_LDO_DIG_LDO_E_FIELD                                                       {4656,1,0}
#define GALLIFREY_T22_V1A_LDO_DIG_LDO_BIAS_CTRL_FIELD                                               {4664,3,0}
#define GALLIFREY_T22_V1A_ANA_TRX_ISOLATE_TX_B_FIELD                                                {4673,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_ISOLATE_RX_B_FIELD                                                {4672,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_PA_E_FIELD                                                 {4681,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_PA_TUNE_DYN_NSTAT_FIELD                                    {4680,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_ATB_CTRL_FIELD                                                 {4688,16,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_DD_IF_E_FIELD                                                  {4709,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_RFSENSE_E_FIELD                                                {4708,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_MXP_E_FIELD                                                    {4707,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_DD2_IQ_E_FIELD                                                 {4706,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_E_FIELD                                                     {4705,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_E_FIELD                                                    {4704,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_VREF_LNA_PREPA_BQF_E_FIELD                                     {4715,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_BQF_E_FIELD                                                {4714,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_LNA_PREPA_E_FIELD                                          {4713,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_E_FIELD                                                 {4712,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_VCMIN_CTRL_FIELD                                            {4721,3,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LP_E_FIELD                                                  {4720,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_VCMBUFFER_CTRL_FIELD                                        {4728,2,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_PHASE_SHIFT_SEL_FIELD                                       {4737,4,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_PHASE_SHIFT_E_FIELD                                         {4736,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_STG2_IBIAS_FIELD                                           {4747,3,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_STG1_IBIAS_FIELD                                           {4744,3,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_STG1_CTRIM_FIELD                                           {4752,3,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_IN_AGC_FIELD                                               {4760,3,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_CMPRDET_TH_FIELD                                           {4768,2,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_CMPRDET_IBIAS_FIELD                                        {4776,3,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_PKDET_E_FIELD                                              {4784,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_PKDET_TH_FIELD                                             {4792,2,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LNA_PKDET_IBIAS_FIELD                                          {4800,3,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_DD2_IQ_IBIAS_FIELD                                             {4808,4,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_RFSENSE_ANASW_E_FIELD                                          {4824,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_RFSENSE_GAIN_FIELD                                             {4822,2,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_RFSENSE_CTRL_BIAS_SCAMP_FIELD                                  {4819,3,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_RFSENSE_CTRL_BIAS_FIELD                                        {4816,3,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_BQF_BIAS_CTRL_FIELD                                        {4832,3,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_VREF_PA_E_FIELD                                                {4844,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_VREF_PA_TUNE_FIELD                                             {4840,4,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_DD_IF_DIV_4B_8_FIELD                                           {4848,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_CTRL_FIELD                                              {4856,4,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_VREF_TUNE_FIELD                                         {4864,6,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_LNA_PREPA_BIAS_CTRL_FIELD                                  {4872,3,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_VREF_LNA_PREPA_BQF_TUNE_FIELD                                  {4880,3,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_PKDET_IB_CTRL_FIELD                                            {4888,3,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ATB_CONFIG_FIELD                                               {4896,16,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_RCCO_E_FIELD                                                   {4917,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_CORE_1_E_FIELD                                            {4916,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_CORE_0_E_FIELD                                            {4915,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_BUFF_E_FIELD                                              {4914,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_TIA_E_FIELD                                                    {4913,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_BQF_E_FIELD                                                    {4912,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_BIAS_REF_FIELD                                            {4923,3,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_BIAS_COMP_FIELD                                           {4920,3,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_TIA_IBIAS_FIELD                                                {4957,3,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_TIA_HP_FIELD                                                   {4956,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_BQF2_DC_COMP_TSMC_E_FIELD                                      {4955,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_BQF1_DC_COMP_TSMC_E_FIELD                                      {4954,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_BQF_DC_COMP_TSMC_I_CTRL_FIELD                                  {4950,4,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_BQF_DCCOMP_IREF_E_FIELD                                        {4949,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_BQF_OD_LP2_FIELD                                               {4948,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_BQF_OD_LP1_FIELD                                               {4947,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_BQF_HP2_FIELD                                                  {4946,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_BQF_HP1_FIELD                                                  {4945,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_BQF_ANASW_E_FIELD                                              {4944,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_BQF_IBIAS_FIELD                                                {4940,4,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_BQF_LN_IBIAS_FIELD                                             {4937,3,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_BQF_R1_FIELD                                                   {4934,3,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_BQF_R2_FIELD                                                   {4930,4,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_BQF_HBW_FIELD                                                  {4929,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_BQF_FSU_FIELD                                                  {4928,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_PKDET_LVL_2_FIELD                                              {4968,4,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_PKDET_LVL_1_FIELD                                              {4964,4,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_PKDET_LVL_0_FIELD                                              {4960,4,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_VREF_I_E_FIELD                                            {4979,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_VREF_V_E_FIELD                                            {4978,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_VREF_TUNE_FIELD                                           {4976,2,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_VCM_PWELL_E_FIELD                                         {4984,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_ATB_CTRL_FIELD                                            {4992,16,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_VREF_E_FIELD                                          {5010,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_BUFFER_E_FIELD                                        {5009,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_ADC_E_FIELD                                           {5008,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_CORE_0_ATB_E_FIELD                                        {5017,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_CORE_0_RST_B_FIELD                                        {5016,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_CORE_1_ATB_E_FIELD                                        {5025,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_CORE_1_RST_B_FIELD                                        {5024,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_BUFF_TIE_DIRECT_E_FIELD                                   {5032,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_AMP_DET_CK_FIELD                                          {5041,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_AMP_DET_RST_FIELD                                         {5040,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_AMP_DET_BIAS_CTRL_FIELD                                   {5050,3,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_AMP_DET_GAIN_FIELD                                        {5048,2,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_VREF_TUNE_FIELD                                       {5056,3,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_ADC_BIAS_CTRL_FIELD                                   {5064,3,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_BUFFER_BIAS_CTRL_FIELD                                {5072,3,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BQF_DC_COMP_TSMC_0_VAL_FIELD                         {5080,7,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BQF_DC_COMP_TSMC_1_VAL_FIELD                         {5088,7,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BQF_DC_COMP_TSMC_0_VAL_FIELD                         {5096,7,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BQF_DC_COMP_TSMC_1_VAL_FIELD                         {5104,7,0}
#define GALLIFREY_T22_V1A_ANA_TRX_VREF_LNA_PREPA_BQF_TUNE_FIELD                                     {5113,3,0}
#define GALLIFREY_T22_V1A_ANA_TRX_VREF_LNA_PREPA_BQF_E_FIELD                                        {5112,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_ADPLL_RST_B_FIELD                                                 {5120,1,0}
#define GALLIFREY_T22_V1A_REFE_LDO_MISC_LDO_BIAS_CTRL_FIELD                                         {5133,3,0}
#define GALLIFREY_T22_V1A_REFE_LDO_MISC_VREF_TUNE_FIELD                                             {5130,3,0}
#define GALLIFREY_T22_V1A_REFE_ISOLATE_MISC_B_FIELD                                                 {5129,1,0}
#define GALLIFREY_T22_V1A_REFE_USE_REFE_REG_FIELD                                                   {5128,1,0}
#define GALLIFREY_T22_V1A_ANA_TRX_ADPLL_ATB_CONFIG_FIELD                                            {5136,16,0}
#define GALLIFREY_T22_V1A_REFE_ATB_CONFIG_FIELD                                                     {5152,16,0}
#define GALLIFREY_T22_V1A_REFE_TS_E_FIELD                                                           {5174,1,0}
#define GALLIFREY_T22_V1A_REFE_LDO_MISC_LDO_E_FIELD                                                 {5173,1,0}
#define GALLIFREY_T22_V1A_REFE_LDO_MISC_VREF_E_FIELD                                                {5172,1,0}
#define GALLIFREY_T22_V1A_REFE_PTATS_THICK_E_FIELD                                                  {5171,1,0}
#define GALLIFREY_T22_V1A_REFE_PTATS_THIN_E_FIELD                                                   {5170,1,0}
#define GALLIFREY_T22_V1A_REFE_BG_BANDGAP_E_FIELD                                                   {5169,1,0}
#define GALLIFREY_T22_V1A_REFE_BG_IEXT_E_FIELD                                                      {5168,1,0}
#define GALLIFREY_T22_V1A_REFE_XTAL_FREQ_TRIM_FIELD                                                 {5176,8,0}
#define GALLIFREY_T22_V1A_REFE_XTAL_SEL_XO_CTRL_FIELD                                               {5209,1,0}
#define GALLIFREY_T22_V1A_REFE_XTAL_XO_E_B_FIELD                                                    {5208,1,0}
#define GALLIFREY_T22_V1A_REFE_XTAL_OSC_PTAT_START_FIELD                                            {5207,1,0}
#define GALLIFREY_T22_V1A_REFE_XTAL_PTAT_LP_FIELD                                                   {5205,2,0}
#define GALLIFREY_T22_V1A_REFE_XTAL_BUFFER_GAIN_FIELD                                               {5203,2,0}
#define GALLIFREY_T22_V1A_REFE_XTAL_XTAL_AMP_FIELD                                                  {5199,4,0}
#define GALLIFREY_T22_V1A_REFE_XTAL_LOW_CLK_READY_TH_FIELD                                          {5198,1,0}
#define GALLIFREY_T22_V1A_REFE_XTAL_AFTERSTARTUP_CURR_SEL_FIELD                                     {5196,2,0}
#define GALLIFREY_T22_V1A_REFE_XTAL_STARTUP_CURR_SEL_FIELD                                          {5194,2,0}
#define GALLIFREY_T22_V1A_REFE_XTAL_INV_CLK_DIG_FIELD                                               {5193,1,0}
#define GALLIFREY_T22_V1A_REFE_XTAL_INV_CLK_PLL_FIELD                                               {5192,1,0}
#define GALLIFREY_T22_V1A_REFE_XTAL_FORCE_CLK_READY_FIELD                                           {5191,1,0}
#define GALLIFREY_T22_V1A_REFE_XTAL_CLK_OUT_E_B_FIELD                                               {5190,1,0}
#define GALLIFREY_T22_V1A_REFE_XTAL_CLK_PLL_E_B_FIELD                                               {5189,1,0}
#define GALLIFREY_T22_V1A_REFE_XTAL_CLK_DIG_E_B_FIELD                                               {5188,1,0}
#define GALLIFREY_T22_V1A_REFE_XTAL_BUFF_E_B_FIELD                                                  {5187,1,0}
#define GALLIFREY_T22_V1A_REFE_XTAL_FAST_H_MODE_FIELD                                               {5186,1,0}
#define GALLIFREY_T22_V1A_REFE_XTAL_LP_MODE_FIELD                                                   {5185,1,0}
#define GALLIFREY_T22_V1A_REFE_XTAL_EXT_CLK_MODE_FIELD                                              {5184,1,0}
#define GALLIFREY_T22_V1A_REFE_TS_PTAT_CTRL_FIELD                                                   {5218,3,0}
#define GALLIFREY_T22_V1A_REFE_TS_FORCE_PTAT_OK_FIELD                                               {5217,1,0}
#define GALLIFREY_T22_V1A_REFE_TS_PTAT_COMP_E_FIELD                                                 {5216,1,0}
#define GALLIFREY_T22_V1A_COMMAND_FIELD                                                             {5376,8,0}
#define GALLIFREY_T22_V1A_CHANNEL_FIELD                                                             {5408,7,0}
#define GALLIFREY_T22_V1A_PT_CMD_FIELD                                                              {5440,8,0}
#define GALLIFREY_T22_V1A_SYNC_DETECTED_FIELD                                                       {5475,1,0}
#define GALLIFREY_T22_V1A_RX_STATUS_FIELD                                                           {5474,1,0}
#define GALLIFREY_T22_V1A_TX_SERIAL_OVERFLOW_FIELD                                                  {5473,1,0}
#define GALLIFREY_T22_V1A_TX_STATUS_FIELD                                                           {5472,1,0}
#define GALLIFREY_T22_V1A_RXPH_STATUS_FIELD                                                         {5480,4,0}
#define GALLIFREY_T22_V1A_P0_DATA_Q_FIELD                                                           {5520,16,0}
#define GALLIFREY_T22_V1A_P0_DATA_I_FIELD                                                           {5504,16,0}
#define GALLIFREY_T22_V1A_P1_DATA_Q_FIELD                                                           {5552,16,0}
#define GALLIFREY_T22_V1A_P1_DATA_I_FIELD                                                           {5536,16,0}
#define GALLIFREY_T22_V1A_RX_DATA_OUT_FIELD                                                         {5568,32,0}
#define GALLIFREY_T22_V1A_TX_FIFO_DATA_FIELD                                                        {5600,8,0}
#define GALLIFREY_T22_V1A_TX_FIFO_DATA_32_FIELD                                                     {5632,32,0}
#define GALLIFREY_T22_V1A_TX_FIFO_COUNT_FIELD                                                       {5664,8,0}
#define GALLIFREY_T22_V1A_TX_FIFO_ACCESS_ERR_FIELD                                                  {5700,3,0}
#define GALLIFREY_T22_V1A_TX_UNDERFLOW_FIELD                                                        {5699,1,0}
#define GALLIFREY_T22_V1A_TX_OVERFLOW_FIELD                                                         {5698,1,0}
#define GALLIFREY_T22_V1A_TX_FULL_FIELD                                                             {5697,1,0}
#define GALLIFREY_T22_V1A_TX_EMPTY_FIELD                                                            {5696,1,0}
#define GALLIFREY_T22_V1A_TX_FIFO_FLUSH_FIELD                                                       {5696,1,0}
#define GALLIFREY_T22_V1A_RX_FIFO_DATA_FIELD                                                        {5728,8,0}
#define GALLIFREY_T22_V1A_RX_FIFO_DATA_32_FIELD                                                     {5760,32,0}
#define GALLIFREY_T22_V1A_RX_FIFO_COUNT_FIELD                                                       {5792,8,0}
#define GALLIFREY_T22_V1A_RX_FIFO_ACCESS_ERR_FIELD                                                  {5828,3,0}
#define GALLIFREY_T22_V1A_RX_UNDERFLOW_FIELD                                                        {5827,1,0}
#define GALLIFREY_T22_V1A_RX_OVERFLOW_FIELD                                                         {5826,1,0}
#define GALLIFREY_T22_V1A_RX_FULL_FIELD                                                             {5825,1,0}
#define GALLIFREY_T22_V1A_RX_EMPTY_FIELD                                                            {5824,1,0}
#define GALLIFREY_T22_V1A_RX_FIFO_FLUSH_FIELD                                                       {5824,1,0}
#define GALLIFREY_T22_V1A_IQ_FIFO_DATA_FIELD                                                        {5856,8,0}
#define GALLIFREY_T22_V1A_IQ_FIFO_DATA_32_FIELD                                                     {5888,32,0}
#define GALLIFREY_T22_V1A_IQ_FIFO_COUNT_FIELD                                                       {5920,8,0}
#define GALLIFREY_T22_V1A_IQ_FIFO_ACCESS_ERR_FIELD                                                  {5956,3,0}
#define GALLIFREY_T22_V1A_IQ_UNDERFLOW_FIELD                                                        {5955,1,0}
#define GALLIFREY_T22_V1A_IQ_OVERFLOW_FIELD                                                         {5954,1,0}
#define GALLIFREY_T22_V1A_IQ_FULL_FIELD                                                             {5953,1,0}
#define GALLIFREY_T22_V1A_IQ_EMPTY_FIELD                                                            {5952,1,0}
#define GALLIFREY_T22_V1A_IQ_FIFO_FLUSH_FIELD                                                       {5952,1,0}
#define GALLIFREY_T22_V1A_DCO_AMP_CAL_BIAS_CTRL_FIELD                                               {5988,4,0}
#define GALLIFREY_T22_V1A_DCO_AMP_CAL_BIAS_LEVEL_FIELD                                              {5986,2,0}
#define GALLIFREY_T22_V1A_DCO_AMP_CAL_ERR_FIELD                                                     {5985,1,0}
#define GALLIFREY_T22_V1A_DCO_AMP_CAL_DONE_FIELD                                                    {5984,1,0}
#define GALLIFREY_T22_V1A_TS_O_FIELD                                                                {5996,2,0}
#define GALLIFREY_T22_V1A_TS_PTAT_OK_FIELD                                                          {5995,1,0}
#define GALLIFREY_T22_V1A_BANDGAP_OK_FIELD                                                          {5994,1,0}
#define GALLIFREY_T22_V1A_OK_CK_DIG_FIELD                                                           {5993,1,0}
#define GALLIFREY_T22_V1A_OK_CK_PLL_FIELD                                                           {5992,1,0}
#define GALLIFREY_T22_V1A_RC_CAL_FIELD                                                              {6000,4,0}
#define GALLIFREY_T22_V1A_RSSI_PKT_FIELD                                                            {6008,8,0}
#define GALLIFREY_T22_V1A_SYNC_DETECT_FLAG_FIELD                                                    {6024,1,0}
#define GALLIFREY_T22_V1A_RXPH_END_FLAG_FIELD                                                       {6023,1,0}
#define GALLIFREY_T22_V1A_RXPH_DATA_FLAG_FIELD                                                      {6022,1,0}
#define GALLIFREY_T22_V1A_TXPH_END_FLAG_FIELD                                                       {6021,1,0}
#define GALLIFREY_T22_V1A_TXPH_READ_FLAG_FIELD                                                      {6020,1,0}
#define GALLIFREY_T22_V1A_RX_END_FLAG_FIELD                                                         {6019,1,0}
#define GALLIFREY_T22_V1A_RX_START_FLAG_FIELD                                                       {6018,1,0}
#define GALLIFREY_T22_V1A_TX_END_FLAG_FIELD                                                         {6017,1,0}
#define GALLIFREY_T22_V1A_TX_START_FLAG_FIELD                                                       {6016,1,0}
#define GALLIFREY_T22_V1A_RSSI_LIN_FIELD                                                            {6048,12,0}
#define GALLIFREY_T22_V1A_ATTENUATION_PKT_FIELD                                                     {6064,7,0}
#define GALLIFREY_T22_V1A_AGC_CFG_ERR_FIELD                                                         {6073,1,0}
#define GALLIFREY_T22_V1A_AGC_INSTR_ERR_FIELD                                                       {6072,1,0}
#define GALLIFREY_T22_V1A_RSSI_MAX_FIELD                                                            {6096,8,0}
#define GALLIFREY_T22_V1A_RSSI_MIN_FIELD                                                            {6088,8,0}
#define GALLIFREY_T22_V1A_RSSI_FIELD                                                                {6080,8,0}
#define GALLIFREY_T22_V1A_AES_LOCK_STATUS_FIELD                                                     {6104,2,0}
#define GALLIFREY_T22_V1A_CFO_PKT_FIELD                                                             {6112,16,0}
#define GALLIFREY_T22_V1A_IQ_CAL_PHA_FIELD                                                          {6136,8,0}
#define GALLIFREY_T22_V1A_IQ_CAL_AMP_FIELD                                                          {6128,8,0}
#define GALLIFREY_T22_V1A_TX_POWER_FIELD                                                            {6144,4,0}
#define GALLIFREY_T22_V1A_HAS_PROT_TIMER_FIELD                                                      {6159,1,0}
#define GALLIFREY_T22_V1A_HAS_32BIT_FIFO_FIELD                                                      {6158,1,0}
#define GALLIFREY_T22_V1A_HAS_AES_FIELD                                                             {6157,1,0}
#define GALLIFREY_T22_V1A_HAS_DF_FIELD                                                              {6156,1,0}
#define GALLIFREY_T22_V1A_HAS_RX_FIFO_FIELD                                                         {6155,1,0}
#define GALLIFREY_T22_V1A_HAS_TX_FIFO_FIELD                                                         {6154,1,0}
#define GALLIFREY_T22_V1A_HAS_APB_FIELD                                                             {6153,1,0}
#define GALLIFREY_T22_V1A_HAS_XO_FIELD                                                              {6152,1,0}
#define GALLIFREY_T22_V1A_REG_HASH_FIELD                                                            {8128,32,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_STAGE_MASK_2_FIELD                                         {2231,3,1}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_STAGE_MASK_1_FIELD                                         {2228,3,1}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_STAGE_MASK_0_FIELD                                         {2225,3,1}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_THIRD_STAGE_IS_RF_FIELD                                    {2224,1,1}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_0_ATTEN_7_FIELD                                  {2282,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_0_ATTEN_6_FIELD                                  {2277,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_0_ATTEN_5_FIELD                                  {2272,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_1_ATTEN_7_FIELD                                  {2311,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_1_ATTEN_6_FIELD                                  {2306,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_1_ATTEN_5_FIELD                                  {2301,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_1_ATTEN_4_FIELD                                  {2296,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_2_ATTEN_7_FIELD                                  {2331,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_2_ATTEN_6_FIELD                                  {2326,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_2_ATTEN_5_FIELD                                  {2321,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_2_ATTEN_4_FIELD                                  {2316,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_2_ATTEN_3_FIELD                                  {2311,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_2_ATTEN_2_FIELD                                  {2306,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_2_ATTEN_1_FIELD                                  {2301,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_2_ATTEN_0_FIELD                                  {2296,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_3_ATTEN_7_FIELD                                  {2331,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_3_ATTEN_6_FIELD                                  {2326,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_3_ATTEN_5_FIELD                                  {2321,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_3_ATTEN_4_FIELD                                  {2316,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_3_ATTEN_3_FIELD                                  {2311,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_3_ATTEN_2_FIELD                                  {2306,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_3_ATTEN_1_FIELD                                  {2301,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_3_ATTEN_0_FIELD                                  {2296,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_4_ATTEN_7_FIELD                                  {2331,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_4_ATTEN_6_FIELD                                  {2326,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_4_ATTEN_5_FIELD                                  {2321,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_4_ATTEN_4_FIELD                                  {2316,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_4_ATTEN_3_FIELD                                  {2311,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_4_ATTEN_2_FIELD                                  {2306,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_4_ATTEN_1_FIELD                                  {2301,5,0}
#define GALLIFREY_T22_V1A_DIG_RX_TOP_AGC_ATTEN_LUT_4_ATTEN_0_FIELD                                  {2296,5,0}
#define GALLIFREY_T22_V1A_LDO_DIG_LDO_ISTAB_FIELD                                                   {4678,2,0}
#define GALLIFREY_T22_V1A_LDO_DIG_LDO_IBIAS_FIELD                                                   {4676,2,0}
#define GALLIFREY_T22_V1A_LDO_DIG_LDO_CM_FIELD                                                      {4674,2,0}
#define GALLIFREY_T22_V1A_LDO_DIG_LDO_RM_FIELD                                                      {4672,2,0}
#define GALLIFREY_T22_V1A_ANA_TRX_ATB_CONFIG_FIELD                                                  {4680,20,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LAST_SLICE_E_FIELD                                          {4745,1,1}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_CTUNE_SINGLE_FIELD                                          {4743,2,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_CTUNE_DIFF_FIELD                                            {4741,2,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_RFIN_VCM_CTRL_FIELD                                         {4736,5,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_BQF_ISTAB_FIELD                                            {4846,2,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_BQF_IBIAS_FIELD                                            {4844,2,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_BQF_CM_FIELD                                               {4842,2,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_BQF_RM_FIELD                                               {4840,2,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_PALOW_ISTAB_FIELD                                          {4852,4,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_PALOW_IBIAS_FIELD                                          {4848,4,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_PALOW_CM_FIELD                                             {4844,4,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_PALOW_RM_FIELD                                             {4840,4,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_PAHIGH_ISTAB_FIELD                                         {4852,4,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_PAHIGH_IBIAS_FIELD                                         {4848,4,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_PAHIGH_CM_FIELD                                            {4844,4,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_PAHIGH_RM_FIELD                                            {4840,4,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_VREF_LNA_BQF_TUNE_FIELD                                        {4849,3,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_VREF_LNA_BQF_E_FIELD                                           {4848,1,1}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_MXP_CL_FIELD                                                   {4850,3,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_MXP_RB_FIELD                                                   {4848,2,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_DD2_GF_INV_CLK_FIELD                                           {4858,1,1}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_DD2_GF_IQ25_E_FIELD                                            {4857,1,1}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_DD2_GF_IQ25_IBIAS_FIELD                                        {4853,4,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_DD2_GF_E_FIELD                                                 {4852,1,1}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_DD2_GF_IBIAS_FIELD                                             {4848,4,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_LOW_E_FIELD                                             {4857,1,1}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_HIGH_E_FIELD                                            {4856,1,1}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_PA_LDO_VREF_E_FIELD                                            {4872,1,1}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_LNA_PREPA_ISTAB_FIELD                                      {4886,2,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_LNA_PREPA_IBIAS_FIELD                                      {4884,2,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_LNA_PREPA_CM_FIELD                                         {4882,2,0}
#define GALLIFREY_T22_V1A_ANA_TRX_RF_LDO_LNA_PREPA_RM_FIELD                                         {4880,2,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_ADC_ISTAB_FIELD                                       {5078,2,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_ADC_IBIAS_FIELD                                       {5076,2,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_ADC_CM_FIELD                                          {5074,2,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_ADC_RM_FIELD                                          {5072,2,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_BUFFER_ISTAB_FIELD                                    {5086,2,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_BUFFER_IBIAS_FIELD                                    {5084,2,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_BUFFER_CM_FIELD                                       {5082,2,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_ADCS_LDO_BUFFER_RM_FIELD                                       {5080,2,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BUFF_DC_FSU_FIELD                                    {5084,1,1}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BUFF_FAST_IN_SST_FIELD                               {5083,1,1}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BUFF_ODLP_E_FIELD                                    {5082,1,1}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BUFF_HP_E_FIELD                                      {5081,1,1}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BUFF_E_FIELD                                         {5080,1,1}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BQF_DC_COMP_GF_M_FIELD                               {5103,7,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_0_BQF_DC_COMP_GF_P_FIELD                               {5096,7,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BUFF_DC_FSU_FIELD                                    {5100,1,1}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BUFF_FAST_IN_SST_FIELD                               {5099,1,1}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BUFF_ODLP_E_FIELD                                    {5098,1,1}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BUFF_HP_E_FIELD                                      {5097,1,1}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BUFF_E_FIELD                                         {5096,1,1}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BQF_DC_COMP_GF_M_FIELD                               {5119,7,0}
#define GALLIFREY_T22_V1A_ANA_TRX_BB_CHANNEL_1_BQF_DC_COMP_GF_P_FIELD                               {5112,7,0}
#define GALLIFREY_T22_V1A_REFE_LDO_MISC_LDO_ISTAB_FIELD                                             {5182,2,0}
#define GALLIFREY_T22_V1A_REFE_LDO_MISC_LDO_IBIAS_FIELD                                             {5180,2,0}
#define GALLIFREY_T22_V1A_REFE_LDO_MISC_LDO_CM_FIELD                                                {5178,2,0}
#define GALLIFREY_T22_V1A_REFE_LDO_MISC_LDO_RM_FIELD                                                {5176,2,0}
#define GALLIFREY_T22_V1A_DIG_CHIP_ID_FIELD                                                         {8184,8,0}

#endif
/* C header for GALLIFREY_T22_V1A_ADPLLDIG */

#ifndef __GALLIFREY_T22_V1A_ADPLLDIG__
#define __GALLIFREY_T22_V1A_ADPLLDIG__

#define GALLIFREY_T22_V1A_ADPLLDIG_BASE                                                       0xe00
#define GALLIFREY_T22_V1A_ADPLLDIG_FREQUENCY_OFFSET                                           0x00
#define GALLIFREY_T22_V1A_ADPLLDIG_FREQUENCY_ADDR                                             (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_FREQUENCY_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_FREQUENCY_SZ                                               24
#define GALLIFREY_T22_V1A_ADPLLDIG_FREQUENCY                                                  (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_FREQUENCY_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_FREQ_CONFIG_OFFSET                                         0x03
#define GALLIFREY_T22_V1A_ADPLLDIG_FREQ_CONFIG_ADDR                                           (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_FREQ_CONFIG_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_FREQ_CONFIG_SZ                                             8
#define GALLIFREY_T22_V1A_ADPLLDIG_FREQ_CONFIG                                                (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_FREQ_CONFIG_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_CHANNEL_SPACING_OFFSET                                     0x04
#define GALLIFREY_T22_V1A_ADPLLDIG_CHANNEL_SPACING_ADDR                                       (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_CHANNEL_SPACING_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_CHANNEL_SPACING_SZ                                         16
#define GALLIFREY_T22_V1A_ADPLLDIG_CHANNEL_SPACING                                            (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_CHANNEL_SPACING_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_CHANNEL_REG_OFFSET                                         0x06
#define GALLIFREY_T22_V1A_ADPLLDIG_CHANNEL_REG_ADDR                                           (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_CHANNEL_REG_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_CHANNEL_REG_SZ                                             8
#define GALLIFREY_T22_V1A_ADPLLDIG_CHANNEL_REG                                                (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_CHANNEL_REG_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_BIAS_OFFSET                                            0x07
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_BIAS_ADDR                                              (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_DCO_BIAS_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_BIAS_SZ                                                8
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_BIAS                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_DCO_BIAS_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_INTERMEDIATE_FREQUENCY_OFFSET                              0x08
#define GALLIFREY_T22_V1A_ADPLLDIG_INTERMEDIATE_FREQUENCY_ADDR                                (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_INTERMEDIATE_FREQUENCY_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_INTERMEDIATE_FREQUENCY_SZ                                  24
#define GALLIFREY_T22_V1A_ADPLLDIG_INTERMEDIATE_FREQUENCY                                     (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_INTERMEDIATE_FREQUENCY_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_CTRL_OFFSET                                            0x0b
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_CTRL_ADDR                                              (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_DCO_CTRL_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_CTRL_SZ                                                8
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_CTRL                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_DCO_CTRL_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_FILTER_1_OFFSET                                            0x0c
#define GALLIFREY_T22_V1A_ADPLLDIG_FILTER_1_ADDR                                              (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_FILTER_1_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_FILTER_1_SZ                                                24
#define GALLIFREY_T22_V1A_ADPLLDIG_FILTER_1                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_FILTER_1_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_SDM_OFFSET                                             0x0f
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_SDM_ADDR                                               (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_DCO_SDM_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_SDM_SZ                                                 8
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_SDM                                                    (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_DCO_SDM_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_FILTER_2_OFFSET                                            0x10
#define GALLIFREY_T22_V1A_ADPLLDIG_FILTER_2_ADDR                                              (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_FILTER_2_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_FILTER_2_SZ                                                32
#define GALLIFREY_T22_V1A_ADPLLDIG_FILTER_2                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_FILTER_2_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_FILTER_TIMING_OFFSET                                       0x14
#define GALLIFREY_T22_V1A_ADPLLDIG_FILTER_TIMING_ADDR                                         (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_FILTER_TIMING_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_FILTER_TIMING_SZ                                           16
#define GALLIFREY_T22_V1A_ADPLLDIG_FILTER_TIMING                                              (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_FILTER_TIMING_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_AMP_DET_OFFSET                                         0x16
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_AMP_DET_ADDR                                           (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_DCO_AMP_DET_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_AMP_DET_SZ                                             16
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_AMP_DET                                                (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_DCO_AMP_DET_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_CAP_OFFSET                                             0x18
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_CAP_ADDR                                               (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_DCO_CAP_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_CAP_SZ                                                 32
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_CAP                                                    (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_DCO_CAP_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_RF_POW_COMP_OFFSET                                     0x1c
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_RF_POW_COMP_ADDR                                       (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_DCO_RF_POW_COMP_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_RF_POW_COMP_SZ                                         72
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_RF_POW_COMP                                            (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_DCO_RF_POW_COMP_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_DD2_OFFSET                                             0x25
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_DD2_ADDR                                               (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_DCO_DD2_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_DD2_SZ                                                 8
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_DD2                                                    (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_DCO_DD2_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_DD2_MISC_OFFSET                                        0x26
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_DD2_MISC_ADDR                                          (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_DCO_DD2_MISC_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_DD2_MISC_SZ                                            8
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_DD2_MISC                                               (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_DCO_DD2_MISC_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_OFFSET                                            0x27
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_ADDR                                              (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_SZ                                                8
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_LF_IC_OFFSET                                      0x28
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_LF_IC_ADDR                                        (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_LF_IC_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_LF_IC_SZ                                          16
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_LF_IC                                             (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_LF_IC_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_HF_IC_OFFSET                                      0x2a
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_HF_IC_ADDR                                        (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_HF_IC_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_HF_IC_SZ                                          16
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_HF_IC                                             (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_HF_IC_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_OFFSET                                            0x2c
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_ADDR                                              (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_SZ                                                16
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_IC_OFFSET                                         0x2e
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_IC_ADDR                                           (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_IC_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_IC_SZ                                             16
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_IC                                                (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_IC_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_COEFS_OFFSET                                      0x30
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_COEFS_ADDR                                        (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_COEFS_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_COEFS_SZ                                          56
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_COEFS                                             (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_COEFS_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_OFFSET                                            0x37
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_ADDR                                              (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_SZ                                                8
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_OFFSET                                            0x38
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_ADDR                                              (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_SZ                                                16
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_DTC_OFFSET                                                 0x3a
#define GALLIFREY_T22_V1A_ADPLLDIG_DTC_ADDR                                                   (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_DTC_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_DTC_SZ                                                     8
#define GALLIFREY_T22_V1A_ADPLLDIG_DTC                                                        (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_DTC_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_TDC_OFFSET                                                 0x3b
#define GALLIFREY_T22_V1A_ADPLLDIG_TDC_ADDR                                                   (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_TDC_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_TDC_SZ                                                     8
#define GALLIFREY_T22_V1A_ADPLLDIG_TDC                                                        (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_TDC_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_COEFS_OFFSET                                      0x3c
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_COEFS_ADDR                                        (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_COEFS_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_COEFS_SZ                                          56
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_COEFS                                             (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_COEFS_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_TDC_VPA_OFFSET                                             0x43
#define GALLIFREY_T22_V1A_ADPLLDIG_TDC_VPA_ADDR                                               (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_TDC_VPA_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_TDC_VPA_SZ                                                 8
#define GALLIFREY_T22_V1A_ADPLLDIG_TDC_VPA                                                    (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_TDC_VPA_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_TDC_KTDC_IC_OFFSET                                         0x44
#define GALLIFREY_T22_V1A_ADPLLDIG_TDC_KTDC_IC_ADDR                                           (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_TDC_KTDC_IC_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_TDC_KTDC_IC_SZ                                             16
#define GALLIFREY_T22_V1A_ADPLLDIG_TDC_KTDC_IC                                                (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_TDC_KTDC_IC_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_TDC_OFFSET_OFFSET                                          0x46
#define GALLIFREY_T22_V1A_ADPLLDIG_TDC_OFFSET_ADDR                                            (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_TDC_OFFSET_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_TDC_OFFSET_SZ                                              8
#define GALLIFREY_T22_V1A_ADPLLDIG_TDC_OFFSET_REG                                             (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_TDC_OFFSET_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_MISC_OFFSET                                            0x47
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_MISC_ADDR                                              (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_PMU_MISC_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_MISC_SZ                                                8
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_MISC                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_PMU_MISC_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_MODULATION_OFFSET                                          0x48
#define GALLIFREY_T22_V1A_ADPLLDIG_MODULATION_ADDR                                            (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_MODULATION_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_MODULATION_SZ                                              16
#define GALLIFREY_T22_V1A_ADPLLDIG_MODULATION                                                 (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_MODULATION_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DTC_OFFSET                                         0x4a
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DTC_ADDR                                           (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DTC_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DTC_SZ                                             8
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DTC                                                (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DTC_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_TDC_OFFSET                                         0x4b
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_TDC_ADDR                                           (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_TDC_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_TDC_SZ                                             8
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_TDC                                                (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_TDC_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DCO_OFFSET                                         0x4c
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DCO_ADDR                                           (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DCO_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DCO_SZ                                             8
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DCO                                                (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DCO_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_OVERRIDES_OFFSET                                           0x4d
#define GALLIFREY_T22_V1A_ADPLLDIG_OVERRIDES_ADDR                                             (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_OVERRIDES_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_OVERRIDES_SZ                                               8
#define GALLIFREY_T22_V1A_ADPLLDIG_OVERRIDES                                                  (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_OVERRIDES_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_REGVALUES_OFFSET                                           0x4e
#define GALLIFREY_T22_V1A_ADPLLDIG_REGVALUES_ADDR                                             (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_REGVALUES_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_REGVALUES_SZ                                               16
#define GALLIFREY_T22_V1A_ADPLLDIG_REGVALUES                                                  (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_REGVALUES_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_SYSTEM_OFFSET                                              0x50
#define GALLIFREY_T22_V1A_ADPLLDIG_SYSTEM_ADDR                                                (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_SYSTEM_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_SYSTEM_SZ                                                  16
#define GALLIFREY_T22_V1A_ADPLLDIG_SYSTEM                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_SYSTEM_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_SYSTEM_TIMING_OFFSET                                       0x52
#define GALLIFREY_T22_V1A_ADPLLDIG_SYSTEM_TIMING_ADDR                                         (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_SYSTEM_TIMING_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_SYSTEM_TIMING_SZ                                           8
#define GALLIFREY_T22_V1A_ADPLLDIG_SYSTEM_TIMING                                              (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_SYSTEM_TIMING_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_DMA_OFFSET                                                 0x53
#define GALLIFREY_T22_V1A_ADPLLDIG_DMA_ADDR                                                   (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_DMA_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_DMA_SZ                                                     8
#define GALLIFREY_T22_V1A_ADPLLDIG_DMA                                                        (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_DMA_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_ADPLL_ENABLES_OFFSET                                       0x54
#define GALLIFREY_T22_V1A_ADPLLDIG_ADPLL_ENABLES_ADDR                                         (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_ADPLL_ENABLES_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_ADPLL_ENABLES_SZ                                           16
#define GALLIFREY_T22_V1A_ADPLLDIG_ADPLL_ENABLES                                              (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_ADPLL_ENABLES_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_CLOCKS_OFFSET                                              0x56
#define GALLIFREY_T22_V1A_ADPLLDIG_CLOCKS_ADDR                                                (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_CLOCKS_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_CLOCKS_SZ                                                  8
#define GALLIFREY_T22_V1A_ADPLLDIG_CLOCKS                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_CLOCKS_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_MEAS_OFFSET                                                0x57
#define GALLIFREY_T22_V1A_ADPLLDIG_MEAS_ADDR                                                  (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_MEAS_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_MEAS_SZ                                                    8
#define GALLIFREY_T22_V1A_ADPLLDIG_MEAS                                                       (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_MEAS_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_ANA_TEST_OFFSET                                            0x58
#define GALLIFREY_T22_V1A_ADPLLDIG_ANA_TEST_ADDR                                              (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_ANA_TEST_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_ANA_TEST_SZ                                                32
#define GALLIFREY_T22_V1A_ADPLLDIG_ANA_TEST                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_ANA_TEST_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCOHF_OUT_OFFSET                                          0x60
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCOHF_OUT_ADDR                                            (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_KDCOHF_OUT_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCOHF_OUT_SZ                                              16
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCOHF_OUT                                                 (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_KDCOHF_OUT_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_OUT_OFFSET                                            0x62
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_OUT_ADDR                                              (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_KDTC_OUT_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_OUT_SZ                                                16
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_OUT                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_KDTC_OUT_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_READOUT_OFFSET                                         0x64
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_READOUT_ADDR                                           (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_DCO_READOUT_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_READOUT_SZ                                             8
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_READOUT                                                (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_DCO_READOUT_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_DEBUG_READOUT_OFFSET                                       0x65
#define GALLIFREY_T22_V1A_ADPLLDIG_DEBUG_READOUT_ADDR                                         (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_DEBUG_READOUT_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_DEBUG_READOUT_SZ                                           8
#define GALLIFREY_T22_V1A_ADPLLDIG_DEBUG_READOUT                                              (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_DEBUG_READOUT_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_MOD_CAP_READOUT_OFFSET                                 0x66
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_MOD_CAP_READOUT_ADDR                                   (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_DCO_MOD_CAP_READOUT_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_MOD_CAP_READOUT_SZ                                     16
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_MOD_CAP_READOUT                                        (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_DCO_MOD_CAP_READOUT_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_CAP_READOUT_OFFSET                                     0x68
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_CAP_READOUT_ADDR                                       (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_DCO_CAP_READOUT_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_CAP_READOUT_SZ                                         24
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_CAP_READOUT                                            (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_DCO_CAP_READOUT_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_FREQDIFF_OFFSET                                            0x6c
#define GALLIFREY_T22_V1A_ADPLLDIG_FREQDIFF_ADDR                                              (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_FREQDIFF_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_FREQDIFF_SZ                                                24
#define GALLIFREY_T22_V1A_ADPLLDIG_FREQDIFF                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_FREQDIFF_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_VARIOUS_READOUT_OFFSET                                     0x6f
#define GALLIFREY_T22_V1A_ADPLLDIG_VARIOUS_READOUT_ADDR                                       (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_VARIOUS_READOUT_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_VARIOUS_READOUT_SZ                                         8
#define GALLIFREY_T22_V1A_ADPLLDIG_VARIOUS_READOUT                                            (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_VARIOUS_READOUT_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_STATUS_OFFSET                                              0x70
#define GALLIFREY_T22_V1A_ADPLLDIG_STATUS_ADDR                                                (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_STATUS_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_STATUS_SZ                                                  16
#define GALLIFREY_T22_V1A_ADPLLDIG_STATUS                                                     (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_STATUS_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_ANATSTEN_IN_OFFSET                                         0x72
#define GALLIFREY_T22_V1A_ADPLLDIG_ANATSTEN_IN_ADDR                                           (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_ANATSTEN_IN_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_ANATSTEN_IN_SZ                                             16
#define GALLIFREY_T22_V1A_ADPLLDIG_ANATSTEN_IN                                                (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_ANATSTEN_IN_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_PLLFCWDT_OFFSET                                            0x74
#define GALLIFREY_T22_V1A_ADPLLDIG_PLLFCWDT_ADDR                                              (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_PLLFCWDT_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_PLLFCWDT_SZ                                                24
#define GALLIFREY_T22_V1A_ADPLLDIG_PLLFCWDT                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_PLLFCWDT_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_DD4_PARAM_NOT_USED_IN_TECHNO_OFFSET                    0x25
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_DD4_PARAM_NOT_USED_IN_TECHNO_ADDR                      (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_DCO_DD4_PARAM_NOT_USED_IN_TECHNO_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_DD4_PARAM_NOT_USED_IN_TECHNO_SZ                        8
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_DD4_PARAM_NOT_USED_IN_TECHNO                           (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_DCO_DD4_PARAM_NOT_USED_IN_TECHNO_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_PMU_CONST_OFFSET                                       0x49
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_PMU_CONST_ADDR                                         (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_PMU_PMU_CONST_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_PMU_CONST_SZ                                           8
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_PMU_CONST                                              (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_PMU_PMU_CONST_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DTC_PARAM_NOT_USED_IN_TECHNO_OFFSET                0x4a
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DTC_PARAM_NOT_USED_IN_TECHNO_ADDR                  (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DTC_PARAM_NOT_USED_IN_TECHNO_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DTC_PARAM_NOT_USED_IN_TECHNO_SZ                    8
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DTC_PARAM_NOT_USED_IN_TECHNO                       (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DTC_PARAM_NOT_USED_IN_TECHNO_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_TDC_PARAM_NOT_USED_IN_TECHNO_OFFSET                0x4b
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_TDC_PARAM_NOT_USED_IN_TECHNO_ADDR                  (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_TDC_PARAM_NOT_USED_IN_TECHNO_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_TDC_PARAM_NOT_USED_IN_TECHNO_SZ                    8
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_TDC_PARAM_NOT_USED_IN_TECHNO                       (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_TDC_PARAM_NOT_USED_IN_TECHNO_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DCO_PARAM_NOT_USED_IN_TECHNO_OFFSET                0x4c
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DCO_PARAM_NOT_USED_IN_TECHNO_ADDR                  (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DCO_PARAM_NOT_USED_IN_TECHNO_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DCO_PARAM_NOT_USED_IN_TECHNO_SZ                    8
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DCO_PARAM_NOT_USED_IN_TECHNO                       (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DCO_PARAM_NOT_USED_IN_TECHNO_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_QUAL_MON_OFFSET                                            0x5c
#define GALLIFREY_T22_V1A_ADPLLDIG_QUAL_MON_ADDR                                              (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_QUAL_MON_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_QUAL_MON_SZ                                                16
#define GALLIFREY_T22_V1A_ADPLLDIG_QUAL_MON                                                   (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_QUAL_MON_ADDR
#define GALLIFREY_T22_V1A_ADPLLDIG_CHIP_ID_OFFSET                                             0x7f
#define GALLIFREY_T22_V1A_ADPLLDIG_CHIP_ID_ADDR                                               (GALLIFREY_T22_V1A_ADPLLDIG_BASE + GALLIFREY_T22_V1A_ADPLLDIG_CHIP_ID_OFFSET)
#define GALLIFREY_T22_V1A_ADPLLDIG_CHIP_ID_SZ                                                 8
#define GALLIFREY_T22_V1A_ADPLLDIG_CHIP_ID                                                    (volatile uint8_t *)GALLIFREY_T22_V1A_ADPLLDIG_CHIP_ID_ADDR


#define GALLIFREY_T22_V1A_ADPLLDIG_CH0_FIELD                                  {28672,23,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DOUBLE_CHN_SPC_FIELD                       {28697,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DOUBLE_F_IF_FIELD                          {28696,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_CHANNEL_SPACE_FIELD                        {28704,16,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_CHANNEL_REG_FIELD                          {28720,7,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_BIAS_LEVEL_CTRL_FIELD                  {28732,2,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_BIAS_CTRL_FIELD                        {28728,4,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_F_IF_TX_FIELD                              {28748,12,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_F_IF_RX_FIELD                              {28736,12,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_CTRL_PTAT_OSC_SEL_FIELD                {28763,2,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_CTRL_FAST_START_EB_FIELD               {28762,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_CTRL_BIAS_PTAT_E_FIELD                 {28761,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_CTRL_CORE_CM_EB_FIELD                  {28760,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_FILTER_FINEKZ_FIELD                        {28784,6,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_FILTER_FINETAU_FIELD                       {28779,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_FILTER_FINEK_FIELD                         {28774,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_FILTER_KMEDIUM_FIELD                       {28771,3,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_FILTER_KCOARSE_FIELD                       {28768,3,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_SDM_MODE_TX_FIELD                      {28795,3,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_SDM_MODE_RX_FIELD                      {28792,3,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_FILTER_RST_TAU_E_FIELD                     {28830,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_FILTER_MODKZ_FIELD                         {28824,6,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_FILTER_MODTAU_FIELD                        {28818,6,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_FILTER_MODTAU_TUNE_FIELD                   {28812,6,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_FILTER_MODK_FIELD                          {28806,6,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_FILTER_MODK_TUNE_FIELD                     {28800,6,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_FILTER_MOD_TC_FIELD                        {28844,4,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_FILTER_FINE_TC_FIELD                       {28840,4,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_FILTER_MEDIUM_TC_FIELD                     {28836,4,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_FILTER_COARSE_TC_FIELD                     {28832,4,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_AMP_DET_INT_ALGO_FIELD                 {28857,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_AMP_DET_CHG_LSB_FIELD                  {28856,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_AMP_DET_ANA_CTRL_E_FIELD               {28855,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_AMP_DET_E_FIELD                        {28854,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_AMP_DET_SEL_FIELD                      {28850,4,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_AMP_DET_TM_FIELD                       {28849,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_AMP_DET_CAL_E_FIELD                    {28848,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_CAP_MASK_MOD_LSB_FIELD                 {28892,2,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_CAP_MOD_IC_FIELD                       {28878,14,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_CAP_FINE_IC_FIELD                      {28872,6,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_CAP_MEDIUM_IC_FIELD                    {28869,3,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_CAP_COARSE_IC_FIELD                    {28864,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_RF_POW_COMP_COEF_8_FIELD               {28960,8,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_RF_POW_COMP_COEF_7_FIELD               {28952,8,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_RF_POW_COMP_COEF_6_FIELD               {28944,8,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_RF_POW_COMP_COEF_5_FIELD               {28936,8,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_RF_POW_COMP_COEF_4_FIELD               {28928,8,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_RF_POW_COMP_COEF_3_FIELD               {28920,8,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_RF_POW_COMP_COEF_2_FIELD               {28912,8,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_RF_POW_COMP_COEF_1_FIELD               {28904,8,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_RF_POW_COMP_COEF_0_FIELD               {28896,8,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_DD2_SECOND_E_FIELD                     {28969,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_DD2_E_FIELD                            {28968,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_DD2_IBIAS_FIELD                        {28976,4,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_DBL_FIELD                         {28985,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_E_FIELD                           {28984,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_LF_IC_FIELD                       {28992,9,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_HF_IC_FIELD                       {29008,9,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_UPDATE_IC_FIELD                   {29035,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_DEV_DTC_FIELD                     {29034,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_DEV_TUNE_FIELD                    {29033,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_POS_NEG_FIELD                     {29032,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_TC_FIELD                          {29029,3,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_EST_DEV_FIELD                     {29027,2,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_LF_E_FIELD                        {29026,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_RX_E_FIELD                        {29025,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_CAL_TX_E_FIELD                        {29024,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_IC_FIELD                          {29040,9,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_KDCO_COEF_10_FIELD                {29106,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_KDCO_COEF_9_FIELD                 {29101,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_KDCO_COEF_8_FIELD                 {29096,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_KDCO_COEF_7_FIELD                 {29091,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_KDCO_COEF_6_FIELD                 {29086,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_KDCO_COEF_5_FIELD                 {29081,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_KDCO_COEF_4_FIELD                 {29076,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_KDCO_COEF_3_FIELD                 {29071,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_KDCO_COEF_2_FIELD                 {29066,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_KDCO_COEF_1_FIELD                 {29061,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_LUT_KDCO_COEF_0_FIELD                 {29056,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_DBL_FIELD                         {29113,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_E_FIELD                           {29112,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_MOD_1PH_FIELD                     {29133,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_PHR_DLY_FIELD                     {29131,2,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_UPDATE_IC_FIELD                   {29130,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_LOOP_GAIN_FIELD                   {29127,3,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_INV_FIELD                         {29126,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_TC_FIELD                          {29122,4,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_MODE_FIELD                        {29121,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_CAL_E_FIELD                           {29120,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DTC_OFFSET_FIELD                           {29136,7,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_TDC_INV_FIELD                              {29151,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_TDC_MOD_DLY_FIELD                          {29149,2,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_TDC_FCW_MOD_E_FIELD                        {29148,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_TDC_RES_DLY_FIELD                          {29146,2,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_TDC_RESIDUE_E_FIELD                        {29145,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_TDC_CAL_E_FIELD                            {29144,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_KDTC_COEF_10_FIELD                {29202,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_KDTC_COEF_9_FIELD                 {29197,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_KDTC_COEF_8_FIELD                 {29192,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_KDTC_COEF_7_FIELD                 {29187,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_KDTC_COEF_6_FIELD                 {29182,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_KDTC_COEF_5_FIELD                 {29177,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_KDTC_COEF_4_FIELD                 {29172,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_KDTC_COEF_3_FIELD                 {29167,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_KDTC_COEF_2_FIELD                 {29162,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_KDTC_COEF_1_FIELD                 {29157,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_LUT_KDTC_COEF_0_FIELD                 {29152,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_TDC_VPA_SETTLE_FIELD                       {29214,2,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_TDC_VPA_SETTLE_TC_FIELD                    {29208,6,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_TDC_KTDC_FIELD                             {29216,9,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_TDC_OFFSET_FIELD                           {29232,6,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_ADPLL_VREF_TUNE_FIELD                  {29240,3,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_MODULATION_RFP_4_EL_LUT_FIELD              {29261,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_MODULATION_RFP_COMP_DEL_FIELD              {29258,3,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_MODULATION_COMPENSATE_RFP_FIELD            {29257,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_MODULATION_START_W_TUNED_FIELD             {29256,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_MODULATION_BACK_IDLE_FIELD                 {29255,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_MODULATION_ZERO_POST_FIELD                 {29254,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_MODULATION_ZERO_PRE_FIELD                  {29253,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_MODULATION_PULL_CNT_FIELD                  {29248,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DTC_BIAS_CTRL_FIELD                {29264,3,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_TDC_BIAS_CTRL_FIELD                {29272,3,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DCO_BIAS_CTRL_FIELD                {29280,3,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_OVERRIDES_SRESET_GAUSS_N_FIELD             {29295,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_OVERRIDES_VPA_E_FIELD                      {29294,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_OVERRIDES_AMPHOLD_FIELD                    {29293,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_OVERRIDES_ENPAIN_FIELD                     {29292,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_OVERRIDES_DCO_AMP_DET_E_FIELD              {29291,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_OVERRIDES_TX_DIV_E_FIELD                   {29290,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_OVERRIDES_RX_DIV_E_FIELD                   {29289,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_OVERRIDES_BASE_FIELD                       {29288,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_REGVALUES_RXBIT_FIELD                      {29304,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_REGVALUES_RX_DIV_E_FIELD                   {29303,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_REGVALUES_TX_DIV_E_FIELD                   {29302,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_REGVALUES_VPA_E_FIELD                      {29301,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_REGVALUES_ACTIVE_FIELD                     {29300,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_REGVALUES_DCOAMPHOLD_FIELD                 {29299,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_REGVALUES_DCO_AMP_DET_E_FIELD              {29298,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_REGVALUES_ENPAIN_FIELD                     {29297,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_REGVALUES_SRESET_GAUSS_N_FIELD             {29296,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_SYSTEM_TGL_DET_E_FIELD                     {29320,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_SYSTEM_HOLD_STATE_FIELD                    {29316,4,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_SYSTEM_SKIP_PA_STATE_FIELD                 {29315,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_SYSTEM_SINGLE_CAL_FIELD                    {29314,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_SYSTEM_DLY_FCWDT_FIELD                     {29312,2,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_SYSTEM_SETTLE_TC_FIELD                     {29332,4,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_SYSTEM_PASETTLE_TC_FIELD                   {29328,4,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DMA_ASYNC_FIELD                            {29343,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DMA_RATE_FIELD                             {29341,2,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DMA_MUX_FIELD                              {29336,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_ADPLL_ENABLES_TDC_CKVIN_E_FIELD            {29353,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_ADPLL_ENABLES_TDC_DTCIN_E_FIELD            {29352,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_ADPLL_ENABLES_ISOLATE_B_FIELD              {29351,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_ADPLL_ENABLES_DTC_E_FIELD                  {29350,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_ADPLL_ENABLES_DCO_CORE_E_FIELD             {29349,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_ADPLL_ENABLES_DCO_BIAS_E_FIELD             {29348,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_ADPLL_ENABLES_VREF_E_FIELD                 {29347,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_ADPLL_ENABLES_LDO_DTC_E_FIELD              {29346,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_ADPLL_ENABLES_LDO_TDC_E_FIELD              {29345,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_ADPLL_ENABLES_LDO_DCO_E_FIELD              {29344,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_CLOCKS_INV_CK_TDC_FIELD                    {29364,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_CLOCKS_INV_CK_PHV_FIELD                    {29363,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_CLOCKS_CK_DCOMOD_E_FIELD                   {29362,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_CLOCKS_INV_CK_DCOMOD_FIELD                 {29361,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_CLOCKS_INV_CK_DMA_FIELD                    {29360,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_MEAS_TMR_E_FIELD                           {29374,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_MEAS_OPEN_LOOP_RF_E_FIELD                  {29373,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_MEAS_OPEN_LOOP_RF_TC_FIELD                 {29368,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_ANA_TEST_SPARE_FIELD                       {29392,16,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_ANA_TEST_E_FIELD                           {29376,16,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDCO_HF_FIELD                              {29440,9,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_KDTC_OUT_FIELD                             {29456,9,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCOAMPDET_ASYNC_FIELD                      {29476,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCOAMP_FIELD                               {29472,4,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_STARTED_FIELD                          {29486,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_AMPL_FIELD                             {29483,3,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_TDCBUB_FIELD                               {29482,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_PHVSA0_FIELD                               {29481,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_PHVSA1_FIELD                               {29480,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCOMOD_FIELD                               {29488,14,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCOCOARSE_FIELD                            {29520,5,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCOMEDIUM_FIELD                            {29512,3,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCOFINE_FIELD                              {29504,6,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_MEASDONE_OUT_FIELD                         {29559,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_FREQDIFF_FIELD                             {29536,23,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_QUALMONDET_FIELD                           {29560,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_TUNE_STATE_FIELD                           {29580,4,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_CAL_KDCOCAL_FIELD                          {29579,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_CAL_KDTCCAL_FIELD                          {29578,1,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_TMRVAL_FIELD                               {29568,10,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_ANATSTSPARE_IN_FIELD                       {29584,16,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_PLLFCWDT_FIELD                             {29600,23,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_DD4_TUNE_IBIAS_FIELD                   {28969,4,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_DCO_DD4_E_FIELD                            {28968,1,1}
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_DTC_LDO_DMY_FIELD                      {29256,2,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DTC_ISTAB_FIELD                    {29270,2,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DTC_IBIAS_FIELD                    {29268,2,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DTC_CM_FIELD                       {29266,2,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DTC_RM_FIELD                       {29264,2,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_TDC_ISTAB_FIELD                    {29278,2,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_TDC_IBIAS_FIELD                    {29276,2,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_TDC_CM_FIELD                       {29274,2,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_TDC_RM_FIELD                       {29272,2,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DCO_ISTAB_FIELD                    {29286,2,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DCO_IBIAS_FIELD                    {29284,2,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DCO_CM_FIELD                       {29282,2,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_PMU_LDO_DCO_RM_FIELD                       {29280,2,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_QUAL_MON_MODE_FIELD                        {29421,2,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_QUAL_MON_TRHLD_FIELD                       {29415,6,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_QUAL_MON_FRC_E_FIELD                       {29414,1,1}
#define GALLIFREY_T22_V1A_ADPLLDIG_QUAL_MON_WND_FIELD                         {29408,6,0}
#define GALLIFREY_T22_V1A_ADPLLDIG_CHIP_ID_FIELD                              {29688,8,0}

#endif
