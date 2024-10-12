#ifndef _W_STRING_H_
#define _W_STRING_H_

#include "c_def.h"

U16 wstrlen ( U16  *buf );
void wstrcpy ( U16  *s, const U16 *d );
void wstrncpy ( U16  *s, const U16 *d, int n );
void strAddUnicodeNull (U8 *buf);
void strCutSpace (U8 *buf);
void wstrCutSpace( U16 *buf);
I16 _wstrcmp ( U16  *s1, U16 *s2 );
I16 wstrcmp ( U16 *s1, U16 *s2);
void strCutLeadingSpace( U8 *buf);

#endif //_W_STRING_H_
