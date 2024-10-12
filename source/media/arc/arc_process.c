#include "os_config.h"

#ifdef ARC_ENABLE

#include "regmap.h"

#include "mem_reloc.h"

#include "hw_timer.h"
#include "hal_gpio.h"

#include "interrupt.h"
#include "fifo.h"

#include "arc_detect.h"
#include "arc_process.h"
#include "app_i2c.h"
#include "optek_cec.h"


#undef ARC_CEC_I2C_ADDR
#define ARC_CEC_I2C_ADDR        0xA0


U8 arc_cec_connect_status;

U8 arc_cec_process_status;

extern U8 cec_physical_addr[2];
extern FIFO cec_rx_fifo;


void arc_cec_i2c_read(U32 parm)
{
	int ret;
	
	DBG_Printf("%s\n\r", __func__);
	
	cec_i2c_init();
	fifo_discard_all_data(&cec_rx_fifo);

	cec_read_physcial_addr_status = CEC_READ_PHYSIC_ADDR;
	cec_i2c_read_and_report_addr();
}

void cec_i2c_read_and_report_addr(void)
{
	int ret;
		
	DBG_Printf("%s\n\r", __func__);

	#ifdef HDMI_I2C_DEBUG
	ret = CEC_I2C_read_sub(ARC_CEC_I2C_ADDR, 0, (U8 *)hdmi_i2c_buf, 256);
	#else
	ret = CEC_I2C_read_sub(ARC_CEC_I2C_ADDR, 0, NULL, 256);
	#endif

	if (ret == I2C_SUCCESS)
	{
		DBG_Puts("CEC I2C Read Ok\n\r");
		
		#if 0
		if (!app_main_data.standby_status)
		{
			app_nav_rmt_convert_ARC();
		}
		#endif
	}
	else
	{
		DBG_Puts("CEC I2C Read err\n\r");
	}
}

#endif //ARC_ENABLE
