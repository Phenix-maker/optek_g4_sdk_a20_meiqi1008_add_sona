#include "os_config.h"

#include "c_def.h"
#include "debug.h"

#include "malloc.h"

#ifdef BTDM52_ENABLE

//#define MALLOC_RTOS

#ifdef MALLOC_RTOS
#define MALLOC_LOCK		tx_mutex_get( &mutex_malloc, TX_WAIT_FOREVER )
#define MALLOC_UNLOCK	tx_mutex_put( &mutex_malloc )
#else
#define MALLOC_LOCK
#define MALLOC_UNLOCK
#endif	//MALLOC_RTOS

//#define MALLOC_BUF_SIZE	0x10000		/*64k*/
//#define MALLOC_BUF_SIZE	(0x4000)		/*16k*/
//#define MALLOC_BUF_SIZE	(0x5000)		/*20k*/
//#define MALLOC_BUF_SIZE	(0x5600)		/*20k*/
//#define MALLOC_BUF_SIZE	(0x4800)			/*18k*/ 
#define MALLOC_BUF_SIZE	(0x3600+0x1000+0x500)
#define MALLOC_MIN_SIZE_SHT	0x6
#define MALLOC_MIN_SIZE	(0x01 << MALLOC_MIN_SIZE_SHT)
#define MALLOC_MAX_INDEX (MALLOC_BUF_SIZE/MALLOC_MIN_SIZE)

U8 malloc_buf[MALLOC_BUF_SIZE];// __attribute__ ((section (".malloc_bss")));

U16 malloc_index[MALLOC_MAX_INDEX];// __attribute__ ((section (".malloc_bss")));

U16 malloc_id;
U16 malloc_next_index;
U16 max_malloc_index_used;
U16 max_malloc_mem_used;

//void *otk_malloc(U32 size);__attribute__ ((section (".internal_ram_1_text")));
//void otk_free(void *ptr);__attribute__ ((section (".internal_ram_1_text")));

void bt_hci_mem_init_cb (void);


void malloc_init(void)
{
	memset(malloc_index,0,sizeof(malloc_index));
	malloc_id = 0x1;
	max_malloc_index_used = 0;
	max_malloc_mem_used = 0;
	malloc_next_index = 0;

	bt_hci_mem_init_cb ();

}

void *otk_malloc(U32 size)
{

	U32 free_block_size,free_start_index;
	U32 i;
	
	if (size == 0)
		return NULL;

	//DBG_Printf ("Malloc %d(S) ",size);

	MALLOC_LOCK;

	free_block_size = 0;

	for (i=malloc_next_index;i<MALLOC_MAX_INDEX;i++)
	{
		if (malloc_index[i] == 0x0) {
			if (free_block_size == 0)
				free_start_index = i;

			free_block_size += MALLOC_MIN_SIZE;
		}
		else {
			free_block_size = 0;
		}

		if (free_block_size >= size) {

			if (max_malloc_index_used < free_start_index) {
				max_malloc_index_used = free_start_index;
				max_malloc_mem_used = (free_start_index << MALLOC_MIN_SIZE_SHT) + size;
			}

			malloc_id++;
			if (malloc_id == 0)
				malloc_id = 0x01;

			//debug
			if (malloc_id == 0x013A) {
				asm ("NOP");
				//asm("break 1,1");
			}

			//malloc_next_index = free_start_index + (size + MALLOC_MIN_SIZE - 1) >> MALLOC_MIN_SIZE_SHT;
			malloc_next_index = free_start_index;
			for (i=free_start_index;i<MALLOC_MAX_INDEX,size>0;i++) {
				malloc_next_index++;
				malloc_index[i] = malloc_id;
				if (size > MALLOC_MIN_SIZE)
					size -= MALLOC_MIN_SIZE;
				else
					size = 0;
			}



			MALLOC_UNLOCK;
			//DBG_Printf ("0x%x(P)\n\r", &malloc_buf[free_start_index<<MALLOC_MIN_SIZE_SHT]);
			return (void *) &malloc_buf[free_start_index<<MALLOC_MIN_SIZE_SHT];

		}
	}

	free_block_size = 0;

	for (i=0;i<malloc_next_index;i++)
	{
		if (malloc_index[i] == 0x0) {
			if (free_block_size == 0)
				free_start_index = i;

			free_block_size += MALLOC_MIN_SIZE;
		}
		else {
			free_block_size = 0;
		}

		if (free_block_size >= size) {

			if (max_malloc_index_used < free_start_index) {
				max_malloc_index_used = free_start_index;
				max_malloc_mem_used = (free_start_index << MALLOC_MIN_SIZE_SHT) + size;
			}

			malloc_id++;
			if (malloc_id == 0)
				malloc_id = 0x01;

			//malloc_next_index = free_start_index + (size + MALLOC_MIN_SIZE - 1) >> MALLOC_MIN_SIZE_SHT;
			malloc_next_index = free_start_index;
			for (i=free_start_index;i<MALLOC_MAX_INDEX,size>0;i++) {
				malloc_next_index++;
				malloc_index[i] = malloc_id;
				if (size > MALLOC_MIN_SIZE)
					size -= MALLOC_MIN_SIZE;
				else
					size = 0;
			}



			MALLOC_UNLOCK;
			//DBG_Printf ("0x%x(P)\n\r", &malloc_buf[free_start_index<<MALLOC_MIN_SIZE_SHT]);
			return (void *) &malloc_buf[free_start_index<<MALLOC_MIN_SIZE_SHT];

		}



	}


	MALLOC_UNLOCK;
	DBG_Assert(FALSE);
	DBG_Printf("Mem alloc err, no enough mem\n\r");
	return NULL;


}

