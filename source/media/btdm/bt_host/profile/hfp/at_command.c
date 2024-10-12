#include "os_config.h"

#include "c_def.h"
#include "debug.h"
#include "oem.h"

#include "string.h"
#include "at_command.h"


#ifdef BTDM52_ENABLE

#define HFP_WIDEBAND

//HSP
const u8 AT_OK[]={"OK"};
const u8 AT_ERROR[]={"ERROR"};
const u8 AT_HSP_BUTT[] = {"AT+CKPD=200"};
const u8 VGS[] = {"+VGS="};
const u8 VGM[] = {"+VGM="};
const u8 VGS1[] = {"+VGS:"};
const u8 VGM1[] = {"+VGM:"};
const u8 AT_VGS[] = {"AT+VGS="};
const u8 AT_VGM[] = {"AT+VGM="};
const u8 AT_BLDN[] = {"AT+BLDN"};
const u8 AT_BCC[] = {"AT+BCC"};

//HFP
#ifndef HFP_WIDEBAND
const u8 AT_BRSF[] = {"AT+BRSF=20"};			//feat 0x15
const u8 AT_BAC[] = {"AT+BAC=1"};				//only CVSD
#else
const u8 AT_BRSF[] = {"AT+BRSF=148"};			//support wideband
const u8 AT_BAC[] = {"AT+BAC=1,2"};				//CVSD and mSBC
#endif	//HFP_WIDEBAND

const u8 AT_BCS[] = {"AT+BCS="};
const u8 BCS[] = {"+BCS:"};

const u8 BRSF[] = {"+BRSF:"};


const u8 AT_CIND_1[] = {"AT+CIND=?"};
const u8 AT_CIND_2[] = {"AT+CIND?"};
const u8 CIND[] = {"+CIND:"};
const u8 CIEV[] = {"+CIEV:"};

const u8 AT_CMER[] = {"AT+CMER=3,0,0,1"};

const u8 ATA[] = {"ATA"};
const u8 AT_CHUP[] = {"AT+CHUP"};
//const u8 AT_CHUP[] = {"AT+CH"};
const u8 AT_BVRA_ACTI[] = {"AT+BVRA=1"};
const u8 AT_BVRA_DEACTI[] = {"AT+BVRA=0"};

//AT+BTRH=1
const u8 AT_BTRH_1[] = {"AT+BTRH=1"};
const u8 AT_BTRH_0[] = {"AT+BTRH=0"};

const u8 AT_NREC[] = {"AT+NREC=0"};
const u8 AT_CLIP[] = {"AT+CLIP=1"};
const u8 CLIP[] = {"+CLIP:"};
const u8 RING[]={"RING"};


const u8 CIND_service[] = {"service"};
const u8 CIND_call[] = {"call"};
const u8 CIND_callsetup[] = {"callsetup"};
const u8 CIND_battchg[] = {"battchg"};
const u8 CIND_signal[] = {"signal"};
const u8 CIND_roam[] = {"roam"};
const u8 CIND_callheld[] = {"callheld"};

const u8 AT_XAPL[] = {"AT+XAPL=OPTK-1010-0100,10"};
const u8 AT_IPHONEACCEV[] = {"AT+IPHONEACCEV=1,1,"};

const u8 AT_XAPL_RESP[] = {"+XAPL=iPhone"};


/* We can't use the standard versions because of relocation headaches. */
#define isxdigit(c)	(('0' <= (c) && (c) <= '9') \
			 || ('a' <= (c) && (c) <= 'f') \
|| ('A' <= (c) && (c) <= 'F'))

#define isdigit(c)	('0' <= (c) && (c) <= '9')
#define islower(c)	('a' <= (c) && (c) <= 'z')
#define toupper(c)	(islower(c) ? ((c) - 'a' + 'A') : (c))


#define MAX_AT_COMMAND_LEN	255


//no embdded 0x0d 0x0a
//upper s and compare wirh d
int mem_upper_cmp (char *s,char *d, int size)
{
	int i;
	for (i=0;i++;i<size) {
		if (toupper(*s++) != *d++) {
			return -1;
		}
	}

	return 0;
}


