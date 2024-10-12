#ifndef	__OTG_H__
#define	__OTG_H__


#include "interrupt.h"



#define OTG_FORCE_FULL_SPEED


//#define USB_STA


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



typedef enum {
	enUSB_HOST_TYPE,
	enUSB_DEVICE_TYPE,
} enUSB_TYPE;


typedef	struct {
	volatile U32 * ctrl;
	U32	dmaEndp1Addr;
	U32	dmaEndp2Addr;
	U32	dmaEndp3Addr;
}	OTG_ENHANCE_CTRL;


typedef struct {
	U8	irq;                   //0x1bc [des] 2.16.1 otgirq
	U8	state;                 //0x1bd [des] 2.16.2 otgstate
	U8	ctrl;                  //0x1be [des] 2.16.3 otgctrl
	U8	status;                //0x1bf [des] 2.16.4 otgstatus
	U8	ien;                   //0x1c0 [des] 2.16.5 otgien
	U8	taaidlbdis;            //0x1c1 [des] 2.16.6 taaidlbdis Timer
	U8	tawaitbcon;            //0x1c2 [des] 2.16.7 tawaitbcon Timer
	U8	tbvbuspls;             //0x1c3 [des] 2.16.8 tbvbuspls timeout time
	U8	reserved[3];
	U8	tbvbusdispls;          //0x1c7 [des] 2.16.9 tbvbusdispls timeout time
} USB_OTG_REGISTERS;

#define	USB_ORG_REGFILE_SIZE	(12)


//more information in usbhs_otg_sd_des.pdf

typedef	struct {
	volatile U8 * rxbc;		    //dev out, host in  //0x00 [des] 2.18.1 out0bc/hcin0bc
	volatile U8 * txbc;         //0x01  [des] 2.18.2 in0bc/hcout0bc
	volatile U8 * cs;           //0x02  [des] 2.18.3 ep0cs/hcep0cs

	volatile U8 * hcctrl;       //0xc0  [des] 2.17.1 hcep0ctrl
	volatile U8 * hcouterr;     //0xc1  [des] 2.17.3 hcoutxerr
	volatile U8 * hcinerr;	    //0xc3  [des] 2.17.5 hcinxerr
	volatile U8 * hcmaxpkt;	    //0x1e0 [des] 2.17.19 hcin0maxpck

	volatile U8 * setupdat;		//8 bytes   //0x180 [des] 2.18.15 setupdat, not in Host mode
	volatile U8 * rxdata;		//64 bytes  //0x140-0x17F [des] 2.18.14 ep0outdata, hcep0indat
	volatile U8 * txdata;		//64 bytes  //0x100-0x13F [des] 2.18.13 ep0indata, hcep0outdat
} ENDPOINT0_REGS;				//in & out


typedef	struct {
	volatile U16 * bc;          //0x08, 0x10, 0x18 [des] 2.18.4 outxbcl, 0x0c, 0x14, 0x1c 2.18.8 inxbcl/hcoutxbcl
	volatile U8 * cs;           //0x0b, 0x13, 0x1B [des] 2.18.6 outxcs,  0x0f, 0x17, 0x1f 2.18.10 inxcs/hcoutxcs
	volatile U8 * con;          //0x0a, 0x12, 0x1A [des] 2.18.7 outxcon, 0x12, 0x16, 0x1e 2.18.11 inxcon
	
	volatile U8 * hcctrl;       //0xc6, 0xca, 0xce [des] 2.17.4 NA/hcinxctrl,  0xc4, 0xc8, 0xcc [des] 2.17.4 hcoutxctrl
	volatile U8 * hcerr;        //0xc7, 0xcb, 0xcf [des] 2.17.5 NA/hcinxerr,   0xc5, 0xc9, 0xcd [des] 2.17.3 hcoutxerr

	volatile U16 * maxpkt;      //0x1e2, 0x1e4, 0x1e6 [des] 2.17.20 NA/hcinxmaxpckl, 0x3e2, 0x3e4, 0x3e6 2.17.22 HC OUTx Max
	volatile U16 * staddr;      //0x304, 0x308, 0x30C [des] 2.18.47 outxstaddr, ep buffers. 0x344, 0x348, 0x34c 2.18.48 inxstaddr

	volatile U8 * fifoByte;     //0x84, 0x88, 0x8C [des] 2.18.12 fifoxdat, 0x84,0x88,0x8c, ...write/read data ~ IN/OUT
	volatile U16 * fifoWord;    //0x84...
	volatile U32 * fifoDword;   //0x84...
} ENDPOINT_REGS;                //in or out


