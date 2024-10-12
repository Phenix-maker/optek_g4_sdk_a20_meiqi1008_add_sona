/**
 ****************************************************************************************
 *
 * @file os_entry.c
 *
 * @brief 
 *
 ****************************************************************************************
 */

#include "c_def.h"
#include "debug.h"
#include "oem.h"

#include "os_config.h"

#include "console_task.h"
#include "system_task.h"
#include "system_timer.h"
#include "app_task.h"
#include "idle_task.h"
#include "audio_task.h"
#include "flash_boot.h"
#include "hw_watchdog.h"

#ifdef BTDM52_ENABLE
#include "btdm_task.h"
#include "host_task.h"
#endif

#ifdef RTOS_ENABLE

/**
 ****************************************************************************************
 * @brief task initialize and freertos kernel scheduler entry
 *
 * @return will not return
 *
 ****************************************************************************************
 */
void os_entry(void)
{
	/* Event creating */
#if (defined HDMI_ENABLE || defined OPTICAL_ENABLE || defined COAXIAL_ENABLE || defined ARC_ENABLE || defined AUX_ENABLE)
	audio_input_rx_task_init();
#endif

#ifdef BTDM52_ENABLE
	btdm_task_init();
	host_task_init();
#endif

	audio_task_init();

	system_task_init();
	app_task_init();

	#if ((defined USB_HOST_ENABLE)||(defined USB_DEV_ENABLE))
	task_usbotg_init();
	#endif
	#ifdef USB_HOST_ENABLE
	task_hdd_init();
	#endif

	idle_task_init();

	#if ((defined WATCH_DOG) && !(defined JTAG_DEBUG_MODE))
	hw_wdt_enable(WDT_TIME_TO_S(7));
	#endif

	/* Finally start the scheduler. */
	vTaskStartScheduler();

	/* Will only reach here if there is insufficient heap available to start
	the scheduler. */
	printf_fatal("scheduler start fail, please check heap size\r\n");
	DBG_Assert(FALSE);
	while (1)
	{
		asm("nop");
	}
}
#endif