u8 at_send_BRSF (u8 *buf)
{
	DBG_Assert((sizeof(AT_BRSF)-1) + 1 <= MAX_AT_COMAND_BUF);

	CFasm_memcpy (&buf[0],AT_BRSF,(sizeof(AT_BRSF)-1));
	buf[(sizeof(AT_BRSF)-1)] = 0x0d;
	
	return (1+(sizeof(AT_BRSF)-1));
}

/*Last call*/
u8 at_send_BLDN (u8 *buf)
{
	DBG_Assert((sizeof(AT_BLDN)-1) + 1 <= MAX_AT_COMAND_BUF);
	
	CFasm_memcpy (&buf[0],AT_BLDN,(sizeof(AT_BLDN)-1));
	buf[(sizeof(AT_BLDN)-1)] = 0x0d;
	
	return (1+(sizeof(AT_BLDN)-1));
}

//audio connect by HF
u8 at_send_BCC (u8 *buf)
{
	DBG_Assert((sizeof(AT_BCC)-1) + 1 <= MAX_AT_COMAND_BUF);
	
	CFasm_memcpy (&buf[0],AT_BCC,(sizeof(AT_BCC)-1));
	buf[(sizeof(AT_BCC)-1)] = 0x0d;
	
	return (1+(sizeof(AT_BCC)-1));
}

u8 at_send_BAC (u8 *buf)
{
	DBG_Assert((sizeof(AT_BAC)-1) + 1 <= MAX_AT_COMAND_BUF);
	
	CFasm_memcpy (&buf[0],AT_BAC,(sizeof(AT_BAC)-1));
	buf[(sizeof(AT_BAC)-1)] = 0x0D;
	
	DBG_Printf ("CODEC_NEGOTIATION.....................\n\r");	
	return (1+(sizeof(AT_BAC)-1));
}

u8 at_send_CIND_1 (u8 *buf)
{
	DBG_Assert((sizeof(AT_CIND_1)-1) + 1 <= MAX_AT_COMAND_BUF);
	
	CFasm_memcpy (&buf[0],AT_CIND_1,(sizeof(AT_CIND_1)-1));
	buf[(sizeof(AT_CIND_1)-1)] = 0x0D;
	
	
	return (1+(sizeof(AT_CIND_1)-1));
}

u8 at_send_CIND_2 (u8 *buf)
{
	DBG_Assert((sizeof(AT_CIND_2)-1) + 1 <= MAX_AT_COMAND_BUF);
	
	CFasm_memcpy (&buf[0],AT_CIND_2,(sizeof(AT_CIND_2)-1));
	buf[(sizeof(AT_CIND_2)-1)] = 0x0D;
	
	
	return (1+(sizeof(AT_CIND_2)-1));
}

u8 at_send_CMER (u8 *buf)
{
	DBG_Assert((sizeof(AT_CMER)-1) + 1 <= MAX_AT_COMAND_BUF);
	
	CFasm_memcpy (&buf[0],AT_CMER,(sizeof(AT_CMER)-1));
	buf[(sizeof(AT_CMER)-1)] = 0x0D;
	
	
	return (1+(sizeof(AT_CMER)-1));
}

u8 at_send_XAPL (u8 *buf)
{
	DBG_Assert((sizeof(AT_XAPL)-1) + 1 <= MAX_AT_COMAND_BUF);
	
	CFasm_memcpy (&buf[0],AT_XAPL,(sizeof(AT_XAPL)-1));
	buf[(sizeof(AT_XAPL)-1)] = 0x0D;
	
	
	return (1+(sizeof(AT_XAPL)-1));
}


u8 at_send_ATA (u8 *buf)
{
	DBG_Assert((sizeof(ATA)-1) + 1 <= MAX_AT_COMAND_BUF);
	
	CFasm_memcpy (&buf[0],ATA,(sizeof(ATA)-1));
	buf[(sizeof(ATA)-1)] = 0x0D;
	
	
	return (1+(sizeof(ATA)-1));
}

