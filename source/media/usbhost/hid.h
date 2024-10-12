#ifndef	__HID_H__
#define	__HID_H__


#define	HID_CLASS_DESPT_HID			0x21
#define	HID_CLASS_DESPT_REPORT		0x22
#define	HID_CLASS_DESPT_PHYSICAL	0x23

#define	HID_CLASSREQ_GET_DESCRIPTOR		( (0x81 << 8) | 0x06 )


#define	HID_CLASSREQ_TYPE_H2D	0x21
#define	HID_CLASSREQ_TYPE_D2H	0xa1

#define	HID_BREQ_GET_RPT		0x01
#define	HID_BREQ_GET_IDLE		0x02
#define	HID_BREQ_GET_PROTOCOL	0x03
#define	HID_BREQ_SET_RPT		0x09
#define	HID_BREQ_SET_IDLE		0x0a
#define	HID_BREQ_SET_PROTOCOL	0x0b

#define	HID_CLASSREQ_GET_RPT	( (HID_CLASSREQ_TYPE_D2H << 8) | HID_BREQ_GET_RPT )
#define	HID_CLASSREQ_SET_RPT	( (HID_CLASSREQ_TYPE_H2D << 8) | HID_BREQ_SET_RPT )

typedef	struct {
	U8	bLength;
	U8	bDesptType;
	U16	bcdHID;
	U8	bCountry;
	U8	bNumClassDespt;
	U8	bClassDesptType;
	U16	wDesptLengh;

	U8	bClassDesptType1;
	U16	wDesptLengh1;
	// more ?
}	__attribute__((packed)) HID_DESCRIPTOR	;

#define	HID_DESCRIPTOR_MIN_SIZE		9



typedef	struct {
	USB_INTERFACE_DESCRIPTOR	*intf;

	HID_DESCRIPTOR	*hid;
	USB_ENDPOINT_DESCRIPTOR	*intIn;
	USB_ENDPOINT_DESCRIPTOR	*intOut;
	
	U8	*report;
	int reportSize;
	//ignore physical descriptor
}	HID_INTF;





int hid_init(void);
int hid_open(USB_DEVICE *usbdev, HID_INTF *intf);
int hid_close(void);

int hid_submitIntIn( void );
int hid_removeIntIn( void );


int hid_parseReportDespt( U8 *rptBuf, int buflen );

#endif //__HID_H__
