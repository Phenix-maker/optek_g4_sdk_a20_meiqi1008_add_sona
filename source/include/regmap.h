#ifndef	_REGMAP_H
#define	_REGMAP_H


#include "c_def.h"


#define	REG8(addr)				(*(volatile U8 *)(addr))
#define	REG16(addr)				(*(volatile U16 *)(addr))
#define	REG32(addr)				(*(volatile U32 *)(addr))
#define	REG64(addr)				(*(volatile U64 *)(addr))

#define	REG(addr)				(*(volatile U32 *)(addr))



/*-----------------------------------------------------------------------------------------------------*/
/*
 * 	POWER_DOWN
 * 
 */

#define POWER_DOWN_BASE_REG				REG( MEM_SELECTOR_BASE_ADDR + 0x00 )

#define BIT_POWER_DOWN_NANDFLASH		bit0
#define BIT_POWER_DOWN_NANDFLASH_POS	0

#define BIT_POWER_DOWN_USB				bit1
#define BIT_POWER_DOWN_USB_POS			1

#define BIT_POWER_DOWN_SD				bit2
#define BIT_POWER_DOWN_SD_POS			2

#define	MEM_SELECTOR_REG				REG(MEM_SELECTOR_BASE_ADDR)

#define	MEM_SELECTOR_MASK				0x07
#define	MEM_SEL_SD_MASK					(1 << 2)
#define	MEM_SEL_USB_MASK				(1 << 1)
#define	MEM_SEL_NANDFLASH_MASK			(1 << 0)


/*-----------------------------------------------------------------------------------------------------*/
/*
 * 	CLKDIV
 * 
 */

#define CLKDIV_REG				REG( CLKDIV_BASE_ADDR + 0x00 )

#define BIT_SDRAM_CLKDIV		bit0
#define BIT_SDRAM_CLKDIV_POS	0

#define BIT_SD_CLKDIV			bit1
#define BIT_SD_CLKDIV_POS		1

#define BIT_NANF_CLKDIV			bit2
#define BIT_NANF_CLKDIV_POS		2

#define BIT_ADC_CLKDIV			bit3
#define BIT_ADC_CLKDIV_POS		3

#define BIT_SYS_CLKDIV			bit4
#define BIT_SYS_CLKDIV_POS		4

#define BIT_IRDA_CLKDIV			bit5
#define BIT_IRDA_CLKDIV_POS		5

/*-------------------------------------------------------------*/
//bit0 -- bit7, clock div = CLK / (2 to the power of N)
#define DIV_SDRAM_CLK_REG	REG( CLKDIV_BASE_ADDR + 0x04)
#define DIV_SD_CLK_REG		REG( CLKDIV_BASE_ADDR + 0x08)
#define DIV_NANF_CLK_REG	REG( CLKDIV_BASE_ADDR + 0x0c)
#define DIV_ADC_CLK_REG		REG( CLKDIV_BASE_ADDR + 0x10)
#define DIV_SYS_CLK_REG		REG( CLKDIV_BASE_ADDR + 0x14)
#define DIV_IRDA_CLK_REG	REG( CLKDIV_BASE_ADDR + 0x18)



/*---------------------------------------------------------------------------------------------
-----------------------Optek SOC Test Register-------------------------------------------------
----------------------------------------------------------------------------------------------*/
#define	INT_CONTROLLER_BASE_ADDR	0x18600000
#define	REG_INTCON_INT_ENA			REG( INT_CONTROLLER_BASE_ADDR + 0 )
#define	REG_INTCON_INT_STA			REG( INT_CONTROLLER_BASE_ADDR + 4 )
#define	REG_INTCON_INT_VER			REG( INT_CONTROLLER_BASE_ADDR + 8 )
#define	REG_INTCON_INT_TEST			REG( INT_CONTROLLER_BASE_ADDR + 12 )

typedef volatile struct{
	U32	day		: 8;	
	U32	month	: 8;	
	U32	year	: 8;	
	U32	ver		: 8;	
} IP_VERSIONs;

#define	bVERSION_DAY(a)		(*(IP_VERSIONs *)&(a)).day
#define	bVERSION_MONTH(a)	(*(IP_VERSIONs *)&(a)).month
#define	bVERSION_YEAR(a)	(*(IP_VERSIONs *)&(a)).year
#define	bVERSION_NUM(a)		(*(IP_VERSIONs *)&(a)).ver

typedef volatile struct{
	U32	uart0	: 1;	//bit0 is uart
	U32	spi		: 1;	//bit1 is spi
	U32	res1	: 2;
	U32 i2c0	: 1;	//bit4 is i2c0
	U32 ahb_a	: 1;	//bit5 is ahb_audio
	U32 sd		: 1;	//bit6 is sd card
	U32 rtc		: 1;	//bit7 is rtc
	U32 uart1	: 1;	//bit8 is uart1
	U32 res2	: 1;	//bit9
	U32 uart2	: 1;	//bit10 is uart2
	U32 irc		: 1;	//bit11 is irc
	U32 cec		: 1;	//bit12 is cec
} INT_CONTROLs;

#define	UART0_INT_ENABLE		(((INT_CONTROLs *)&REG_INTCON_INT_ENA)->uart0 = 1)
#define	UART1_INT_ENABLE		(((INT_CONTROLs *)&REG_INTCON_INT_ENA)->uart1 = 1)
#define	UART2_INT_ENABLE		(((INT_CONTROLs *)&REG_INTCON_INT_ENA)->uart2 = 1)
#define	SPI_INT_ENABLE			(((INT_CONTROLs *)&REG_INTCON_INT_ENA)->spi = 1)
#define I2C0_INT_ENABLE			(((INT_CONTROLs *)&REG_INTCON_INT_ENA)->i2c0 = 1)
#define SD_INT_ENABLE			(((INT_CONTROLs *)&REG_INTCON_INT_ENA)->sd = 1)
#define RTC_INT_ENABLE			(((INT_CONTROLs *)&REG_INTCON_INT_ENA)->rtc = 1)
#define CEC_INT_ENABLE			(((INT_CONTROLs *)&REG_INTCON_INT_ENA)->cec = 1)

#define	UART0_INT_DISABLE		(((INT_CONTROLs *)&REG_INTCON_INT_ENA)->uart0 = 0)
#define	UART1_INT_DISABLE		(((INT_CONTROLs *)&REG_INTCON_INT_ENA)->uart1 = 0)
#define	UART2_INT_DISABLE		(((INT_CONTROLs *)&REG_INTCON_INT_ENA)->uart2 = 0)
#define	SPI_INT_DISABLE			(((INT_CONTROLs *)&REG_INTCON_INT_ENA)->spi = 0)
#define I2C0_INT_DISABLE		(((INT_CONTROLs *)&REG_INTCON_INT_ENA)->i2c0 = 0)
#define SD_INT_DISABLE			(((INT_CONTROLs *)&REG_INTCON_INT_ENA)->sd = 0)
#define RTC_INT_DISABLE			(((INT_CONTROLs *)&REG_INTCON_INT_ENA)->rtc = 0)
#define CEC_INT_DISABLE			(((INT_CONTROLs *)&REG_INTCON_INT_ENA)->cec = 0)

#define MDM_BASE_ADDR           0x18500000

#define	CODEC_BASE_ADDR         0x18100000

#define BTRF_AGC_BASE_ADDR      0x18400000
/*
 *	base addr define
 */
#define	BASE_ADDR_GPIO          0x18700000
#define	BASE_ADDR_MISC          0x18800000
#define	BASE_ADDR_INTC          0x18600000

/*
*
* TEST  CEVA MDM  and AGC
*/
#define TEST_REG_MDM           REG32(MDM_BASE_ADDR + (0xd8<<2))

#define TEST_REG_MDM_2         REG32(MDM_BASE_ADDR + (0xdA<<2))

#define TEST_REG_AGC0          REG32(BTRF_AGC_BASE_ADDR)
#define TEST_REG_AGC1          REG32(BTRF_AGC_BASE_ADDR + (1<<2))

typedef volatile struct {

U32 min_rf_signal_s   : 4;// = regConfig[3:0];
U32 rf_gain_stb       : 3;//= regConfig[6:4];
U32 rf_gain_init      : 3;//= regConfig[9:7];
U32 mac2rfdac_mode    : 2;//= regConfig[11:10];
U32 rfadc2mac_mode    : 2;//= regConfig[13:12];

} BTRF_AGC_CONTROL0;//TEST_REG_AGC0


typedef volatile struct {

U32 rf_adc_lev_max      : 10;//= regConfig_2[9:0];
U32 rf_adc_lev_min      : 10;//= regConfig_2[19:10];

} BTRF_AGC_CONTROL1;//TEST_REG_AGC1


/*audio codec**********/
#define	REG_CODEC_R01    REG8( CODEC_BASE_ADDR + (0x01<<2))
#define	REG_CODEC_R02    REG8( CODEC_BASE_ADDR + (0x02<<2))
#define	REG_CODEC_R03    REG8( CODEC_BASE_ADDR + (0x03<<2))
#define	REG_CODEC_R04    REG8( CODEC_BASE_ADDR + (0x04<<2))
#define	REG_CODEC_R05    REG8( CODEC_BASE_ADDR + (0x05<<2))
#define	REG_CODEC_R06    REG8( CODEC_BASE_ADDR + (0x06<<2))
#define	REG_CODEC_R07    REG8( CODEC_BASE_ADDR + (0x07<<2))
#define	REG_CODEC_R08    REG8( CODEC_BASE_ADDR + (0x08<<2))
#define	REG_CODEC_R09    REG8( CODEC_BASE_ADDR + (0x09<<2))

#define	REG_CODEC_R0A    REG8( CODEC_BASE_ADDR + (0x0A<<2))
#define	REG_CODEC_R0B    REG8( CODEC_BASE_ADDR + (0x0B<<2))
#define	REG_CODEC_R0C    REG8( CODEC_BASE_ADDR + (0x0C<<2))
#define	REG_CODEC_R0D    REG8( CODEC_BASE_ADDR + (0x0D<<2))
#define	REG_CODEC_R0E    REG8( CODEC_BASE_ADDR + (0x0E<<2))
#define	REG_CODEC_R0F    REG8( CODEC_BASE_ADDR + (0x0F<<2))

#define	REG_CODEC_R10    REG8( CODEC_BASE_ADDR + (0x10<<2))
#define	REG_CODEC_R11    REG8( CODEC_BASE_ADDR + (0x11<<2))

#define	REG_CODEC_R12    REG8( CODEC_BASE_ADDR + (0x12<<2))
#define	REG_CODEC_R13    REG8( CODEC_BASE_ADDR + (0x13<<2))
#define	REG_CODEC_R14    REG8( CODEC_BASE_ADDR + (0x14<<2))
#define	REG_CODEC_R15    REG8( CODEC_BASE_ADDR + (0x15<<2))
#define	REG_CODEC_R16    REG8( CODEC_BASE_ADDR + (0x16<<2))
#define	REG_CODEC_R17    REG8( CODEC_BASE_ADDR + (0x17<<2))
#define	REG_CODEC_R18    REG8( CODEC_BASE_ADDR + (0x18<<2))
#define	REG_CODEC_R19    REG8( CODEC_BASE_ADDR + (0x19<<2))

#define	REG_CODEC_R1A    REG8( CODEC_BASE_ADDR + (0x1A<<2))
#define	REG_CODEC_R1B    REG8( CODEC_BASE_ADDR + (0x1B<<2))
#define	REG_CODEC_R1C    REG8( CODEC_BASE_ADDR + (0x1C<<2))
#define	REG_CODEC_R1D    REG8( CODEC_BASE_ADDR + (0x1D<<2))
#define	REG_CODEC_R1E    REG8( CODEC_BASE_ADDR + (0x1E<<2))
#define	REG_CODEC_R1F    REG8( CODEC_BASE_ADDR + (0x1F<<2))

#define	REG_CODEC_R20    REG8( CODEC_BASE_ADDR + (0x20<<2))
#define	REG_CODEC_R21    REG8( CODEC_BASE_ADDR + (0x21<<2))
#define	REG_CODEC_R22    REG8( CODEC_BASE_ADDR + (0x22<<2))
#define	REG_CODEC_R23    REG8( CODEC_BASE_ADDR + (0x23<<2))
#define	REG_CODEC_R24    REG8( CODEC_BASE_ADDR + (0x24<<2))
#define	REG_CODEC_R25    REG8( CODEC_BASE_ADDR + (0x25<<2))
#define	REG_CODEC_R26    REG8( CODEC_BASE_ADDR + (0x26<<2))
#define	REG_CODEC_R27    REG8( CODEC_BASE_ADDR + (0x27<<2))
#define	REG_CODEC_R28    REG8( CODEC_BASE_ADDR + (0x28<<2))
#define	REG_CODEC_R29    REG8( CODEC_BASE_ADDR + (0x29<<2))

#define	REG_CODEC_R2A    REG8( CODEC_BASE_ADDR + (0x2A<<2))
#define	REG_CODEC_R2B    REG8( CODEC_BASE_ADDR + (0x2B<<2))
#define	REG_CODEC_R2C    REG8( CODEC_BASE_ADDR + (0x2C<<2))
#define	REG_CODEC_R2D    REG8( CODEC_BASE_ADDR + (0x2D<<2))
#define	REG_CODEC_R2E    REG8( CODEC_BASE_ADDR + (0x2E<<2))
#define	REG_CODEC_R2F    REG8( CODEC_BASE_ADDR + (0x2F<<2))

#define	REG_CODEC_R30    REG8( CODEC_BASE_ADDR + (0x30<<2))
#define	REG_CODEC_R31    REG8( CODEC_BASE_ADDR + (0x31<<2))
#define	REG_CODEC_R32    REG8( CODEC_BASE_ADDR + (0x32<<2))
#define	REG_CODEC_R33    REG8( CODEC_BASE_ADDR + (0x33<<2))
#define	REG_CODEC_R34    REG8( CODEC_BASE_ADDR + (0x34<<2))
#define	REG_CODEC_R35    REG8( CODEC_BASE_ADDR + (0x35<<2))
#define	REG_CODEC_R36    REG8( CODEC_BASE_ADDR + (0x36<<2))
#define	REG_CODEC_R37    REG8( CODEC_BASE_ADDR + (0x37<<2))
#define	REG_CODEC_R38    REG8( CODEC_BASE_ADDR + (0x38<<2))
#define	REG_CODEC_R39    REG8( CODEC_BASE_ADDR + (0x39<<2))

#define	REG_CODEC_R3A    REG8( CODEC_BASE_ADDR + (0x3A<<2))
#define	REG_CODEC_R3B    REG8( CODEC_BASE_ADDR + (0x3B<<2))
#define	REG_CODEC_R3C    REG8( CODEC_BASE_ADDR + (0x3C<<2))
#define	REG_CODEC_R3D    REG8( CODEC_BASE_ADDR + (0x3D<<2))
#define	REG_CODEC_R3E    REG8( CODEC_BASE_ADDR + (0x3E<<2))
#define	REG_CODEC_R3F    REG8( CODEC_BASE_ADDR + (0x3F<<2))

#define	REG_CODEC_R40    REG8( CODEC_BASE_ADDR + (0x40<<2))
#define	REG_CODEC_R41    REG8( CODEC_BASE_ADDR + (0x41<<2))
#define	REG_CODEC_R42    REG8( CODEC_BASE_ADDR + (0x42<<2))
#define	REG_CODEC_R43    REG8( CODEC_BASE_ADDR + (0x43<<2))
#define	REG_CODEC_R44    REG8( CODEC_BASE_ADDR + (0x44<<2))
#define	REG_CODEC_R45    REG8( CODEC_BASE_ADDR + (0x45<<2))
#define	REG_CODEC_R47    REG8( CODEC_BASE_ADDR + (0x47<<2))
#define	REG_CODEC_R48    REG8( CODEC_BASE_ADDR + (0x48<<2))
#define	REG_CODEC_R49    REG8( CODEC_BASE_ADDR + (0x49<<2))
#define	REG_CODEC_R4A    REG8( CODEC_BASE_ADDR + (0x4A<<2))
#define	REG_CODEC_R4B    REG8( CODEC_BASE_ADDR + (0x4B<<2))
#define	REG_CODEC_R4C    REG8( CODEC_BASE_ADDR + (0x4C<<2))
#define	REG_CODEC_R4D    REG8( CODEC_BASE_ADDR + (0x4D<<2))
#define	REG_CODEC_R4E    REG8( CODEC_BASE_ADDR + (0x4E<<2))
#define	REG_CODEC_R4F    REG8( CODEC_BASE_ADDR + (0x4F<<2))