typedef struct {
	volatile U8	* ivect;         //0x1a0 [des] 2.18.38 hcivect
	volatile U8	* fifoivect;     //0x1a1 [des] 2.18.39 hcfifoivect

	volatile U16 * txirq;        //0x188 [des] 2.18.18 in07irq/hcout07irq 
	volatile U16 * txien;        //0x194 [des] 2.18.29 IN 0 to 7 Ep Int Enable
	volatile U16 * rxirq;        //0x18a [des] 2.18.16 out07irq/hcin07irq
	volatile U16 * rxien;        //0x196 [des] 2.18.27 OUT 0 to 7 Ep Int Enable
	
	volatile U8	* usbirq;        //0x18c [des] 2.18.20 usbirq
	volatile U8	* usbien;        //0x198 [des] 2.18.31 usbien

	volatile U16 * outpngirq;    //dev only  //0x18e [des] 2.18.21 out07pngirq
	volatile U16 * outpngien;    //0x19a [des] 2.18.32 out07pngien

	volatile U16 * txfullirq;    //0x190 [des] 2.18.25 hcout07fullirq
	volatile U16 * txfullien;    //0x19c [des] 2.18.36 hcout07fullien
	volatile U16 * rxemptyirq;   //0x192 [des] 2.18.23 hcin07emptirq
	volatile U16 * rxemptyien;   //0x19e [des] 2.18.36 hcin07rmptien


	volatile U8	* hcinerrIrq;    //0x1b4 [des] 2.17.11 hcin07errirq
	volatile U8	* hcinerrIen;    //0x1b8 [des] 2.17.15 hcin07errien
	volatile U8	* hcouterrIrq;   //0x1b6 [des] 2.17.13 hcout07errirq
	volatile U8	* hcouterrIen;   //0x1ba [des] 2.17.11 hcout07errien

} USB_IRQ_REGS;

typedef	struct {
	volatile U8 * enprst;        //[des] 2.18.40 endprst
	volatile U8 * usbcs;         //[des] 2.18.41 usbcs: 1 = discon
	volatile U8 * funcaddr;      //[des] 2.18.44 fnaddr
	volatile U8 * clkgate;       //[des] 2.18.45 clkgate

	volatile U8 * fifoctl;       //[des] 2.18.46 fifoctrl
} COMMON_CTRL_REG;

typedef	struct {

	ENDPOINT0_REGS	*endp0;      //control
	
	ENDPOINT_REGS	*endp1In;   
	ENDPOINT_REGS	*endp1Out;   //data OUT

	ENDPOINT_REGS	*endp2In;    //data IN
	ENDPOINT_REGS	*endp2Out;

	ENDPOINT_REGS	*endp3In;    //data IN for HID
	ENDPOINT_REGS	*endp3Out;

	USB_IRQ_REGS	*irq;
	
	COMMON_CTRL_REG	*ctrl;
	
	volatile U16 * framecnt;

} USB_DEVICE_REG; //usbdevReg = &usbDevRegs, *usbdevReg

typedef	struct {

	ENDPOINT0_REGS	*endp0;
	
	ENDPOINT_REGS	*endp1In;
	ENDPOINT_REGS	*endp1Out;

	ENDPOINT_REGS	*endp2In;
	ENDPOINT_REGS	*endp2Out;

	ENDPOINT_REGS	*endp3In;	//HID
	ENDPOINT_REGS	*endp3Out;

	USB_IRQ_REGS	*irq; //see const USB_IRQ_REGS	irqAddr in otg.c

	COMMON_CTRL_REG	*ctrl;

	volatile U8 * portctrl;
	volatile U16 * framenum;
	volatile U16 * frameRemain;
} USB_HOST_REG; //usbhostReg=&usbHostRegs, *usbhostReg


extern volatile USB_OTG_REGISTERS *otgRegister;
extern OTG_ENHANCE_CTRL *otgEnhCtrl;

extern USB_HOST_REG	*usbhostReg;
extern USB_DEVICE_REG	*usbdevReg;

// all single buffer
#define	ENDPOINT0_BUF_SIZE	64 //bytes
#define	ENDPOINT1_BUF_SIZE	1024
#define	ENDPOINT2_BUF_SIZE	128



// host transmit error type
#define	HC_ERR_SHIFT	2
#define	HC_ERR_MASK		(7 << HC_ERR_SHIFT)

#define	HC_ERR_NO_ERR	(0 << 2)
#define	HC_ERR_CRC		(1 << 2)
#define	HC_ERR_TOGGLE	(2 << 2)
#define	HC_ERR_STALL	(3 << 2)
#define	HC_ERR_TIMEOUT	(4 << 2)
#define	HC_ERR_PID		(5 << 2)
#define	HC_ERR_OVERRUN	(6 << 2)
#define	HC_ERR_UNDERRUN	(7 << 2)


//**********************************************************************************************


#define	USB_INT            LEVEL2_INT8_MASK

#define	USB_INT_ENABLE     xt_ints_on( USB_INT )
#define	USB_INT_DISABLE    xt_ints_off( USB_INT )

#define	BYTE_SPLIT1(x)	((U8)(x & 0xFF))
#define	BYTE_SPLIT2(x)	((U8)(x >> 8))
#define	BYTE_SPLIT3(x)	((U8)(x >> 16))
#define	BYTE_SPLIT4(x)	((U8)(x >> 24))


extern U8 usb_highspeed_flag;


//in host mode
int otg_isDevPlugin( void );
//in dve mode
int otg_isHostPlugin( void );


int otg_init( enUSB_TYPE usb_type );
int otg_open( enUSB_TYPE usb_type );
int otg_close( enUSB_TYPE usb_type );

void otg_phy_standby (void);

void otg_intProcess_for_host(void);
void otg_intProcess_for_dev(void);
void hcd_intProcess(void);
void dcd_intProcess(void);

void usbotg_isr( void );

#endif //__OTG_H__
