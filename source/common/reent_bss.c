/*
 * Copyright (c) 2009-2018 Cadence Design Systems, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/* Default copy of _reent struct for XCLIB, plus global pointer to it.
   This will take care of cases where the library is used with a single
   thread. Can be overridden by OS if needed.
*/

#include <sys/reent.h>

#include "mem_reloc.h"


void _init_reent_bss(void) __FLASH_BOOT_TEXT;


#pragma weak g_reent
#pragma weak _reent_ptr


struct _reent g_reent  __attribute__ ((section (".clib.percpu.bss")));
struct _reent * _reent_ptr    __attribute__ ((section (".clib.percpu.bss")));


/*
 * _init_reent_bss() is called internally by the library to init the
 *   single global copy of state for single-threaded executables. This
 *   assumes that the structure is allocated in BSS and already zeroed.
 */
void _init_reent_bss(void)
{
  memset (&g_reent, 0, sizeof(g_reent) );

  _reent_ptr = &g_reent;

  _reent_ptr->_Randseed = 1;
  _reent_ptr->_strtok_p = "";
}

