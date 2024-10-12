#include "os_config.h"

#include "regmap.h"

#include "mem_reloc.h"
#include "hw_timer.h"
#include "debug.h"
#include "hal_gpio.h"
#include "hal_key_adc.h"
#define TEST_SW_VERSION		0x23082701 //yy/mm/dd/s. no

#if 0
void hw_pwm_test(void)
{

    // hal_gpio_mux_func_select(PWM_TEST_PIN,MUX_SEL_FUNCTION1);
    DBG_Printf("pwm test\n\r");
    U32 val;
    U32 pwmA_cntl;
    U32 pwmB_cntl;
    PWM_CONTROLs *pPwmA_cntl;
    PWM_CONTROLs *pPwmB_cntl;
    
    REG_MISC_SW_VER = TEST_SW_VERSION;
    val = REG_MISC_CLKGATE;
    MISC_CLKGATE_CONTROLs *pGate = (MISC_CLKGATE_CONTROLs *)&val;
    pGate->u_pwm0_gate = 1;//open ir clk gate
    pGate->u_pwm1_gate = 1; //if needed
    REG_MISC_CLKGATE = val;
    
    pPwmA_cntl = REG_PWMA_CTL;
    pPwmB_cntl = REG_PWMB_CTL;

    pPwmA_cntl =&pwmA_cntl;
    pPwmB_cntl =&pwmB_cntl;


    pPwmA_cntl->div =2;
    pPwmB_cntl->div =2;

    REG_PWMA_PERIOD = 0xfff;
    REG_PWMA0_DUTY = 0xfff;     //0->disable,= period 100%
    REG_PWMA1_DUTY = 0x3ff;
    REG_PWMA2_DUTY = 0x1ff;
    REG_PWMA3_DUTY = 0xcff;
    
    REG_PWMB_PERIOD = 0xcff;
    REG_PWMB0_DUTY = 0x5ff;  //0->disable,=period 100%
    REG_PWMB1_DUTY = 0x4ff;
    REG_PWMB2_DUTY = 0x2ff;
    REG_PWMB3_DUTY = 0x1ff;
    pPwmA_cntl->pwm_ena = 1;    //after reset, pwm duty is , disable
    pPwmB_cntl->pwm_ena = 1;

  

    REG_PWMA_CTL = pwmA_cntl;
    REG_PWMB_CTL = pwmB_cntl;

    pPwmA_cntl->duty_update =1; 
    pPwmB_cntl->duty_update=1;

    REG_PWMA_CTL = pwmA_cntl;
    REG_PWMB_CTL = pwmB_cntl;
    delayms(10);

    REG_MISC_STATUS = 0x10;

    REG_PWMA0_DUTY = 0x7ff; 
    // REG_PWMA2_DUTY = 0x7ff;
    // // REG_PWMA3_DUTY = 0x0;
    // REG_PWMA3_DUTY = 0x1ff;
    

    // // REG_PWMB2_DUTY = 0x0;
    // REG_PWMB2_DUTY = 0x1ff;
    // REG_PWMB3_DUTY = 0x6ff;

    
    pPwmA_cntl->duty_update=1;
    pPwmB_cntl->duty_update=1;

    REG_PWMA_CTL = pwmA_cntl;
    REG_PWMB_CTL = pwmB_cntl;
    delayms(20);
    REG_MISC_STATUS = 0x20;


    // asm("waiti 1");
    // while(1);
}
#endif
void hw_pwm_init(void)
{
    hal_gpio_mux_func_select(I_CTL,MUX_SEL_FUNCTION1);
    DBG_Printf("hw pwm init\n\r");
    U32 val;
    U32 pwmA_cntl;
    U32 pwmB_cntl;
    PWM_CONTROLs *pPwmA_cntl;
    PWM_CONTROLs *pPwmB_cntl;
    
    REG_MISC_SW_VER = TEST_SW_VERSION;
    val = REG_MISC_CLKGATE;
    MISC_CLKGATE_CONTROLs *pGate = (MISC_CLKGATE_CONTROLs *)&val;
    pGate->u_pwm0_gate = 1;//open ir clk gate
    pGate->u_pwm1_gate = 1; //if needed
    REG_MISC_CLKGATE = val;
    
    pPwmA_cntl = REG_PWMA_CTL;
    pPwmB_cntl = REG_PWMB_CTL;

    pPwmA_cntl =&pwmA_cntl;
    pPwmB_cntl =&pwmB_cntl;


    pPwmA_cntl->div =2;
    pPwmB_cntl->div =2;

    REG_PWMA_PERIOD = 0xfff;
    REG_PWMA0_DUTY = 1;     //gpio 3.3v
    // REG_PWMA1_DUTY = 0x3ff;
    // REG_PWMA2_DUTY = 0x1ff;
    // REG_PWMA3_DUTY = 0xcff;
    // pPwmA_cntl->pwm_ena = 1;    //after reset, pwm duty is , disable
    // pPwmB_cntl->pwm_ena = 1;
}

void hw_pwm_set(U32 duty)
{
    DBG_Printf("hw pwm set\n\r");
    U32 pwmA_cntl;
    U32 pwmB_cntl;
    PWM_CONTROLs *pPwmA_cntl;
    PWM_CONTROLs *pPwmB_cntl;
    pPwmA_cntl =&pwmA_cntl;
    pPwmB_cntl =&pwmB_cntl;

    REG_PWMA_CTL = pwmA_cntl;
    REG_PWMB_CTL = pwmB_cntl;
    pPwmA_cntl->pwm_ena = 1;    //after reset, pwm duty is , disable
    pPwmB_cntl->pwm_ena = 1;

    pPwmA_cntl->duty_update =1; 
    pPwmB_cntl->duty_update=1;

    REG_PWMA_CTL = pwmA_cntl;
    REG_PWMB_CTL = pwmB_cntl;
    delayms(10);

    REG_MISC_STATUS = 0x10;

    REG_PWMA0_DUTY = duty; 
    // REG_PWMA2_DUTY = 0x7ff;
    // // REG_PWMA3_DUTY = 0x0;
    // REG_PWMA3_DUTY = 0x1ff;
    

    // // REG_PWMB2_DUTY = 0x0;
    // REG_PWMB2_DUTY = 0x1ff;
    // REG_PWMB3_DUTY = 0x6ff;

    
    pPwmA_cntl->duty_update=1;
    pPwmB_cntl->duty_update=1;

    REG_PWMA_CTL = pwmA_cntl;
    REG_PWMB_CTL = pwmB_cntl;
    delayms(20);
    REG_MISC_STATUS = 0x20;
}


