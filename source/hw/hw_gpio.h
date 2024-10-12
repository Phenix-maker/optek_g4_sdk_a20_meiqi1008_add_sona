#ifndef _HW_GPIO_H
#define _HW_GPIO_H


//#define ENABLE_SIMPLE_GPIO_CONTROL

#undef __inline__
#define __inline__


/*
 ****************************************************************************************
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
__inline__ void gpio0_SetOutput_level(U32 mask, U8 level);
__inline__ void gpio1_SetOutput_level(U32 mask, U8 level);

__inline__ U32 gpio0_input_level_read(U32 mask);
__inline__ U32 gpio1_input_level_read(U32 mask);
__inline__ U32 gpio0_group_input_level_read(void);

void gpio0_interrupt_set(U32 mask,U8 enable, U8 Trigger_type, U8 Trigger_level);
void gpio1_interrupt_set(U32 mask,U8 enable, U8 Trigger_type, U8 Trigger_level);

__inline__ U32 gpio0_interrupt_st_mask_read(U32 mask);
__inline__ U32 gpio1_interrupt_st_mask_read(U32 mask);

__inline__ U32 gpio0_interrupt_st_group_read(void);
__inline__ U32 gpio1_interrupt_st_group_read(void);

__inline__ void gpio0_MUX_select(U32 mask, U8 select);
__inline__ void gpio1_MUX_select(U32 mask, U8 select);



#endif //_HW_GPIO_H