#define	REG_CODEC_R51    REG8( CODEC_BASE_ADDR + (0x51<<2))
#define	REG_CODEC_R52    REG8( CODEC_BASE_ADDR + (0x52<<2))
#define	REG_CODEC_R53    REG8( CODEC_BASE_ADDR + (0x53<<2))
#define	REG_CODEC_R54    REG8( CODEC_BASE_ADDR + (0x54<<2))
#define	REG_CODEC_R55    REG8( CODEC_BASE_ADDR + (0x55<<2))
#define	REG_CODEC_R56    REG8( CODEC_BASE_ADDR + (0x56<<2))
#define	REG_CODEC_R57    REG8( CODEC_BASE_ADDR + (0x57<<2))
#define	REG_CODEC_R58    REG8( CODEC_BASE_ADDR + (0x58<<2))
#define	REG_CODEC_R5A    REG8( CODEC_BASE_ADDR + (0x5A<<2))
#define	REG_CODEC_R5B    REG8( CODEC_BASE_ADDR + (0x5B<<2))


/*
 * 	gpio
 */

#define	REG_GPIO0_DIN			REG( BASE_ADDR_GPIO + 0 )
#define	REG_GPIO0_DOUT			REG( BASE_ADDR_GPIO + 4 )
#define	REG_GPIO0_DOE			REG( BASE_ADDR_GPIO + 8 )
#define REG_GPIO0_INTEN			REG( BASE_ADDR_GPIO + 12 )
#define	REG_GPIO0_INTST			REG( BASE_ADDR_GPIO + 16 )
#define	REG_GPIO0_INTTP0		REG( BASE_ADDR_GPIO + 20 )
#define	REG_GPIO0_INTTP1		REG( BASE_ADDR_GPIO + 24 )
#define	REG_GPIO0_FUNC			REG( BASE_ADDR_GPIO + 28 )


#define	REG_GPIO1_DIN			REG( BASE_ADDR_GPIO + 32 )
#define	REG_GPIO1_DOUT			REG( BASE_ADDR_GPIO + 36 )
#define	REG_GPIO1_DOE			REG( BASE_ADDR_GPIO + 40 )
#define REG_GPIO1_INTEN			REG( BASE_ADDR_GPIO + 44 )
#define	REG_GPIO1_INTST			REG( BASE_ADDR_GPIO + 48 )
#define	REG_GPIO1_INTTP0		REG( BASE_ADDR_GPIO + 52 )
#define	REG_GPIO1_INTTP1		REG( BASE_ADDR_GPIO + 56 )
#define	REG_GPIO1_FUNC			REG( BASE_ADDR_GPIO + 60 )


#define REG_GPIO0_IE            REG( BASE_ADDR_GPIO + 32 )
#define REG_GPIO1_IE            REG( BASE_ADDR_GPIO + 36 )
#define	REG_GPIO3_FUNC          REG( BASE_ADDR_GPIO + 40 )
#define	REG_GPIO4_FUNC          REG( BASE_ADDR_GPIO + 44 )
#define	REG_GPIO_SREG          	REG( BASE_ADDR_GPIO + 48 )
#define	REG_GPIO_VER          	REG( BASE_ADDR_GPIO + 52 )



typedef volatile struct {
	U32 bit_0 				: 1;
	U32 bit_1 				: 1;
	U32 bit_2 				: 1;
	U32 bit_3 				: 1;
	U32 bit_4 				: 1;
	U32 bit_5 				: 1;
	U32 bit_6 				: 1;
	U32 bit_7 				: 1;
	U32 bit_8 				: 1;
	U32 bit_9 				: 1;
	U32 bit_10 				: 1;
	U32 bit_11 				: 1;
	U32 bit_12 				: 1;
	U32 bit_13 				: 1;
	U32 bit_14 				: 1;
	U32 bit_15 				: 1;
	U32 bit_16 				: 1;
	U32 bit_17 				: 1;
	U32 bit_18 				: 1;
	U32 bit_19 				: 1;
	U32 bit_20 				: 1;
	U32 bit_21 				: 1;
	U32 bit_22 				: 1;
	U32 bit_23 				: 1;
	U32 bit_24 				: 1;
	U32 bit_25 				: 1;
	U32 bit_26 				: 1;
	U32 bit_27 				: 1;
	U32 bit_28 				: 1;
	U32 bit_29 				: 1;
	U32 bit_30 				: 1;
	U32 bit_31 				: 1;
} GPIO_PINs;


/*
 * 	misc
 */

#define	REG_MISC_CFG			REG( BASE_ADDR_MISC + 0 )
#define	REG_MISC_PLL			REG( BASE_ADDR_MISC + 4 )
#define	REG_MISC_CLKGATE		REG( BASE_ADDR_MISC + 8 )
#define	REG_MISC_ADC_CTL		REG( BASE_ADDR_MISC + 12 )
#define	REG_MISC_ADC_DATA		REG( BASE_ADDR_MISC + 16 )
#define	REG_MISC_CPUCLK_2		REG( BASE_ADDR_MISC + 20 )
#define	REG_MISC_CPUCLK_3     	REG( BASE_ADDR_MISC + 24 )

#define	REG_MISC_CPUCLK_5     	REG( BASE_ADDR_MISC + 12*4 )
#define	REG_MISC_CPUCLK_6     	REG( BASE_ADDR_MISC + 13*4 )


#define REG_MISC_VERSION        REG( BASE_ADDR_MISC + 28 )
#define REG_MISC_IDRAM_SEL      REG( BASE_ADDR_MISC + 32 )
                                                                   
#define REG_MISC_CONTROL        REG( BASE_ADDR_MISC + 36 )   
#define REG_MISC_FRACPLL       	REG( BASE_ADDR_MISC + 40 )

#define REG_MISC_FRACPLL_FRAC    REG( BASE_ADDR_MISC + 44 )


#define REG_MISC_BTDM           REG( BASE_ADDR_MISC + 56 )
#define REG_MISC_CPUCLK        	REG( BASE_ADDR_MISC + 60 )
#define REG_MISC_AUDIO_CODEC    REG( BASE_ADDR_MISC + 64)
#define REG_MISC_USBPHY         REG( BASE_ADDR_MISC + 68)
#define REG_MISC_RTCWD          REG( BASE_ADDR_MISC + 72)
#define REG_MISC_RTCRD          REG( BASE_ADDR_MISC + 76)
#define REG_MISC_RTC            REG( BASE_ADDR_MISC + 80)
#define REG_MISC_AON_CONTROL    REG_MISC_RTC     
#define REG_MISC_BTDM_2         REG( BASE_ADDR_MISC + 84)
//added for soc_btdm52
#define REG_MISC_STATUS         REG( BASE_ADDR_MISC + 88)
#define REG_MISC_MEM_RETEIN     REG( BASE_ADDR_MISC + 92)

#define REG_MISC_TOUCH_1     	REG( BASE_ADDR_MISC + 96)
#define REG_MISC_TOUCH_2     	REG( BASE_ADDR_MISC + 100)

#define REG_MISC_SW_VER         REG( BASE_ADDR_MISC + 104)


typedef volatile struct {
	U32	 AHB_Audio_Controller_gate   :1; 
	U32	 sd_gate                    :1;	
	U32	 usbOtgSd_gate              :1;    
	U32  AHB_BTDM_IF_gate           :1;    
	U32	 AHB_MDM_IF_pclk_gate      	:1;  
	U32  u_uart0_gate               :1;		
	U32  u_uart1_gate               :1;	
	U32  u_uart2_gate               :1;	
	U32  hdmi_cec_gate              :1;	
	U32  spi_gate                   :1;	
	U32  qadc_dac_apb_gate          :1;	
	U32  i2c_gate                   :1;	   
	U32	 u_pwm0_gate 				:1;	
	U32	 u_pwm1_gate                :1;	            			
	U32	 u_timer_gate               :1;	                  
	U32  psram_gate                	:1;	          
	U32	 u_rotary_gate              :1;	
	U32  u_rotary2_gate             :1;					
	U32  res_1                      :1;					
	U32  IR_controller_gate         :1;	
	U32  lp_bt_usb_gate             :1;
	U32  gpio_gate					:1;
	U32  lp_bt_gate					:1;

} MISC_CLKGATE_CONTROLs;




typedef volatile struct {
	U32 rsv                     	:6;
	U32 bt_dbg_sel_1                :3;
	U32 bt_dbg_sel_2                :3;
	U32 bt_dbg_en                   :1;		//enable btdm debug out		
	U32 bt_txen_sel					:1;		//0->sel rf txen,1->sel mac txen 
	U32 audio_dac_d_sel				:2;		//sel audio dac from mi2s-data[3:0]	
} MISC_BTDM_CONTROLs;//REG_MISC_BTDM


typedef volatile struct {
	U32 bt_clksel				:6; //16->16M
	U32 bt_clk_src 				:1; //0->from xtal/3, 1->from icytrx_dm gpio
	U32 bt_clk2lp32k 			:1; //0->from rx 32k, 1->from btdm clk (16mHz)/488
	U32 bt_tx_if 				:1; //0->serial, 1->parallel
	U32 bt_rx_if 				:1; //0->serial, 1->parallel
	U32 ble_optek_wlink_rate 	:2; //0->ble 1M, 1->ble 2M, 2->ble s8, 3->ble s2
	U32 rsv					    :13;
	U32 bt_PA_en 				:1;
	U32 bt_WCI_en 				:2;
	U32 bt_irq_en 				:1; 
} MISC_BTDM_2_CONTROLs;//REG_MISC_BTDM_2


typedef volatile struct {
    U32 rsv                		:1;
    U32 sadc_en         		:1;
    U32 sadc_start    			:1;
	U32 reserve_1bit			:1;
    U32 sadc_clkDiv           	:10;
    U32 sadc_addr       		:2;
	U32 reserve_2bit			:2;
    U32 sadc_diff    			:1;
    U32 sadc_df                 :1;
    U32 sadc_int_en 			:1;
	U32 sadc_eoc				:1;
	U32 sadc_int_staus			:1;	
} MISC_SADC_CONTROLs;

/*
	touch sensor
*/
typedef volatile struct {
    U32 sensor_pd               :1;		//1->touch sensor power down, 0->normal operation
    U32 cal_en         			:1;		//1->calibrattion enable, 0->normal opteration
    U32 fast_mode    			:1;		//1->20ms,0->200ms
    U32 cal_mode_sel           	:1;		//1->sel cal val, 0->sel sencor_th
    U32 sensor_th       		:16;	//sensor threshhold val
	U32 int_status 				:1;		//int clear by reading MISC_TOUCH_1_CONTROLs
	U32 int_en					:1;		//		
} MISC_TOUCH_1_CONTROLs;

typedef volatile struct {
    U32 cal                		:16;	//cal val		
    U32 sensor         			:16;	//set val	
} MISC_TOUCH_2_CONTROLs;


//rtc
typedef volatile struct {
	U32 aon_addr                : 4;
	U32 aon_spi_rwn             : 1; //rd :1; wr :0;
	U32 aon_spi_go              : 1;
	U32 aon_en                  : 1;
	U32 btdm_en                 : 1;
	U32 cpu_mem_emaw            : 2;
	U32 cpu_mem_ema             : 3;
	U32 cpu_mem_emas            : 1;
	U32 cpu_mem_iram0_rtein     : 4;
	
	U32 aon_rw_fini             : 1;
	U32 aon_rw_go_fini          : 1;

	U32 rsv						: 2;
	U32 batt_lev_det_en			: 1;

	U32 touch_pd				: 1;
	U32 touch_start				: 1;
		
} MISC_AON_CONTROLs;

typedef volatile struct {
	U32 cpu_mem_dram0_rtein     : 16;
	U32 cpu_mem_dram1_rtein     : 16;	
} MISC_MEM_RETEIN_CONTROLs;


typedef volatile struct {

	U32	clk_sel		     		: 1; 	// 0 select rc_32k, 1 selelct crystal 32k
	U32 alarm0En		 		: 1; 	// regCtl[2];
	U32 alarm1En		 		: 1; 	// regCtl[3];
	U32 alarm2En		 		: 1; 	// regCtl[4];
	U32 alarm0Int        		: 1; 	// write 1 clear alarm0 int, read alarm0 int status
	U32 alarm1Int        		: 1; 	// write 1 clear alarm0 int, read alarm0 int status
	U32 alarm2Int        		: 1; 	// write 1 clear alarm0 int, read alarm0 int status
	U32 rc_32k_trim		 		: 6; 	// trimming rc 32k;
	U32 uart_en   	 	 		: 1;	
	U32 soc_ramp		 		: 5;	//step is 32k, psram needs about 150us, flash needs about 75us
	U32 soc_wdt_tov		 		: 5;	//step is 1/4 second
	U32 reg_power        		: 1; 	// 0 request power off
	U32 soc_wdt_en		 		: 1;
	U32 alarm0_pwr_on_en 		: 1;	//1->alarm0 pwr on
	U32 alarm1_pwr_on_en 		: 1;	//1->alarm1 pwr on
	U32 alarm2_pwr_on_en 		: 1;	//1->alarm2 pwr on
	U32 avd_en			 		: 1;	
	U32 charger_det_en		 	: 1;	//1->detect fro charger, 0->detect from uart
	U32 charging_cv_end_en	 	: 1;	//1->cv to cgarging end automatically, 0->no charging end, adapter provide power to soc
} AON_CTRL_ST;

/*
 * charger_cu
 * 2'b00 -> 25mA
 * 2'b01 -> 50mA
 * 2'b10 -> 100mA
 * 2'b11 -> 150mA
*/

/*
 * charger_batt_type
 * 2'b00 -> 4.0V
 * 2'b01 -> 4.1V
 * 2'b10 -> 4.2V
 * 2'b11 -> 4.35V
*/

typedef volatile struct {
	U32 core_dcdc_vsel             : 5;
	U32 btdm_rf_dcdc_vsel          : 5;
	U32 ldo_2p5v_vsel              : 5;
	U32 charger_cu 				   : 2;
	U32 charger_batt_type		   : 2;
	U32 charger_pwr_on_en		   : 1;			//det charger in, power on
	U32 rm_charger_pwr_on_en	   : 1;			//det removce charger, power on
	U32 charger_cv_time			   : 8;			//steps 64s
	U32 charging_end		 	   : 1;			//1->force charging end, 0->normal
} AON_CTRL2_ST;


typedef volatile struct {
	U32 qLR_08_VSEL             : 5;
	U32 qLR_18_VSEL				: 5;
	U32 btdm_rf_pwron_en		: 1;
	U32 rf_ck_err_en			: 1;
} AON_CTRL3_ST;


/*
 * charger_batt_lev
 * 3'b000 <=3.0
 * 3'b001 3.0
 * 3'b010 3.2
 * 3'b011 3.4
 * 3'b100 3.6
 * 3'b101 3.8
 * 3'b110 4.0
 * 3'b111 >=4.2
*/

typedef volatile struct {
	U32 charger_det             	: 1;		//valid if charger_enb = 0, 
	U32 charger_batt_lev        	: 3;		
	U32 charger_cc_mode         	: 1;		//constant current
	U32 charger_cv_mode 			: 1;		//constant voltage
	U32 cc100m		   				: 1;		//1->25~150m, 0->5mA
	U32 aon_adapter_det				: 1;		//adapter_in det
	U32 charger_enb					: 1;		//1->not in charging, 0->in charging
} AON_CHARGER_ST;


typedef volatile struct {
	U32 sec        :17;
	U32 days       :15;							//if days is zero, means every day alarm
} AON_DAY_SEC_ST;

typedef volatile struct {
	U32 count_32K  : 15;
	U32 sec        : 17;
} AON_SEC_32K_ST;

/* REG_MISC_USBPHY */
typedef volatile struct {
	U32 utmi_iddig_sw       : 1;
	U32 utmi_sw_vbusValid   : 1;
	U32 utmi_sw_avalid		: 1;
	U32 utmi_sw_bvalid		: 1;
	U32 utmi_sw_sessend		: 1;
} MISC_USBPHY_CONTROLs;


