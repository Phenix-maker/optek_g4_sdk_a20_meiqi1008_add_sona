#include "c_def.h"
#include "oem.h"

#include "string.h"
#include "wstring.h"

#if 0
U16 wstrlen ( U16  *buf )
{
	U16 len = 0;

	while (1) {
		if ( *buf++ == 0x0000) {
			break;
		}
		else {
			len++;
		}
	}

	return len;
}

void wstrcpy ( U16  *d, const U16 *s )
{
	while (*s != 0) {
		*d++ = *s++;
	}

	*d = 0x0;				/*null*/
}

void wstrncpy ( U16  *d, const U16 *s, int n )
{
	int i;

	for (i = 0; i<n; i++) {
		if(*s == 0) break;
		*d++ = *s++;
	}

	*d = 0x0;				/*null*/
}

void strAddUnicodeNull (U8 *buf)
{
	int len;

	len = strlen ( (I8 *) buf);

	if (len & 0x01) {
		buf[len++] = 0x0;
		buf[len++] = 0x0;
		buf[len] = 0x0;
	}
	else {
		buf[len++] = 0x0;
		buf[len] = 0x0;
	}
}

void strCutSpace (U8 *buf)
{
	int len,i;
	U8 *pBuf;

	len = strlen ( (I8 *) buf);

	pBuf = buf + len -1;

	for (i=0; i < len; i++) {
		if ( *pBuf == 0x20 ) {
			*pBuf-- = 0;
		}
		else {
			break;
		}
	}
}

void wstrCutSpace( U16 *buf)
{
	int i, len;
	U16 *pBuf;

	len = wstrlen (buf);

	pBuf = buf + len -1;

	for (i=0; i < len; i++) {
		if ( (*pBuf == 0x0020) || (*pBuf == 0x2000) ) {
			*pBuf-- = 0;
		}
		else {
			break;
		}
	}
}

I16 wstrcmp ( U16 *s1, U16 *s2)
{
	I16 len,i;
	I16 retVal;

	len = wstrlen (s1);
	if (len < wstrlen (s2) ) {
		len = wstrlen (s2);
	}

	if (len == 0)
	{
		retVal = -1;
	}
	else
	{
		retVal = 0;
	}

	for (i=0; i<len; i++) {
		
		if (*s1 == 0x0) {
			retVal = -1;
			break;
		}

		if (*s2 == 0x0) {
			retVal = 1;
			break;
		}
		
		if (*s1 < *s2) {
			
			retVal = -1;
			break;		
		
		}

		if (*s1++ > *s2++) {
			
			retVal = 1;
			break;				
		}
	}

	return retVal;
}

I16 _wstrcmp ( U16  *s1, U16 *s2 )
{
	U16 len,i;
	I16 retVal;
	U16 cmps1,cmps2;

	len = wstrlen (s1);
	if (len < wstrlen (s2) ) {
		len = wstrlen (s2);
	}

	if (len == 0)
	{
		retVal = -1;
	}
	else
	{
		retVal = 0;
	}


	for (i=0; i<len; i++) {
		
		cmps1 = s1[i];
		cmps2 = s2[i];

		if ( (cmps1 & 0xFF00) != 0x0 ) {
			/*Unicode, the fisrt byte is char for 8 bit char*/
			
			if ( (cmps1 >= 0x4100) && (cmps1 <= 0x5A00) ) {
				/*upper chars, change to lower chars*/
				cmps1 += 0x2000;
			}
		}
		
		if ( (cmps1 & 0x00FF) != 0x0) {
			/*Unicode, the second byte is char for 8 bit char*/
		
			if ( ((cmps1 & 0x00FF) >= 0x41) && ((cmps1 & 0x00FF) <= 0x5A) ) {
				/*upper chars, change to lower chars*/
				cmps1 += 0x20;
			}
		}

		if ( (cmps2 & 0xFF00) != 0x0 ) {
			/*Unicode, the fisrt byte is char for 8 bit char*/
			
			if ( (cmps2 >= 0x4100) && (cmps2 <= 0x5A00) ) {
				/*upper chars, change to lower chars*/
				cmps2 += 0x2000;
			}
		}
		
		if ( (cmps2 & 0x00FF) != 0x0) {
			/*Unicode, the second byte is char for 8 bit char*/
		
			if ( ((cmps2 & 0x00FF) >= 0x41) && ((cmps2 & 0x00FF) <= 0x5A) ) {
				/*upper chars, change to lower chars*/
				cmps2 += 0x20;
			}
		}
		
		if (cmps1 == 0x0) {
			retVal = -1;
			break;
		}
		
		if (cmps2 == 0x0) {
			retVal = 1;
			break;
		}

		if (cmps1 < cmps2) {
			
			retVal = -1;
			break;		
		}

		if (cmps1 > cmps2) {
			
			retVal = 1;
			break;				
		}
	}

	if (retVal != 0) {
		return retVal;
	}
	else {
		return ( wstrcmp (s1, s2) );
	}
}

/************************************************************************************************
 *	@ WORD wUpper (WORD cWord )																	*
 *		convert all the characters a-z to Upper characters A-Z									*
 *	input:																						*
 *		the character needed to be changed														*
 *	output:																						*
 *		the character changed to Upper,                											*
 ************************************************************************************************/	


BYTE Upper (BYTE cByte)
{
	if (cByte >= 'a' && cByte <= 'z')
		cByte -= 0x20;
	return cByte;
}



