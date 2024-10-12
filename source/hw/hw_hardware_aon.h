#ifndef __HW_HARDWARE_AON_H__
#define __HW_HARDWARE_AON_H__


typedef enum {
    LDO_VOLTAGE_1P8V  = 0x16,
    LDO_VOLTAGE_2P5V  = 0x1A,
    LDO_VOLTAGE_3P3V  = 0x1F,
} LDO_VOLTAGE_SEL;


//TRIM_32K_PARAM (0 ~ 63)
//Please use the value of 58 for OTK6266D_BOARD, otherwise the system will crash!
//#define TRIM_32K_PARAM      58
//#define TRIM_32K_PARAM      50
//#define TRIM_32K_PARAM      40
//#define TRIM_32K_PARAM      39
//#define TRIM_32K_PARAM      38
//#define TRIM_32K_PARAM      35

#define TRIM_32K_PARAM      38


void hw_ldo_init(LDO_VOLTAGE_SEL value);
void hw_dcdc_voltage_set(u8 vsel_val);

#endif //__HW_HARDWARE_AON_H__