/* REG_MISC_CFG */
typedef volatile struct {
	U32	regConfig_cpuClkSel	          	: 2;//2'b10 sel 24M crystal, 2'b01 sel pll or rtc
	U32 regConfig_pllRtcSel	          	: 2;//2'b01 sel pll, 2'b10 sel rtc	//	regConfig[3:2];
	U32 regConfig_crystalDivSel	      	: 2;// regConfig[5:4];
	U32 regConfig_crystalEna		  	: 1;// regConfig[6];
	U32 regConfig_crystalDiv	      	: 6;// regConfig[12:7];	//6 bits div
	U32 regConfig_mclkOutEna		  	: 1;// regConfig[13];
	U32 regConfig_clk12M_AVclk_sel	  	: 1;// regConfig[14];
	U32 regConfig_padAVclk_sel		  	: 1;// regConfig[15];
	U32 lp_clk_from_24M_en            	: 1;// regConfig[16];
	U32 lp_clk_sel                    	: 1;// regConfig[17];
	U32 lp_clk_en                     	: 1;// regConfig[18];
	U32 CLK_12M_en                    	: 1;// regConfig[19];
	U32 CLK_24Div_gen_en              	: 1;// regConfig[20];

	U32 mclk1_s        					: 1;// regConfig[21];   // 0: mclk_1_out = pllFrac_256x;  1: mclk_1_out = CLK_12M;
	U32 mclk2_s        					: 1;// regConfig[22];   // 0: mclk_2_out = pllFrac_256x;  1: mclk_2_out = lp_clk;
	U32 mclk1_in_out   					: 1;// regConfig[23];   // 0: input   1: output
	U32 mclk2_in_out   					: 1;// regConfig[24];    // 0: input    1:output
	U32 clk24M_I_trim  					: 3;// regConfig[30:25];
	U32 clk24M_C_trim  					: 3;
} MISC_CFG_CONTROLs;



typedef volatile struct {
	U32 pllFrac_PD                   : 1;//reg[0]
	U32 pllFrac_REFDIV               : 6;//reg[1:6]
	U32 pllFrac_FBDIV                : 12;//reg[7:18]
	U32 pllFrac_POSTDIV1             : 3;//reg[19:21]
	U32 pllFrac_POSTDIV2             : 3;//reg[22:24]
	U32 pllFrac_VCOPD             	 : 1;//reg[25]
	U32 pllFrac_fref_sel			 : 1;//reg[26]
	U32 pllFrac_Lock                 : 1;//reg[27]
} MISC_PLLFRAC_CONTROLs;

#define MISC_PLLFRAC1_CONTROLs MISC_PLLFRAC_CONTROLs

typedef volatile struct {
	U32 pllfrac_FBFRAC : 24;	
} MISC_PLLFRAC2_CONTROLs;



//BTDM52
typedef volatile struct {	
	U32	ahbClkDiv			: 4; 	// cpuClk / 2*N
	U32	periClkdDiv			: 4; 	// reset value 4->48Mhz/8
	U32	periClkBypass    	: 1;	// reset value 0
	U32	cpuDynamicClkEn		: 1;	// 1->enable cpu dynamic clk
	U32	cpuClkSeln			: 2;	// 2->sel xtal clk, 1->sel pll clk
	U32	xtal_48Mx2_sel		: 1;	// 1->sel xtal48Mx2, 0->sel xtal48M

	U32 pSramClkDiv			: 4;	// cpuClk / N (N=2,3,4...)
	U32 pSramClkx2Ena		: 1;	// 0->CLK_psram = CLK_ahb, 0->CLK_psram = CLK_ahb x 2
	U32 btdmClkdiv			: 3;	// 48Mhz / N
	U32 sFlashClkHalf		: 1;	// reerved
	U32 jtag_en				: 1;

	U32 ir_sel				: 2;	//ir sel ==2'b01 -> UART0 rx pin as IR pin, ir sel ==2'b10 -> UART1 rx pin as IR pin, ir sel ==2'b11-> UART3 rx pin as IR pin
	U32 reserve				: 2;
	U32 spi_2_en			: 1;

} MISC_CPUCLK_CONTROLs;


typedef volatile struct {
	U32 cpuClkDiv				: 6;
	U32 cpuClkBypass			: 1;
	U32	waitiCpuClkDiv			: 6; 		//in waiti state, cpu clk
	U32	waitiCpuClkBypass       : 1;
	U32 dmaCpuClkDiv			: 6;		//in dma state, cpu clk
	U32 dmaCpuClkBypass			: 1;
} MISC_CPUCLK2_CONTROLs;


typedef volatile struct {
	U32	pSram_CLK_dly           : 7;
	U32	sf_CLK_dly				: 7;
	U32	crystal_CLK_dly			: 7;
} MISC_CPUCLK3_CONTROLs;


enum {
	pllFrac_sel_FOUTPOSTDIV = 0,
	pllFrac_sel_FOUT1PH0 = 1,
	pllFrac_sel_FOUT2 = 2,
	pllFrac_sel_Crystal_48MHz = 3,       //for pllFrac_apwm_sel
	pllFrac_sel_FOUT3 = 3,
	pllFrac_sel_FOUT4 = 4,	             //only for spdif and cpu
	pllFrac_sel_VCO_div_9 = 4,	
	pllFrac_sel_MCLK_in_2x = 4,          //only for audio clock				
	pllFrac_sel_VCO_div_2 = 5,
	pllFrac_sel_VCO_div_13p5 = 5,        //fro 32k sample frequency				
	pllFrac_sel_Crystal_96MHz = 6,	
	pllFrac_sel_32k = 7,                 //for pllFrac_cpu_sel	
	pllFrac_sel_zero = 7,                //gated off for pllFrac_apwm_sel,pllFrac_spdif_sel,pllFrac_512x_sel
};			


/*
	CPU clocks from fracPLL

	491.52MHz, 	393.216MHz, 196.608MHz, 131.072MHz, 98.304MHz				for 48kHz, 96kHz and 192kHz audio sampling rate
		       	393.216MHz, 196.608MHz, 131.072MHz, 98.304MHz				for 32kHz, 64kHz and 128kHz audio sampling rate, audio output using48kHz x N (1,2,4)
	451.584MHz, 361.2672MHz. 180.6336MHz, 120.4224Mhz, 90.3168MHz			for 44.1kHz, 88.2kHz and 176.4kHz audio sampling rate
	Crystal 96MHz, 48MHz													for PLLess audio application
*/	

typedef volatile struct {
	U32 pllFrac_cpu_sel			: 3;
	U32 pllFrac_apwm_sel		: 3;
	U32 pllFrac_spdif_sel		: 3;
	U32 pllFrac_spdif_dec_n		: 4;
	U32 pllFrac_spdif_dec_en	: 1;
	U32 pdm_pwm_ds_1			: 1;
	U32 pdm_pwm_ds_2			: 1;
	U32 pdm_pwm_ds_3			: 1;
	U32 pdm_pwm_ds_4			: 1;
	U32 clk_sflash_ds			: 1;
	U32 clk_psram_ds			: 1;
} MISC_CPUCLK5_CONTROLs;


/*-----------------------------------------------------------------------------------------------------*/
/*
 * 	AHB Audio Block
 * 
 */

//digital audio post processing
//need more study with rtl
#define DA_PP_BASE_ADDR		              	0x13000000
#define DA_PP_CLASSD_FIFO_ADDR				0x13000400
#define DA_SPDIF_IN_FIFO_ADDR		       	0x13000800
#define DA_CD_IN_FIFO_ADDR					0x13001000
#define DA_I2S_IN_FIFO_ADDR					0x13001400
#define DA_I2S_OUT_FIFO_ADDR				0x13001800
#define DA_DMIC_FIFO_ADDR					0x13001C00              //dma 14
#define DA_ANC_BASE_ADDR		          	0x13002000



#define DA_PP_CLASSD_EN                       *( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*0))
#define DA_PP_MUTE_EN                         *( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*1))
#define DA_PP_CLASSD_CONFIG                   *( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*2))
#define DA_PP_CLASSD_CONFIG_AHB_2             *( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*3))
#define SPDIF_CONFIG                          *( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*4))
#define SPDIF_ENC_STATUS_L                    *( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*5))
#define SPDIF_ENC_STATUS_H                    *( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*6))
#define SPDIF_DEC_SAMPLE_RATE_DETECT          *( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*7))
#define I2S_STEREO_CONFIG                     *( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*8))
#define MI2S_RX_CONFIG                        *( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*9))
#define I2S_TDM_CONFIG_CDEF					  *( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*10))
#define DA_PP_CLASSD_CONFIG_AHB_3             *( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*15))
#define DA_PP_DC_BLOCK_R               		  *( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*16))			/*sampling clock is 8x*/
#define DA_PP_D2AYN_CONFIG               	  *( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*17))			/*11b, max 2047. for example 44.1 * 625 / 588 (DIV) = 46.875*/
#define DA_PP_D2AYN_FRAC               		  *( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*18))			/*frac 32b*/
#define DA_PP_PDM_SDM_LIM               	  *( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*19))			/*sdm limitation, defaut is 0x446*/



#define ANC_FB_EQ_BAND1_b0					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*30))
#define ANC_FB_EQ_BAND1_b1					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*31))
#define ANC_FB_EQ_BAND1_b2					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*32))
#define ANC_FB_EQ_BAND1_a1					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*33))
#define ANC_FB_EQ_BAND1_a2					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*34))

#define ANC_FB_EQ_BAND2_b0					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*35))
#define ANC_FB_EQ_BAND2_b1					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*36))
#define ANC_FB_EQ_BAND2_b2					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*37))
#define ANC_FB_EQ_BAND2_a1					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*38))
#define ANC_FB_EQ_BAND2_a2					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*39))

#define ANC_FB_EQ_BAND3_b0					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*40))
#define ANC_FB_EQ_BAND3_b1					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*41))
#define ANC_FB_EQ_BAND3_b2					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*42))
#define ANC_FB_EQ_BAND3_a1					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*43))
#define ANC_FB_EQ_BAND3_a2					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*44))


#define ANC_FF_EQ_BAND1_b0					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*45))
#define ANC_FF_EQ_BAND1_b1					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*46))
#define ANC_FF_EQ_BAND1_b2					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*47))
#define ANC_FF_EQ_BAND1_a1					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*48))
#define ANC_FF_EQ_BAND1_a2					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*49))

#define ANC_FF_EQ_BAND2_b0					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*50))
#define ANC_FF_EQ_BAND2_b1					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*51))
#define ANC_FF_EQ_BAND2_b2					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*52))
#define ANC_FF_EQ_BAND2_a1					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*53))
#define ANC_FF_EQ_BAND2_a2					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*54))

#define ANC_FF_EQ_BAND3_b0					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*55))
#define ANC_FF_EQ_BAND3_b1					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*56))
#define ANC_FF_EQ_BAND3_b2					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*57))
#define ANC_FF_EQ_BAND3_a1					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*58))
#define ANC_FF_EQ_BAND3_a2					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*59))

#define ANC_FF_EQ_BAND4_b0					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*60))
#define ANC_FF_EQ_BAND4_b1					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*61))
#define ANC_FF_EQ_BAND4_b2					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*62))
#define ANC_FF_EQ_BAND4_a1					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*63))
#define ANC_FF_EQ_BAND4_a2					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*64))

#define ANC_FF_EQ_BAND5_b0					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*65))
#define ANC_FF_EQ_BAND5_b1					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*66))
#define ANC_FF_EQ_BAND5_b2					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*67))
#define ANC_FF_EQ_BAND5_a1					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*68))
#define ANC_FF_EQ_BAND5_a2					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*69))

#define ANC_FF_EQ_BAND6_b0					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*70))
#define ANC_FF_EQ_BAND6_b1					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*71))
#define ANC_FF_EQ_BAND6_b2					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*72))
#define ANC_FF_EQ_BAND6_a1					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*73))
#define ANC_FF_EQ_BAND6_a2					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*74))

#define ANC_CONFIG							*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*75))
#define ANC_CONFIG_2						*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*76))

#define ANC_W								*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*77))
#define ANC_W2								*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*78))

#define QADC_DMIC_CONFIG					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*79))
#define QADC_DMIC_CONFIG_2					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*80))
#define QADC_DMIC_CONFIG_3					*( (volatile U32 *) (void *) (DA_PP_BASE_ADDR + 4*81))


#define ANC_FF_w1							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*0))
#define ANC_FF_w2							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*1))
#define ANC_FF_w3							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*2))
#define ANC_FF_w4							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*3))
#define ANC_FF_w5							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*4))
#define ANC_FF_w6							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*5))
#define ANC_FF_w7							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*6))
#define ANC_FF_w8							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*7))
#define ANC_FF_w9							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*8))
#define ANC_FF_w10							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*9))
#define ANC_FF_w11							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*10))
#define ANC_FF_w12							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*11))
#define ANC_FF_w13							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*12))
#define ANC_FF_w14							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*13))
#define ANC_FF_w15							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*14))
#define ANC_FF_w16							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*15))

#define ANC_FB_w1							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*16))
#define ANC_FB_w2							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*17))
#define ANC_FB_w3							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*18))
#define ANC_FB_w4							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*19))
#define ANC_FB_w5							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*20))
#define ANC_FB_w6							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*21))
#define ANC_FB_w7							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*22))
#define ANC_FB_w8							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*23))
#define ANC_FB_w9							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*24))
#define ANC_FB_w10							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*25))
#define ANC_FB_w11							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*26))
#define ANC_FB_w12							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*27))

#define ANC_FBMIC_MUSIC_w1					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*28))
#define ANC_FBMIC_MUSIC_w2					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*29))
#define ANC_FBMIC_MUSIC_w3					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*30))
#define ANC_FBMIC_MUSIC_w4					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*31))
#define ANC_FBMIC_MUSIC_w5					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*32))
#define ANC_FBMIC_MUSIC_w6					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*33))
#define ANC_FBMIC_MUSIC_w7					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*34))
#define ANC_FBMIC_MUSIC_w8					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*35))
#define ANC_FBMIC_MUSIC_w9					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*36))
#define ANC_FBMIC_MUSIC_w10					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*37))
#define ANC_FBMIC_MUSIC_w11					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*38))
#define ANC_FBMIC_MUSIC_w12					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*39))
#define ANC_FBMIC_MUSIC_w13					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*40))
#define ANC_FBMIC_MUSIC_w14					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*41))
#define ANC_FBMIC_MUSIC_w15					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*42))
#define ANC_FBMIC_MUSIC_w16					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*43))

#define ANC_SHZ_w1							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*44))
#define ANC_SHZ_w2							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*45))
#define ANC_SHZ_w3							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*46))
#define ANC_SHZ_w4							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*47))
#define ANC_SHZ_w5							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*48))
#define ANC_SHZ_w6							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*49))
#define ANC_SHZ_w7							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*50))
#define ANC_SHZ_w8							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*51))
#define ANC_SHZ_w9							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*52))
#define ANC_SHZ_w10							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*53))
#define ANC_SHZ_w11							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*54))
#define ANC_SHZ_w12							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*55))
#define ANC_SHZ_w13							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*56))
#define ANC_SHZ_w14							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*57))
#define ANC_SHZ_w15							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*58))
#define ANC_SHZ_w16							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*59))

#define ANC_FFMIC_ECHO_w1					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*60))
#define ANC_FFMIC_ECHO_w2					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*61))
#define ANC_FFMIC_ECHO_w3					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*62))
#define ANC_FFMIC_ECHO_w4					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*63))
#define ANC_FFMIC_ECHO_w5					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*64))
#define ANC_FFMIC_ECHO_w6					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*65))
#define ANC_FFMIC_ECHO_w7					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*66))
#define ANC_FFMIC_ECHO_w8					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*67))
#define ANC_FFMIC_ECHO_w9					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*68))
#define ANC_FFMIC_ECHO_w10					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*69))
#define ANC_FFMIC_ECHO_w11					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*70))
#define ANC_FFMIC_ECHO_w12					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*71))
#define ANC_FFMIC_ECHO_w13					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*72))
#define ANC_FFMIC_ECHO_w14					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*73))
#define ANC_FFMIC_ECHO_w15					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*74))
#define ANC_FFMIC_ECHO_w16					*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*75))


#define ANC_SHZ_b0							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*76))
#define ANC_SHZ_b1							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*77))
#define ANC_SHZ_b2							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*78))
#define ANC_SHZ_a1							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*79))
#define ANC_SHZ_a2							*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*80))

#define ANC_LMS_GAMA						*( (volatile U32 *) (void *) (DA_ANC_BASE_ADDR + 4*81))

/*
anc end
*/