u8 at_send_CHUP (u8 *buf)
{
	DBG_Assert((sizeof(AT_CHUP)-1) + 1 <= MAX_AT_COMAND_BUF);
	
	CFasm_memcpy (&buf[0],AT_CHUP,(sizeof(AT_CHUP)-1));
	buf[(sizeof(AT_CHUP)-1)] = 0x0D;
	
	
	return (1+(sizeof(AT_CHUP)-1));
}

u8 at_send_NREC (u8 *buf)
{
	DBG_Assert((sizeof(AT_NREC)-1) + 1 <= MAX_AT_COMAND_BUF);
	
	CFasm_memcpy (&buf[0],AT_NREC,(sizeof(AT_NREC)-1));
	buf[(sizeof(AT_NREC)-1)] = 0x0D;
	
	
	return (1+(sizeof(AT_NREC)-1));
}

u8 at_send_BTRH_1 (u8 *buf)
{
	DBG_Assert((sizeof(AT_BTRH_1)-1) + 1 <= MAX_AT_COMAND_BUF);
	
	CFasm_memcpy (&buf[0],AT_BTRH_1,(sizeof(AT_BTRH_1)-1));
	buf[(sizeof(AT_BTRH_1)-1)] = 0x0D;
	
	
	return (1+(sizeof(AT_BTRH_1)-1));
}

u8 at_send_at_cmd (u8 *buf,const U8 *str)
{
	U8 len = strlen(str);
	DBG_Assert(len + 1 <= MAX_AT_COMAND_BUF);
	
	CFasm_memcpy (&buf[0],str,len);
	buf[len] = 0x0D;
	
	
	return (1+len);
}



u8 at_send_CLIP (u8 *buf)
{
	DBG_Assert((sizeof(AT_CLIP)-1) + 1 <= MAX_AT_COMAND_BUF);
	
	CFasm_memcpy (&buf[0],AT_CLIP,(sizeof(AT_CLIP)-1));
	buf[(sizeof(AT_CLIP)-1)] = 0x0D;
	
	
	return (1+(sizeof(AT_CLIP)-1));
}

u8 at_send_VGS (u8 *buf, u8 vol)
{
	u8 len;

	DBG_Assert((sizeof(AT_VGS)-1) + 1 <= MAX_AT_COMAND_BUF);
	
	if (vol > 15) {
		DBG_Assert(FALSE);
		vol = 15;
	}
	
	CFasm_memcpy (&buf[0],AT_VGS,(sizeof(AT_VGS)-1));
	
	len = (sizeof(AT_VGS)-1);
	buf +=  len;
	
	if (vol >= 10) {
		*buf++ = 0x31;
		*buf++ = 0x30 + (vol - 10);
		len += 2;
	}
	else {
		*buf++ = 0x30 + vol;
		len += 1;
	}

	*buf = 0x0D;
	len += 1;
	
	return len;
}

u8 at_send_iphone_accev (u8 *buf, u8 batt)
{
	u8 len;
	
	DBG_Assert((sizeof(AT_IPHONEACCEV)-1) + 1 <= MAX_AT_COMAND_BUF);
	
	if (batt > 9) {
		DBG_Assert(FALSE);
		batt = 9;
	}
	
	CFasm_memcpy (&buf[0],AT_IPHONEACCEV,(sizeof(AT_IPHONEACCEV)-1));
	
	len = (sizeof(AT_IPHONEACCEV)-1);
	buf +=  len;
	

	*buf++ = 0x30 + batt;
	len += 1;
	
	
	*buf = 0x0D;
	len += 1;
	
	return len;
}



u8 at_BRSF (u8 *buf, int *feat)
{
	u8 *pTmp;
	int val;
	int i;

	for (i=0; i<MAX_AT_COMMAND_LEN;i++) {
		if (*buf != 0x0D && *buf != 0x0A && *buf != 0x20) {
			break;
		} 
		else {
			buf++;
		}
	}

	if (memcmp (buf,BRSF,(sizeof(BRSF)-1)) == 0x0) {
		pTmp = &buf[sizeof(BRSF)-1];
		val = atoi(pTmp);
		*feat = val;

		DBG_Printf ("Phones feat 0x%x..............\n\r",val);

		return TRUE;

	}
	else {
		*feat = 0;	
		return FALSE;
	}
}

