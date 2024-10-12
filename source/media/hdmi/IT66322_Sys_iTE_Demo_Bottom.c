//*****************************************/
//  Copyright (C) 2009-2022
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <IT66322_Sys_iTE_Demo_Bottom.c>
//   @author Hojim.Tseng@ite.com.tw
//   @date   2022/12/06
//   @fileversion: IT66322_1.37
//******************************************/

#include "c_def.h"
#include "debug.h"
#include "oem.h"


#ifdef HDMI_ENABLE
#include "os_config.h"

#include "regmap.h"

#include "mem_reloc.h"

#include "hw_pll.h"
#include "hal_gpio.h"

#include "app_i2c.h"
#include "hdmi_process.h"

#include "interrupt.h"

#include "ITEMT_Sys.h"




#define SB_I2C_Write(u8Offset, u16Len, u8Buf)	I2C0_write_sub(0xB0, u8Offset, u8Buf, u16Len)
#define SB_I2C_Read(u8Offset, u16Len, u8Buf)	I2C0_read_sub(0xB0, u8Offset, u8Buf, u16Len)


//****************************************************************************
//****************************************************************************
iTE_u8	g_u8eArcLatency = 0;
iTE_u8  u8SbVol = 0;
//****************************************************************************
iTE_u8	g_u8SbRxSel = 0;			// 0xF0
iTE_u8	g_u8SbAudioMode = 0;		// 0xF1
iTE_u8	g_u8SbTxSta = 0;			// 0xF2

iTE_u8	g_u8IteCecReq = 0;
iTE_u8	g_u8IteRdy = 0;
iTE_u8	g_u8SbPower = 0;
iTE_u8	g_u8SbCecMode = SB_CEC_AUTO;
/*
*iTE_u8	g_u8ArcRetryDelayCnt = 0;
*/
iTE_u8	g_u8SbEdidPaAB = 0xFF;
iTE_u8	g_u8SbEdidPaCD = 0xFF;
iTE_u8	g_u8SbInfoCA = 0;


//****************************************************************************
void I2C_Slave_Dump(void)
{
	iTE_u16 u16Cnt;
	iTE_u8	u8I2cData[0x100];
	iTE_pu8	pu8I2cData;
	if(0)
	{	
	for(u16Cnt = 0; u16Cnt < 0x100; u16Cnt+= 0x10){
		SB_I2C_Read(u16Cnt, 0x10, &u8I2cData[u16Cnt]);
	}

	for(u16Cnt = 0; u16Cnt < 0x100; u16Cnt += 0x10)
	{
		pu8I2cData = &u8I2cData[u16Cnt];
		iTE_Msg(("\r0x%02X: %02X %02X %02X %02X  %02X %02X %02X %02X   %02X %02X %02X %02X  %02X %02X %02X %02X\n", u16Cnt, pu8I2cData[0], pu8I2cData[1],
			pu8I2cData[2], pu8I2cData[3], pu8I2cData[4], pu8I2cData[5], pu8I2cData[6], pu8I2cData[7], pu8I2cData[8], pu8I2cData[9], pu8I2cData[10], pu8I2cData[11],
			pu8I2cData[12], pu8I2cData[13], pu8I2cData[14], pu8I2cData[15]));
	}

		iTE_Msg(("SB_Power[%d]\n", (u8I2cData[I2C_SYS_ADO_MODE] & 0x03)));
		iTE_Msg(("Rx_Sel[%d], AdoDecode_RxSel[%d]\n", (u8I2cData[I2C_SYS_RX_SEL] & 0x0F), (u8I2cData[I2C_SYS_RX_SEL] >> 4)));
		iTE_Msg(("Tx_DisOut[%d], Tx_AdoMute[%d]\n", (u8I2cData[I2C_SYS_ADO_MODE] & 0x80) >> 7, (u8I2cData[I2C_SYS_ADO_MODE] & 0x40) >> 6));
		iTE_Msg(("TV_CEC[%d], SB_CEC[%d]\n", (u8I2cData[I2C_SYS_TX_STA] & 0x02) >> 1, (u8I2cData[I2C_SYS_ADO_MODE] & 0x20) >> 5));
		if(g_u8SbCecMode){
			iTE_Msg(("SB CEC Off\n"));
		}else{
			iTE_Msg(("SB CEC Auto\n"));
		}
		iTE_Msg(("AudioSystemEnable[%d], eARC/ARC_enable[%d]\n", (u8I2cData[I2C_SYS_ADO_MODE] & 0x08) >> 3, (u8I2cData[I2C_SYS_ADO_MODE] & 0x10) >> 4));
		iTE_Msg(("Audio Ready, HDMI[%d], eARC[%d], ARC[%d]\n", ((u8I2cData[I2C_ADO_SRC_STA] & 0x4) >> 2),((u8I2cData[I2C_ADO_SRC_STA] & 0x2) >> 1), (u8I2cData[I2C_ADO_SRC_STA] & 0x1) ));
		iTE_Msg(("Audio Sourc Sel[%d] \n", u8I2cData[I2C_ADO_SEL]));

		iTE_Msg(("SbTxSta = %X, SbAudioMode %X \n", g_u8SbTxSta, g_u8SbAudioMode));
	}
	iTE_Msg(("\n\n\n"));
}

//****************************************************************************
void SB_SysI2cChange(iTE_u8 u8I2cChg)
{
	if(u8I2cChg)
	{
		iTE_u8	u8Temp;

		SB_I2C_Read(I2C_SYS_CHANGE, 1, &u8Temp);
		iTE_Msg(("SB Read = %X\n", u8Temp));

		u8I2cChg |= u8Temp;
		SB_I2C_Write(I2C_SYS_CHANGE, 1, &u8I2cChg);
		iTE_Msg(("SB Write = %X\n", u8I2cChg));

		/*
		*I2C_Slave_Dump();
		*/
	}
}

//****************************************************************************

iTE_u8	const 	u8SBSpkAlloc[] = {0x83,	0x6F, 0x0F, 0x0C};
iTE_u8	const 	u8SBAdb[] = {

0x3B,				// Tag=1 (Audio Data Block), Length=27
0x0F, 0x7F, 0x07,	// LPCM : 8-ch, 32~192K
0x15, 0x07, 0x50,	// AC-3 : 6-ch, 32~48K
0x35, 0x06, 0x3C,	// AAC	: 6-ch, 44~48K
0x3E, 0x1E, 0xC0,	// DTS	: 7-ch, 44~96K
0x4D, 0x02, 0x00,	// DSD	: 6-ch, 44K
0x57, 0x06, 0x00,	// HBR	: 8-ch, 44~48K (Dolby Digital)
0x5F, 0x7E, 0x01,	// HBR	: 8-ch, 44~192K (DTS-HD)
0x67, 0x7E, 0x00,	// HBR	: 8-ch, 44~192K (Dolby TrueHD) MAT
0xFF, 0x7F, 0x6F,	// LPCM : 16-ch, 32~192K (3D Audio)
};

#define SB_ADB_LEN	(sizeof(u8SBAdb))

#define SB_VOL_MAX		(100)
#define SB_VOL_DEFAULT	(30)

iTE_u8	const  u8eArcCapTable[] = {
		0x01,				// Capabilities Data Structure Version = 0x01
#ifdef _SL870ATC_
							0x01, 0x2C, 		// BLOCK_ID=1, 44-byte
#else
							0x01, 0x26, 		// BLOCK_ID=1, 38-byte change by peter for SL-870 HFR5-2-36
#endif
							0x3B,				// Tag=1 (Audio Data Block), Length=27
							0x0F, 0x7F, 0x07,	// LPCM : 8-ch, 32~192K
							0x15, 0x07, 0x50,	// AC-3 : 6-ch, 32~48K
							0x35, 0x06, 0x3C,	// AAC	: 6-ch, 44~48K
							0x3E, 0x1E, 0xC0,	// DTS	: 7-ch, 44~96K
							0x4D, 0x02, 0x00,	// DSD	: 6-ch, 44K
							0x57, 0x06, 0x00,	// HBR	: 8-ch, 44~48K (Dolby Digital)
							0x5F, 0x7E, 0x01,	// HBR	: 8-ch, 44~192K (DTS-HD)
							0x67, 0x7E, 0x00,	// HBR	: 8-ch, 44~192K (Dolby TrueHD) MAT
							0xFF, 0x7F, 0x6F,	// LPCM : 16-ch, 32~192K (3D Audio)
							//0x20 offset
							0x83,				// Tag=4 (Speaker Allocation Data Block), 3-bye
							0x6F, 0x0F, 0x0C,

#ifdef _SL870ATC_
							0xEB, 0x13, 		   // Tag Code=7, Length=11, Extended Tag Code=0x13 (Room Configuration Data Block)
							0x00, 0x6F, 0x0F, 0x0C,
							0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //change by peter for SL-870 HFR5-2-36
#else
							//0x24 offset
							0xE5, 0x13, 		   // change by peter for SL-870 HFR5-2-36
							0x00, 0x6F, 0x0F, 0x0C,
#endif

#ifdef _QD980ATC_       // temp for QD980 HFR5-2-36  //0x2A offset
							0x02, 0x0C, 		// BLOCK_ID=2, 12-byte
							0xEB, 0x14, 		   // Tag Code=7, Length=11, Extended Tag Code=0x14
#else
							0x02, 0x0A, 		// BLOCK_ID=2, 10-byte	  // should be marked for QD980 HFR5-2-26
#endif

							0x20, 0x00, 0x00, 0x00, 0x00,
							0x21, 0x01, 0x01, 0x01, 0x01,

							0x03, 0x01, 		// BLOCK_ID=3, 1-byte
							0x89,				// Supports_AI=1, ONE_BIT_AUDIO_LAYOUT=1 (12-ch), MULTI_CH_LPCM_LAYOUT=1 (16-ch)

#ifndef _QD980ATC_
							0x00, 0x00, 		   // should be marked for QD980 HFR5-2-26
#endif

#ifndef _SL870ATC_
							0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
#endif
};
#define EARC_CAP_TABLE_SIZE		sizeof(u8eArcCapTable)