//mi2s in
typedef volatile struct{
	U32 mi2s_enable 				: 1;				//  1'b1->enable, 1'b0->disable
	U32 mi2s_mode 					: 1;				//  1'b0->slave, 1'b1->master
	U32 mi2s_24_bit 				: 1;				//  1'b1->24b, 1'b0->16b
	U32 mi2s_fmt 					: 1;				//  1'b0->i2s, 1'b1->dsp
	U32 mi2s_swap 					: 1;				//  for cd right justify, high is left, for i2s, low is left
	U32 cdrom_en 					: 1;				//  1'b1->enable cdrom function
	U32 cdrom_discramble 			: 1;				//  1'b1->cd rom disacrable
	U32 cdrom_mode 					: 2;				//  mode21 or mode 2
	U32 mi2s_bck_div 				: 4;				//  256x_mclk / 2*bck_div; 0->bypass
	U32 mi2s_lrck_div 				: 6;				//  bck / lrck_div
	U32 mi2s_chan_mask 				: 4;				//  mi2s chan mask,  for cd it should be 4'b0001
	U32 mi2s_cd_sel 				: 1;				//  1'b0->cd in, 1'b1->mi2s in
	U32 mi2s_fifo_th				: 6;				//  mi2s fifo th, should be > dma bust size + margin(2)
	U32 mi2s_bck_lrck_sel			: 2;				//  2'b00->mi2s in, 2'b01->mi2s out, 2'b10->i2s srereo
} MI2S_RX_CONFIG_AHB;

//i2s st in/out
typedef volatile struct{
	U32 i2s_tx_enable 				: 1;				//  1'b1->enable, 1'b0->disable
	U32 i2s_rx_enable 				: 1;				//  1'b0->disable, 1'b1->enable
	U32 i2s_tx_24_bit 				: 1;				//  1'b1->24b, 1'b0->16b
	U32 i2s_rx_24_bit 				: 1;				//  1'b1->24b, 1'b0->16b
	U32 i2s_fmt 					: 1;				//  1'b0->i2s, 1'b1->dsp A
	U32 i2s_bck_lrck_sel 			: 2;				//  2'b00->i2s st, 2'b01->from mi2s out, 2'b10->from mi2s in
	U32 i2s_lrck_swap 				: 1;				//  1'b0->no swap, 1'b1->L/R swap
	U32 i2s_bck_div 				: 4;				//  256x_mclk / 2*bck_div; 0->bypass
	U32 i2s_lrck_div 				: 6;				//  bck / lrck_div
	U32 i2s_rx_mode 				: 1;				//  1'b0->slave, 1'b1->master
	U32 i2s_out_fifo_th				: 6;				//  fifo read min th, for example 2 chs 24 bit should set > 2 + margin(2)
	U32 i2s_in_fifo_th				: 6;				//  mi2s fifo th, should be > dma bust size + margin(2)
} I2S_ST_CONFIG_AHB;

//i2s dsp 
typedef volatile struct{
U32 i2s_stereo_data_in_2 			: 1;				//  1'b1->i2s stereo 2 data in
U32 i2s_stereo_tdm_32b 				: 1;				//  1'b1->i2s stereo dsp 32 clocks in one frame
U32 mi2s_in_tdm_32b 				: 1;				//  1'b1->mi2s in dsp 32 clocks in one frame 
U32 i2s_stereo_tdm_mono 			: 1;				//  1'b1->i2s stereo dsp mono
U32 mi2s_in_tdm_mono 				: 1;				//  1'b1->mi2s in dsp mono 
U32 cdef 							: 11;				//  cd ef, readonly
} I2S_TDM_CONFIG_CDEF_AHB;


//pdm/pwm/mi2s out
typedef volatile struct {
	U32 classd_data_24_bit          : 1;			//	1'b1->pcm is 24bit, 1'b0->pcm is 16bit
	U32 classd_6db                  : 1;			//	for 24b, 1'b1->pop data is 28b data, for 16b 1'b1->+6db gain
	U32 mode                   		: 1;			//	1'b1->pdm, 1'b0->pwm
	U32 osr_div                     : 3;			//	CLK_osr_32 = CLK_Audio/2^osr_div, 0->bypass
	U32 stream_type                 : 2;			//	2'b00->48k,2'b01->96k,2'b10->192k
	U32 sigma_delta_quan            : 4;			//	pwm sdm quan depends on pwm clk and sdm_ext
	U32 sdm_ext						: 1;			//	for pdm, 1'b1->sdm128x, 1'b0->sdm64x; for pwm 1'b1->sdm16x, 1'b0->sdm8x
	U32 pwm_sdm_o5           		: 1;			//	for pwm 1'b1->sdm O5, 1'b0->sdm O4
	U32 sd_reset_en                 : 1;			//	1'b1->zero data reset sdm, 1'b0->zero data don't reset sdm
	U32 mi2s_da_pp_n                : 1;			//	if mi2s_da_pp_n = 1'b1, d2asyn_en should be 1'b0
	U32 pwm_mi2s_chs                : 3;			//	3'b000->2 chs, 3'b001->4 chs,3'b010->6 chs, 3'b011->8 chs, 3'b100->10 chs, 3'b101->12 chs
	U32 classd_clk_err_en           : 1;
	U32 amclk_sel					: 1;			//	1'b0->256x, 1'b1->512x
	U32 pwm_delay                   : 6;
	U32 mi2s_lrck_swap         		: 1;
	U32 mi2s_sf_sel                 : 2;			//	if mi2s_da_pp_n = 1'b1->same with stream_type, others 2'b00->48k, 2'b01->96k, 2'b10->192k
	U32 mi2s_i2s_en                 : 1;			//	1'b1->enable mi2s, 1'b0->disable mi2s
	U32 pwm_reset_en                : 1;
} CLASSD_CONFIG_AHB;


//pwm/pdm config
typedef volatile struct {

	U32 pwm_drv_manul 				: 1;			//	1'b1->drv adjust by manual, 1'b0->drv adjust by auto
	U32 pwm_err_en 					: 1;			//	if enable, pwm_err reset pwm output
	U32 pwm_drv_in 					: 3;
	U32 pwm_drv_max 				: 3;
	U32 pwm_min 					: 5;			//	pwm min width
	U32 syncK						: 2;			//	2'b00->K2, 2'b01->K3, 2'b10->K4, default is K4 -> 40k < -100db, only for pdm
	U32 fifo_rst					: 1;			//	1'b1->reset classd fifo, 1'b0->normal work			
	U32 sdm_noise_en				: 1;			//	1'b1->add random noise to sdm input, only for pdm. default is disable
	U32 rsv							: 1;
	U32 dc_block_en					: 1;			//	1'b1->enable dc block, difault is disable
	U32 pwm_clk_en 					: 1;			//	for 768k dac, no need pwm clock		
	U32 osrn_en						: 1;			//	1'b0->no osr,sdm,pwm clock
	U32 ch_swap 					: 1;			//	1'b1->swap L/R chan
	U32 ch2_disable 				: 1;			//	only ch1 output
	U32 ch_mono						: 1;			//	1'b1->only mono music/voice input
	U32 fifo_rd_th					: 6;			//	fifo read min th, for example 8 chs 24 bit should set > 8  + margin(2)
	U32 sample_rx_sel				: 1;			//	1'b0->spdif_rx_samples,1'b1->mis in samples
	U32 sample_tx_sel				: 1;			//	1'b0->pwm/pdm/mi2s out samples,1'b1->i2s st samples
} CLASSD_CONFIG_AHB_2;


typedef volatile struct {
	U32 pwm_out_current 			: 16;			//	classd_config_ahb_3[15:0];
	U32 pwm_out_mode 				: 4;			//	classd_config_ahb_3[19:16];
	U32 pwm_mute 					: 4;			//	classd_config_ahb_3[23:20];
	U32 pwm_dt_sel 					: 1;			//	classd_config_ahb_3[24];
} CLASSD_CONFIG_AHB_3;


typedef volatile struct {
	U32 d2asyn_step          		: 11;
	U32 d2asyn_rom_L625         	: 1;			//	1'b1->46.875k, 1'b0->48k
	U32 d2asyn_en               	: 1;			//	1'b1->d2asyn enable, 1'b0->d2asyn disable
} CLASSD_D2ASYN_CONFIGs;


typedef volatile struct {	
	U32 spdif_dec_enable            : 1;
	U32 spdif_data_sel              : 2;  			//	spdif in pad selelction
	U32 spdif_dec_inv               : 1;
	U32 spdif_dec_sr_detect_start   : 1;			//	1'b1->detect sample frequency
	U32 spdif_dec_aux_mask          : 1;			//	1'b1->ony audio data
	U32 sync_err_auto_recover       : 1;			//	1'b1->if err, audo recovery, 1'b0->if err, stop rx
	U32 spdif_dec_data_24b          : 1;    		//	1'b1->24b, 1'b0->16b
	U32 spdif_fifo_rd_th			: 6;			//	spdif dec fifo th, should be > dma bust size + margin(2)
	U32 spdif_dec_clk_x4			: 1;			//	1'b1->spdif dec clk x 4,1'b0->spdif dec clk x 2
	U32 spdif_enc_enable            : 1;
	U32 spdif_enc_inv               : 1;
} SPDIF_CONFIG_AHB;


typedef volatile struct {	
   U32  dmic_clk_en     			:1;				//	should start dmic clk, then enable dmic
   U32  dmic1_en     				:1;
   U32  dmic2_en     				:1;
   U32  dmic3_en     				:1;
   U32  dmic4_en     				:1;
   U32  dmic_syncN					:2; 			//	2'b00->N=1(768k),2'b01->N=2(1.5M),2'b10->N=4(3M),2'b11->N=8(6M) 
   U32  dmic_synK					:1;				//	1'b1->K4, 1'b0->K3	
   U32  dmic_gain					:3;				//	3'b000->0db, 3'b001->+6db, 3'b010->+12db, 3'b011->+18db,  3'b111->-6db, 3'b110->-12db, 3'b101->-18db
   U32  dmic_se_eq					:1;				//	1'b0->dmic serial output without eq (for ANC low latancy), 1'b1->dmic serial output with eq	
   U32  dmic_eq_mask    			:4;				//	1'b1->enable eq band, 1;b0->dsiable eq band
   U32  dmic_down_N 				:7;				//	dmic down sample number based on 16x. 7'd16->dmic 48k output
   U32  dmic_16b					:1;				//	1'b1->dmic fifo 16b input, 1'b0->dmic fifo 32b input
   U32  dmic_fifo_th				:6;				//	fifo th full valus
   U32  dmic_fifo_en				:1;				//	enable dmic fifo
} DMIC_CONFIG_AHB;


typedef volatile struct {	
	U32 qadc_A_768k 				:1;				//	1'b1->768k for ANC
	U32 qadc_B_768k					:1;				//	1'b1->768k for ANC
	U32 qadc_C_768k 				:1;				//	1'b1->768k for ANC
	U32 qadc_D_768k					:1;				//	1'b1->768k for ANC

	U32 ancA_ff_sel					:3;				//	feed-forward mic A selection
	U32 ancB_ff_sel					:3;				//	feed-forward mic B selection
	U32 ancA_fb_sel					:3;				//	feedward mic A selection
	U32 ancB_fb_sel					:3;				//	feedward mic B selection

	U32 qadc_fifo_24b				:1;				//	1'b0->fifo data is 16b, 1'b1->fifo data is 32b with 4 bits guard

	U32 qadc_A_fifo_en				:1;				//	1'b1->qadc A to fifo, 1'b0->quad A not to fifo
	U32 qadc_B_fifo_en				:1;				//	1'b1->qadc B to fifo, 1'b0->quad B not to fifo
	U32 qadc_C_fifo_en				:1;				//	1'b1->qadc C to fifo, 1'b0->quad C not to fifo
	U32 qadc_D_fifo_en				:1;				//	1'b1->qadc D to fifo, 1'b0->quad D not to fifo

	U32 qadc3_dmic_rstn 			:1;
	U32 qadc3_dmic_vdd_ok 			:1;
	U32 qadc3_dmic_avdd_ok 			:1;
	U32 qadc3_dmic_force_sleep_ana 	:1;
	U32 qadc3_parallel 				:1;

	U32 qadc_A_master				:1;				//	1'b1->in msater, 1'b0->in slave. read only
	U32 qadc_B_master				:1;				//	1'b1->in msater, 1'b0->in slave. read only
	U32 qadc_C_master				:1;				//	1'b1->in msater, 1'b0->in slave. read only
	U32 qadc_D_master				:1;				//	1'b1->in msater, 1'b0->in slave. read only
	U32 dac_master				    :1;				//	1'b1->in msater, 1'b0->in slave. read only
} QADC_DMIC_CONFIG_AHB;

typedef volatile struct {	
	U32 qadc_dmic_lro_div			:12;			//	mclk/(qadc_dmic_lr_div+1)->sample frequency,for example 255->48Khz, if qadc_X_dmic_parallel = 1'b0, it should be 255

	U32 qadc_dmic_rstn 				:1;
	U32 qadc_dmic_vdd_ok 			:1;
	U32 qadc_dmic_avdd_ok 			:1;
	U32 qadc_dmic_force_sleep_ana 	:1;

	U32 qadc2_dmic_rstn 			:1;
	U32 qadc2_dmic_vdd_ok 			:1;
	U32 qadc2_dmic_avdd_ok 			:1;
	U32 qadc2_dmic_force_sleep_ana 	:1;

	U32 qadc_dmic_si2c 				:1; 			//	qadc input, 1'b1->i2c, 1'b0->apb

	U32 qadc_A_dmic_parallel 		:1;				//	1'1b->parallel data, 1'b0->serail data
	U32 qadc_B_dmic_parallel 		:1;				//	1'1b->parallel data, 1'b0->serail data
	U32 qadc_C_dmic_parallel 		:1;				//	1'1b->parallel data, 1'b0->serail data	
	U32 qadc_D_dmic_parallel 		:1;				//	1'1b->parallel data, 1'b0->serail data

	U32 qadc_clk_en 				:1;				//	1'b1->gen word sync for parallel other than 768k	
	U32 qadc_dmic_i2c_en 			:1;				//	1'b1->reqire i2c control, 1'b0->release i2c 	
	U32 qadc_i2s_clk_en 			:1;				//	1'b1->gen qadc bck, lrck			
	U32 qadc_dac_en		            :1;
	U32 qadc_mi2s_fifo_en			:1;				//	1'b1->qadc i2s fifo, 1'b0->parallel fifo
	U32 qadc_fifo_anc_music 		:1;				//	1'b1->cature music ch1 into qadc fifo (ch4 or ch8) for anc debug

	U32 qadc_dummy 		            :1;				//	dummy bit
} QADC_DMIC_CONFIG2_AHB;


typedef volatile struct {	
	U32 anc_en 						:1;				//	1'b1->ena enable,1'b0->ena disable
	U32 hybrid_fb_ena 				:1;				//	1'be->ebanle hybrid anc, 1'b0->only feed-forward anc support
	U32 fb_mixing_en 				:1;				//	1'b1->err+anc_out as feed backward anc ref signal, 1'b0->only err as ref sigrnal;

	U32 music_z_N	 				:3; 			//	to match speak to fb mic delay
	U32 music_echo_N 				:3;				//	to match speak to ff mic delay
	U32 ff_din_Z_N 					:4;				//	to match ff mic to speak delay
	U32 ff_lms_N_16 				:1;				//	1'b1->feed forward lms with 16 coeffs. 1'b0->12 coeffs
	U32 leaky_en	 				:1;				//	1'b1->ff/fb LMS leaky enable, 1'b0->ff/fb LMS leaky disable
	U32 adaptive_en 				:1;				//	1'b1->daptive enable, 1'b0->sdaptive disable, coeffs is configed by fw

	U32 music_lms_N_16 				:1;				//	1'b1->music lms with 16 coeffs. 1'b0->12 coeffs	
	U32 debug_sel 					:6;				//	sel which coeffs out 
	U32 debug_music_echo 			:1;				//	1'b1->debug speak to ff mic echo cancellation and get coeffs, 1'b0->normal work

	U32 update_cn 					:3;				//	coeffs update time
	U32 lms_w_freezing 				:1; 			//	1'b1->freeze update LMS coeffs

	U32 lms_hybrid_u_scale 			:2; 			//	2'b00->u=1/16,2'b01->u=1/32,2'b10->u=1/64,2'b11->u=1/128
	U32 lms_music_u_scale 			:2;  			//	2'b00->u=1/16,2'b01->u=1/32,2'b10->u=1/64,2'b11->u=1/128
} ANC_CONFIGs;


typedef volatile struct {	
	U32 shz_iir_ena 				:1;				//	1'b1->enable 
	U32 ff_iir_mask 				:6;				//	bit 1->enable
	U32 fb_iir_mask 				:3;				//	bit 1->enable
} ANC_CONFIGs_2;