I16 ustrcmp ( BYTE *s1, BYTE *s2)
{
	I16 len,i;
	I16 retVal;
	BYTE w1, w2;

	len = strlen ((const char *) s1);
	if (len < strlen ((const char *) s2) ) {
		len = strlen ((const char *) s2);
	}

	if (len == 0)
	{
		retVal = -1;
	}
	else
	{
		retVal = 0;
	}


	for (i=0; i<len; i++) {
		
		w1= Upper (*s1++);
		w2 = Upper(*s2++);

		if (w1 == 0x0) {
			retVal = -1;
			break;
		}
		
		if (w1 < w2) {
			
			retVal = -1;
			break;		
		}

		if (w1 > w2) {
			
			retVal = 1;
			break;				
		}
	}

	return retVal;
}

/************************************************************************************************
 *	@ WORD wUpper (WORD cWord )																	*
 *		convert all the characters a-z to Upper characters A-Z									*
 *	input:																						*
 *		the character needed to be changed														*
 *	output:																						*
 *		the character changed to Upper,                											*
 ************************************************************************************************/	

WORD wUpper (WORD cWord)
{
	if (cWord >= (WORD) 'a' && cWord <= (WORD) 'z') {
			cWord -= 0x20;	
	}
	
	return cWord;
}


I16 uwstrcmp ( U16 *s1, U16 *s2)
{
	I16 len,i;
	I16 retVal;
	U16	w1, w2;

	len = wstrlen (s1);
	if (len < wstrlen (s2) ) {
		len = wstrlen (s2);
	}

	if (len == 0)
	{
		retVal = -1;
	}
	else
	{
		retVal = 0;
	}

	for (i=0; i<len; i++) {

		w1= wUpper (*s1++);
		w2 = wUpper(*s2++);

		if (w1 == 0x0) {
			retVal = -1;
			break;
		}

		if (w1 < w2) {

			retVal = -1;
			break;
		}

		if (w1 > w2) {

			retVal = 1;
			break;
		}
	}

	return retVal;
}


void wstrCutEmbededSpace( U16 *buf)
{
	int i, len;
	U16 *pBuf;

	len = wstrlen (buf);

	pBuf = buf;;

	for (i=0; i < len; i++) {
		if ( buf[i] != 0x0020 ) {
			*pBuf++ = buf[i];
		}
	}

	*pBuf = 0x0;
}

void strCutTrailingSpace( U8 *buf)
{
	int i, len;
	U8 *pBuf;

	len = strlen ((const char *) buf);

	pBuf = buf + len -1;

	for (i=0; i < len; i++) {
		if ( (*pBuf == 0x20) ) {
			*pBuf-- = 0;
		}
		else {
			break;
		}
	}
}


void wstrCutLeadingSpace( U16 *buf)
{
	U16 i, len, s;

	len = wstrlen (buf);

	//cacaulte how many space words in the leading 
	s = len;
	for (i=0; i<len; i++) {
		if ( (buf[i] != 0x0020) ) {
			s = i;
			break;
		}
	}

	if (i==0) {
		//no leading space
		return;
	}
	else if (s == len) {
		//all space in the string
		*buf = 0x00;
		return;
	}
	else {
	
		for (i=0; i< (len - s) + 1; i++) {		//including NULL
			buf[i] = buf[i+s];
		}
	}
}

void wstrCutTrailingSpace( U16 *buf)
{
	int i, len;
	U16 *pBuf;
	
	len = wstrlen (buf);
	pBuf = buf + len -1;

	for (i=0; i < len; i++) {
		if ( (*pBuf == 0x0020) ) {
			*pBuf-- = 0;
		}
		else {
			break;
		}
	}
}

void wstrCutEmbededZero( U16 *buf)
{
	int i, len;
	U8 *pChar;

	len = wstrlen (buf);

	pChar = (U8 *) buf;;

	for (i=0; i < len; i++) {
		if ((buf[i] & 0xFF00) != 0x0) {
			*pChar++ = (U8) ((buf[i] & 0xFF00) >> 8);
		}
		if ((buf[i] & 0x00FF) != 0x0) {
			*pChar++ = (U8) (buf[i] & 0x00FF);
		}
	}

	*pChar = 0x0;
}

void wstrCutLeadingPeriod( U16 *buf)
{
	U16 i, len, s;

	len = wstrlen (buf);

	//cacaulte how many space words in the leading 
	for (i=0; i<len; i++) {
		if ( (buf[i] != 0x002E) ) {
			s = i;
			break;
		}
	}

	if (i==0) {
		//no leading space
		return;
	}
	else if (s == len) {
		//all space in the string
		*buf = 0x00;
		return;
	}
	else {
	
		for (i=0; i< (len - s) + 1; i++) {		//including NULL
			buf[i] = buf[i+s];
		}
	}
}

void wstrCutTrailingPeriod( U16 *buf)
{
	int i, len;
	U16 *pBuf;

	len = wstrlen (buf);

	pBuf = buf + len -1;

	for (i=0; i < len; i++) {
		if ( (*pBuf == 0x002E) ) {
			*pBuf-- = 0;
		}
		else {
			break;
		}
	}
}

void strCutLeadingSpace( U8 *buf)
{
	U16 i, len, s;

	len = strlen ( (const char *) buf);


	//cacaulte how many space words in the leading
	s = len;
	for (i=0; i<len; i++) {
		if ( (buf[i] != 0x20) ) {
			s = i;
			break;
		}
	}

	if (i==0) {
		//no leading space
		return;
	}
	else if (s == len) {
		//all space in the string
		*buf = 0x00;
		return;
	}
	else {
	
		for (i=0; i< (len - s) + 1; i++) {		//including NULL
			buf[i] = buf[i+s];
		}
	}

}
#endif