/*
*#if (EARC_CAP_TABLE_SIZE > 0x100)
*#pragma message("EARC_CAP_TABLE_SIZE > 0x100")
*#endif
*/

iTE_u8	const  u8CecVendorId[3] = {0x00, 0xE0, 0x36};
iTE_u8	const  u8CecOsdName[14] = "SOUNDBAR";

static iTE_u8	const 	g_u8OSDString[13] = "";
//****************************************************************************
iTE_u32	g_u32SbAdoDecInfo = 0;
#define IgnorAdoSta  (0)//(ADO_CONF_ACTIVE_MASK)
iTE_u8	g_u8ChipId = SB_I2C_NO_ITE;

//****************************************************************************
iTE_u8 SB_DefaultSetting(void)
{
	iTE_u8	u8I2cReg[3];
	iTE_u8	u8RxMode;
	iTE_u8	u8I2cChg = 0;

	iTE_u8	u8AdoSel = 0;
	
	iTE_Msg(("SB_DefaultSetting \n"));

	u8I2cReg[0] = SB_VOL_DEFAULT;
	SB_I2C_Write(I2C_ADO_VOL, 1, u8I2cReg);
	u8I2cReg[0] = SB_VOL_MAX;
	u8I2cReg[1] = 0;
	u8I2cReg[2] = 0;
	SB_I2C_Write(I2C_VOLUME_MAX, 3, u8I2cReg);

	/*
	 * 0x00 -> 128fs default
	 * 0x01 -> 256fs
	 * 0x03 -> 512fs
	 */

	u8I2cReg[0] = 0x01;//256fs

	SB_I2C_Write(I2C_MCLK_FREQ_SEL, 1, u8I2cReg);
	
	/*
	*SB_I2C_Write(I2C_ADO_SEL, 1, &u8AdoSel);
	*/
	
	u8I2cChg |= I2C_UPD_ADO_SET;

	u8I2cReg[0] = 8;
	SB_I2C_Write(I2C_EDID_UPDATE, 1, u8I2cReg);
	SB_I2C_Write(I2C_EDID_SPK_ALLOC, 4, (iTE_pu8)u8SBSpkAlloc);
	SB_I2C_Write(I2C_EDID_ADB0, 31, (iTE_pu8)u8SBAdb);
	u8I2cChg |= I2C_UPD_EDID_SET;

	g_u8SbRxSel = 00;
	SB_I2C_Write(I2C_SYS_RX_SEL, 1, &g_u8SbRxSel);

	u8RxMode = 0x85;
	SB_I2C_Write(I2C_SYS_RX_MODE_0, 1, &u8RxMode);
	SB_I2C_Write(I2C_SYS_RX_MODE_1, 1, &u8RxMode);

	/*
	*g_u8SbAudioMode = 0x20;
	*SB_I2C_Write(I2C_SYS_ADO_MODE, 1, &g_u8SbAudioMode);
	*/
	
	u8I2cChg |= I2C_UPD_SYS_SET;

	u8RxMode = 0;
	SB_I2C_Write(I2C_SYS_CEC_TRANS_CNT, 1, &u8RxMode);
	u8RxMode = 0xA0; //eVendorCommandWithID
	if((g_u8ChipId == SB_I2C_IT6622B) || (g_u8ChipId == SB_I2C_IT6622A)){
		SB_I2C_Write(I2C_SYS_CEC_LATCH_CNT_6622, 1, &u8RxMode);
	}else{
		SB_I2C_Write(I2C_SYS_CEC_LATCH_CNT, 1, &u8RxMode);
	}
	SB_I2C_Write(I2C_CEC_TRANS_DATA, 1, &u8RxMode);
	u8RxMode = 0x89; // eVendorCommand
	SB_I2C_Write(I2C_CEC_TRANS_DATA + 1, 1, &u8RxMode);
	u8RxMode = 0x00;
	SB_I2C_Write(I2C_CEC_TRANS_DATA + 2, 1, &u8RxMode);
	u8I2cChg |= I2C_UPD_CEC_FIRE_SET;
	return u8I2cChg;
}
//****************************************************************************
iTE_u8 SB_PowerOff(void)
{
	iTE_u8	u8I2cChg;
	iTE_Msg(("SB_PowerOff \n"));
	g_u8SbPower = 0;
	u8I2cChg = SB_DefaultSetting();
	// 00: power down mode
	g_u8SbAudioMode &= 0xFC;
	SB_I2C_Write(I2C_SYS_ADO_MODE, 1, &g_u8SbAudioMode);
	u8I2cChg |= I2C_UPD_SYS_SET;
	return u8I2cChg;
}
//****************************************************************************
iTE_u8 SB_PowerOn(void)
{
	iTE_u8	u8I2cChg = 0;
	iTE_u8 	u8RxMode;
	iTE_Msg(("SB_PowerOn \n"));
	g_u8SbPower = SB_POWER_ON;
	// Enable HDMI audio decode
	switch(g_u8SbRxSel & 0x0F){
		case 	0:	g_u8SbRxSel = I2C_HDMI_SELECT_R0 | I2C_HDMI_ADO_SEL_R0;	break;
		case	1:	g_u8SbRxSel = I2C_HDMI_SELECT_R1 | I2C_HDMI_ADO_SEL_R1;	break;
		default:	g_u8SbRxSel = I2C_HDMI_SELECT_R0 | I2C_HDMI_ADO_SEL_R0;	break;
	}
	
	/*
	*g_u8SbRxSel &= 0xF;
	*g_u8SbRxSel |= g_u8SbRxSel << 4;
	*/
	
	SB_I2C_Write(I2C_SYS_RX_SEL, 1, &g_u8SbRxSel);
	// 1: power on mode
	g_u8SbAudioMode = 0x39;//0x01;//0x41//0x61;
	SB_I2C_Write(I2C_SYS_ADO_MODE, 1, &g_u8SbAudioMode);
	// SB CEC auto mode
	g_u8SbCecMode = SB_CEC_AUTO;
	// Rx Force ON & using SB ADB
	u8RxMode = I2C_RX0_ACTIVE_FORCE | I2C_RX0_HDCP_RPT_SET | I2C_RX0_HDCP_VER_23 | I2C_RX0_EDID_SB;//u8RxMode;
	SB_I2C_Write(I2C_SYS_RX_MODE_0, 1, &u8RxMode);
	u8RxMode = I2C_RX1_ACTIVE_FORCE | I2C_RX1_HDCP_RPT_SET | I2C_RX1_HDCP_VER_23 | I2C_RX1_EDID_SB;//u8RxMode;
	SB_I2C_Write(I2C_SYS_RX_MODE_1, 1, &u8RxMode);

	u8I2cChg |= I2C_UPD_SYS_SET;

	return u8I2cChg;
}
//****************************************************************************
iTE_u8 SB_Standby(void)
{
	iTE_u8	u8RxMode;
	iTE_Msg(("SB_Standby \n"));
	g_u8SbPower = SB_STANDBY;
	// 10: standby mode
	I2C_STA_CHANGE(g_u8SbAudioMode, I2C_MODE_POWER_MASK | I2C_MODE_CEC_MASK, I2C_MODE_POWER_STANDBY | I2C_MODE_CEC_EN);
	/*
	*g_u8SbAudioMode &= 0xFC;
	*g_u8SbAudioMode |= 0x22;
	*/
	SB_I2C_Write(I2C_SYS_ADO_MODE, 1, &g_u8SbAudioMode);

	u8RxMode = I2C_RX0_ACTIVE_FORCE | I2C_RX0_HDCP_RPT_SET | I2C_RX0_HDCP_VER_23 | I2C_RX0_EDID_SB;//u8RxMode;
	/*
	*SB_I2C_Write(I2C_SYS_RX_MODE_0, 1, &u8RxMode);
	*/
	u8RxMode = I2C_RX1_ACTIVE_FORCE | I2C_RX1_HDCP_RPT_SET | I2C_RX1_HDCP_VER_23 | I2C_RX1_EDID_SB;//u8RxMode;
	/*
	*SB_I2C_Write(I2C_SYS_RX_MODE_1, 1, &u8RxMode);
	*/
	return I2C_UPD_SYS_SET;
}
//****************************************************************************
iTE_u8 SB_PassThrough(void)
{
	iTE_u8	u8RxMode;
	iTE_Msg(("SB_PassThrough \n"));
	g_u8SbPower = SB_PASS_THROUGH;
	// Disable HDMI Audio decode
	/*
	*g_u8SbRxSel |= 0xF0;
	*/
	I2C_STA_CHANGE(g_u8SbRxSel, I2C_HDMI_ADO_SEL_MASK, 	I2C_HDMI_ADO_SEL_NONE);
	SB_I2C_Write(I2C_SYS_RX_SEL, 1, &g_u8SbRxSel);
	// Power On, Audio System Off, eARC/ARC Off, SB CEC Off, Tx Output & audio out.
	g_u8SbCecMode = SB_CEC_OFF;
	g_u8SbAudioMode = I2C_MODE_TX_OUT_EN | I2C_MODE_TX_ADO_EN | I2C_MODE_CEC_DIS | I2C_MODE_EARC_DIS | I2C_MODE_ADO_SYS_DIS | I2C_MODE_POWER_ON;// 0x01;
	SB_I2C_Write(I2C_SYS_ADO_MODE, 1, &g_u8SbAudioMode);
	// Rx active when Tx select to.
	/*
	*u8RxMode = 0x00;
	*/
	u8RxMode = I2C_RX0_ACTIVE_BY_SEL | I2C_RX0_HDCP_RPT_CLR | I2C_RX0_HDCP_VER_AUTO | I2C_RX0_EDID_TV;//u8RxMode;
	SB_I2C_Write(I2C_SYS_RX_MODE_0, 1, &u8RxMode);
	u8RxMode = I2C_RX1_ACTIVE_BY_SEL | I2C_RX1_HDCP_RPT_CLR | I2C_RX1_HDCP_VER_AUTO | I2C_RX1_EDID_TV;
	SB_I2C_Write(I2C_SYS_RX_MODE_1, 1, &u8RxMode);
	return I2C_UPD_SYS_SET;
}
//****************************************************************************
iTE_u8 SB_EdidChg(iTE_u1 u8EdidChg)
{
	iTE_u8 u8I2cChg = 0;
	iTE_u8	u8EdidMode[2];

	SB_I2C_Read(I2C_SYS_RX_MODE_0, 2, u8EdidMode);

	if(u8EdidChg != (u8EdidMode[0] & 0x01)){
		u8EdidMode[0] &= 0xFE;
		u8EdidMode[0] |= u8EdidChg;
		u8I2cChg = 0x80;
	}
	if(u8EdidChg != (u8EdidMode[1] & 0x01)){
		u8EdidMode[1] &= 0xFE;
		u8EdidMode[1] |= u8EdidChg;
		u8I2cChg = 0x80;
	}
	if(u8I2cChg){
		SB_I2C_Write(I2C_SYS_RX_MODE_0, 2, u8EdidMode);
		/*
		*SB_I2C_Write(I2C_ADO_SEL, 1, &u8EdidChg);
		*u8I2cChg |= 0x02;
		*/
	}
	return u8I2cChg;
}
//****************************************************************************

