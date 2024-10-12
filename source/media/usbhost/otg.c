#include "os_config.h"


#if	( (defined USB_HOST_ENABLE) || (defined USB_DEV_ENABLE) )

#include "mem_reloc.h"

#include "hw_timer.h"

#include "IRP.h"
#include "interrupt.h"

#include "usbd.h"
#include "hcd.h"
#include "otg.h"
#include "usbdev.h"

#ifdef AUDIO_CLOCK_SYNC_ENABLE
#include "audio_clock_sync.h"
#endif

U8 usb_highspeed_flag;



volatile USB_OTG_REGISTERS * otgRegister;
OTG_ENHANCE_CTRL otgEnhanceCtrl, *otgEnhCtrl; 

USB_HOST_REG	usbHostRegs,	*usbhostReg;
USB_DEVICE_REG	usbDevRegs,		*usbdevReg;

typedef	struct {
	U8	oldstate;
	U8	curstate;
	
	IRP	irp;
} OTG;

OTG	otg;


#define DBG_iPrintf


void otg_intProcess(void)
{
	IRP	*irp = (IRP *)&otg.irp;
	volatile U8 reg8;
	
	reg8 = (otgRegister->irq & otgRegister->ien);
	if( reg8 )
	{
		otgRegister->irq	=	reg8;

		if( reg8 & 0x01 ) //2.16.1: a_idle or b_idle state.
		{
			if( otgRegister->state == 0x08 ) //2.16.2: otgstate 0x08 b_idle.
			{
				DBG_iPrintf("\n\rPC_discon\n\r");
						
				#ifdef USB_DEV_ENABLE
				//dev
				otgRegister->ctrl &= ~0x01;			//2.16.3 set otgctrl bit0 to 0, end the session.
				otgRegister->ctrl |=  0x02;			//JJ+ set bit1 to forces the bus power-down.
				*usbdevReg->ctrl->usbcs |= 0x40;	//2.18.41 usbcs: discon bit, 1 is discon. JJ+

				if( (otg.curstate == 0x08) && (otg.oldstate == 0x09 ) && dcd_isOpen() )
				{
					dcd_hostUnplug();

					FILL_OTG_IRP( irp, enIRPT_DEV_STACK, IRP_PARAM_HOST_DISCONNECT );
					usbmsg_isendIRP( irp );
				}
				#endif
			}
			else //2.16.1: a_idle.
			{
				//host
				otgRegister->ctrl &= ~0x02;
				otgRegister->ctrl |= 0x01;
			}
		}
				
		if( reg8 & 0x08 ) //2.16.1: vbuserrirq, free the bus, then power-down bus.
		{
			otgRegister->ctrl &= ~0x01;
			otgRegister->ctrl |= 0x02;
			//send error msg out ?
		}
		
		if( reg8 & 0x04 ) //2.16.1: a_host or b_host state entering.
		{
			//host --> take this as dev connect			
			DBG_iPrintf("\n\rPC conn1, 0x%x\n\r", otg.curstate);

			#if (defined USB_HOST_ENABLE || defined USB_DEV_ENABLE)
			if( otg.curstate == 0x03 )
			{
				FILL_OTG_IRP( irp, enIRPT_USBD, IRP_PARAM_DEV_CONNECT );
			}
			else
			{
				FILL_OTG_IRP( irp, enIRPT_USBD, IRP_PARAM_DEV_DISCONNECT );
			}
			
			usbmsg_isendIRP( irp );
			#endif
		}
		
		if( reg8 & 0x10 ) //2.16.1: a_peri or b_peri state entering.
		{
			//dev --> take this as host connnect
			DBG_iPrintf("\n\rPC conn2\n\r");

			#ifdef USB_DEV_ENABLE
			if( dcd_isOpen() )
			{
				FILL_OTG_IRP( irp, enIRPT_DEV_STACK, IRP_PARAM_HOST_CONNECT );
				usbmsg_isendIRP( irp );
			}
			#endif
		}
	}
}

