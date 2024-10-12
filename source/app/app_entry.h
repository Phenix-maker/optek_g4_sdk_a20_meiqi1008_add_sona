#ifndef _APP_ENTRY_H_
#define _APP_ENTRY_H_


void app_PreTaskSch_init(void);
void app_PostTaskSch_init(void);

const char *app_version_get(void);
const char *app_date_get(void);
const char *app_time_get(void);

#endif //_APP_ENTRY_H_