iTE_u8 SB_CecPower(iTE_u1 bEn)
{
	iTE_u8	u8I2cChg = 0;
	if(bEn){
		if((g_u8SbAudioMode & 0x03) != 0x01){
			g_u8SbAudioMode &= 0xFC;
			g_u8SbAudioMode |= 0x01;
			SB_I2C_Write(I2C_SYS_ADO_MODE, 1, &g_u8SbAudioMode);
			u8I2cChg |= 0x80;
			iTE_Msg(("CEC -> Power On \n"));
		}
	}else{
		g_u8SbAudioMode &= 0xFC;
		g_u8SbAudioMode |= 0x00;
		SB_I2C_Write(I2C_SYS_ADO_MODE, 1, &g_u8SbAudioMode);
		u8I2cChg |= 0x80;
		iTE_Msg(("CEC -> Power Off \n"));
	}
	return u8I2cChg;
}
//****************************************************************************
void SB_I2cWriteEarcCap(iTE_u8 u8Bank)
{
	#define EARC_CAP_BANK_SIZE		(0x80)
	iTE_u8	u8eArcCap[EARC_CAP_BANK_SIZE];
	iTE_u8	u8Cnt;
	iTE_pu8	pu8Ptr = u8eArcCap;
	if(u8Bank == 0){
		if(EARC_CAP_TABLE_SIZE > EARC_CAP_BANK_SIZE){
			u8Cnt = EARC_CAP_BANK_SIZE;
		}else{
			u8Cnt = EARC_CAP_TABLE_SIZE;
		}
		SB_I2C_Write(0x40, u8Cnt, (iTE_pu8)&u8eArcCapTable[0]);
	}else{
		if(EARC_CAP_TABLE_SIZE > EARC_CAP_BANK_SIZE){
			if(EARC_CAP_TABLE_SIZE > 0x100){
				u8Cnt = EARC_CAP_BANK_SIZE;
				iTE_Msg(("ERROR, Wrong eARC capability size %d\n", EARC_CAP_TABLE_SIZE));
			}else{
				u8Cnt = (iTE_u8)EARC_CAP_TABLE_SIZE - EARC_CAP_BANK_SIZE;
			}
			SB_I2C_Write(0x40, u8Cnt, (iTE_pu8)&u8eArcCapTable[EARC_CAP_BANK_SIZE]);
		}else{
			u8Cnt = 0;
		}
	}

	memset(pu8Ptr,0,EARC_CAP_BANK_SIZE-u8Cnt);

	if(u8Cnt < EARC_CAP_BANK_SIZE){
		SB_I2C_Write(0x40 + u8Cnt, EARC_CAP_BANK_SIZE - u8Cnt, u8eArcCap);
	}
}
//****************************************************************************
iTE_u8	const u8IteChipId[][4] = {
		{SB_I2C_IT66322, IT66322_CHIP_ID_0, IT66322_CHIP_ID_1, IT66322_CHIP_ID_2},
		{SB_I2C_IT66323, IT66323_CHIP_ID_0, IT66323_CHIP_ID_1, IT66323_CHIP_ID_2},
		{SB_I2C_IT66324, IT66324_CHIP_ID_0, IT66324_CHIP_ID_1, IT66324_CHIP_ID_2},
		{SB_I2C_IT6622A, IT6622A_CHIP_ID_0, IT6622A_CHIP_ID_1, IT6622A_CHIP_ID_2},
		{SB_I2C_IT6622B, IT6622B_CHIP_ID_0, IT6622B_CHIP_ID_1, IT6622B_CHIP_ID_2},
		{SB_I2C_IT66320, IT66320_CHIP_ID_0, IT66320_CHIP_ID_1, IT66320_CHIP_ID_2},
};
#define ITE_CHIP_SUP_MAX	(sizeof(u8IteChipId)/3)
//****************************************************************************
void SB_I2C_ClearInt(void)
{
	iTE_u8	u8Data;

	switch(g_u8ChipId){
		case	SB_I2C_IT66322:
		case	SB_I2C_IT66323:
		case	SB_I2C_IT6622A:
				u8Data = 0;
			break;
		case	SB_I2C_IT66324:
		case	SB_I2C_IT6622B:
		case	SB_I2C_IT66320:
				u8Data = 0xFF;
			break;
		default:
				iTE_Msg(("***** ITE not found *****\r\n"));
			return;
	}
	SB_I2C_Write(I2C_SYS_INT, 1, &u8Data);
}
//****************************************************************************
iTE_u8	SB_I2C_ChipGet(void)
{
	iTE_u8	u8Ver[9];
	iTE_u8	u8Cnt;
	int ret = 0;

	ret = SB_I2C_Read(I2C_ITE_CHIP, 9, u8Ver);
	if (ret != I2C_SUCCESS)
	{
		iTE_Msg(("i2c read ret: %d\n\r", ret));
		return SB_I2C_NO_ITE;
	}

	iTE_Msg(("SB Protocol Version: %02X %02X %02X %02X %02X %02X %02X %02X %02X\n", u8Ver[0], u8Ver[1], u8Ver[2], u8Ver[3], u8Ver[4], u8Ver[5], u8Ver[6], u8Ver[7], u8Ver[8]));

	for(u8Cnt = 0; u8Cnt < ITE_CHIP_SUP_MAX; u8Cnt++)
	{
		if((u8Ver[0] == u8IteChipId[u8Cnt][1]) && (u8Ver[1] == u8IteChipId[u8Cnt][2]) && (u8Ver[2] == u8IteChipId[u8Cnt][3]))
		{
			return u8IteChipId[u8Cnt][0];
		}
	}

	
}
//****************************************************************************

