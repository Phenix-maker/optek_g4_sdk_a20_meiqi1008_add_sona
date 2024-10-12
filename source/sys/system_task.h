#ifndef _SYSTEM_TASK_H_
#define _SYSTEM_TASK_H_

typedef struct {
	U32 parm;
} system_msg_stru;


typedef struct {
    U64 tick;
    QueueHandle_t task_queue;
    TaskHandle_t pxCreatedTask;
} SYSTEM_TASK_ENV_STRU;

void system_task_init(void);
void system_msgSend ( U32 parm );

#endif
