#include "os_config.h"

#include "mem_reloc.h"

#include "hw_gpio.h"
#include "hw_timer.h"
#include "hw_hardware_aon.h"


#define WTIME 20

#define AON_POWER_OFF_ON_TEST
#define WTD_TEST
//#define AON_RET_MEM_TEST


void hw_hardware_aonreg_write(U32 *addr, U32 val) __attribute__ ((section (".flash_boot_text")));
void hw_wdt_disable(void) __attribute__ ((section (".flash_boot_text")));

void hw_hardware_aonreg_write (U32 *addr, U32 val)
{
	U32 addr_val;
	addr_val = (U32) addr;
	addr_val = AON_TRANS_ID | ((addr_val >> 2) & 0xFF);

	//disable int
	*addr = val;
	AON_WRITE_TRANS = addr_val;	//AON_TRANS_ID | ((addr_val >> 2) & 0xFF);
	//enable int
}



void hw_32k_trim_set(U8 trim)
{
	volatile U32 val;
	AON_CTRL_ST *reg_val_cfg = (AON_CTRL_ST *)&val;

	val = AON_CTRL_ADDR;
	reg_val_cfg->rc_32k_trim = trim;
	hw_hardware_aonreg_write(&AON_CTRL_ADDR, val);
}

void hw_dcdc_voltage_set(u8 vsel_val)
{
	U32 val;
	AON_CTRL2_ST *reg2_val_cfg = (AON_CTRL2_ST *)&val;

	val = AON_CTRL2_ADDR;
	reg2_val_cfg->core_dcdc_vsel = vsel_val;
	hw_hardware_aonreg_write(&AON_CTRL2_ADDR, val);
	
	val = AON_CTRL2_ADDR;
	printf_info("dcdc voltage set: %d\r\n", reg2_val_cfg->core_dcdc_vsel);

}

void hw_ldo_init(LDO_VOLTAGE_SEL value)
{
	U32 val;
	AON_CTRL2_ST *reg2_val_cfg = (AON_CTRL2_ST *)&val;

	val = AON_CTRL2_ADDR;
	reg2_val_cfg->ldo_2p5v_vsel = value;
	hw_hardware_aonreg_write(&AON_CTRL2_ADDR, val);
}


/*
* 00000 -> 0.58V
* 00001 -> 0.60V
* 00010 -> 0.65V
* 00011 -> 0.70V
* step V0.05V
* 01111 -> 1.30V
* 11111 -> 0.62V
* */

void hw_aon_ctrl3_st_init(void)
{
	U32 val;
	AON_CTRL3_ST *reg3_val_cfg = (AON_CTRL3_ST *)&val;

	val = AON_CTRL3_ADDR;
	DBG_Printf("%s,0x%x\n\r",__func__,val);
	reg3_val_cfg->qLR_08_VSEL = 3;
	hw_hardware_aonreg_write(&AON_CTRL3_ADDR, val);
}