void SB_Audio_Interrupt_Rx(void)
{
	static iTE_u8 u8I2cIntRec = 0;
	iTE_u8 u8I2cChg = 0;
	iTE_u8 u8I2cInt;
	int ret;

	DBG_Printf("SB Int Rx\n\r");

	ret = SB_I2C_Read(I2C_SYS_CHANGE, 1, &u8I2cChg);
	if (ret != I2C_SUCCESS)
	{
		return;
	}
	
	ret = SB_I2C_Read(I2C_SYS_INT, 1, &u8I2cInt);
	if (ret != I2C_SUCCESS)
	{
		return;
	}
	
	if (u8I2cInt)
	{
		u8I2cInt |= u8I2cIntRec;
		u8I2cIntRec = 0;

		SB_I2C_ClearInt();

		u8I2cChg = 0;

		if ( (u8I2cInt & I2C_INT_AUDIO_MASK) == I2C_INT_AUDIO_SET)
		{
			DBG_Printf("I2C INT Audio Get\n\r");

			SB_Audio_Status_Get(&hdmi_infor);

			#ifdef HDMI_INT_CHECKTOUT_USED_TIMER
			app_timer_hdmi_int_finish_wait_time_set_from_isr(500);
			#else
			hdmi_int_process_finish_set(&hdmi_infor);
			hdmi_infor.int_count = 0;
			#endif

			DBG_Printf	("hdmi infor int get,tx mode:%d,stream_type:%d,channel:%d,sample_rate:%d\n\r",
						hdmi_infor.tx_mode,
						hdmi_infor.stream_type,
						hdmi_infor.channel,
						hdmi_infor.sample_rate
						);
		}

		#if (EN_CEC_SYS == iTE_TRUE)
		if ( (u8I2cInt & I2C_INT_SYS_MASK) == I2C_INT_SYS_SET)
		{
			DBG_Printf("I2C INT Sys Get\n\r");
			SB_CEC_Interrupt_Handle(&u8I2cChg);
		}
		#endif
	}
	else
	{
		DBG_Printf("I2C Read INT No Get\n\r");
	}

	SB_SysI2cChange(u8I2cChg);
}

int SB_Init(void)
{		
	static iTE_u8	u8I2cIntRec = 0;
	iTE_u8	u8I2cChg = 0;
	iTE_u8	u8I2cInt;
	int ret;
	

	ret = SB_I2C_Read(I2C_SYS_CHANGE, 1, &u8I2cChg);
	if (ret != I2C_SUCCESS)
	{
		DBG_Printf("HDMI Init FAIL\n\r");
		return FALSE;
	}

	ret =SB_I2C_Read(I2C_SYS_INT, 1, &u8I2cInt);
	if (ret != I2C_SUCCESS)
	{
		return FALSE;
	}

	if (hdmi_infor.init_status == HDMI_INIT_PROCESS_START)
	{
		DBG_Printf("HDMI Init Start\n\r");

		if(g_u8ChipId == SB_I2C_NO_ITE)
		{
			g_u8ChipId = SB_I2C_ChipGet();
		}

		if(g_u8ChipId == SB_I2C_NO_ITE)
		{
			iTE_Msg(("* ITE chip not found *\n"));
			return FALSE;
		}
				
		if((u8I2cInt & I2C_INT_READY_MASK) == I2C_INT_READY_SET)
		{
			DBG_Printf("* I2C_INT_READY_SET *\n\r");

			iTE_u8	u8Data = 0;
			u8I2cIntRec |= u8I2cInt & (~I2C_INT_READY_SET);
			SB_I2C_ClearInt();
			ret = SB_I2C_Write(I2C_SYS_CHANGE, 1, &u8Data);
			
			if(u8I2cChg == 0)
			{
				SB_I2cWriteEarcCap(0);
				u8I2cChg = I2C_INIT_SET | I2C_INIT_STAG_0 | I2C_INIT_RDY_SET;	//0x81
				DBG_Printf(" I2C_INT_READY_SET 1\n\r");
			}
			else if(u8I2cChg == (I2C_INIT_SET | I2C_INIT_STAG_0))
			{
				SB_I2cWriteEarcCap(1);
				u8I2cChg = I2C_INIT_SET | I2C_INIT_STAG_1 | I2C_INIT_RDY_SET;	//0x83;
				DBG_Printf(" I2C_INT_READY_SET 2\n\r");
			}
			else if(u8I2cChg == (I2C_INIT_SET | I2C_INIT_STAG_1))
			{
				ret = SB_I2C_Write(0x40, 0x03, (iTE_pu8)u8CecVendorId);
				ret = SB_I2C_Write(0x43, 0x0D, (iTE_u8*)g_u8OSDString) ; //CEC OSD string
				ret = SB_I2C_Write(0x50, 0x14, (iTE_pu8)u8CecOsdName);

				u8I2cChg = I2C_INIT_SET | I2C_INIT_STAG_2 | I2C_INIT_RDY_SET;	//0x85;
				DBG_Printf(" I2C_INT_READY_SET 3\n\r");
			}
			else if(u8I2cChg == (I2C_INIT_SET | I2C_INIT_STAG_2))
			{
				u8I2cChg = 0;
				if(u8I2cIntRec)
				{
					u8I2cChg |= I2C_UPD_TRI_INT_SET;
				}
				g_u8IteRdy = 1;
				u8I2cChg |= SB_DefaultSetting();
				/*
				*u8I2cChg |= SB_Standby();
				*/
				u8I2cChg |= SB_PowerOn();


				DBG_Printf(" I2C_INT_READY_SET 4\n\r");
				
				DBG_Printf("HDMI Init End\n\r");
			}
			else if(u8I2cChg == (I2C_INIT_SET | I2C_INIT_STAG_UPDATE))
			{
				DBG_Printf(" I2C_INT_READY_SET 5\n\r");
				static unsigned char u8ready = FALSE;
				if(!u8ready)
				{
					//FW ready be update
					u8ready = TRUE;
				}
				else
				{
					//exit update
					u8ready = FALSE;
				}
				u8I2cChg = 0;
			}
			
			iTE_Msg(("SB init = %X\n", u8I2cChg));
			SB_SysI2cChange(u8I2cChg);			
		}
		else
		{
			hdmi_infor.init_status = HDMI_INIT_PROCESS_END;
			hdmi_infor.init_enable = TRUE;
			DBG_Printf("Jump to HDMI_INFOR_GET\n\r");
			goto HDMI_INFOR_GET;
		}
	}
	else
	{
HDMI_INFOR_GET:
		DBG_Printf("hdmi status get\n\r");

		if (hdmi_infor.init_enable == TRUE)
		{
			hdmi_infor.init_enable = FALSE;

			SB_Audio_Status_Get(&hdmi_infor);
			
			#ifdef HDMI_INT_CHECKTOUT_USED_TIMER
			app_timer_hdmi_int_finish_wait_time_set_from_isr(50);
			#else
			hdmi_int_process_finish_set(&hdmi_infor);
			#endif

			DBG_Printf	("hdmi infor init get,tx mode:%d,stream_type:%d,channel:%d,sample_rate:%d\n\r",
						hdmi_infor.tx_mode,
						hdmi_infor.stream_type,
						hdmi_infor.channel,
						hdmi_infor.sample_rate
						);

			HDMI_INT_ENABLE;
		}
	}
}