/*-----------------------------------------------------------------------------------------------------*/
/*
 * 	SPI
 * 
 */
#define	BASE_ADDR_SPI				0x10300000
//#define	BASE_ADDR_SPI				0x10500000	//spi_2
#define	BASE_ADDR_SPI_TEST			0x10f00000

#define	BASE_ADDR_SPI2				0x10500000
#define	REG_SPI2_CONTROL			REG( BASE_ADDR_SPI2 + 0  )
#define	REG_SPI2_FORMAT				REG( BASE_ADDR_SPI2 + 4	 )
#define	REG_SPI2_INT_MASK			REG( BASE_ADDR_SPI2 + 8	 )
#define	REG_SPI2_STATUS				REG( BASE_ADDR_SPI2 + 12 )
#define	REG_SPI2_TX_DATA			REG( BASE_ADDR_SPI2 + 16 )
#define	REG_SPI2_RX_DATA			REG( BASE_ADDR_SPI2 + 20 )
#define	REG_SPI2_MAX_TRANS_NUM  	REG( BASE_ADDR_SPI2 + 24 )
#define	REG_SPI2_RXTX_NUM       	REG( BASE_ADDR_SPI2 + 28 )
#define	REG_SPI2_QSPI_CMD  	  		REG( BASE_ADDR_SPI2 + 32 )
#define	REG_SPI2_QSPI_CMD_2    		REG( BASE_ADDR_SPI2 + 36 )

#define	REG_SPI2_VERSION			REG( BASE_ADDR_SPI2 + 40 )


//-------------------------------------------------------------------------------------------
#define	REG_SPI_CONTROL	REG( BASE_ADDR_SPI + 0 )
typedef volatile struct{
	U32	reset						:1;				//	-	1 reset, 0 normal
	U32	slave						:1;				//	slave=1, 0 master
	U32 enable						:1;				//	enable=1
	U32 txFifoEn					:1;				//	txFifoEn,	if txFifoEn is 1, fill the tx fifo first even if enable is 0
	U32 rxFifoEn					:1;				//	rxFifoEn
	U32 dmaSel						:1;				//	0->spi write dma, 1->spi read dma
	U32 div							:4;				//	clkDivisor=2,4,8,16,32,64,128,256,512,1024,2048,4096
	U32 bits						:5;				//	7=8 bits,9 bits,10 bits,11 bits,12 bits,13 bits,14 bits,15 bits,16 bits,
	U32	qspi						:1;				//	1->qspi out, shoud set din_out = 1;
} SPI_CONTROLs;

//spi tx fifo (bits,depth,tfull) = (32,32,19)
//spi rx fifo (bits,depth,tfull) = (16,16,9)

//-------------------------------------------------------------------------------------------
#define	REG_SPI_FORMAT	REG( BASE_ADDR_SPI + 4)
typedef volatile struct{
	U32	cpha						:1;				//	0=sck low in idle,1=sck high
	U32 cpol						:1;				//	0=data valid at sck high,1=valid at low
	U32 rsv_1						:3;	
	U32 autocsActive 				:1;				//	0 -> ----|____|---- , 1 -> ____|----|____
	U32 rsv_2						:3;				//	inactive cs hold time
	U32 cssel						:2;	
	U32 din_out						:1;				//	1->spi din and dout is same pin, only for mater mode
} SPI_FORMATs;

//-------------------------------------------------------------------------------------------
#define	REG_SPI_INT_MASK	REG( BASE_ADDR_SPI + 8)
#define	REG_SPI_STATUS		REG( BASE_ADDR_SPI + 12)

/*
	T->Threshhod	
*/
typedef volatile struct{
	U32	txBufEmpty					:1;				//	txBufEmpty
	U32 txBufTFull_n				:1;				//	~txBufTfull
	U32 txBufFull					:1;				//	 txBufFull
	U32 rxBufEmpty					:1;				//	rxBufEmpty
	U32 rxBufEmpty_n				:1;				//	~rxBufEmpty
	U32 rxBufFull					:1;
	U32 rxBufTFull					:1;				//	rxBufHalfFull,
	U32 idle						:1;				//	idle
	U32 txErr						:1;				//	no enough tx data, tx erri
} SPI_INT_MASKs;

#define SPI_STATUSs SPI_INT_MASKs

#define SPI_INT_CONTROL SPI_INT_MASKs

//-------------------------------------------------------------------------------------------
#define	REG_SPI_TX_DATA	REG( BASE_ADDR_SPI + 16 )
#define	REG_SPI_RX_DATA	REG( BASE_ADDR_SPI + 20 )

/*
	for Master and auto mode
	spi tx will start and continous on the following condition:
		tx data is filled
		if no tx data filled, tx num < {REG_SPI_TRANS_NUM_H,REG_SPI_MAX_TRANS_NUM}
	spi rx will start after spi tx send byte(s) >= {REG_SPI_TRANS_NUM_H,REG_SPI_RXTX_NUM byte(s)}	
*/

#define	REG_SPI_MAX_TRANS_NUM  REG( BASE_ADDR_SPI + 24 ) /*20b*/
#define	REG_SPI_RXTX_NUM       REG( BASE_ADDR_SPI + 28 ) /*20b*/

/* qspi only support the following 3 commands */

#define ST_TFT_QSPI             0xDE /* cmd + 24b addr + q data */
#define WINBOND_QSPI_WRITE      0x32 /* cmd + 24b addr + q data */
#define WINBOND_QSPI_READ       0x6B /* cmd + 24b addr + 8b dummy + q data */

#define	REG_SPI_QSPI_CMD  	  REG( BASE_ADDR_SPI + 32 )
#define	REG_SPI_QSPI_CMD_2    REG( BASE_ADDR_SPI + 36 )

#define	REG_SPI_VERSION	REG( BASE_ADDR_SPI + 40 )

/*--------------------------------------------------------------------------------------------*/
//MISC
/*--------------------------------------------------------------------------------------------*/

#define BASE_ADDR_MISC	0x18800000
#define MISC_CONFIG_REG REG(BASE_ADDR_MISC)
typedef volatile struct {
	U32	mclksel		: 2;	// set 2bit( 01 is internal PLL,	10 is external PLL)
} MISC_CLKSELs;

#define bI2SMISC_CLKSEL	(*(MISC_CLKSELs*)&MISC_CONFIG_REG).mclksel

#define MISC_VERSION_REG REG(BASE_ADDR_MISC + 0x04)




/*-----------------------------------------------------------------------------------------------------*/
/*
 * 	UART
 * 
 */

#define	UART0_BASE_ADDR		0x10200000
#define	UART1_BASE_ADDR		0x10400000
#define	UART2_BASE_ADDR		0x10C00000

#define	REG_UART0_CONTRL	REG( UART0_BASE_ADDR + 0 )
#define	REG_UART0_BAUDRATE	REG( UART0_BASE_ADDR + 4 )				//8 bit baudrate
#define	REG_UART0_STATUS	REG( UART0_BASE_ADDR + 8 )
#define	REG_UART0_INT_ENA	REG( UART0_BASE_ADDR + 12 )
#define	REG_UART0_TX_DATA	REG( UART0_BASE_ADDR + 20 )
#define	REG_UART0_RX_DATA	REG( UART0_BASE_ADDR + 24 )
#define	REG_UART0_VERSION	REG( UART0_BASE_ADDR + 28 )

#define	REG_UART1_CONTRL	REG( UART1_BASE_ADDR + 0 )
#define	REG_UART1_BAUDRATE	REG( UART1_BASE_ADDR + 4 )				
#define	REG_UART1_STATUS	REG( UART1_BASE_ADDR + 8 )
#define	REG_UART1_INT_ENA	REG( UART1_BASE_ADDR + 12 )
#define	REG_UART1_TX_DATA	REG( UART1_BASE_ADDR + 20 )
#define	REG_UART1_RX_DATA	REG( UART1_BASE_ADDR + 24 )
#define	REG_UART1_VERSION	REG( UART1_BASE_ADDR + 28 )

#define	REG_UART2_CONTRL	REG( UART2_BASE_ADDR + 0 )
#define	REG_UART2_BAUDRATE	REG( UART2_BASE_ADDR + 4 )				
#define	REG_UART2_STATUS	REG( UART2_BASE_ADDR + 8 )
#define	REG_UART2_INT_ENA	REG( UART2_BASE_ADDR + 12 )
#define	REG_UART2_TX_DATA	REG( UART2_BASE_ADDR + 20 )
#define	REG_UART2_RX_DATA	REG( UART2_BASE_ADDR + 24 )
#define	REG_UART2_VERSION	REG( UART2_BASE_ADDR + 28 )

typedef volatile struct{
	U32	tx_ena			: 1;		//tx_ena
	U32	rx_ena			: 1;		//rx_ena
	U32	tx_fifo_ena		: 1;		//tx_fifo_ena,if tx_fifo_ena is 1, fill the tx fifo first even if tx_ena is 0
	U32	rx_fifo_ena		: 1;		//rx_fifo_ena
	U32 parity			: 2;		//parity
} UART_CONTROLs;


typedef volatile struct{
	U32	txBufEmpty			: 1;	//txBufEmpty
	U32 txBufTFull_n		: 1 ;	//~txBufTfull
	U32 txBufFull			: 1;	//txBufFull
	U32 rxBufEmpty			: 1;	//rxBufEmpty
	U32 rxBufEmpty_n		: 1;	//~rxBufEmpty
	U32 rxBufFull			: 1;	//rxBufFull
	U32 rxBufTFull			: 1;	//rxBufTFull,
	U32 rxFrameErr			: 1;	//err
} UART_STATUSs;


typedef volatile struct{
	U32 tx_buf_empty 		:1;
	U32 tx_buf_half_empty 	:1;
	U32 tx_buf_full 		:1;
	U32 rx_buf_empty 		:1;
	U32 rx_buf_not_empty 	:1;
	U32 rx_buf_full 		:1;
	U32 rx_buf_half_full 	:1;
	U32 rx_overrun 			:1;
	U32 int_err				:1;
} UART_INT_ENA;

typedef volatile struct{
	U32	txBufEmpty			: 1;	//txBufEmpty
	U32 txBufTFull_n		: 1 ;	//~txBufTfull
	U32 txBufFull			: 1;	//txBufFull
	U32 rxBufEmpty			: 1;	//rxBufEmpty
	U32 rxBufEmpty_n		: 1;	//~rxBufEmpty
	U32 rxBufFull			: 1;	//rxBufFull
	U32 rxBufTFull			: 1;	//rxBufTFull,
	U32 rxFrameErr			: 1;	//err
} UART_INT_MASKs;

typedef	struct {
	UART_CONTROLs	control;
	U32	baudrate_divisor;
	UART_STATUSs status;
	UART_INT_ENA intEnable;
	U32	reserved;
	U32	txBuf;
	U32	rxBuf;
	U32	version;
} UART_REGISTERS;


/*------------------------------------------------------------------------------
*
*	I2C
*/

#define BASE_ADDR_I2C			0x10700000

//-------------------------------------------------------------------------------------------
#define	REG_I2C_CONTROL	REG( BASE_ADDR_I2C + 0 )

typedef volatile struct{
	U32	rsv			: 1;		//1 reset, 0 normal
	U32	slave		: 1;		//slave=1, 0 master
	U32 enable		: 1;		//enable
	U32 txFifoEn	: 1;		//txFifoEn,if txFifoEn is 1, fill the tx fifo first even if enable is 0
	U32 rxFifoEn	: 1;		//rxFifiEnable
	U32 dmaSel		: 1;		/*0->i2c write dma, 1->i2c read dma*/
} I2C_CONTROLs;

//-------------------------------------------------------------------------------------------
#define	REG_I2C_FORMAT	REG( BASE_ADDR_I2C + 4)
typedef volatile struct{
	U32 div			: 4;		//clkDivisor=2,4,8,16,32,64,128,256,512,1024,2048,4096		
} I2C_FORMATs;

//-------------------------------------------------------------------------------------------
#define	REG_I2C_INT_ENABLE	REG( BASE_ADDR_I2C + 8)
#define	REG_I2C_INT_STATUS	REG( BASE_ADDR_I2C + 12)

/*
	T->Threshhod	
*/
typedef volatile struct{
	U32	txBufEmpty			: 1;	//txBufEmpty
	U32 txBufTFull_n		: 1 ;	//~txBufTfull
	U32 txBufFull			: 1;	// txBufFull
	U32 rxBufEmpty_n		: 1;	//~rxBufEmpty
	U32 rxBufFull			: 1;
	U32 rxBufTFull			: 1;	//rxBufHalfFull,
	U32 idle				: 1;	//idle
	U32 txErr				: 1;	//no enough tx data, tx erri
} I2C_INT_MASKs;

#define I2C_STATUSs I2C_INT_MASKs

typedef volatile struct{
	U32	read_write_n		: 1;	//max_trans_num h
	U32 addr				: 7;	//rxtx_num h
} I2C_ADDR;

typedef volatile struct{
	U32	rsv_1				: 1;	//max_trans_num h
	U32 mask				: 7;	//rxtx_num h
} I2C_ADDR_MASK;

typedef volatile struct{
	U32	read_write_n		: 1;	//max_trans_num h
	U32 addr				: 7;	//rxtx_num h
} I2C_ADDR_RXed;


//-------------------------------------------------------------------------------------------
#define	REG_I2C_TX_DATA				REG( BASE_ADDR_I2C + 16 )
#define	REG_I2C_RX_DATA				REG( BASE_ADDR_I2C + 20 )
#define	REG_I2C_VERSION				REG( BASE_ADDR_I2C + 24 )

#define	REG_I2C_MAX_TX_NUM_H		REG( BASE_ADDR_I2C + 28 )
#define	REG_I2C_MAX_TX_NUM			REG( BASE_ADDR_I2C + 32 )
#define	REG_I2C_MAX_RX_NUM_H		REG( BASE_ADDR_I2C + 36 )
#define	REG_I2C_MAX_RX_NUM			REG( BASE_ADDR_I2C + 40 )


#define	REG_I2C_ADDR				REG( BASE_ADDR_I2C + 44 )
#define	REG_I2C_ADDR_MASK			REG( BASE_ADDR_I2C + 48 )
#define	REG_I2C_RX_ADDR				REG( BASE_ADDR_I2C + 52 )


/*-----------------------------------------------------------------------------------------------------*/
/*
 * 	HDMI CEC 
 * 
 */

#define	HDMICEC_BASE_ADDR       0x18000000

#define	REG_HDMICEC_CONTRL	REG( HDMICEC_BASE_ADDR + 0 )

typedef volatile struct{
	U32 enable		: 1;		//enable
	U32 laddr		: 4;		//logic address
} HDMICEC_CONTROLs;

#define	REG_HDMICEC_STATUS	REG( HDMICEC_BASE_ADDR + 4 )
#define	REG_HDMICEC_INT_ENA	REG( HDMICEC_BASE_ADDR + 8 )

typedef volatile struct{
	U32	txEmpty				: 1;	//
	U32 txFifoUnderflow		: 1;	//
	U32 txAckErr			: 1;	//
	U32 rxOverrun			: 1;	//
	U32 rxEmpty_n			: 1;	//
	U32 txBusErr			: 1;	//
	U32 txFifoOverflow		: 1;	//
	U32 idle				: 1;	//
} HDMICEC_STATUSs;

#define HDMICEC_INT_MASKs	HDMICEC_STATUSs

#define	REG_HDMICEC_TX_DATA	REG( HDMICEC_BASE_ADDR + 12 )
#define	REG_HDMICEC_RX_DATA	REG( HDMICEC_BASE_ADDR + 16 )
#define	REG_HDMICEC_VERSION	REG( HDMICEC_BASE_ADDR + 20 )

typedef volatile struct{
	U32	eom				: 1;	//
	U32 data			: 8;	//
} HDMICEC_DATAs;

/*-----------------------------------------------------------------------------------------------------*/
/*
 * 	QADC/DAC/DMIC 
 * 
 */

#define	QADC_DAC_DMIC_BASE_ADDR       		0x18100000

#define QADC1_BASE_ADDR			  			0x18100000
#define QADC2_BASE_ADDR			  			0x18100200
#define QADC3_BASE_ADDR			  			0x18100400
#define QADC4_BASE_ADDR			  			0x18100600

