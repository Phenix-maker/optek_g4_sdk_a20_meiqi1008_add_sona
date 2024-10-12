#ifndef __ARC_DETECT_H__
#define __ARC_DETECT_H__

/*
 * ARC detect pin assignment
 * GPIO0_31_INDEX -> for OTK6269T test board v1.0
 * GPIO0_17_INDEX -> for OTK6266D test board v1.0
 * GPIO0_19_INDEX -> for OTK626x Series of SoC Evoluotion Board v2.0
 */
#define ARC_DETECT_PIN          GPIO0_19_INDEX

#define ARC_DETECT_READ         hal_gpio_input_level_read(ARC_DETECT_PIN)


typedef enum {
 enARC_PLUG   = 0,
 enARC_UNPLUG = 1,
} enARC_INSERT_STATUS;

extern volatile U8 arc_detect_status;

void arc_dectect_pin_init(void);
void arc_dectect_status_init(void);
void arc_detect_plug(int param);
void arc_detect_unplug(int param);
void arc_dectect_polling(void);


#endif //__ARC_DETECT_H__