int SB_CEC_Status_Set(void)
{
	iTE_u8 	u8Temp 	 = 0;
	iTE_u8 	u8I2cChg = 0;
	iTE_u8	u8I2c0xF1[4];
	int ret;

	ret = SB_I2C_Read(I2C_SYS_CHANGE, 1, &u8I2cChg);
	
	ret = SB_I2C_Read(I2C_SYS_ADO_MODE, 4, u8I2c0xF1);

	u8Temp = g_u8SbTxSta ^ u8I2c0xF1[1];
	g_u8SbTxSta = u8I2c0xF1[1];

	if(u8Temp)
	{
		if(u8Temp & I2C_TV_STA_HPD_MASK)
		{
			iTE_Msg(("TV HPD = %d\n", g_u8SbTxSta & I2C_TV_STA_HPD_MASK));
		}

		if(u8Temp & I2C_TV_STA_CEC_MASK)
		{
			if((g_u8SbTxSta & I2C_TV_STA_CEC_MASK) == I2C_TV_STA_CEC_ON)
			{
				g_u8SbAudioMode |= I2C_MODE_CEC_EN;
			}
			else
			{
			}
			SB_I2C_Write(I2C_SYS_ADO_MODE, 1, &g_u8SbAudioMode);
			u8I2cChg |= I2C_UPD_SYS_SET;
		}
		if(u8Temp & I2C_TV_STA_PWR_ON_MASK)
		{
			if((g_u8SbTxSta & I2C_TV_STA_PWR_ON_MASK) == I2C_TV_STA_PWR_ON_SET)
			{
				iTE_Msg(("TV CEC ON \n"));
				if(g_u8SbCecMode == SB_CEC_AUTO)
				{
					if((g_u8SbAudioMode & I2C_MODE_POWER_MASK) == I2C_MODE_POWER_ON)
					{
						g_u8SbAudioMode |= I2C_MODE_CEC_EN | I2C_MODE_EARC_EN | I2C_MODE_ADO_SYS_EN;// 0x38;//0x20;
						iTE_Msg(("SB CEC ON, Audio system On, eARC/ARC Enable \n"));
					}
				}
			}
			else
			{
				iTE_Msg(("TV CEC OFF \n"));
				if(g_u8SbCecMode == SB_CEC_AUTO)
				{
					/*
					*g_u8SbAudioMode &= ~(I2C_MODE_CEC_EN | I2C_MODE_EARC_EN | I2C_MODE_ADO_SYS_EN);//~0x38;//0x20;
					*/
					iTE_Msg(("SB CEC OFF, Audio system OFF, eARC/ARC Disable \n"));
				}

			}
			SB_I2C_Write(I2C_SYS_ADO_MODE, 1, &g_u8SbAudioMode);
			u8I2cChg |= I2C_UPD_SYS_SET;
		}
		if(u8Temp & 0x10)
		{
			iTE_u8	u8AdoSel;
			if((g_u8SbAudioMode & I2C_MODE_CEC_MASK) == I2C_MODE_CEC_EN)
			{
				if((g_u8SbTxSta & I2C_TV_STA_REQ_ADO_MASK) == I2C_TV_STA_REQ_ADO_SET)
				{
					iTE_Msg(("CEC On switch to TV\n"));
					u8AdoSel = 1;
				}
				else
				{
					iTE_Msg(("CEC On switch to HDMI\n"));
					u8AdoSel = 0;
				}
			}
			else
			{
				iTE_Msg(("CEC Off switch to HDMI\n"));
				u8AdoSel = 0;
				STA_CHANGE(g_u8SbTxSta, I2C_TV_STA_REQ_ADO_MASK, I2C_TV_STA_REQ_ADO_CLR);
			}
			SB_I2C_Write(I2C_ADO_SEL, 1, &u8AdoSel);
			u8I2cChg |= I2C_UPD_ADO_SET;
		}

		SB_SysI2cChange(u8I2cChg);
	}

	return 0;
}

int SB_CEC_Req_Set(void)
{
	iTE_u8 	u8Temp 	 = 0;
	iTE_u8 	u8I2cChg = 0;
	iTE_u8	u8I2c0xF1[4];
	int ret;

	ret = SB_I2C_Read(I2C_SYS_CHANGE, 1, &u8I2cChg);
	if (ret != I2C_SUCCESS)
	{
		return FALSE;
	}
	
	ret = SB_I2C_Read(I2C_SYS_ADO_MODE, 4, u8I2c0xF1);
	if (ret != I2C_SUCCESS)
	{
		return FALSE;
	}

	u8Temp = g_u8SbTxSta ^ u8I2c0xF1[1];

	if(u8I2c0xF1[2])
	{
		g_u8IteCecReq = u8I2c0xF1[2];
		iTE_Msg(("CEC REQ [0x%02X]\n", g_u8IteCecReq));
		if(g_u8IteCecReq & I2C_CEC_CMD_SWITCH_RX_MASK)
		{
			if(u8I2c0xF1[1] & I2C_TV_STA_REQ_ADO_MASK)
			{
			
			}
			else
			{
				switch(u8I2c0xF1[1] & I2C_TV_STA_REQ_SWITCH_MASK)
				{
					case	I2C_TV_STA_REQ_SWITCH_R0:
							g_u8SbRxSel = I2C_HDMI_SELECT_R0 | I2C_HDMI_ADO_SEL_R0;
							break;
					case	I2C_TV_STA_REQ_SWITCH_R1:
							g_u8SbRxSel = I2C_HDMI_SELECT_R1 | I2C_HDMI_ADO_SEL_R1;
							break;
					default:
						break;
				}

				/*
				*u8Temp = I2C_HDMI_SEL_W_INF | g_u8SbRxSel;
				*/
				
				u8Temp = g_u8SbRxSel;
				if(g_u8IteCecReq & I2C_CEC_CMD_ROUT_CHG_MASK)
				{
					u8Temp |= I2C_HDMI_SEL_W_INF;
				}
				SB_I2C_Write(I2C_SYS_RX_SEL, 1, &u8Temp);
				if(g_u8SbAudioMode & 0x80)
				{
					g_u8SbAudioMode &= 0x7F;
					SB_I2C_Write(I2C_SYS_ADO_MODE, 1, &g_u8SbAudioMode);
				}
				u8I2cChg |= I2C_UPD_SYS_SET;
				iTE_Msg(("CEC Routing Change to R%d\n", g_u8SbRxSel & 0xF));
			}
		}
		if(g_u8IteCecReq & I2C_CEC_CMD_LATCH_MASK)
		{
			iTE_u8	u8CecCmd[0x10], u8Cnt, u8LatchCnt;

			ret = SB_I2C_Read(I2C_CEC_LATCH_DATA, 0x10, u8CecCmd);

			if((g_u8ChipId == SB_I2C_IT6622B) || (g_u8ChipId == SB_I2C_IT6622A))
			{
				SB_I2C_Write(I2C_SYS_CEC_LATCH_CNT_6622, 1, &u8LatchCnt);
			}
			else
			{
				SB_I2C_Read(I2C_SYS_CEC_LATCH_CNT, 0x01, &u8LatchCnt);
			}
			iTE_Msg(("CEC Command Latch 0x%02X, Cnt = %d\n", u8CecCmd[1], u8LatchCnt));
			for(u8Cnt = 0; u8Cnt < u8LatchCnt; u8Cnt++)
			{
				iTE_MsgA((" %02X", u8CecCmd[u8Cnt]));
			}
			iTE_MsgA(("\n"));

		}
		if(g_u8IteCecReq & (I2C_CEC_CMD_SYS_ADO_ON_MASK | I2C_CEC_CMD_ACTIVE_SRC_MASK))
		{
			u8I2cChg |= SB_CecPower(1);
		}
		if(g_u8IteCecReq & I2C_CEC_CMD_STANDBY_MASK)
		{
			u8I2cChg |= SB_CecPower(0);
		}

		if(g_u8IteCecReq & 0x20)
		{
			u8I2cChg |= SB_EdidChg(1);
		}
		else if(g_u8IteCecReq & 0x02)
		{
			u8I2cChg |= SB_EdidChg(0);
		}

		u8Temp = 0;
		SB_I2C_Write(I2C_SYS_CEC_RECEIVE, 1, &u8Temp);
		
		SB_SysI2cChange(u8I2cChg);
	}
}

int SB_CEC_Power_Set(void)
{
	iTE_u8 	u8I2cChg = 0;
	iTE_u8	u8I2c0xF1[4];
	int ret;

	ret = SB_I2C_Read(I2C_SYS_CHANGE, 1, &u8I2cChg);
	if (ret != I2C_SUCCESS)
	{
		return FALSE;
	}
	
	ret = SB_I2C_Read(I2C_SYS_ADO_MODE, 4, u8I2c0xF1);
	if (ret != I2C_SUCCESS)
	{
		return FALSE;
	}

	if(u8I2c0xF1[3])
	{
		iTE_u8	u8CecReq2 = u8I2c0xF1[3];

		iTE_Msg(("CEC REQ2 [0x%02X]\n", u8CecReq2));
		if(u8CecReq2 & I2C_CEC_CMD_POWER_MASK)
		{
			if((g_u8SbAudioMode & 0x03) == 0x01)
			{
				u8I2cChg |= SB_CecPower(0);
			}
			else
			{
				u8I2cChg |= SB_CecPower(1);
			}
		}

		if(u8CecReq2 & I2C_CEC_CMD_POWER_ON_MASK)
		{
			u8I2cChg |= SB_CecPower(1);
		}

		if(u8CecReq2 & I2C_CEC_CMD_POWER_OFF_MASK)
		{
			u8I2cChg |= SB_CecPower(0);
		}
		
		u8CecReq2 = 0;
		SB_I2C_Write(I2C_SYS_CEC_RECEIVE2, 1, &u8CecReq2);

		SB_SysI2cChange(u8I2cChg);
	}
}

