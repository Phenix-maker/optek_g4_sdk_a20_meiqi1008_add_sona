#ifndef	_KEY_SCAN_H_
#define	_KEY_SCAN_H_


void KeyScan_init(void);
void KeyScan_open (void);
void KeyScan_close (void);
void KeyScan_fini (void);
void KeyScan(U32 ticks);

#endif //_KEY_SCAN_H_