#define READ_ADDR_OFFSET 					0X800
#define QADC1_BASE_READ_ADDR			  	(QADC1_BASE_ADDR + READ_ADDR_OFFSET)
#define QADC2_BASE_RAED_ADDR			  	(QADC2_BASE_ADDR + READ_ADDR_OFFSET)
#define QADC3_BASE_READ_ADDR			  	(QADC3_BASE_ADDR + READ_ADDR_OFFSET)
#define QADC4_BASE_READ_ADDR			  	(QADC4_BASE_ADDR + READ_ADDR_OFFSET)


#define QADC1_I2C_ADDRESS					0x25
#define QADC2_I2C_ADDRESS	 				0x2A
#define QADC3_I2C_ADDRESS 					0x22
#define QADC4_I2C_ADDRESS 					0x2c

#define SR									0x00
#define SR_PUN								0x01
#define CR_VIC								0x02
#define CR_CK								0x03
#define CR_ADCA_AI							0x04
#define CR_ADCA_AI_SB						0x05
#define CR_ADCA_AI_MIX_1					0x06
#define CR_ADCB_AI							0x07
#define CR_ADCB_AI_SB						0x08
#define CR_ADCB_AI_MIX_1					0x09
#define CR_ADCA_FREQ_SEL					0x0A
#define CR_ADCB_FREQ_SEL					0x0B
#define CR_ADCA_DMD_FLT_SEL					0x0C
#define CR_ADCA_DEC_FLT_SEL					0x0D
#define CR_ADCA_DEC_OSR						0x0E
#define CR_ADCB_DMD_FLT_SEL					0x0F
#define CR_ADCB_DEC_FLT_SEL					0x10
#define CR_ADCB_DEC_OSR						0x11
#define CR_ADCA_HPF							0x12
#define CR_ADCB_HPF							0x13
#define CR_ADCA_WNF							0x14
#define CR_ADCB_WNF							0x15
#define CR_DMIC_ADCA_SB						0x16
#define CR_DMIC_ADCA						0x17
#define CR_DMIC_ADCA_12_RATE				0x18	
#define CR_MIC_ADCA_12_SEL					0x19
#define CR_DMIC_ADCB_SB						0x1A
#define CR_DMIC_ADCB						0x1B
#define CR_DMIC_ADCB_12_RATE				0x1C
#define CR_MIC_ADCB_12_SEL					0x1D
#define CR_ADC_MICA1						0x1E
#define CR_ADC_MICA2						0x1F
#define CR_ADC_MICB1						0x20			
#define CR_ADC_MICB2						0x21
#define GCR_MICA12							0x22
#define GCR_MICB12							0x23
#define CR_ADCA12							0x24
#define CR_ADCB12							0x25
#define CR_ADCA1_DGAIN						0x26
#define CR_ADCA2_DGAIN						0x27
#define CR_ADCB1_DGAIN						0x28
#define CR_ADCB2_DGAIN						0x29
#define ADCA_12_ALC_0						0x2A
#define ADCA_12_ALC_1						0x2B
#define ADCA_12_ALC_2						0x2C
#define ADCA_12_ALC_3						0x2D
#define ADCA_12_ALC_4						0x2E
#define ADCB_12_ALC_0						0x2F
#define ADCB_12_ALC_1						0x30
#define ADCB_12_ALC_2						0x31
#define ADCB_12_ALC_3						0x32
#define ADCB_12_ALC_4						0x33
#define TR_DIG0								0x34
#define TR_DIG1								0x35
#define TR_DIG3								0x36
#define TR_DIG4								0x37
#define TR_DIG5								0x38
#define TR_DIG6								0x39
#define TR_ANA0								0x3A
#define TR_ANA1								0x3B
#define TR_ANA2								0x3C
#define TR_ANA6								0x3D
#define SR_TR_ANA0							0x3E
#define FR_TR_ANA0							0x3F


#define QADC1_READ_CR_VIC					REG(QADC1_BASE_READ_ADDR+4*CR_VIC)

#define QADC1_SR							REG(QADC1_BASE_ADDR+4*SR)				
#define QADC1_SR_PUN						REG(QADC1_BASE_ADDR+4*SR_PUN)				
#define QADC1_CR_VIC						REG(QADC1_BASE_ADDR+4*CR_VIC)				
#define QADC1_CR_CK							REG(QADC1_BASE_ADDR+4*CR_CK)				
#define QADC1_CR_ADCA_AI					REG(QADC1_BASE_ADDR+4*CR_ADCA_AI)			
#define QADC1_CR_ADCA_AI_SB					REG(QADC1_BASE_ADDR+4*CR_ADCA_AI_SB)			
#define QADC1_CR_ADCA_AI_MIX_1				REG(QADC1_BASE_ADDR+4*CR_ADCA_AI_MIX_1)		
#define QADC1_CR_ADCB_AI					REG(QADC1_BASE_ADDR+4*CR_ADCB_AI)			
#define QADC1_CR_ADCB_AI_SB					REG(QADC1_BASE_ADDR+4*CR_ADCB_AI_SB)			
#define QADC1_CR_ADCB_AI_MIX_1				REG(QADC1_BASE_ADDR+4*CR_ADCB_AI_MIX_1)		
#define QADC1_CR_ADCA_FREQ_SEL				REG(QADC1_BASE_ADDR+4*CR_ADCA_FREQ_SEL)		
#define QADC1_CR_ADCB_FREQ_SEL				REG(QADC1_BASE_ADDR+4*CR_ADCB_FREQ_SEL)		
#define QADC1_CR_ADCA_DMD_FLT_SEL			REG(QADC1_BASE_ADDR+4*CR_ADCA_DMD_FLT_SEL)		
#define QADC1_CR_ADCA_DEC_FLT_SEL			REG(QADC1_BASE_ADDR+4*CR_ADCA_DEC_FLT_SEL)		
#define QADC1_CR_ADCA_DEC_OSR				REG(QADC1_BASE_ADDR+4*CR_ADCA_DEC_OSR)			
#define QADC1_CR_ADCB_DMD_FLT_SEL			REG(QADC1_BASE_ADDR+4*CR_ADCB_DMD_FLT_SEL)		
#define QADC1_CR_ADCB_DEC_FLT_SEL			REG(QADC1_BASE_ADDR+4*CR_ADCB_DEC_FLT_SEL)		
#define QADC1_CR_ADCB_DEC_OSR				REG(QADC1_BASE_ADDR+4*CR_ADCB_DEC_OSR)			
#define QADC1_CR_ADCA_HPF					REG(QADC1_BASE_ADDR+4*CR_ADCA_HPF)			
#define QADC1_CR_ADCB_HPF					REG(QADC1_BASE_ADDR+4*CR_ADCB_HPF)			
#define QADC1_CR_ADCA_WNF					REG(QADC1_BASE_ADDR+4*CR_ADCA_WNF)			
#define QADC1_CR_ADCB_WNF					REG(QADC1_BASE_ADDR+4*CR_ADCB_WNF)	
#define QADC1_CR_DMIC_ADCA_SB				REG(QADC1_BASE_ADDR+4*CR_DMIC_ADCA_SB)			
#define QADC1_CR_DMIC_ADCA					REG(QADC1_BASE_ADDR+4*CR_DMIC_ADCA)			
#define QADC1_CR_DMIC_ADCA_12_RATE			REG(QADC1_BASE_ADDR+4*CR_DMIC_ADCA_12_RATE)		
#define QADC1_CR_MIC_ADCA_12_SEL			REG(QADC1_BASE_ADDR+4*CR_MIC_ADCA_12_SEL)		
#define QADC1_CR_DMIC_ADCB_SB				REG(QADC1_BASE_ADDR+4*CR_DMIC_ADCB_SB)			
#define QADC1_CR_DMIC_ADCB					REG(QADC1_BASE_ADDR+4*CR_DMIC_ADCB)			
#define QADC1_CR_DMIC_ADCB_12_RATE			REG(QADC1_BASE_ADDR+4*CR_DMIC_ADCB_12_RATE)		
#define QADC1_CR_MIC_ADCB_12_SEL			REG(QADC1_BASE_ADDR+4*CR_MIC_ADCB_12_SEL)		
#define QADC1_CR_ADC_MICA1					REG(QADC1_BASE_ADDR+4*CR_ADC_MICA1)			
#define QADC1_CR_ADC_MICA2					REG(QADC1_BASE_ADDR+4*CR_ADC_MICA2)			
#define QADC1_CR_ADC_MICB1					REG(QADC1_BASE_ADDR+4*CR_ADC_MICB1)			
#define QADC1_CR_ADC_MICB2					REG(QADC1_BASE_ADDR+4*CR_ADC_MICB2)			
#define QADC1_GCR_MICA12					REG(QADC1_BASE_ADDR+4*GCR_MICA12)			
#define QADC1_GCR_MICB12					REG(QADC1_BASE_ADDR+4*GCR_MICB12)			
#define QADC1_CR_ADCA12						REG(QADC1_BASE_ADDR+4*CR_ADCA12)			
#define QADC1_CR_ADCB12						REG(QADC1_BASE_ADDR+4*CR_ADCB12)			
#define QADC1_CR_ADCA1_DGAIN				REG(QADC1_BASE_ADDR+4*CR_ADCA1_DGAIN)			
#define QADC1_CR_ADCA2_DGAIN				REG(QADC1_BASE_ADDR+4*CR_ADCA2_DGAIN)			
#define QADC1_CR_ADCB1_DGAIN				REG(QADC1_BASE_ADDR+4*CR_ADCB1_DGAIN)			
#define QADC1_CR_ADCB2_DGAIN				REG(QADC1_BASE_ADDR+4*CR_ADCB2_DGAIN)			
#define QADC1_ADCA_12_ALC_0					REG(QADC1_BASE_ADDR+4*ADCA_12_ALC_0)			
#define QADC1_ADCA_12_ALC_1					REG(QADC1_BASE_ADDR+4*ADCA_12_ALC_1)			
#define QADC1_ADCA_12_ALC_2					REG(QADC1_BASE_ADDR+4*ADCA_12_ALC_2)			
#define QADC1_ADCA_12_ALC_3					REG(QADC1_BASE_ADDR+4*ADCA_12_ALC_3)			
#define QADC1_ADCA_12_ALC_4					REG(QADC1_BASE_ADDR+4*ADCA_12_ALC_4)			
#define QADC1_ADCB_12_ALC_0					REG(QADC1_BASE_ADDR+4*ADCB_12_ALC_0)			
#define QADC1_ADCB_12_ALC_1					REG(QADC1_BASE_ADDR+4*ADCB_12_ALC_1)			
#define QADC1_ADCB_12_ALC_2					REG(QADC1_BASE_ADDR+4*ADCB_12_ALC_2)			
#define QADC1_ADCB_12_ALC_3					REG(QADC1_BASE_ADDR+4*ADCB_12_ALC_3)			
#define QADC1_ADCB_12_ALC_4					REG(QADC1_BASE_ADDR+4*ADCB_12_ALC_4)			
#define QADC1_TR_DIG0						REG(QADC1_BASE_ADDR+4*TR_DIG0)				
#define QADC1_TR_DIG1						REG(QADC1_BASE_ADDR+4*TR_DIG1)				
#define QADC1_TR_DIG3						REG(QADC1_BASE_ADDR+4*TR_DIG3)				
#define QADC1_TR_DIG4						REG(QADC1_BASE_ADDR+4*TR_DIG4)				
#define QADC1_TR_DIG5						REG(QADC1_BASE_ADDR+4*TR_DIG5)				
#define QADC1_TR_DIG6						REG(QADC1_BASE_ADDR+4*TR_DIG6)				
#define QADC1_TR_ANA0						REG(QADC1_BASE_ADDR+4*TR_ANA0)				
#define QADC1_TR_ANA1						REG(QADC1_BASE_ADDR+4*TR_ANA1)				
#define QADC1_TR_ANA2						REG(QADC1_BASE_ADDR+4*TR_ANA2)				
#define QADC1_TR_ANA6						REG(QADC1_BASE_ADDR+4*TR_ANA6)				
#define QADC1_SR_TR_ANA0					REG(QADC1_BASE_ADDR+4*SR_TR_ANA0)			
#define QADC1_FR_TR_ANA0					REG(QADC1_BASE_ADDR+4*FR_TR_ANA0)			

#define QADC2_SR							REG(QADC2_BASE_ADDR+4*SR)				
#define QADC2_SR_PUN						REG(QADC2_BASE_ADDR+4*SR_PUN)				
#define QADC2_CR_VIC						REG(QADC2_BASE_ADDR+4*CR_VIC)				
#define QADC2_CR_CK							REG(QADC2_BASE_ADDR+4*CR_CK)				
#define QADC2_CR_ADCA_AI					REG(QADC2_BASE_ADDR+4*CR_ADCA_AI)			
#define QADC2_CR_ADCA_AI_SB					REG(QADC2_BASE_ADDR+4*CR_ADCA_AI_SB)			
#define QADC2_CR_ADCA_AI_MIX_1				REG(QADC2_BASE_ADDR+4*CR_ADCA_AI_MIX_1)		
#define QADC2_CR_ADCB_AI					REG(QADC2_BASE_ADDR+4*CR_ADCB_AI)			
#define QADC2_CR_ADCB_AI_SB					REG(QADC2_BASE_ADDR+4*CR_ADCB_AI_SB)			
#define QADC2_CR_ADCB_AI_MIX_1				REG(QADC2_BASE_ADDR+4*CR_ADCB_AI_MIX_1)		
#define QADC2_CR_ADCA_FREQ_SEL				REG(QADC2_BASE_ADDR+4*CR_ADCA_FREQ_SEL)		
#define QADC2_CR_ADCB_FREQ_SEL				REG(QADC2_BASE_ADDR+4*CR_ADCB_FREQ_SEL)		
#define QADC2_CR_ADCA_DMD_FLT_SEL			REG(QADC2_BASE_ADDR+4*CR_ADCA_DMD_FLT_SEL)		
#define QADC2_CR_ADCA_DEC_FLT_SEL			REG(QADC2_BASE_ADDR+4*CR_ADCA_DEC_FLT_SEL)		
#define QADC2_CR_ADCA_DEC_OSR				REG(QADC2_BASE_ADDR+4*CR_ADCA_DEC_OSR)			
#define QADC2_CR_ADCB_DMD_FLT_SEL			REG(QADC2_BASE_ADDR+4*CR_ADCB_DMD_FLT_SEL)		
#define QADC2_CR_ADCB_DEC_FLT_SEL			REG(QADC2_BASE_ADDR+4*CR_ADCB_DEC_FLT_SEL)		
#define QADC2_CR_ADCB_DEC_OSR				REG(QADC2_BASE_ADDR+4*CR_ADCB_DEC_OSR)			
#define QADC2_CR_ADCA_HPF					REG(QADC2_BASE_ADDR+4*CR_ADCA_HPF)			
#define QADC2_CR_ADCB_HPF					REG(QADC2_BASE_ADDR+4*CR_ADCB_HPF)			
#define QADC2_CR_ADCA_WNF					REG(QADC2_BASE_ADDR+4*CR_ADCA_WNF)			
#define QADC2_CR_ADCB_WNF					REG(QADC2_BASE_ADDR+4*CR_ADCB_WNF)		
#define QADC2_CR_DMIC_ADCA_SB				REG(QADC2_BASE_ADDR+4*CR_DMIC_ADCA_SB)			
#define QADC2_CR_DMIC_ADCA					REG(QADC2_BASE_ADDR+4*CR_DMIC_ADCA)			
#define QADC2_CR_DMIC_ADCA_12_RATE			REG(QADC2_BASE_ADDR+4*CR_DMIC_ADCA_12_RATE)		
#define QADC2_CR_MIC_ADCA_12_SEL			REG(QADC2_BASE_ADDR+4*CR_MIC_ADCA_12_SEL)		
#define QADC2_CR_DMIC_ADCB_SB				REG(QADC2_BASE_ADDR+4*CR_DMIC_ADCB_SB)			
#define QADC2_CR_DMIC_ADCB					REG(QADC2_BASE_ADDR+4*CR_DMIC_ADCB)			
#define QADC2_CR_DMIC_ADCB_12_RATE			REG(QADC2_BASE_ADDR+4*CR_DMIC_ADCB_12_RATE)		
#define QADC2_CR_MIC_ADCB_12_SEL			REG(QADC2_BASE_ADDR+4*CR_MIC_ADCB_12_SEL)		
#define QADC2_CR_ADC_MICA1					REG(QADC2_BASE_ADDR+4*CR_ADC_MICA1)			
#define QADC2_CR_ADC_MICA2					REG(QADC2_BASE_ADDR+4*CR_ADC_MICA2)			
#define QADC2_CR_ADC_MICB1					REG(QADC2_BASE_ADDR+4*CR_ADC_MICB1)			
#define QADC2_CR_ADC_MICB2					REG(QADC2_BASE_ADDR+4*CR_ADC_MICB2)			
#define QADC2_GCR_MICA12					REG(QADC2_BASE_ADDR+4*GCR_MICA12)			
#define QADC2_GCR_MICB12					REG(QADC2_BASE_ADDR+4*GCR_MICB12)			
#define QADC2_CR_ADCA12						REG(QADC2_BASE_ADDR+4*CR_ADCA12)			
#define QADC2_CR_ADCB12						REG(QADC2_BASE_ADDR+4*CR_ADCB12)			
#define QADC2_CR_ADCA1_DGAIN				REG(QADC2_BASE_ADDR+4*CR_ADCA1_DGAIN)			
#define QADC2_CR_ADCA2_DGAIN				REG(QADC2_BASE_ADDR+4*CR_ADCA2_DGAIN)			
#define QADC2_CR_ADCB1_DGAIN				REG(QADC2_BASE_ADDR+4*CR_ADCB1_DGAIN)			
#define QADC2_CR_ADCB2_DGAIN				REG(QADC2_BASE_ADDR+4*CR_ADCB2_DGAIN)			
#define QADC2_ADCA_12_ALC_0					REG(QADC2_BASE_ADDR+4*ADCA_12_ALC_0)			
#define QADC2_ADCA_12_ALC_1					REG(QADC2_BASE_ADDR+4*ADCA_12_ALC_1)			
#define QADC2_ADCA_12_ALC_2					REG(QADC2_BASE_ADDR+4*ADCA_12_ALC_2)			
#define QADC2_ADCA_12_ALC_3					REG(QADC2_BASE_ADDR+4*ADCA_12_ALC_3)			
#define QADC2_ADCA_12_ALC_4					REG(QADC2_BASE_ADDR+4*ADCA_12_ALC_4)			
#define QADC2_ADCB_12_ALC_0					REG(QADC2_BASE_ADDR+4*ADCB_12_ALC_0)			
#define QADC2_ADCB_12_ALC_1					REG(QADC2_BASE_ADDR+4*ADCB_12_ALC_1)			
#define QADC2_ADCB_12_ALC_2					REG(QADC2_BASE_ADDR+4*ADCB_12_ALC_2)			
#define QADC2_ADCB_12_ALC_3					REG(QADC2_BASE_ADDR+4*ADCB_12_ALC_3)			
#define QADC2_ADCB_12_ALC_4					REG(QADC2_BASE_ADDR+4*ADCB_12_ALC_4)			
#define QADC2_TR_DIG0						REG(QADC2_BASE_ADDR+4*TR_DIG0)				
#define QADC2_TR_DIG1						REG(QADC2_BASE_ADDR+4*TR_DIG1)				
#define QADC2_TR_DIG3						REG(QADC2_BASE_ADDR+4*TR_DIG3)				
#define QADC2_TR_DIG4						REG(QADC2_BASE_ADDR+4*TR_DIG4)				
#define QADC2_TR_DIG5						REG(QADC2_BASE_ADDR+4*TR_DIG5)				
#define QADC2_TR_DIG6						REG(QADC2_BASE_ADDR+4*TR_DIG6)				
#define QADC2_TR_ANA0						REG(QADC2_BASE_ADDR+4*TR_ANA0)				
#define QADC2_TR_ANA1						REG(QADC2_BASE_ADDR+4*TR_ANA1)				
#define QADC2_TR_ANA2						REG(QADC2_BASE_ADDR+4*TR_ANA2)				
#define QADC2_TR_ANA6						REG(QADC2_BASE_ADDR+4*TR_ANA6)				
#define QADC2_SR_TR_ANA0					REG(QADC2_BASE_ADDR+4*SR_TR_ANA0)			
#define QADC2_FR_TR_ANA0					REG(QADC2_BASE_ADDR+4*FR_TR_ANA0)			