//for getting the values of Preamble AB and Preamble CD
int SB_Sync_Word_PaAB_PaCD_Get(void)
{
	iTE_u8	u8EdidData[2];
	int ret = I2C_SUCCESS;

	ret = SB_I2C_Read(I2C_EDID_PA_AB, 2, u8EdidData);
	if (ret != I2C_SUCCESS)
	{
		return FALSE;
	}

	if ((g_u8SbEdidPaAB != u8EdidData[0]) || (g_u8SbEdidPaCD != u8EdidData[1]))
	{
		iTE_Msg(("PA change %02X%02X->%02X%02X\n", g_u8SbEdidPaAB, g_u8SbEdidPaCD, u8EdidData[0], u8EdidData[1]));
		g_u8SbEdidPaAB = u8EdidData[0];
		g_u8SbEdidPaCD = u8EdidData[1];
	}

	return TRUE;
}

int SB_Audio_Status_Get(HDMI_INFOR_STRU *parm)
{
	iTE_u8	u8IT66322CA;
	iTE_u8	u8IT66322AdoInfo[5];
	iTE_u32 u32IT66322AdoInfo;
	int ret = I2C_SUCCESS;

	ret = SB_I2C_Read(I2C_ADO_INFO, 5, u8IT66322AdoInfo);
	if (ret != I2C_SUCCESS)
	{
		return FALSE;
	}

	u8IT66322CA = u8IT66322AdoInfo[4];
	u32IT66322AdoInfo = u8IT66322AdoInfo[3];
	u32IT66322AdoInfo <<= 8;
	u32IT66322AdoInfo |= u8IT66322AdoInfo[2];
	u32IT66322AdoInfo <<= 8;
	u32IT66322AdoInfo |= u8IT66322AdoInfo[1];
	u32IT66322AdoInfo <<= 8;
	u32IT66322AdoInfo |= u8IT66322AdoInfo[0];

	if(((g_u32SbAdoDecInfo & (~IgnorAdoSta)) != (u32IT66322AdoInfo & (~IgnorAdoSta))) ||(g_u8SbInfoCA != u8IT66322CA))
	{
		g_u32SbAdoDecInfo = u32IT66322AdoInfo;
		g_u8SbInfoCA = u8IT66322CA;

		if(g_u32SbAdoDecInfo & ADO_CONF_ACTIVE_MASK)
		{
			if(parm != NULL)
			{
				parm->tx_mode = SB_Audio_Stream_Tx_Type_Get(g_u32SbAdoDecInfo);
				parm->stream_type = SB_Audio_Stream_Type_Get(g_u32SbAdoDecInfo);

				parm->channel = SB_Audio_Stream_Channel_Get(g_u32SbAdoDecInfo);
				parm->sample_rate = SB_Audio_Stream_Sample_Rate_Get(g_u32SbAdoDecInfo, parm);
			}
		}
	}

	return TRUE;
}

int SB_Audio_Vol_Get(HDMI_INFOR_STRU *parm)
{
	iTE_u8	u8AudioVolume;
	int ret = I2C_SUCCESS;

	ret = SB_I2C_Read(I2C_ADO_VOL, 1, &u8AudioVolume);
	if (ret != I2C_SUCCESS)
	{
		return FALSE;
	}

	if(u8SbVol != u8AudioVolume)
	{
		iTE_Msg(("+u8SbVol=%d, u8AudioVolume=%d\n", u8SbVol, u8AudioVolume));

		u8SbVol = u8AudioVolume;
		iTE_Msg(("-u8SbVol=%d, u8AudioVolume=%d\n", u8SbVol, u8AudioVolume));

		if (parm != NULL)
		{
			parm->vol = u8SbVol;
		}
	}

	return TRUE;
}

int SB_eARC_LatenReq_Interrupt_Get(iTE_u8 *u8I2cChg)
{
	iTE_u8	u8eArcLatency;
	int ret = I2C_SUCCESS;

	ret = SB_I2C_Read(I2C_EARC_LATEN, 1, &u8eArcLatency);
	if (ret != I2C_SUCCESS)
	{
		return FALSE;
	}

	if(u8eArcLatency == ~g_u8eArcLatency)
	{
		iTE_u8	u8eArcLatenReq;
		SB_I2C_Read(I2C_EARC_LATEN_REQ, 1, &u8eArcLatenReq);
		if (ret != I2C_SUCCESS)
		{
			return FALSE;
		}

		g_u8eArcLatency = u8eArcLatenReq;

		SB_I2C_Write(I2C_EARC_LATEN, 1, &g_u8eArcLatency);
		if (ret != I2C_SUCCESS)
		{
			return FALSE;
		}

		u8eArcLatenReq = ~u8eArcLatenReq;
		SB_I2C_Write(I2C_EARC_LATEN_REQ, 1, &u8eArcLatenReq);
		if (ret != I2C_SUCCESS)
		{
			return FALSE;
		}

		*u8I2cChg |= I2C_UPD_ADO_SET;
		iTE_Msg(("eARC Latency Req = %d ms, Latency = %d ms\n", u8eArcLatenReq, g_u8eArcLatency));
	}

	return TRUE;
}

