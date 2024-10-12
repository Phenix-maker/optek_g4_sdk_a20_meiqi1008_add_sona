#ifndef	__DCD_H__
#define	__DCD_H__


//in endp has 0x80
int dcd_setToggle( int endpAddr, int toggle );	
int dcd_stall( int endpAddr, int stall );
int dcd_isStall( int endpAddr );
int dcd_setAddr( U8 addr );
int dcd_isConnected( void );

int dcd_ctlStatusStageAck( void );

DTD *dcd_removeDtd( DPIPE *pipe );
int dcd_addDtd( DTD *dtd );


int dcd_init(void);
int dcd_open(void);
int dcd_close(void);

#endif