void usbotg_isr( void ) __USBHOST_TEXT;
void usbotg_isr( void )
{
	U32 reg32;
	U8 reg8;

	reg32 = *otgEnhCtrl->ctrl;
	*otgEnhCtrl->ctrl &= ~ENH_CTRL_INT_STAT_MASK;
	reg32 &= ENH_CTRL_INT_STAT_MASK;
	reg32 >>= ENH_CTRL_INT_STAT_SHIFT;

	if( reg32 & ENH_CTRL_INT_USB )
	{
	#ifdef USB_INT_CHANGEs_TO_LEVEL1

		#ifdef CLOCK_SYNC_WITH_MASTER_ENABLE
		volatile U8	reg8;
		reg8 = *usbdevReg->irq->usbirq; //[des] 2.18.20 usbirq
		if( reg8 & 0x02 )
		{
			//[des] 2.18.20 usbirq sofir: receive a SOF pkt
			devcur_ccount = read_ccount();
			hcountms++;

			if (devfirst_ccount == 0)
			{
				devfirst_ccount = devcur_ccount;
				hcountms = 0;
			}
		}
		#endif //CLOCK_SYNC_WITH_MASTER_ENABLE

		#ifdef AUDIO_CLOCK_SYNC_ENABLE
		reg8	=	*usbdevReg->irq->usbirq;    //[des] 2.18.20 usbirq
		//reg8	&=	*usbdevReg->irq->usbien;    //[des] 2.18.31 usbien
		if( reg8 & 0x02 )
		{
			usbdev_clock_sync_irq();
		}
		#endif

		SOFT5_INT_SET;
	#else
		#ifdef AUDIO_CLOCK_SYNC_ENABLE
		reg8	=	*usbdevReg->irq->usbirq;    //[des] 2.18.20 usbirq
		//reg8	&=	*usbdevReg->irq->usbien;    //[des] 2.18.31 usbien
		if( reg8 & 0x02 )
		{
			usbdev_clock_sync_irq();
		}
		#endif
	#endif
	}

	otg.curstate = otgRegister->state;

	//otg
	otg_intProcess();

	if( otg.curstate == 0x03 ) //host
	{
		#ifdef USB_HOST_ENABLE
		hcd_intProcess();
		#endif
	}

	#ifdef USB_DEV_ENABLE
	if( otg.curstate == 0x09 ) //dev
	{
		dcd_intProcess();
	}
	#else
	DBG_assert( otg.curstate != 0x09 ); //dev
	#endif

	if( otg.curstate != otg.oldstate )
	{
		otg.oldstate = otg.curstate;
	}
}

int dcd_hostUnplug( void );
int dcd_isOpen( void );

void otg_intProcess(void) __USBHOST_TEXT;


#if (defined USB_DEV_ENABLE || defined USB_SPEAKER)

int otg_isDevPlugin( void )
{
	//in host mode
	return ( (otgRegister->status & 0x02) && (otgRegister->state == 0x03) );
}

int otg_isHostPlugin( void )
{
	//in dev mode
	//FIXME
	return ( (otgRegister->status & 0x04) && (otgRegister->state == 0x09) );
}

#endif


#define REG_ADDR(x)    (void *)(USB_DEVICE_BASE_ADDR + x)


const ENDPOINT0_REGS endp0Addr = {
	REG_ADDR(0x00),	REG_ADDR(0x01),	REG_ADDR(0x02),	
	REG_ADDR(0xc0),	REG_ADDR(0xc1),	REG_ADDR(0xc3),	REG_ADDR(0x1e0),	
	REG_ADDR(0x180),REG_ADDR(0x140), REG_ADDR(0x100)
};

//in the host mode: rx, in the device mode: tx 
const ENDPOINT_REGS	endp1RxAddr	=	{
	REG_ADDR(0x08),	REG_ADDR(0x0b),	REG_ADDR(0x0a),
	REG_ADDR(0xc6),	REG_ADDR(0xc7),	
	REG_ADDR(0x1e2),REG_ADDR(0x304),
	REG_ADDR(0x84),	REG_ADDR(0x84),	REG_ADDR(0x84)
};

//in the host mode: rx, in the device mode: tx 
const ENDPOINT_REGS	endp2RxAddr	=	{
	REG_ADDR(0x10),	REG_ADDR(0x13),	REG_ADDR(0x12),
	REG_ADDR(0xca),	REG_ADDR(0xcb),	
	REG_ADDR(0x1e4),REG_ADDR(0x308),
	REG_ADDR(0x88),	REG_ADDR(0x88),	REG_ADDR(0x88)
};