void SB_CEC_Interrupt_Handle(iTE_u8 *u8I2cChg)
{
	iTE_u8	u8I2c0xF1[4];
	iTE_u8	u8Temp;

	SB_I2C_Read(I2C_SYS_ADO_MODE, 4, u8I2c0xF1);
	u8Temp = g_u8SbTxSta ^ u8I2c0xF1[1];
	g_u8SbTxSta = u8I2c0xF1[1];

	if(u8Temp)
	{
		if(u8Temp & I2C_TV_STA_HPD_MASK)
		{
			iTE_Msg(("TV HPD = %d\n", g_u8SbTxSta & I2C_TV_STA_HPD_MASK));
		}

		if(u8Temp & I2C_TV_STA_CEC_MASK)
		{
			if((g_u8SbTxSta & I2C_TV_STA_CEC_MASK) == I2C_TV_STA_CEC_ON)
			{
				g_u8SbAudioMode |= I2C_MODE_CEC_EN;
			}
			else
			{
			}
			SB_I2C_Write(I2C_SYS_ADO_MODE, 1, &g_u8SbAudioMode);
			*u8I2cChg |= I2C_UPD_SYS_SET;
		}
		if(u8Temp & I2C_TV_STA_PWR_ON_MASK)
		{
			if((g_u8SbTxSta & I2C_TV_STA_PWR_ON_MASK) == I2C_TV_STA_PWR_ON_SET)
			{
				iTE_Msg(("TV CEC ON \n"));
				if(g_u8SbCecMode == SB_CEC_AUTO)
				{
					if((g_u8SbAudioMode & I2C_MODE_POWER_MASK) == I2C_MODE_POWER_ON)
					{
						g_u8SbAudioMode |= I2C_MODE_CEC_EN | I2C_MODE_EARC_EN | I2C_MODE_ADO_SYS_EN;// 0x38;//0x20;
						iTE_Msg(("SB CEC ON, Audio system On, eARC/ARC Enable \n"));
					}
				}
			}
			else
			{
				iTE_Msg(("TV CEC OFF \n"));
				if(g_u8SbCecMode == SB_CEC_AUTO)
				{
					/*
					*g_u8SbAudioMode &= ~(I2C_MODE_CEC_EN | I2C_MODE_EARC_EN | I2C_MODE_ADO_SYS_EN);//~0x38;//0x20;
					*/
					iTE_Msg(("SB CEC OFF, Audio system OFF, eARC/ARC Disable \n"));
				}

			}
			SB_I2C_Write(I2C_SYS_ADO_MODE, 1, &g_u8SbAudioMode);
			*u8I2cChg |= I2C_UPD_SYS_SET;
		}
		if(u8Temp & 0x10)
		{
			iTE_u8	u8AdoSel;
			if((g_u8SbAudioMode & I2C_MODE_CEC_MASK) == I2C_MODE_CEC_EN)
			{
				if((g_u8SbTxSta & I2C_TV_STA_REQ_ADO_MASK) == I2C_TV_STA_REQ_ADO_SET)
				{
					iTE_Msg(("CEC On switch to TV\n"));
					u8AdoSel = 1;
				}
				else
				{
					iTE_Msg(("CEC On switch to HDMI\n"));
					u8AdoSel = 0;
				}
			}
			else
			{
				iTE_Msg(("CEC Off switch to HDMI\n"));
				u8AdoSel = 0;
				STA_CHANGE(g_u8SbTxSta, I2C_TV_STA_REQ_ADO_MASK, I2C_TV_STA_REQ_ADO_CLR);
			}
			SB_I2C_Write(I2C_ADO_SEL, 1, &u8AdoSel);
			*u8I2cChg |= I2C_UPD_ADO_SET;
		}
	}

	if(u8I2c0xF1[2])
	{
		g_u8IteCecReq = u8I2c0xF1[2];
		iTE_Msg(("CEC REQ [0x%02X]\n", g_u8IteCecReq));
		if(g_u8IteCecReq & I2C_CEC_CMD_SWITCH_RX_MASK)
		{
			if(u8I2c0xF1[1] & I2C_TV_STA_REQ_ADO_MASK)
			{
			
			}
			else
			{
				switch(u8I2c0xF1[1] & I2C_TV_STA_REQ_SWITCH_MASK)
				{
					case	I2C_TV_STA_REQ_SWITCH_R0:
							g_u8SbRxSel = I2C_HDMI_SELECT_R0 | I2C_HDMI_ADO_SEL_R0;
							break;
					case	I2C_TV_STA_REQ_SWITCH_R1:
							g_u8SbRxSel = I2C_HDMI_SELECT_R1 | I2C_HDMI_ADO_SEL_R1;
							break;
					default:
						break;
				}

				/*
				*u8Temp = I2C_HDMI_SEL_W_INF | g_u8SbRxSel;
				*/
				u8Temp = g_u8SbRxSel;
				if(g_u8IteCecReq & I2C_CEC_CMD_ROUT_CHG_MASK)
				{
					u8Temp |= I2C_HDMI_SEL_W_INF;
				}
				SB_I2C_Write(I2C_SYS_RX_SEL, 1, &u8Temp);
				if(g_u8SbAudioMode & 0x80)
				{
					g_u8SbAudioMode &= 0x7F;
					SB_I2C_Write(I2C_SYS_ADO_MODE, 1, &g_u8SbAudioMode);
				}
				*u8I2cChg |= I2C_UPD_SYS_SET;
				iTE_Msg(("CEC Routing Change to R%d\n", g_u8SbRxSel & 0xF));
			}
		}
		if(g_u8IteCecReq & I2C_CEC_CMD_LATCH_MASK)
		{
			iTE_u8	u8CecCmd[0x10], u8Cnt, u8LatchCnt;
			SB_I2C_Read(I2C_CEC_LATCH_DATA, 0x10, u8CecCmd);
			if((g_u8ChipId == SB_I2C_IT6622B) || (g_u8ChipId == SB_I2C_IT6622A))
			{
				SB_I2C_Write(I2C_SYS_CEC_LATCH_CNT_6622, 1, &u8LatchCnt);
			}
			else
			{
				SB_I2C_Read(I2C_SYS_CEC_LATCH_CNT, 0x01, &u8LatchCnt);
			}
			iTE_Msg(("CEC Command Latch 0x%02X, Cnt = %d\n", u8CecCmd[1], u8LatchCnt));
			for(u8Cnt = 0; u8Cnt < u8LatchCnt; u8Cnt++)
			{
				iTE_MsgA((" %02X", u8CecCmd[u8Cnt]));
			}
			iTE_MsgA(("\n"));

		}

		if(g_u8IteCecReq & (I2C_CEC_CMD_SYS_ADO_ON_MASK | I2C_CEC_CMD_ACTIVE_SRC_MASK))
		{
			*u8I2cChg |= SB_CecPower(1);
		}

		if(g_u8IteCecReq & I2C_CEC_CMD_STANDBY_MASK)
		{
			*u8I2cChg |= SB_CecPower(0);
		}

		if(g_u8IteCecReq & 0x20)
		{
			*u8I2cChg |= SB_EdidChg(1);
		}
		else if(g_u8IteCecReq & 0x02)
		{
			*u8I2cChg |= SB_EdidChg(0);
		}

		u8Temp = 0;
		SB_I2C_Write(I2C_SYS_CEC_RECEIVE, 1, &u8Temp);
	}

	if(u8I2c0xF1[3])
	{
		iTE_u8	u8CecReq2 = u8I2c0xF1[3];
		iTE_Msg(("CEC REQ2 [0x%02X]\n", u8CecReq2));
		if(u8CecReq2 & I2C_CEC_CMD_POWER_MASK)
		{
			if((g_u8SbAudioMode & 0x03) == 0x01)
			{
				*u8I2cChg |= SB_CecPower(0);
			}
			else
			{
				*u8I2cChg |= SB_CecPower(1);
			}
		}

		if(u8CecReq2 & I2C_CEC_CMD_POWER_ON_MASK)
		{
			*u8I2cChg |= SB_CecPower(1);
		}

		if(u8CecReq2 & I2C_CEC_CMD_POWER_OFF_MASK)
		{
			*u8I2cChg |= SB_CecPower(0);
		}
		u8CecReq2 = 0;
		SB_I2C_Write(I2C_SYS_CEC_RECEIVE2, 1, &u8CecReq2);
	}
}

iTE_u32 SB_Audio_Stream_Sample_Rate_Get(iTE_u32 reg, HDMI_INFOR_STRU *pHdmiInfor)
{
	int sample_rate_index;
	U32 sample_rate;
	U32 total_sample_rate;

	sample_rate_index = (reg & ADO_CONF_SAMPLE_FREQ_MASK);
	DBG_Printf("sample_rate_index:%d\n\r", sample_rate_index);

	switch (sample_rate_index)
	{
	case ADO_CONF_SAMPLE_FREQ_32K:
		total_sample_rate = AudSample_Freq_32K;
		break;
	
	case ADO_CONF_SAMPLE_FREQ_44K:
		total_sample_rate = AudSample_Freq_44K;
		break;

	case ADO_CONF_SAMPLE_FREQ_48K:
		total_sample_rate = AudSample_Freq_48K;
		break;

	case ADO_CONF_SAMPLE_FREQ_64K:
		total_sample_rate = AudSample_Freq_64K;
		break;
		
	case ADO_CONF_SAMPLE_FREQ_88K:
		total_sample_rate = AudSample_Freq_88K;
		break;

	case ADO_CONF_SAMPLE_FREQ_96K:
		total_sample_rate = AudSample_Freq_96K;
		break;

	case ADO_CONF_SAMPLE_FREQ_128K:
		total_sample_rate = AudSample_Freq_128K;
		break;

	case ADO_CONF_SAMPLE_FREQ_176K:
		total_sample_rate = AudSample_Freq_176K;
		break;

	case ADO_CONF_SAMPLE_FREQ_192K:
		total_sample_rate = AudSample_Freq_192K;
		break;

	case ADO_CONF_SAMPLE_FREQ_256K:
		total_sample_rate = AudSample_Freq_256K;
		break;

	case ADO_CONF_SAMPLE_FREQ_352K:
		total_sample_rate = AudSample_Freq_352K;
		break;

	case ADO_CONF_SAMPLE_FREQ_384K:
		total_sample_rate = AudSample_Freq_324K;
		break;

	case ADO_CONF_SAMPLE_FREQ_512K:
		total_sample_rate = AudSample_Freq_512K;
		break;

	case ADO_CONF_SAMPLE_FREQ_705K:
		total_sample_rate = AudSample_Freq_705K;
		break;

	case ADO_CONF_SAMPLE_FREQ_768K:
		total_sample_rate = AudSample_Freq_768K;
		break;

	case ADO_CONF_SAMPLE_FREQ_1024K:
		total_sample_rate = AudSample_Freq_1024K;
		break;

	case ADO_CONF_SAMPLE_FREQ_1411K:
		total_sample_rate = AudSample_Freq_1411K;
		break;	

	case ADO_CONF_SAMPLE_FREQ_1536K:
		total_sample_rate = AudSample_Freq_1536K;
		break;		

	case ADO_CONF_SAMPLE_FREQ_ERROR:
		total_sample_rate = AudSample_Freq_err;
		break;

	default:
		total_sample_rate = AudSample_Freq_Unkown;
		break;
	}

	switch (pHdmiInfor->stream_type)
	{
	case AudType_LPCM:
		sample_rate = total_sample_rate;
		break;

	case AudType_NLPCM:
		if (pHdmiInfor->channel == 0)
		{
			sample_rate = AudSample_Freq_48K;
		}
		else
		{
			sample_rate = total_sample_rate/(pHdmiInfor->channel/2);
		}
		break;
	
	case AudType_HBR:
		sample_rate = total_sample_rate/(pHdmiInfor->channel/2);
		break;
	
	case AudType_DSD:
		asm("break 1, 1");
		break;

	default:
		break;
	}
	
	return sample_rate;
}