u8 at_VGS (u8 *buf, u8 *vol)
{
	u8 *pTmp;
	int val;
	int i;
	
	for (i=0; i<MAX_AT_COMMAND_LEN;i++) {
		if (*buf != 0x0D && *buf != 0x0A && *buf != 0x20) {
			break;
		} 
		else {
			buf++;
		}
	}
	
	if (memcmp (buf,VGS,(sizeof(VGS)-1)) == 0x0 || memcmp (buf,VGS1,(sizeof(VGS1)-1)) == 0x0) {
		pTmp = &buf[sizeof(VGS)-1];
		val = atoi(pTmp);
		*vol = (u8) val;
		
		return TRUE;
		
	}
	else {
		*vol = 0;	
		return FALSE;
	}
}
	
u8 at_VGM (u8 *buf, u8 *vol)
{
	u8 *pTmp;
	int val;
	int i;
	
	for (i=0; i<MAX_AT_COMMAND_LEN;i++) {
		if (*buf != 0x0D && *buf != 0x0A && *buf != 0x20) {
			break;
		} 
		else {
			buf++;
		}
	}
	
	if (memcmp (buf,VGM,(sizeof(VGM)-1)) == 0x0) {
		pTmp = &buf[sizeof(VGM)-1];
		val = atoi(pTmp);
		*vol = (u8) val;
		
		return TRUE;
		
	}
	else {
		*vol = 0;	
		return FALSE;
	}
}

u8 at_OK(u8 *buf)
{
	int i;
		
	for (i=0; i<MAX_AT_COMMAND_LEN;i++) {
		if (*buf != 0x0D && *buf != 0x0A && *buf != 0x20) {
			break;
		} 
		else {
			buf++;
		}
	}

	if (memcmp (&buf[0],AT_OK,(sizeof(AT_OK)-1)) == 0x0) {
		return TRUE;
		
	}
	else {
		return FALSE;
	}
}

u8 at_ERROR(u8 *buf)
{
	int i;
	
	for (i=0; i<MAX_AT_COMMAND_LEN;i++) {
		if (*buf != 0x0D && *buf != 0x0A && *buf != 0x20) {
			break;
		} 
		else {
			buf++;
		}
	}

	if (memcmp (&buf[0],AT_ERROR,(sizeof(AT_ERROR)-1)) == 0x0) {
		return TRUE;
		
	}
	else {
		return FALSE;
	}
}

u8 at_CIND(u8 *buf)
{
	int i;
	
	for (i=0; i<MAX_AT_COMMAND_LEN;i++) {
		if (*buf != 0x0D && *buf != 0x0A && *buf != 0x20) {
			break;
		} 
		else {
			buf++;
		}
	}

	if (memcmp (&buf[0],CIND,(sizeof(CIND)-1)) == 0x0) {
		return TRUE;
		
	}
	else {
		return FALSE;
	}
}

u8 at_RING(u8 *buf)
{
	int i;
	
	for (i=0; i<MAX_AT_COMMAND_LEN;i++) {
		if (*buf != 0x0D && *buf != 0x0A && *buf != 0x20) {
			break;
		} 
		else {
			buf++;
		}
	}

	if (memcmp (&buf[0],RING,(sizeof(RING)-1)) == 0x0) {
		return TRUE;
		
	}
	else {
		return FALSE;
	}
}

u8 at_XAPL(u8 *buf)
{
	int i;
	
	for (i=0; i<MAX_AT_COMMAND_LEN;i++) {
		if (*buf != 0x0D && *buf != 0x0A && *buf != 0x20) {
			break;
		} 
		else {
			buf++;
		}
	}
	
	if (memcmp (&buf[0],AT_XAPL_RESP,(sizeof(AT_XAPL_RESP)-1)) == 0x0) {
		return TRUE;
		
	}
	else {
		return FALSE;
	}
}