//in the host mode: rx, in the device mode: tx 
const ENDPOINT_REGS	endp3RxAddr	=	{   //ep3 Rx=Host OUT byte counter, cs, con,...
	REG_ADDR(0x18),	REG_ADDR(0x1b),	REG_ADDR(0x1a),
	REG_ADDR(0xce),	REG_ADDR(0xcf),	
	REG_ADDR(0x1e6), REG_ADDR(0x30c),//2.17.20 HC INx Max, 2.18.47 outxstaddr
	REG_ADDR(0x8c),	REG_ADDR(0x8c),	REG_ADDR(0x8c)
};


const ENDPOINT_REGS	endp1TxAddr	=	{
	REG_ADDR(0x0c),	REG_ADDR(0x0f),	REG_ADDR(0x0e),
	REG_ADDR(0xc4),	REG_ADDR(0xc5),	
	REG_ADDR(0x3e2),REG_ADDR(0x344),
	REG_ADDR(0x84),	REG_ADDR(0x84),	REG_ADDR(0x84)
};

const ENDPOINT_REGS	endp2TxAddr	=	{
	REG_ADDR(0x14),	REG_ADDR(0x17),	REG_ADDR(0x16),
	REG_ADDR(0xc8),	REG_ADDR(0xc9),	
	REG_ADDR(0x3e4),REG_ADDR(0x348),
	REG_ADDR(0x88),	REG_ADDR(0x88),	REG_ADDR(0x88)
};

const ENDPOINT_REGS	endp3TxAddr	=	{   //ep3 Tx=Host IN byte counter, cs, con,... !!!
	REG_ADDR(0x1c),	REG_ADDR(0x1f),	REG_ADDR(0x1e),
	REG_ADDR(0xcc),	REG_ADDR(0xcd),	
	REG_ADDR(0x3e6), REG_ADDR(0x34c),//2.17.22 HC OUTx Max, 2.18.48 inxstaddr
	REG_ADDR(0x8c),	REG_ADDR(0x8c),	REG_ADDR(0x8c)
};


const COMMON_CTRL_REG	commCtlAddr	=	{
	REG_ADDR(0x1a2),	REG_ADDR(0x1a3),	REG_ADDR(0x1a6),	REG_ADDR(0x1a7),
	REG_ADDR(0x1a8)
};


const USB_IRQ_REGS	irqAddr	=	{
	REG_ADDR(0x1a0),	REG_ADDR(0x1a1),
	REG_ADDR(0x188),	REG_ADDR(0x194),	REG_ADDR(0x18a),	REG_ADDR(0x196),
	REG_ADDR(0x18c),	REG_ADDR(0x198),
	REG_ADDR(0x18e),	REG_ADDR(0x19a),
	REG_ADDR(0x190),	REG_ADDR(0x19c),	REG_ADDR(0x192),	REG_ADDR(0x19e),
	REG_ADDR(0x1b4),	REG_ADDR(0x1b8),	REG_ADDR(0x1b6),	REG_ADDR(0x1ba)
};


void otg_phy_set(enUSB_TYPE usb_type)
{
	U32 val;

	DBG_Printf("\n\rotg_phy_set\n\r");

	MISC_CLKGATE_CONTROLs *pGate = (MISC_CLKGATE_CONTROLs *) &val;
	MISC_USBPHY_CONTROLs *pUsbCntl = (MISC_USBPHY_CONTROLs *) &val;

	val = REG_MISC_CLKGATE;
	pGate->usbOtgSd_gate = 1;		
	REG_MISC_CLKGATE = val;	

	//set usb otg host or slave
	val = REG_MISC_USBPHY; //jj+

	if (usb_type == enUSB_HOST_TYPE)
	{
		pUsbCntl->utmi_iddig_sw = 0; //0->USB Host OTG ID
	}
	else
	{
		pUsbCntl->utmi_iddig_sw = 1; //1->USB Slave OTG ID
	}

	REG_MISC_USBPHY = val;
	REG_MISC_STATUS = 0x80000010;

	pUsbCntl->utmi_sw_sessend = 0;				//vbus > 0.8v
	REG_MISC_USBPHY = val;
	REG_MISC_STATUS = 0x80000011;

	pUsbCntl->utmi_sw_avalid = 1;				//vbus > 2.0v
	REG_MISC_USBPHY = val;
	REG_MISC_STATUS = 0x80000012;

	pUsbCntl->utmi_sw_bvalid = 1;				//vbus > 4.0v
	REG_MISC_USBPHY = val;
	REG_MISC_STATUS = 0x80000013;

	pUsbCntl->utmi_sw_vbusValid = 1;			//vbus > 4.75v
	REG_MISC_USBPHY = val;

	REG_MISC_STATUS = 0x80000014;
}