#define QADC3_SR								REG(QADC3_BASE_ADDR+0x00)

/*
 * 	AON AHB
 * 
 */

#define AON_BASE_ADDR			  	0x1A000000
#define AON_RETAIN_RAM_ADDR			0x1A000100

#define AON_WRITE_TRANS			  	REG(0x1A0000FC)
#define AON_RETTAIM_MEM_TRANS		REG(0x1A0001FC)

#define	AON_TRANS_ID				0x8E89BE00		//0x8E89BED6 //last byte is write addr, optek link sync

#define	AON_CTRL_ADDR				REG(AON_BASE_ADDR+4*0)
#define	AON_TIMER_WT_ADDR			REG(AON_BASE_ADDR+4*1)
#define	AON_ALARM0_ADDR				REG(AON_BASE_ADDR+4*2)
#define	AON_ALARM1_ADDR				REG(AON_BASE_ADDR+4*3)
#define	AON_ALARM2_ADDR				REG(AON_BASE_ADDR+4*4)
#define	AON_TIMER_RD_ADDR			REG(AON_BASE_ADDR+4*5)
#define	AON_TIMER32k_RD_ADDR		REG(AON_BASE_ADDR+4*6)		
#define	AON_CTRL2_ADDR				REG(AON_BASE_ADDR+4*7)
#define	AON_CHARGER_ST_ADDR			REG(AON_BASE_ADDR+4*8)
#define	AON_CTRL3_ADDR				REG(AON_BASE_ADDR+4*9)
#define	AON_VER_ADDR				REG(AON_BASE_ADDR+4*10)


/*-----------------------------------------------------------------------------------------------------*/
/*
 * 	PSRAM
 * 
 */

typedef volatile struct{
	U16	manufacturer		: 4;	//
	U16 columAddr			: 4;	//
	U16 rowAddr				: 5;	//
} PSRAM_ID0s;

typedef volatile struct{
	U16	deviceType			: 4;	//
} PSRAM_ID1s;


#define PSRAM_BURST_LEN_128				0
#define PSRAM_BURST_LEN_64				1
#define PSRAM_BURST_LEN_32				3
#define PSRAM_BURST_LEN_16				2

#define PSRAM_INIT_LATENCY_7			2	
#define PSRAM_INIT_LATENCY_6			1
#define PSRAM_INIT_LATENCY_5			0
#define PSRAM_INIT_LATENCY_4			15
#define PSRAM_INIT_LATENCY_3			14

#define PSRAM_DRIVER_STRENTH_23ohms		0
#define PSRAM_DRIVER_STRENTH_115ohms	1
#define PSRAM_DRIVER_STRENTH_67ohms		2
#define PSRAM_DRIVER_STRENTH_46ohms		3
#define PSRAM_DRIVER_STRENTH_34ohms		4
#define PSRAM_DRIVER_STRENTH_27ohms		5
#define PSRAM_DRIVER_STRENTH_22ohms		6
#define PSRAM_DRIVER_STRENTH_10ohms		7


typedef volatile struct{
	U16	burstLen		: 2;	//
	U16 hybridBurst		: 1;	//
	U16 fixedLatency	: 1;	//
	U16 initLatency		: 4;	//
	U16 rsv 			: 4;	//
	U16 driveStrenth	: 3;	//
	U16 powerdownEn		: 1;	//	
} PSRAM_CONFIG0s;


#define PSRAM_REFRESH_FULL		0
#define PSRAM_REFRESH_B2		1
#define PSRAM_REFRESH_B4		2
#define PSRAM_REFRESH_B8		3
#define PSRAM_REFRESH_T2		5
#define PSRAM_REFRESH_T4		6
#define PSRAM_REFRESH_T8		7

typedef volatile struct{
	U16	refreshInterval		: 2;	//
	U16 partialRefresh		: 3;	//
	U16 hybridSleep			: 1;	//
	U16 masterClockType		: 1;	//
} PSRAM_CONFIG1s;

#define PSRAM_CONFIG_REG_BASE_ADDR	0x19000000

#define PSRAM_ID0			REG16 (PSRAM_CONFIG_REG_BASE_ADDR + 0)
#define PSRAM_ID1			REG16 (PSRAM_CONFIG_REG_BASE_ADDR + 2)

#define PSRAM_CONFIG0		REG16 (PSRAM_CONFIG_REG_BASE_ADDR + 0x1000)			//CA24, Addr 12 (16b)
#define PSRAM_CONFIG1		REG16 (PSRAM_CONFIG_REG_BASE_ADDR + 0x1000 + 2)


/*-----------------------------------------------------------------------------------------------------*/
/*
 * 	RTC 
 * 
 */

#define RTC_BASE_ADDR			0x18900000

//------------------------------------------------------------------------------------
#define REG_RTC_BATT_MODE		REG(RTC_BASE_ADDR)

//------------------------------------------------------------------------------------
#define REG_RTC_CONTROL			REG(RTC_BASE_ADDR + 4)

typedef volatile struct {
	U32 enable			: 1;		//Control RTC enable
	U32 preset_ena		: 1;		//Preset permitted (read only)
	U32 second_int_ena	: 1;		//Second int enable
	U32 alm0_int_ena	: 1;		//Alarm0 int enable
	U32 alm1_int_ena	: 1;		//Alarm1 int enable
	U32 second_intstat	: 1;		//int status (write 1 clear int)
	U32 alm0_intstat	: 1;		//(write 1 clear)
	U32 alm1_intstat	: 1;		//(write 1 clear)
} RTC_CONTROLs;

#define bRTC_CTL_ENA			(*(RTC_CONTROLs *)&REG_RTC_CONTROL).enable
#define bRTC_CTL_PRESETENA		(*(RTC_CONTROLs *)&REG_RTC_CONTROL).preset_ena
#define bRTC_CTL_SECINTENA		(*(RTC_CONTROLs *)&REG_RTC_CONTROL).second_int_ena
#define bRTC_CTL_ALM0INTENA		(*(RTC_CONTROLs *)&REG_RTC_CONTROL).alm0_int_ena
#define bRTC_CTL_ALM1INTENA		(*(RTC_CONTROLs *)&REG_RTC_CONTROL).alm1_int_ena
#define bRTC_CTL_SECINTSTA 		(*(RTC_CONTROLs *)&REG_RTC_CONTROL).second_intstat
#define bRTC_CTL_ALM0INTSTA 	(*(RTC_CONTROLs *)&REG_RTC_CONTROL).alm0_intstat
#define bRTC_CTL_ALM1INTSTA 	(*(RTC_CONTROLs *)&REG_RTC_CONTROL).alm1_intstat

//---------------------------------------------------------------------------------
#define REG_RTC_TIMER				REG(RTC_BASE_ADDR + 8)
#define REG_RTC_TIMER_RD			REG(RTC_BASE_ADDR + 24)
#define REG_RTC_TIMER_WT			REG(RTC_BASE_ADDR + 8)

typedef volatile struct {
	U32 daily_times : 17;			//Every day of timer(bit0~bit16)
	U32 days		: 15;			//all days (bit17~bit31)	
} RTC_TIMERs;

#define bRTC_TIMER_DAILY_TIMES		(*(RTC_TIMERs *)&REG_RTC_TIMER).daily_times
#define bRTC_TIMER_DAYS				(*(RTC_TIMERs *)&REG_RTC_TIMER).days

//---------------------------------------------------------------------------------
#define REG_RTC_ALARM0				REG(RTC_BASE_ADDR + 12)
#define REG_RTC_ALARM1				REG(RTC_BASE_ADDR + 16)

typedef volatile struct {
	U32 daily : 17;				//Set 0 is every day alarm(bit0~bit16)
	U32 days : 15;				//If daily != 0, enable normal alarm(daily+days)
} RTC_ALARMs;

#define bRTC_ALARM0_DAILY			(*(RTC_ALARMs *)&REG_RTC_ALARM0).daily
#define bRTC_ALARM0_DAYS			(*(RTC_ALARMs *)&REG_RTC_ALARM0).days

#define bRTC_ALARM1_DAILY			(*(RTC_ALARMs *)&REG_RTC_ALARM1).daily
#define bRTC_ALARM1_DAYS			(*(RTC_ALARMs *)&REG_RTC_ALARM1).days

//---------------------------------------------------------------------------------
#define REG_RTC_VERSION			REG(RTC_BASE_ADDR + 20)


/*-----------------------------------------------------------------------------------------------------*/
/*
 * 	PWM
 * 
 *------------------------------------------------------------------------------------*/
typedef volatile struct{
	U32	pwm_ena			: 1;	//pwm ch ena
	U32 div				: 6;	//pwm clk clk_peri/N, 0->bypass
	U32 duty_update		: 1;	//duty update
} PWM_CONTROLs;

/*
0->pwm out 1'b0
1-N
1->pwm out {1'b1, (N-1){1'b0}}
....
N->pwm out {N{1'b1}}
*/

#define PWMA_BASE_ADDR			0x18300000
#define REG_PWMA_CTL			REG(PWMA_BASE_ADDR)
#define REG_PWMA_PERIOD			REG(PWMA_BASE_ADDR + 4)			//low 12b
#define REG_PWMA0_DUTY			REG(PWMA_BASE_ADDR + 8)			//low 12b
#define REG_PWMA1_DUTY			REG(PWMA_BASE_ADDR + 12)			//low 12b
#define REG_PWMA2_DUTY			REG(PWMA_BASE_ADDR + 16)			//low 12b
#define REG_PWMA3_DUTY			REG(PWMA_BASE_ADDR + 20)			//low 12b
#define REG_PWMA_VERSION		REG(PWMA_BASE_ADDR + 24)


#define PWMB_BASE_ADDR			0x18400000
#define REG_PWMB_CTL			REG(PWMB_BASE_ADDR)
#define REG_PWMB_PERIOD			REG(PWMB_BASE_ADDR + 4)			//low 12b
#define REG_PWMB0_DUTY			REG(PWMB_BASE_ADDR + 8)			//low 12b
#define REG_PWMB1_DUTY			REG(PWMB_BASE_ADDR + 12)			//low 12b
#define REG_PWMB2_DUTY			REG(PWMB_BASE_ADDR + 16)			//low 12b
#define REG_PWMB3_DUTY			REG(PWMB_BASE_ADDR + 20)			//low 12b
#define REG_PWMB_VERSION		REG(PWMB_BASE_ADDR + 24)


/*-----------------------------------------------------------------------------------------------------*/
/*
 * 	ROTARY 
 * 
 *------------------------------------------------------------------------------------*/
#define ROTARY_BASE_ADDR			0x18A00000
#define REG_ROTARY_CTL				REG(ROTARY_BASE_ADDR + 0)
#define REG_ROTARY_CNT				REG(ROTARY_BASE_ADDR + 4)			/*read cnt, not clear cnt*/
#define REG_ROTARY_VERSION			REG(ROTARY_BASE_ADDR + 8)


#define ROTARY1_BASE_ADDR			0x18B00000
#define REG_ROTARY1_CTL				REG(ROTARY1_BASE_ADDR + 0)
#define REG_ROTARY1_CNT				REG(ROTARY1_BASE_ADDR + 4)			/*read cnt, not clear cnt*/
#define REG_ROTARY1_VERSION			REG(ROTARY1_BASE_ADDR + 8)

typedef volatile struct{
	U32	Ena				: 1;	//enable
	U32 ABswap			: 1;	//
	U32 Ainvert			: 1;	//
	U32 Binvert			: 1;	//
	U32 TimeOV			: 24;	//time to idle, based on 32k
} ROTARY_CONTROLs;

typedef volatile struct{
	U32	ClkWise				: 16;
	U32 antiClkWise			: 16;
} ROTARY_CNTs;


/*------------------------------------------------------------------------------------*/
/*
 * 	WATCH DOG
 * 
 *------------------------------------------------------------------------------------*/
#define WATDOG_BASE_ADDR		0x18D00000

#define REG_WATDOG_CTRL			REG(WATDOG_BASE_ADDR)
#define REG_WATDOG_UNLOCK		REG(WATDOG_BASE_ADDR + 4)
#define REG_WATDOG_RESET		REG(WATDOG_BASE_ADDR + 8)
#define REG_WATDOG_TIME			REG(WATDOG_BASE_ADDR + 12)			/*based on 1ms*/
#define REG_WATDOG_VERSION		REG(WATDOG_BASE_ADDR + 16)

#define UNLOCK_DWORD                   0x6F70746B

#define WDT_RESET_DWORD0               0x55AA9BDF
#define WDT_RESET_DWORD1               0x1357AA55

#define WATCHDOG_ENABLE                (1 << 0)					/*default is enable, should always enable*/
#define WATCHDOG_DISABLE              ~(1 << 0)