u8 at_CLIP(u8 *buf, U8 *clip_buf, u8 n)
{
	int i;
	
	for (i=0; i<MAX_AT_COMMAND_LEN;i++) {
		if (*buf != 0x0D && *buf != 0x0A && *buf != 0x20) {
			break;
		} 
		else {
			buf++;
		}
	}

	if (memcmp (&buf[0],CLIP,(sizeof(CLIP)-1)) == 0x0) {
		buf = &buf[0+sizeof(CLIP)-1];
		if (*buf == 0x20)
			buf++;

		CFasm_memcpy (clip_buf,buf,n);
		return TRUE;		
	}
	else {
		return FALSE;
	}
}




u8 at_CIND_process(u8 *buf, u8 *cind_index)
{
	u16 i;
	
	for (i=0; i<MAX_AT_COMMAND_LEN;i++) {
		if (*buf != 0x0D && *buf != 0x0A && *buf != 0x20) {
			break;
		} 
		else {
			buf++;
		}
	}

	if (memcmp (&buf[0],CIND,(sizeof(CIND)-1)) == 0x0) {

		buf = &buf[0 + (sizeof(CIND)-1)];

		for (i=0;i<(CIND_callheld_index);i++) {

			while (*buf < 'a' || *buf > 'z') {

				if (*buf == 0x0D || *buf == 0x0A) {
					return i; 
				}
				buf++;				
			}
		
			if (memcmp(buf,CIND_service,(sizeof(CIND_service)-1)) == 0) {
				buf += sizeof(CIND_service)-1;
				cind_index[i] = CIND_service_index; 

			}
			else if (memcmp(buf,CIND_callsetup,(sizeof(CIND_callsetup)-1)) == 0) {
				buf += sizeof(CIND_callsetup)-1;
				cind_index[i] = CIND_callsetup_index; 
			}			
			else if (memcmp(buf,CIND_battchg,(sizeof(CIND_battchg)-1)) == 0) {
				buf += sizeof(CIND_battchg)-1;
				cind_index[i] = CIND_battchg_index; 
			}			
			else if (memcmp(buf,CIND_signal,(sizeof(CIND_signal)-1)) == 0) {
				buf += sizeof(CIND_signal)-1;
				cind_index[i] = CIND_signal_index; 
			}			
			else if (memcmp(buf,CIND_roam,(sizeof(CIND_roam)-1)) == 0) {
				buf += sizeof(CIND_roam)-1;
				cind_index[i] = CIND_roam_index; 
			}			
			else if (memcmp(buf,CIND_callheld,(sizeof(CIND_callheld)-1)) == 0) {
				buf += sizeof(CIND_callheld)-1;
				cind_index[i] = CIND_callheld_index; 
			}
			else if (memcmp(buf,CIND_call,(sizeof(CIND_call)-1)) == 0) {
				buf += sizeof(CIND_call)-1;
				cind_index[i] = CIND_call_index; 
			}
			else {
				DBG_Printf ("CIND unkown\n\r");
				//DBG_Assert(FALSE);
				//pass the current string				
				while (1) {
					buf++;
					if(*buf == '"' || *buf == ',' || *buf == 0x0d || *buf == 0x05)
						break;				
				}

			}
						
		}

		return i;
		
	}
	else {
		return 0;
	}

}

u8 at_CIND_process_cstatus(u8 *buf, u8 *cind_cstatus)
{
	u16 i;
	
	for (i=0; i<MAX_AT_COMMAND_LEN;i++) {
		if (*buf != 0x0D && *buf != 0x0A && *buf != 0x20) {
			break;
		} 
		else {
			buf++;
		}
	}
	
	if (memcmp (&buf[0],CIND,(sizeof(CIND)-1)) == 0x0) {
		
		buf = &buf[0 + (sizeof(CIND)-1)];
		
		for (i=0;i<(CIND_callheld_index-1);i++) {
			
			while (*buf < '0' || *buf > '9') {
				
				if (*buf == 0x0D || *buf == 0x0A) {
					return i; 
				}
				buf++;				
			}
			
			cind_cstatus[i] = *buf - '0';
			buf+= 2;			
		}
		
		return i;
		
	}
	else {
		return 0;
	}
	
}

