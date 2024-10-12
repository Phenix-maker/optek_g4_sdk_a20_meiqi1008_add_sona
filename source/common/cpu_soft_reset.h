/*
 * cpu_soft_reset.h
 *
 *  Created on: 2024/1/29
 *      Author: Windows
 */

#ifndef __CPU_SOFT_RESET__
#define __CPU_SOFT_RESET__

extern U8 sram_boot_config[16];
extern const U8 flash_boot_config[16];

void CPU_SoftReset_Flag_Setting(U8 *pBuf, U8 reset);
void app_nav_upgrade_project(void);

#endif /* __CPU_SOFT_RESET__ */
