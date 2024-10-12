#include "os_type.h"


#ifdef RTOS_ENABLE


#include <xtensa/config/core.h>
#include "xtensa_context.h"


/*
 * Orginal
 *#define INT_DISABLE    XCHAL_EXCM_LEVEL  // Disable interrupts value 
*/
#define INT_DISABLE    3  /* Disable interrupts value */

#define INT_ENABLE     0x0               /* Enable interrupt value   */




    .text
/*    .section  .interrupt_control_text, "ax"*/

/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    _tx_interrupt_control                                               */ 
/*                                                           5.1          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*                                                                        */
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function is responsible for changing the interrupt lockout     */ 
/*    posture of the system.                                              */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    new_posture                           New interrupt lockout posture */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    old_posture                           Old interrupt lockout posture */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    Application Code                                                    */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  10-10-2006     Ross Morley              Initial Version 5.0           */ 
/*  04-02-2007     Ross Morley              Modified comment(s),          */ 
/*                                            resulting in version 5.1    */ 
/*                                                                        */ 
/**************************************************************************/ 
//  UINT   _tx_interrupt_control(unsigned long new_posture)
//  {

	.section  .internal_ram_1_text, "ax"

    .globl  _interrupt_control
    .type   _interrupt_control, @function
    .align  4
_interrupt_control:

    ENTRY0

    rsr     a3, PS                          /* old PS */
    #if PS_INTLEVEL_SHIFT != 0
    ssli    a2, a2, PS_INTLEVEL_SHIFT       /* param is new intlevel */
    #endif
    movi    a5, PS_INTLEVEL_MASK
    and     a4, a2, a5                      /* new INTLEVEL field of PS */
    and     a2, a3, a5                      /* old INTLEVEL field of PS */
    sub     a3, a3, a2                      /* remove old INTLEVEL field */
    add     a3, a3, a4                      /* insert new INTLEVEL field */
    wsr     a3, PS                          /* write it to PS */
    #if PS_INTLEVEL_SHIFT != 0
    srli    a2, a2, PS_INTLEVEL_SHIFT       /* return old intlevel */
    #endif
    rsync                                   /* wait till new level effective */

    RET0

//  }

#endif