int otg_init( enUSB_TYPE usb_type )
{
	if (usb_type == enUSB_HOST_TYPE)
	{
		DBG_Printf("%s\n\r", __func__);

		otg_phy_set(enUSB_HOST_TYPE);

		otgRegister	=	(USB_OTG_REGISTERS *)USB_OTG_BASE_ADDR;
		DBG_assert( USB_ORG_REGFILE_SIZE == sizeof(USB_OTG_REGISTERS) );
		
		otgEnhCtrl	=	&otgEnhanceCtrl;

		otgEnhCtrl->ctrl	=	(U32*)OTG_CTL_REG_ADDR;
		otgEnhCtrl->dmaEndp1Addr	=	OTG_DMA_ENDP1_ADDR;
		otgEnhCtrl->dmaEndp2Addr	=	OTG_DMA_ENDP2_ADDR;
		otgEnhCtrl->dmaEndp3Addr	=	OTG_DMA_ENDP3_ADDR;
		REG_MISC_STATUS = 0x80000211;
		
		usbhostReg	=	&usbHostRegs;
		usbdevReg	=	&usbDevRegs;
		REG_MISC_STATUS = 0x80000212;

		usbhostReg->endp0		=	(ENDPOINT0_REGS*)&endp0Addr;
		usbhostReg->endp1In		=	(ENDPOINT_REGS*)&endp1RxAddr;
		usbhostReg->endp1Out	=	(ENDPOINT_REGS*)&endp1TxAddr;
		usbhostReg->endp2In		=	(ENDPOINT_REGS*)&endp2RxAddr;
		usbhostReg->endp2Out	=	(ENDPOINT_REGS*)&endp2TxAddr;

		usbhostReg->endp3In		=	(ENDPOINT_REGS*)&endp3RxAddr;	//HID
		usbhostReg->endp3Out	=	(ENDPOINT_REGS*)&endp3TxAddr;

		usbhostReg->ctrl		=	(COMMON_CTRL_REG*)&commCtlAddr;
		usbhostReg->irq			=	(USB_IRQ_REGS*)&irqAddr;

		usbhostReg->portctrl	=	(U8*)REG_ADDR(0x1ab);   //0x1ab [des] 2.17.6 hcportctrl 
		usbhostReg->framenum	=	(U16*)REG_ADDR(0x1ac);  //0x1ac [des] 2.17.7 hcfrmnrl 
		usbhostReg->frameRemain	=	(U16*)REG_ADDR(0x1ae);  //0x1ae [des] 2.17.8 hcfrmnrh
		REG_MISC_STATUS = 0x80000213;
	}
	else
	{
		#ifdef USB_DEV_ENABLE
		DBG_Printf("%s\n\r", optek_lib_usb_device_version_get());
		delayms(2);
		DBG_Printf("Build time:%s", optek_lib_usb_device_date_get());
		DBG_Printf(" %s\n\r", optek_lib_usb_device_time_get());
		#endif

		otgRegister	=	(USB_OTG_REGISTERS *)USB_OTG_BASE_ADDR;
		
		otg_phy_set(enUSB_DEVICE_TYPE);

		otgEnhCtrl	=	&otgEnhanceCtrl;

		otgEnhCtrl->ctrl	=	(U32*)OTG_CTL_REG_ADDR;
		otgEnhCtrl->dmaEndp1Addr	=	OTG_DMA_ENDP1_ADDR;
		otgEnhCtrl->dmaEndp2Addr	=	OTG_DMA_ENDP2_ADDR;
		otgEnhCtrl->dmaEndp3Addr	=	OTG_DMA_ENDP3_ADDR;
		REG_MISC_STATUS = 0x80000221;
		
		usbhostReg	=	&usbHostRegs;
		usbdevReg	=	&usbDevRegs;
		REG_MISC_STATUS = 0x80000222;


		usbdevReg->endp0		=	(ENDPOINT0_REGS*)&endp0Addr;    //rxbc, txbc, cs, ... setupdat, rxdata, txdata
		usbdevReg->endp1In		=	(ENDPOINT_REGS*)&endp1TxAddr;   //useless !
		usbdevReg->endp1Out		=	(ENDPOINT_REGS*)&endp1RxAddr;   //ep1 0x01, Spk Out from host,*endp1Out,Out for Device in dcd_open()
		usbdevReg->endp2In		=	(ENDPOINT_REGS*)&endp2TxAddr;   //ep2 0x82, Mic In to host,*endp2In, IN in dcd_open()
		usbdevReg->endp2Out		=	(ENDPOINT_REGS*)&endp2RxAddr;   //useless !

		//HID Int Input
		usbdevReg->endp3In		=	(ENDPOINT_REGS*)&endp3TxAddr;   //ep3 0x83, HID,*endp3In, IN in dcd_open()
		usbdevReg->endp3Out		=	(ENDPOINT_REGS*)&endp3RxAddr;   //useless !
		usbdevReg->ctrl			=	(COMMON_CTRL_REG*)&commCtlAddr;
		usbdevReg->irq			=	(USB_IRQ_REGS*)&irqAddr;
		
		usbdevReg->framecnt		=	(U16*)REG_ADDR(0x1a4);
		REG_MISC_STATUS = 0x80000223;
	}

	return TRUE;
}