u8 at_send_BVRA_ACTI (u8 *buf)
{
	DBG_Assert((sizeof(AT_BVRA_ACTI)-1) + 1 <= MAX_AT_COMAND_BUF);
	CFasm_memcpy (&buf[0],AT_BVRA_ACTI,(sizeof(AT_BVRA_ACTI)-1));
	buf[(sizeof(AT_BVRA_ACTI)-1)] = 0x0D;
	return (1+(sizeof(AT_BVRA_ACTI)-1));
}

u8 at_send_BVRA_DEACTI (u8 *buf)
{
	DBG_Assert((sizeof(AT_BVRA_DEACTI)-1) + 1 <= MAX_AT_COMAND_BUF);
	CFasm_memcpy (&buf[0],AT_BVRA_DEACTI,(sizeof(AT_BVRA_DEACTI)-1));
	buf[(sizeof(AT_BVRA_DEACTI)-1)] = 0x0D;
	return (1+(sizeof(AT_BVRA_DEACTI)-1));
}

u8 at_CIEV(u8 *buf, u8 *ciev_ind)
{
	u16 i;
	
	for (i=0; i<MAX_AT_COMMAND_LEN;i++) {
		if (*buf != 0x0D && *buf != 0x0A && *buf != 0x20) {
			break;
		} 
		else {
			buf++;
		}
	}
	
	if (memcmp (&buf[0],CIEV,(sizeof(CIND)-1)) == 0x0) {
		
		buf = &buf[0 + (sizeof(CIEV)-1)];
		
			
		while (*buf < '0' || *buf > '9') {
				
			if (*buf == 0x0D || *buf == 0x0A) {
				return 0; 
			}
			buf++;				
		}
			
		ciev_ind[0] = *buf - '0';
		buf+= 2;			
		ciev_ind[1] = *buf - '0';

		
		return 1;
		
	}
	else {
		return 0;
	}
}


const u8 CIEV_test[] = {"+CIEV: 3,0"};

u8 at_CIEV_test(u8 *buf)
{
	int i;

	for (i=0; i<MAX_AT_COMMAND_LEN;i++) {
		if (*buf != 0x0D && *buf != 0x0A && *buf != 0x20) {
			break;
		} 
		else {
			buf++;
		}
	}

	if (memcmp (&buf[0],CIEV_test,(sizeof(CIEV_test)-1)) == 0x0) {
		return TRUE;		
	}
	else {
		return FALSE;
	}
}

u8 at_BCS (u8 *buf, u8 *codec)
{
	u8 *pTmp;
	int val;
	int i;
	
	for (i=0; i<MAX_AT_COMMAND_LEN;i++) {
		if (*buf != 0x0D && *buf != 0x0A && *buf != 0x20) {
			break;
		} 
		else {
			buf++;
		}
	}
	
	if (memcmp (buf,BCS,(sizeof(BCS)-1)) == 0x0) {
		pTmp = &buf[sizeof(BCS)-1];
		val = atoi(pTmp);
		*codec = (u8) val;
		
		DBG_Printf ("Phones set codec %d..............\n\r",val);
		
		return TRUE;
		
	}
	else
	{
		return FALSE;
	}
}

u8 at_send_BCS (u8 *buf, u8 codec)
{
	u8 len;
	
	DBG_Assert((sizeof(AT_BCS)-1) + 1 <= MAX_AT_COMAND_BUF);
	DBG_Assert (codec <= 2 && codec > 0);
	
	CFasm_memcpy (&buf[0],AT_BCS,(sizeof(AT_BCS)-1));
	
	len = (sizeof(AT_BCS)-1);
	buf +=  len;
	

	*buf++ = 0x30 + codec;
	len += 1;
	
	
	*buf = 0x0D;
	len += 1;
	
	return len;
}

u8 at_send_ERROR (u8 *buf)
{
	u8 len;
	
	DBG_Assert((sizeof(AT_ERROR)-1) + 1 <= MAX_AT_COMAND_BUF);
	
	CFasm_memcpy (&buf[0],AT_ERROR,(sizeof(AT_ERROR)-1));
	
	len = (sizeof(AT_ERROR)-1);
	buf +=  len;
	
	*buf = 0x0D;
	len += 1;
	
	return len;
}

#endif