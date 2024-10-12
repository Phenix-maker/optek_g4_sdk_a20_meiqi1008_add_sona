#ifndef _MALLOC_H
#define _MALLOC_H

void malloc_init(void);
void *otk_malloc(U32 size);
void otk_free(void *ptr);

void malloc_test (void);
void malloc_check (void);


#endif