int otg_open( enUSB_TYPE usb_type )
{
	volatile U32 reg32;
	int i;

	DBG_Printf("%s\n\r", __func__);

	memset( &otg, 0, sizeof(otg) );
	otg.curstate = otg.oldstate = 0xFF; //invalid state
	REG_MISC_STATUS = 0x80000031;

	for( i = 0; i < 1; i ++ )
	{
		#ifdef OTG_FORCE_FULL_SPEED
		reg32 = ENH_CTRL_PHY_CLK_ENA|ENH_CTRL_PHY_REG_ENA|ENH_CTRL_PHY_PLL_ENA|ENH_CTRL_DIS_HSPEED | ENH_CTRL_PHY_NO_SUSPEND;
		*otgEnhCtrl->ctrl = reg32;

		delayus(10);
		reg32 |= ENH_CTRL_RESET;
		*otgEnhCtrl->ctrl = reg32;
		REG_MISC_STATUS = 0x80000032;

		delayus(10);
		reg32 = ENH_CTRL_PHY_CLK_ENA|ENH_CTRL_PHY_REG_ENA|ENH_CTRL_PHY_PLL_ENA|ENH_CTRL_DIS_HSPEED | ENH_CTRL_PHY_NO_SUSPEND;
		#endif

		reg32 |= (ENH_CTRL_INT_USB<<ENH_CTRL_INT_ENA_SHIFT) ;
		*otgEnhCtrl->ctrl = reg32;

		delayus(10);
		REG_MISC_STATUS = 0x80000033;

		otgRegister->taaidlbdis = 0x80;
		otgRegister->tawaitbcon = 0x80;
		otgRegister->ien		= 0x1d;
	}
	REG_MISC_STATUS = 0x80000034;

	if (!(usb_type == enUSB_HOST_TYPE))
	{
		#ifdef USB_DEV_ENABLE
		//patch for device mode
		otgRegister->ctrl = 0x00;
		REG_MISC_STATUS = 0x23;
		#endif
	}

	//patch for ID tied low
	otgRegister->ctrl = 0x01;
	REG_MISC_STATUS = 0x24;

	REG_MISC_STATUS = 0x80000035;

	return TRUE;
}

void otg_phy_standby(void)  __attribute__ ((section (".flash_boot_text")));
void otg_phy_standby (void)
{
	U32 *pReg;
	
	pReg = (U32 *) OTG_CTL_REG_ADDR;
	*pReg = 0x0; //phy standby
}


int otg_close( enUSB_TYPE usb_type )
{
	DBG_Printf("%s\n\r", __func__);

	USB_INT_DISABLE;
	otgRegister->ien = 0x00;
	*otgEnhCtrl->ctrl &= ~ENH_CTRL_INT_ENA_MASK;
	*otgEnhCtrl->ctrl &= ~(ENH_CTRL_PHY_CLK_ENA | ENH_CTRL_PHY_PLL_ENA | ENH_CTRL_PHY_REG_ENA | ENH_CTRL_PHY_NO_SUSPEND);

	return TRUE;
}

int otg_power_down(void)
{
	DBG_Printf("%s\n\r", __func__);

	USB_INT_DISABLE;
	*otgEnhCtrl->ctrl = 0;

	return 0;
}

#endif //USB_HOST_ENABLE
