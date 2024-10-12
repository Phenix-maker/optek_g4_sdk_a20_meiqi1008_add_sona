#ifndef __HW_HDMI_CEC_H__
#define __HW_HDMI_CEC_H__


extern U8 cec_rx_buf[64];
extern U8 cec_tx_buf[16];


/*
 ****************************************************************************************
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
void hw_cec_init( void );

void hw_cec_tx(U8 *buf, U8 len);
void hw_cec_rx_fromIsr(void);

#endif //__HW_HDMI_CEC_H__