iTE_u8 SB_Audio_Stream_Type_Get(iTE_u32 parm)
{
	iTE_u8 ret;
	
	switch ((parm & ADO_CONF_TYPE_MASK))
	{
	case ADO_CONF_TYPE_LPCM:
		ret = AudType_LPCM;
		break;
	
	case ADO_CONF_TYPE_NLPCM:
		ret = AudType_NLPCM;
		break;

	case ADO_CONF_TYPE_HBR:
		ret = AudType_HBR;
		break;

	case ADO_CONF_TYPE_DSD:
		ret = AudType_DSD;
		break;

	default:
		ret = AudType_Unkown;
		break;
	}

	return ret;
}

iTE_u8 SB_Audio_Stream_Tx_Type_Get(iTE_u32 parm)
{
	iTE_u8 ret;

	switch ((parm & ADO_CONF_SEL_MASK))
	{
	case ADO_CONF_SEL_I2S:
		ret = AudSel_I2S;
		break;
	
	case ADO_CONF_SEL_SPDIF:
		ret = AudSel_SPDIF;
		break;

	default:
		ret = AudSel_Unkown;
		break;
	}

	return ret;
}

iTE_u8 SB_Audio_Stream_Channel_Get(iTE_u32 parm)
{
	iTE_u8 ret;

	switch ((parm & ADO_CONF_CH_MASK))
	{
	case ADO_CONF_CH_0:
		ret = AudCH_0;
		break;
	
	case ADO_CONF_CH_2:
		ret = AudCH_2;
		break;

	case ADO_CONF_CH_3:
		ret = AudCH_3;
		break;

	case ADO_CONF_CH_4:
		ret = AudCH_4;
		break;
		
	case ADO_CONF_CH_5:
		ret = AudCH_5;
		break;

	case ADO_CONF_CH_6:
		ret = AudCH_6;
		break;

	case ADO_CONF_CH_7:
		ret = AudCH_7;
		break;

	case ADO_CONF_CH_8:
		ret = AudCH_8;
		break;

	case ADO_CONF_CH_10:
		ret = AudCH_10;
		break;

	case ADO_CONF_CH_12:
		ret = AudCH_12;
		break;

	case ADO_CONF_CH_16:
		ret = AudCH_16;
		break;

	case ADO_CONF_CH_32:
		ret = AudCH_32;
		break;
			
	default:
		ret = AudCH_Unkown;
		break;
	}

	return ret;
}


//****************************************************************************
void SB_HostControl(HDMI_HOST_CMD_TYPE u8Cmd,iTE_u8 volume)
{
	iTE_Msg(("\n\n**** SB_HostControl %02X g_u8IteRdy=%d\n\n", u8Cmd, g_u8IteRdy));

	iTE_u8	u8I2cChg = 0;
	iTE_u8	u8Temp;
	SB_I2C_Read(I2C_SYS_CHANGE, 1, &u8I2cChg);
	switch(u8Cmd){
		case	CMD_POWER_STANDBY:
				iTE_Msg(("SB Standby\n"));
				u8I2cChg |= SB_Standby();
			break;
		case	CMD_POWER_ON:
				iTE_Msg(("SB Power ON\n"));
				g_u8SbRxSel &= I2C_HDMI_SELECT_MASK;
				g_u8SbRxSel |= g_u8SbRxSel << 4;
				SB_I2C_Write(I2C_SYS_RX_SEL, 1, &g_u8SbRxSel);
				SB_I2C_Read(I2C_SYS_TX_STA, 1, &u8Temp);
				if(u8Temp & 0x02){
					g_u8SbAudioMode = 0x39;
				}else{
					g_u8SbAudioMode = 0x01;
				}
				SB_I2C_Write(I2C_SYS_ADO_MODE, 1, &g_u8SbAudioMode);
				g_u8SbCecMode = SB_CEC_AUTO;
				u8Temp = 0x85;
				SB_I2C_Write(I2C_SYS_RX_MODE_0, 1, &u8Temp);
				SB_I2C_Write(I2C_SYS_RX_MODE_1, 1, &u8Temp);
				u8I2cChg |= 0x80;
			break;
		case	CMD_POWER_OFF:
				iTE_Msg(("SB Power OFF \n"));
				u8I2cChg |= SB_PowerOff();
			break;


		switch(u8Cmd){
			case	CMD_SET_VOLUME:					// SB Set Volume
					u8SbVol = volume;
					SB_I2C_Write(I2C_ADO_VOL, 1, &u8SbVol);
					u8I2cChg |= 0x02;
					iTE_Msg(("u8SbVol  %X  %d \n", u8SbVol, u8SbVol&0x7F));
				break;					
			case	CMD_MODE_HDMI_1:					// SB HDMI 1
					iTE_Msg(("HDMI 1 \n"));
					g_u8SbRxSel = 0x00 | 0x04;
					SB_I2C_Write(I2C_SYS_RX_SEL, 1, &g_u8SbRxSel);

					g_u8SbAudioMode = 0x39;
					SB_I2C_Write(I2C_SYS_ADO_MODE, 1, &g_u8SbAudioMode);
						
					u8I2cChg |= 0x80;
					if(1){
						iTE_u8	u8AdoSel = 0;
						SB_I2C_Write(I2C_ADO_SEL, 1, &u8AdoSel);
						u8I2cChg |= 0x02;
					}
				break;
			case	CMD_MODE_HDMI_2:					// SB HDMI 2
					iTE_Msg(("HDMI 2 \n"));
					g_u8SbRxSel = 0x11 | 0x04;

					g_u8SbAudioMode = 0x39;
					SB_I2C_Write(I2C_SYS_ADO_MODE, 1, &g_u8SbAudioMode);

					SB_I2C_Write(I2C_SYS_RX_SEL, 1, &g_u8SbRxSel);
					u8I2cChg |= 0x80;
					if(1){
						iTE_u8	u8AdoSel = 0;
						SB_I2C_Write(I2C_ADO_SEL, 1, &u8AdoSel);
						u8I2cChg |= 0x02;
					}
				break;
			case	CMD_MODE_HDMI_3:					// SB HDMI 3
					iTE_Msg(("HDMI 3 \n"));
					g_u8SbRxSel = 0x22 | 0x04;

					g_u8SbAudioMode = 0x39;
					SB_I2C_Write(I2C_SYS_ADO_MODE, 1, &g_u8SbAudioMode);
						
					SB_I2C_Write(I2C_SYS_RX_SEL, 1, &g_u8SbRxSel);
					u8I2cChg |= 0x80;
					if(1){
						iTE_u8	u8AdoSel = 0;
						SB_I2C_Write(I2C_ADO_SEL, 1, &u8AdoSel);
						u8I2cChg |= 0x02;
					}
				break;
			case	CMD_MODE_ARC_EARC:					// SB ARC
					g_u8SbAudioMode = 0xF9;
					SB_I2C_Write(I2C_SYS_ADO_MODE, 1, &g_u8SbAudioMode);
					u8I2cChg |= 0x80;
					if(1){
						iTE_u8	u8AdoSel = 1;
						SB_I2C_Write(I2C_ADO_SEL, 1, &u8AdoSel);
						u8I2cChg |= 0x02;
					}
					iTE_Msg(("ARC \n"));
				break;
			case	CMD_MODE_EXT_I2S_1:
					iTE_Msg(("CMD_MODE_EXT_I2S_1 \n"));
					if(1){
						iTE_u8	u8AdoSel = 3;
						SB_I2C_Write(I2C_ADO_SEL, 1, &u8AdoSel);
						u8I2cChg |= 0x02;
						g_u8SbAudioMode = 0xF1;
						SB_I2C_Write(I2C_SYS_ADO_MODE, 1, &g_u8SbAudioMode);
						u8I2cChg |= 0x80;
					}
			       break;
			case	CMD_MODE_EXT_SPDIF_1:
					iTE_Msg(("CMD_MODE_EXT_SPDIF_1 \n"));
					if(1){
						iTE_u8	u8AdoSel = 5;
						SB_I2C_Write(I2C_ADO_SEL, 1, &u8AdoSel);
						u8I2cChg |= 0x02;
						g_u8SbAudioMode = 0xF1;
						SB_I2C_Write(I2C_SYS_ADO_MODE, 1, &g_u8SbAudioMode);
						u8I2cChg |= 0x80;
					}						
					break;
			case	CMD_MODE_OTHER:
					iTE_Msg(("Other Mode,ITE Audio mute \n"));
					if(1){
						iTE_u8	u8AdoSel = 0x08;
						SB_I2C_Write(I2C_ADO_SEL, 1, &u8AdoSel);
						u8I2cChg |= 0x02;
						g_u8SbAudioMode = 0xF1;
						SB_I2C_Write(I2C_SYS_ADO_MODE, 1, &g_u8SbAudioMode);
						u8I2cChg |= 0x80;
					}

				break;
			default:
					iTE_Msg(("NoSupport\n"));
					I2C_Slave_Dump();
				break;
		}

		SB_SysI2cChange(u8I2cChg);
	}
}

//****************************************************************************

#endif //HDMI_ENABLE