void otk_free(void *ptr)
{
	U32 i;
	U16 malloc_id_ptr;
	U32 d,start_index;

	if (ptr == NULL) {
		//DBG_Assert(FALSE);
		return;
	}

	//DBG_Printf ("Malloc free 0x%x\n\r",ptr);

	MALLOC_LOCK;

	d = (U32) ptr - (U32) malloc_buf;
	start_index = (d >> MALLOC_MIN_SIZE_SHT);

	if ( d != (start_index << MALLOC_MIN_SIZE_SHT) || start_index >= MALLOC_MAX_INDEX) {
		DBG_Assert(FALSE);
	}
	else {
		
		malloc_id_ptr = malloc_index[start_index];

		if (malloc_next_index > start_index)
			malloc_next_index = start_index;		//next malloc search start point

		for (i=start_index;i<MALLOC_MAX_INDEX;i++) {
			if (malloc_index[i] == malloc_id_ptr) {
				malloc_index[i] = 0x0;
			}
			else {
				break;
			}			
		}
	}	

	MALLOC_UNLOCK;
}

void malloc_check_clean(void)
{
	U32 i;
	for (i=0;i<MALLOC_MAX_INDEX;i++) {
		if (malloc_index[i] != 0x0) {
			DBG_Assert(FALSE);
		}
	}
}

void malloc_test (void)
{
	void *test_ptr_a;
	void *test_ptr_b;
	void *test_ptr_c;
	void *test_ptr_d;
	void *test_ptr_e;
	void *test_ptr_f;
	void *test_ptr_g;
	void *test_ptr_h;
	void *test_ptr_i;
	void *test_ptr_j;
	void *test_ptr_k;
	void *test_ptr_l;
	void *test_ptr_m;
	void *test_ptr_n;
	void *test_ptr_o;
	void *test_ptr_p;
	void *test_ptr_q;



	test_ptr_a = otk_malloc(0x25);

	malloc_next_index = 1022;

	test_ptr_b = otk_malloc(0x250);
	test_ptr_c = otk_malloc(0x78);
	test_ptr_d = otk_malloc(2352);
	test_ptr_e = otk_malloc(4);
	test_ptr_f = otk_malloc(8);

	otk_free(test_ptr_c);


	test_ptr_g = otk_malloc(2048);
	test_ptr_h = otk_malloc(1152*2);
	test_ptr_i = otk_malloc(83);
	test_ptr_j = otk_malloc(325);
	test_ptr_k = otk_malloc(876);
	test_ptr_l = otk_malloc(45);
	test_ptr_m = otk_malloc(87);
	test_ptr_n = otk_malloc(765);

	otk_free(test_ptr_d);
	otk_free(test_ptr_m);
	otk_free(test_ptr_g);

	test_ptr_o = otk_malloc(45);
	test_ptr_p = otk_malloc(587);
	test_ptr_q = otk_malloc(2765);

	otk_free(test_ptr_n);
	//otk_free(test_ptr_c);
	otk_free(test_ptr_h);
	otk_free(test_ptr_j);
	otk_free(test_ptr_a);
	otk_free(test_ptr_e);
	otk_free(test_ptr_k);
	otk_free(test_ptr_f);
	otk_free(test_ptr_b);
	otk_free(test_ptr_l);
	otk_free(test_ptr_i);

	otk_free(test_ptr_o);
	otk_free(test_ptr_p);
	otk_free(test_ptr_q);

	malloc_check_clean();
}

//check how many free space and print it
void malloc_check (void)
{
	u32 i;
	u32 free_space;

	free_space = 0;
	for (i=0;i<MALLOC_MAX_INDEX;i++)
	{
		if (malloc_index[i] == 0x0) {
			free_space += MALLOC_MIN_SIZE;
		}
	}

	DBG_Printf ("Malloc free space %d\n\r", free_space);
}

#endif