//1K prescale bit
#define WATCHDOG_CLK_PRESCALE_ENABLE    (1 << 1)
#define WATCHDOG_CLK_PRESCALE_DISABLE  ~(1 << 1)


/*------------------------------------------------------------------------------------*/
/*
 *  SPI Flash
 *
 *------------------------------------------------------------------------------------*/

#define FLASH_MAX_SIZE   0x200000	
#define FLASH_START_ADDR 0x6A000000
#define FLASH_END_ADDR   (FLASH_START_ADDR + FLASH_MAX_SIZE)

#define FLASH_SECTOR_SIZE 0x1000

#define ADDR_SPI_CMD                0x11000000
#define ADDR_SPI_DATA               0x6A000000

#define SPI_SREG_CONTINOUS_PROGRAM

#define ADDR_SPI_STATUS             REG8(ADDR_SPI_CMD + 0x050000)		//read status
#define ADDR_SPI_1_NO_FAST          REG8(ADDR_SPI_CMD + 0x051000)		//read status, set 1 bit interface, no fast read

#define ADDR_SPI_4_NO_FAST          REG8(ADDR_SPI_CMD + 0x052000)		//read status, set 4 bit interface, no fast read
#define ADDR_SPI_4_FAST             REG8(ADDR_SPI_CMD + 0x053000)		//read status, set 4 bit interface, fast read
#define ADDR_SPI_4_DTR             	REG8(ADDR_SPI_CMD + 0x056000)		//read status, set 4 bit interface, fast read



#define ADDR_SPI_STATUS2            REG8(ADDR_SPI_CMD + 0x350000)		//read status2
#define ADDR_SPI_WR_DISABLE         REG8(ADDR_SPI_CMD + 0x040000)		//write disable
#define ADDR_SPI_WR_ENABLE          REG8(ADDR_SPI_CMD + 0x060000)		//write enable
#define ADDR_SPI_WR_STATUS_2BYTES   REG16(ADDR_SPI_CMD + 0x011000)	//write status, two bytes for Winbond Q series
#define ADDR_SPI_JEDEC_ID           REG32(ADDR_SPI_CMD + 0x9F0000)	//read JEDEC ID
#define ADDR_SPI_MODE_BIT_RESET     REG8(ADDR_SPI_CMD + 0xFF0000)	//Continous Read Mode bit reset
#define ADDR_SPI_POWER_DOWN         REG8(ADDR_SPI_CMD + 0xB90000)		//Deep power down
#define ADDR_SPI_POWER_ON           REG8(ADDR_SPI_CMD + 0xAB0000)		//Relase power down
#define ADDR_SPI_BLOCK_ERASE        REG32(ADDR_SPI_CMD + 0xD80000)	//Block Erase
#define ADDR_SPI_SECTOR_ERASE       REG32(ADDR_SPI_CMD + 0x200000)	//SECTOR Erase
#define ADDR_SPI_PAGE_PROGRAM       REG32(ADDR_SPI_CMD + 0x020000)	//page program
#define ADDR_SPI_FAST_PAGE_PROGRAM  REG32(ADDR_SPI_CMD + 0x320000)	//page program
#define ADDR_SPI_NORMAL_READ        REG8(ADDR_SPI_CMD + 0x030000)		//normal read
#define ADDR_SPI_CHIP_ERASE         REG8(ADDR_SPI_CMD + 0xc70000)		//chip erase
#define ADDR_SPI_PROGRAM_START     	REG32(ADDR_SPI_CMD + 0xFE0000)	//Mode bit reset
#define SFLASH_QUAD_ENABLE          (ADDR_SPI_WR_STATUS_2BYTES=0x0200)

//W25Q16JV, GD 25LF16E
#define ADDR_SPI_4_DTR_NO_FAST     	REG8(ADDR_SPI_CMD + 0x055000)		//read status, set 4 bit double rate interface, no fast read, only gd
#define ADDR_SPI_4_DTR_FAST         REG8(ADDR_SPI_CMD + 0x056000)		//read status, set 4 bit double rate interface, fast read, only gd
#define ADDR_SET_BURST_WRAP			REG8(ADDR_SPI_CMD + 0x776000)		//wrap 64 bytes
#define ADDR_SET_READ_PARMS			REG8(ADDR_SPI_CMD + 0xC06000)		//wrap 64 bytes, dummy 4
#define ADDR_SPI_ENABLE_PD			REG8(ADDR_SPI_CMD + 0xB90000)		//enablle deep power down
#define ADDR_SPI_RELEASE_PD			REG8(ADDR_SPI_CMD + 0xAB0000)		//release deep power down
#define ADDR_SPI_SUSPEND			REG8(ADDR_SPI_CMD + 0x750000)	
#define ADDR_SPI_RESUME				REG8(ADDR_SPI_CMD + 0x7A0000)


#define ADDR_SPI_SREG_1				0x001000	
#define ADDR_SPI_SREG_2				0x002000	
#define ADDR_SPI_SREG_3				0x003000	

#define ADDR_SPI_SREG_1_ERASE     	(ADDR_SPI_CMD + 0x440000 + ADDR_SPI_SREG_1)			//security register erase, last 16b is sreg addr
#define ADDR_SPI_SREG_1_PROGRAM     (ADDR_SPI_CMD + 0x420000 + ADDR_SPI_SREG_1)			//security register program
#define ADDR_SPI_SREG_1_READ     	(ADDR_SPI_CMD + 0x480000 + ADDR_SPI_SREG_1)			//security register read,  last 16b is sreg addr

#define ADDR_SPI_SREG_2_ERASE     	(ADDR_SPI_CMD + 0x440000 + ADDR_SPI_SREG_2)			//security register erase, last 16b is sreg addr
#define ADDR_SPI_SREG_2_PROGRAM     (ADDR_SPI_CMD + 0x420000 + ADDR_SPI_SREG_2)			//security register program, last 16b is sreg addr
#define ADDR_SPI_SREG_2_READ     	(ADDR_SPI_CMD + 0x480000 + ADDR_SPI_SREG_2)			//security register read,  last 16b is sreg addr

#define ADDR_SPI_SREG_3_ERASE     	(ADDR_SPI_CMD + 0x440000 + ADDR_SPI_SREG_3)			//security register erase, last 16b is sreg addr
#define ADDR_SPI_SREG_3_PROGRAM     (ADDR_SPI_CMD + 0x420000 + ADDR_SPI_SREG_3)			//security register program, last 16b is sreg addr, please note continous sreg program setting
#define ADDR_SPI_SREG_3_READ     	(ADDR_SPI_CMD + 0x480000 + ADDR_SPI_SREG_3)			//security register read,  last 16b is sreg addr


#define ADDR_SPI_SREG_1_WORD_PROGRAM     (ADDR_SPI_CMD + 0x460000 + ADDR_SPI_SREG_1)			//security register word (32b) program, last 16b is sreg addr,
#define ADDR_SPI_SREG_2_WORD_PROGRAM     (ADDR_SPI_CMD + 0x460000 + ADDR_SPI_SREG_2)			//security register word (32b) program, last 16b is sreg addr,
#define ADDR_SPI_SREG_3_WORD_PROGRAM     (ADDR_SPI_CMD + 0x460000 + ADDR_SPI_SREG_3)			//security register word (32b) program, last 16b is sreg addr,


typedef volatile struct{
	U8	busy			: 1;
	U8  wel 			: 1;
	U8  bp0				: 1;
	U8  bp1				: 1;
	U8  bp2				: 1;
	U8  tb 				: 1;
	U8  sec 			: 1;
	U8  srp0			: 1;
} SPI_FLASH_STATUS1s;

typedef volatile struct{
	U8   srp1			: 1;
	U8   qe				: 1;		//for GD, is is always 1
	U8	 res			: 1;
	U8	 lb1			: 1;
	U8	 lb2			: 1;
	U8	 lb3			: 1;
	U8	 cmp			: 1;
	U8	 sus			: 1;
} SPI_FLASH_STATUS2s;

/*-----------------------------------------------------------------------------------------------------*/
/*
 * 	Timer
 * 
 *------------------------------------------------------------------------------------*/

#define	BASE_ADDR_TIMER		0x18200000

#define ADDR_TIMER_CTRL			REG16(BASE_ADDR_TIMER + 0)
typedef volatile struct{
	U16   timer_1_rx_en			: 1;
	U16   timer_2_rx_en			: 1;
	U16   timer_3_rx_en			: 1;
	U16   timer_4_rx_en			: 1;
	U16   timer_5_rx_en			: 1;
	U16   timer_6_rx_en			: 1;
	U16   timer_7_rx_en			: 1;
	U16   timer_8_rx_en			: 1;
	U16	  timer_1_en			: 1;
	U16	  timer_2_en			: 1;
	U16	  timer_3_en			: 1;
	U16	  timer_4_en			: 1;
	U16	  timer_5_en			: 1;
	U16	  timer_6_en			: 1;
	U16	  timer_7_en			: 1;
	U16	  timer_8_en			: 1;
} TIMER_CTRLs;

/***base clk is peri clk ****/
#define ADDR_TIMER_PRESCALE		REG16(BASE_ADDR_TIMER + 4)//div = prescale+1			//LSB 4b

#define ADDR_TIMER_IDLE_LEV		REG16(BASE_ADDR_TIMER + 4*2)
typedef volatile struct{
	U16   timer_1_idle_lev			: 1;
	U16   timer_2_idle_lev			: 1;
	U16   timer_3_idle_lev			: 1;
	U16   timer_4_idle_lev			: 1;
	U16   timer_5_idle_lev			: 1;
	U16   timer_6_idle_lev			: 1;
	U16   timer_7_idle_lev			: 1;
	U16   timer_8_idle_lev			: 1;
	U16	  timer_1_int_mask			: 1;
	U16	  timer_2_int_mask			: 1;
	U16	  timer_3_int_mask			: 1;
	U16	  timer_4_int_mask			: 1;
	U16	  timer_5_int_mask			: 1;
	U16	  timer_6_int_mask			: 1;
	U16	  timer_7_int_mask			: 1;
	U16	  timer_8_int_mask			: 1;
} TIMER_IDEL_LEV_INT_MASKs;

#define ADDR_TIMER_INT_CLR		REG16(BASE_ADDR_TIMER + 4*3)
#define ADDR_TIMER_STATUS		REG16(BASE_ADDR_TIMER + 4*3)

typedef volatile struct{
	U16   timer_1_int_clr			: 1;
	U16   timer_2_int_clr			: 1;
	U16   timer_3_int_clr			: 1;
	U16   timer_4_int_clr			: 1;
	U16   timer_5_int_clr			: 1;
	U16   timer_6_int_clr			: 1;
	U16   timer_7_int_clr			: 1;
	U16   timer_8_int_clr			: 1;
} TIMER_INT_CLRs;

typedef volatile struct{
	U16   timer_1_status			: 1;
	U16   timer_2_status			: 1;
	U16   timer_3_status			: 1;
	U16   timer_4_status			: 1;
	U16   timer_5_status			: 1;
	U16   timer_6_status			: 1;
	U16   timer_7_status			: 1;
	U16   timer_8_status			: 1;
} TIMER_STATUS;


#define ADDR_TIMER_1_VAL		REG16(BASE_ADDR_TIMER + 4*4)
#define ADDR_TIMER_2_VAL		REG16(BASE_ADDR_TIMER + 4*5)
#define ADDR_TIMER_3_VAL		REG16(BASE_ADDR_TIMER + 4*6)
#define ADDR_TIMER_4_VAL		REG16(BASE_ADDR_TIMER + 4*7)
#define ADDR_TIMER_5_VAL		REG16(BASE_ADDR_TIMER + 4*8)
#define ADDR_TIMER_6_VAL		REG16(BASE_ADDR_TIMER + 4*9)
#define ADDR_TIMER_7_VAL		REG16(BASE_ADDR_TIMER + 4*10)
#define ADDR_TIMER_8_VAL		REG16(BASE_ADDR_TIMER + 4*11)

typedef volatile struct{
	U16   width			: 15;
	U16   polar			: 1;			//High is 1, Low is 0
} TIMER_WIDTHs;

/************************************************************************************************************************/

//OTG
#define	USB_BASE_ADDR			0x16000000


#define	USB_OTG_REG_OFFSET		0x1bc
#define	USB_HOST_REG_OFFSET		0
#define	USB_DEVICE_REG_OFFSET	0

#define	USB_OTG_BASE_ADDR		(USB_BASE_ADDR + USB_OTG_REG_OFFSET)
#define	USB_HOST_BASE_ADDR		(USB_BASE_ADDR + USB_HOST_REG_OFFSET)
#define	USB_DEVICE_BASE_ADDR	(USB_BASE_ADDR + USB_DEVICE_REG_OFFSET)


#define	OTG_CTL_REG_ADDR		(USB_BASE_ADDR + 0x400)

#define	OTG_DMA_BASE_ADDR		(USB_BASE_ADDR + 0x800)
#define	OTG_DMA_ENDP1_ADDR		OTG_DMA_BASE_ADDR
#define	OTG_DMA_ENDP2_ADDR		(OTG_DMA_BASE_ADDR + 0x100)
#define	OTG_DMA_ENDP3_ADDR		(OTG_DMA_BASE_ADDR + 0x200)


//enhanced ctrl reg
#define	ENH_CTRL_RESET			(1<<0)
#define	ENH_CTRL_PHY_CLK_ENA	(1<<1)
#define	ENH_CTRL_PHY_REG_ENA	(1<<2)
#define	ENH_CTRL_PHY_PLL_ENA	(1<<3)
#define	ENH_CTRL_DIS_HSPEED		(1<<4)

#define	ENH_CTRL_INT_ENA_MASK	(7<<5)
#define	ENH_CTRL_INT_STAT_MASK	(7<<8)

#define	ENH_CTRL_INT_ENA_SHIFT	(5)
#define	ENH_CTRL_INT_STAT_SHIFT	(8)

#define	ENH_CTRL_INT_USB		(1<<0)
#define	ENH_CTRL_INT_FIFO		(1<<1)
#define	ENH_CTRL_INT_WKUP		(1<<2)
#define	ENH_CTRL_PHY_NO_SUSPEND	(1<<26)


//SD Card
#define	SD_BASE_ADDR              0x15000000

#define	SD_TX_DATA_PORT_OFFSET    0x100
#define	SD_RX_DATA_PORT_OFFSET    0x200

#define	SD_TX_DATA_PORT_ADDR      (SD_BASE_ADDR + SD_TX_DATA_PORT_OFFSET)
#define	SD_RX_DATA_PORT_ADDR      (SD_BASE_ADDR + SD_RX_DATA_PORT_OFFSET)

#define	SD_TX_DATA_PORT           REG( SD_TX_DATA_PORT_ADDR )
#define	SD_RX_DATA_PORT           REG( SD_RX_DATA_PORT_ADDR )

/// Macro to write a EM field (32-bit wide)
#define SD_WR_RG(addr, value)        *((volatile U16 *)(addr | SD_BASE_ADDR)) = (value)
#define SD_WR_RG_32(addr, value)     *((volatile U32 *)(addr | SD_BASE_ADDR)) = (value)

#define SD_RD_RG(addr)               *((volatile U16 *)(addr | SD_BASE_ADDR))
#define SD_RD_RG_32(addr)            *((volatile U32 *)(addr | SD_BASE_ADDR))

/*-----------------------------------------------------------------------------------------------------*/
/*
 * 	IR
 * 
 *------------------------------------------------------------------------------------*/

typedef volatile struct{
	U32	ir_ena			: 1;	//ir ena
	U32 eco				: 1;	//eco
	U32 min_width		: 8;	//min lev width
	U32 spdif_det_en	: 1;
	U32 int_mask		: 1;
} IR_CONTROLs;

typedef volatile struct{
	U32	int_status		: 1;	//int
	U32 ov				: 1;	//err, overfolw
	U32 eco_exit		: 1;	//
	U32 spdif_aint		: 1;	//in eco, spdif change
} IR_STATUS;

#define IR_INT_MASKs IR_STATUS

#define IR_BASE_ADDR			0x18C00000
#define REG_IR_CTL              REG(IR_BASE_ADDR)     /*bit5-bit2 min, bit 1 eco, bit 0 en*/
#define REG_IR_INTSTAT          REG(IR_BASE_ADDR + 4) /*bit 1 eco release, bit 0 rx not Empty*/
#define REG_IR_RXFIFO           REG(IR_BASE_ADDR + 8) /*4 byte contect, bit 7 ir H/L, bit [6:0] lev width based on 32k (0.0305ms)*/


/*-----------------------------------------------------------------------------------------------------*/

#endif //_REGMAP_H
