#ifndef _SYSTEM_TIMER_H_
#define _SYSTEM_TIMER_H_


/*
 ****************************************************************************************
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
/**
 * @struct TIMER_DATA
 * @brief Consisting of timer information
 */
typedef struct {
	U16 remoteKeyUpTimer;
	//for enable remote interrupt
	U16 remoteIntEanbleTimer;
	U8 rmtFrameEndTimer;

} TIMER_DATA;

extern TIMER_DATA timer_data;


/**
 * @struct SYSTEM_TIMER_ENV_STRU
 * @brief Consisting of system timer information
 */
typedef struct {
    U32 timer_tick;
    TimerHandle_t handler;
} SYSTEM_TIMER_ENV_STRU;

extern SYSTEM_TIMER_ENV_STRU system_timer_env;


/*
 ****************************************************************************************
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
void system_timer_init(void);
U32 system_timer_get(void);

#endif //_SYSTEM_TIMER_H_
