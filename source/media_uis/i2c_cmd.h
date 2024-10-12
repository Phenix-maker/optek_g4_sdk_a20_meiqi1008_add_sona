#ifndef	_CD_CMD_H_
#define	_CD_CMD_H_

/*
	description:
	this file is used for CD/PC I2C command define
*/

//#undef  U8
//#define	U8 unsigned char


#define	MAX_CMD_PARAM_SIZE	120


typedef struct {
    U8 packetLen;		//exclude self
	U8 cmdType;			//key or cmd
    U8 cmdCode;
    U8 param[MAX_CMD_PARAM_SIZE];
}  CMD_PACKET;

typedef struct {
	U8 keyCode;
	U8 type;			//up or down
}	KEY_MESSAGE;

/*
	key code define
*/
#define	KEY_CODE_PLAY_PAUSE		0x01
#define	KEY_CODE_STOP			0x02
#define	KEY_CODE_OPEN			0x03		
#define	KEY_CODE_CLOSE			0x04
#define	KEY_CODE_DBB			0x05
#define	KEY_CODE_FF				0x06
#define	KEY_CODE_FR				0x07
//#define	KEY_CODE_VOL_PLUS		0x08
//#define	KEY_CODE_VOL_MINUS		0x09
#define KEY_SEARCH_FORWARD		0x08
#define KEY_SEARCH_BACKWARD		0x09
#define	KEY_CODE_ESP			0x0A
#define	KEY_CODE_PROG			0x0B
#define	KEY_CODE_DISPLAY		0x0C
#define	KEY_CODE_MODE			0x0D
#define KEY_CODE_ALBUM_PLUS     0x0E
#define KEY_CODE_ALBUM_MINUS    0x0F
//#define	KEY_CODE_RPT		0x10
#define KEY_CODE_CLR_PROG		0x10
//#define	KEY_CODE_RND		0x11
#define	KEY_CODE_REC			0x11
#define	KEY_CODE_REM			0x12
#define	KEY_CODE_INTRO			0x13
//#define	KEY_CODE_MEM		0x14
#define	KEY_CODE_PAUSE			0x14
#define	KEY_CODE_0				0x15
#define	KEY_CODE_1				0x16
#define	KEY_CODE_2				0x17
#define	KEY_CODE_3				0x18
#define	KEY_CODE_4				0x19
#define	KEY_CODE_5				0x1A
//#define	KEY_CODE_6			0x1B
#define	KEY_CODE_DEL			0x1B
#define	KEY_CODE_7				0x1C
#define	KEY_CODE_8				0x1D
#define	KEY_CODE_9				0x1E
#define	KEY_CODE_10PLUS			0x1F	


//virtual key define
#define	KEY_VKEY_SOURCE_CD		0x10
#define	KEY_VKEY_SOURCE_HDD		0x12


#define	CMD_TYPE_KEY	0x01
#define	CMD_TYPE_CMD	0x10
//define key up..down
#define	I2C_KEYMSG_UP		0x01
#define	I2C_KEYMSG_DOWN		0x10
/****************************************************
CMDTYPE command format to slave
struct {
    BYTE packetLen;		//exclude self
	BYTE cmdType;		//=CMD_TYPE_CMD
    BYTE cmdCode;		//command that is one of CMDTYPE
    BYTE param[MAX_CMD_PARAM_SIZE];//is null
}
****************************************************/
/****************************************************
key command format to slave
struct {
    BYTE packetLen;		//exclude self
	BYTE cmdType;		//=CMD_TYPE_KEY
    BYTE cmdCode;		//keycode,see key code section for detail
    BYTE param[MAX_CMD_PARAM_SIZE];//KEYMSG_UP or KEYMSG_DOWN in param[0],other array is not used
}
****************************************************/

typedef enum {
	enSOURCE_CD = 1,
	enSOURCE_HDD,
	enSOURCE_RADIO,
	enSOURCE_AUX
}	enum_WORK_SOURCE;

typedef enum {
	enMODE_DECODE,
	enMODE_ENCODE,
	enMODE_RIPPLE
}	enum_WORK_MODE;

enum {
	PROG_ON,
	PROG_OFF
};

/*
enum {
	STREAM_UNKNOWN,
	STREAM_MP3,
	STREAM_MP2,
	STREAM_MP1,
	STREAM_WMA,
	STREAM_AAC,
	STREAM_BIN,
	STREAM_CDDA
};

enum {
	NORMAL_PLAY = 0,
		REPEAT_ONE,
		REPEAT_DIR,
		REPEAT_ALL,
		REPEAT_ARTIST,
		RANDOM_ALL,
		RANDOM_REPEAT_ALL,
		RANDOM_IN_DIR,						//random dir
		RANDOM_IN_DIR_REPEAT_ALL,
		RANDOM_ARTIST,
		DIR_RANDOM,						//random in dir
		DIR_RANDOM_REPEAT_ALL,
		INTRO_PLAY
}
*/

typedef enum {
	enCMD_GET_STATE  = 1,						//return  HDD file rcv/buffer status,full/empty
	enCMD_GET_FILE_NAME,
	enCMD_GET_FOLDER_NAME,
	enCMD_GET_ARTIST,
	enCMD_GET_TITLE,
	enCMD_GET_STREAM_TYPE,
	enCMD_GET_DISC_TYPE,
	enCMD_GET_DISC_INFO,
	enCMD_GET_PROG_STATUS,
	
	enCMD_SET_SOURCE,							//cd/hdd/fm
	enCMD_SET_MODE,									//ripple/encode/decode
	enCMD_HDD_PLAY_TRACK,							//track no, start time, file length ...
	enCMD_HDD_GET_TXD_START_POS,		//calculate by start time
	enCMD_HDD_START_FILE_TXD,
	enCMD_HDD_START_FILE_RCV,
	enCMD_HDD_STOP_TRANCEIVE,
	enCMD_HDD_FLUSH_FILE,
	
	enCMD_GET_TRACK_INFO,
	enSUBCMD_GET_TRACK_NO,
	enSUBCMD_GET_TRACK_NAME,
	enSUBCMD_GET_TRACK_ID3,
	enSUBCMD_GET_TRACK_BPS_SAMPLE_RATE,
	enCMD_GET_PLAYING_INFO,
	enCMD_GET_SW_VERSION,
	
	enCMD_GET_PLAYING_TIME,

	enCMD_CANCEL
} CMDTYPE;

//return: cd/mp3 disc, total tracks, total folders

/***********************************************
1.enCMD_GET_STATE format that return from slave:
	struct {
		BYTE len;
		BYTE STATE;
		#if STATE_PLAYING
		..............
		#else
		WORD ALBM;
		WORD TATAL_TRACK;
		DWORD TOTAL_TIME;
		#endif
	}STATE_CONTENT;
***********************************************/
typedef enum {
	enSTATE_INIT = 0,
	enSTATE_OPEN,
	enSTATE_OPENING,
	enSTATE_CLOSED,
	enSTATE_CLOSING,
	enSTATE_READING_TOC,
	enSTATE_READING_FILE_SYSTEM,
	enSTATE_NO_DISC,
	enSTATE_NO_AUDIO_FILES,
	enSTATE_DISC_READY,
	enSTATE_PLAYING,
	enSTATE_PAUSE,
	enSTATE_STOP,
	enSTATE_BROWSE_TRACK,
	enSTATE_BROWSE_ALBUM,
	
	
	enSTATE_WAIT_MUSIC_INIT,
	enSTATE_BUFFER_FULL,		//for hdd mode use, buffer full/empty, music end
	
	enSTATE_FATAL_ERR,
	enSTATE_NEED_UPGRADE_DISC,
	enSTATE_READ_UPGRADE_FILE,
	enSTATE_ERASE_FLASH,
	enSTATE_PROGRAM_FLASH,
	enSTATE_UPGRADE_SUCCESS,
	enSTATE_UPGARDE_FAIL,

	enSTATE_FAST_PLAYING,
	enSTATE_BROWSE_TRACK_10P,
	enSTATE_BROWSE_ALBUM_10P,
	enSTATE_PLAYING_10P,
	enSTATE_PAUSE_10P,

	enSTATE_PGM_BROWSE_TRACK,
	enSTATE_PGM_BROWSE_TRACK_10P,
	enSTATE_PGM_BROWSE_ALBUM

}	enum_STATE_MACHINE;

//playing time
#define	NO_PLAYING_TIME_INFO		-1
#define	PLAYING_TIME_NOT_READY		-2
#define	VBR_PLAYING_TIME_UNKNOWN	-3


/*
	functions def
*/
int cdcmd_init(void);
int cdcmd_open(void);
int cdcmd_close(void);

int cdcmd_receiveCmdPacket( CMD_PACKET * packet );
int cdcmd_sendKeyMessage( KEY_MESSAGE * keyMsg );
int cdcmd_sendCmd( U8 cmdCode, U8 * param, U8 paramLen );


int cdcmd_cmdSetSource(int source);
int cdcmd_cmdSetMode( int mode );



/*Philips MP3 Module*/
/* actions */
#define AC_Bus_Resync                             0x03    /* 3 */
#define AC_Clear_Program                          0x05    /* 1 */
#define AC_Get_Track_Album_Name_XY                0x06    /* 3 */
#define AC_Get_Track_Num_In_Album_XY              0x09    /* 1 */
#define AC_Next_Album                             0x0a    /* 0 */
#define AC_Next_Track                             0x0c    /* 0 */
#define AC_Pause                                  0x0f    /* 0 */
#define AC_Play                                   0x11    /* 1 */
#define AC_Play_Track_XY                          0x12    /* 4 */
#define AC_Prepare_Power_Down                     0x14    /* 0 */
#define AC_Previous_Album                         0x17    /* 0 */
#define AC_Previous_Track                         0x18    /* 0 */
#define AC_Program_Current_Track                  0x1b    /* 0 */
#define AC_Read_Disc_Content                      0x1d    /* 0 */
#define AC_Read_ID3_XY                            0x1e    /* 3 */
#define AC_Release_Disc                           0x21    /* 0 */
#define AC_Search_Backward                        0x22    /* 1 */
#define AC_Search_FastBackward                    0x23    /* 1 */
#define AC_Search_Forward                         0x24    /* 1 */
#define AC_Search_FastForward                     0x25    /* 1 */
#define AC_Service_Focus_Search_Restart           0x27    /* 0 */
#define AC_Service_Mode_Enter                     0x2b    /* 0 */
#define AC_Service_Sledge                         0x2d    /* 1 */
#define AC_Set_Track_XY                           0x2e    /* 3 */
#define AC_Stop                                   0x30    /* 0 */
//add
#define AT_Get_Track_ID3_Aritst                   0x33    /* 1 */
#define AT_Get_Track_ID3_Title                    0x35    /* 1 */

//Test
#define AT_Get_Track_ID3_Aritst_Test              0x37    /* 1 */
#define AT_Get_Track_ID3_Album_Test               0x38    /* 1 */
#define AT_Get_Track_ID3_Title_Test               0x39    /* 1 */
#define AT_Get_Track_Folder_Name                  0x40    /* 1 */
#define AT_Get_Album_Info                         0x41    /* 1 */


/* attributes */
#define AT_Fetch_Event                            0x60    /* 0 */
#define AT_Get_Current_Bitrate                    0x63    /* 0 */
#define AT_Get_Current_Compression_Algorithm      0x65    /* 0 */
#define AT_Get_Current_Progress_Info              0x77    /* 0 */
#define AT_Get_Current_Samplerate                 0x66    /* 0 */
#define AT_Get_Current_Time                       0x69    /* 0 */
#define AT_Get_Current_Track_Album_Number         0x6a    /* 0 */
//#define AT_Get_ID3_Tag                          0x6c    /* 1 */
#define AT_Get_Module_Type                        0x6f    /* 0 */
#define AT_Get_Play_Mode                          0x71    /* 0 */
#define AT_Get_Program_Details                    0x72    /* 0 */
#define AT_Get_Program_Element                    0x74    /* 1 */
#define AT_Get_State                              0x78    /* 0 */
#define AT_Get_SW_Version                         0x7b    /* 0 */
#define AT_Get_Track_Length_XY                    0x7d    /* 3 */
#define AT_Set_Max_Name_Length                    0x7e    /* 1 */
#define AT_Set_Module_Type                        0x81    /* 4 */

/* events */
#define EV_Disc_Contents_Ready                    0xb1    /* 8 */
#define EV_Disc_Error                             0xb2    /* 1 */
#define EV_Disc_Stopped                           0xb4    /* 0 */
#define EV_Display_Info_Changed                   0xb7    /* 1 */
#define EV_File_Fragmented                        0xbd    /* 0 */
#define EV_Invalid_Data_Received                  0xbb    /* 0 */
#define EV_No_Event                               0xb8    /* 0 */
#define EV_Send_Buffer_Full                       0xbe    /* 0 */
#define EV_Service_Focus_Status                   0xc0    /* 1 */
#define EV_Slave_Off                              0xff    /* 0 */
#define EV_State                                  0xc3    /* 1 */
#define EV_Track_Album_Name_XY                    0xc5    /* 3+x */
#define EV_Track_Num_In_Album_XY                  0xc6    /* 3 */
#define EV_USB					                  0xc7    /* 3 */
//add
#define EV_Track_ID3_Artist                       0xa1    /* 3+x */
#define EV_Track_ID3_Title                        0xa4    /* 3+x */
#define EV_Track_ID3_ALBUM                        0xa2    /* 3+x */
#define EV_Album_INFO                             0xa3    /* 3+x */


/*MP3 Modlue State*/
#define MSTATE_POWEROFF     0	/* ready for power off */
#define MSTATE_IDLE         1	/* idle (no TOC) */
#define MSTATE_TOCREAD      2	/* TOC reading */
#define MSTATE_ID3READ      3	/* ID3 reading */
#define MSTATE_STOP         4	/* stop */
#define MSTATE_PLAY         5	/* play */
#define MSTATE_SERVICE      6	/* service */

/* repeat state */
#define REPEAT_OFF			0
#define REPEAT_TRACK		7
#define REPEAT_DISC			5

#define SHUFFLE_OFF			0
#define SHUFFLE_ALL			1
#define REPEAT_SHUFFLE_ALL	2


#define MSTATE_USB_PLUG_IN	            8
#define MSTATE_USB_NO_CARD_READER       9
#define MSTATE_USB_NO_USB_DEVICE        10
#define MSTATE_USB_FILE_SYSTEM_ERROR    11
#define MSTATE_USB_NO_AUDIO_FILES       12
#define MSTATE_USB_DEVICE_UNPLUG        13
#define MSTATE_USB_ERROR_PLAYING_MUSIC  14
#define MSTATE_USB_DEVICE_NOT_SURPPORT  15
#define MSTATE_USB_UPGRADEING		    16
#define MSTATE_USB_UPGRADE_SUCCESS	    17
#define MSTATE_USB_UPGRADE_FAILURE 	    18
#define MSTATE_USB_SLOW_USB 			19

#define AT_Get_Usb_Bitrate              0x87
#define EV_USB_Bitrate                  0xc9
	#define	MODULE_BITRATE_OK			0
	#define	MODULE_BITRATE_NO_USB		1
	#define	MODULE_BITRATE_NO_TEST_FILE	2
	#define	MODULE_BITRATE_TEST_ERROR	3

#define AC_Sop_Ripping				0x83
/*------------------------------------------------------------------------------------------------------------------*/
#define AC_Start_Ripping			0x84
#define START_RIPPING_DISC			0
#define START_RIPPING_PROGRAM		1
#define START_RIPPING_ALBUM			2
#define START_RIPPING_TRACK			3
#define START_RIPPING_PLAYLIST		4
#define START_RIPPING_TO_END		5

/*  	acRParam[0]: 0: ripping disc		1:ripping program	2:ripping album	
				3: ripping track	4:ripping playlist	5:ripping from current track to end
       acRParam[1]: track number high byte
       acRParam[2]: track number low byte
       acRParam[3]: album number
 */

/*------------------------------------------------------------------------------------------------------------------*/
#define AC_Del_Ripping			0x85
#define DEL_RIPPING_DISC		0
#define DEL_RIPPING_PROGRAM		1
#define DEL_RIPPING_ALBUM		2
#define DEL_RIPPING_TRACK		3
#define DEL_RIPPING_PLAYLIST	4
#define DEL_RIPPING_TO_END		5
/*  	acRParam[0]: 0: ripping disc		1:ripping program	2:ripping album	3: ripping track	4:ripping playlist
       acRParam[1]: track number high byte
       acRParam[2]: track number low byte
       acRParam[3]: album number
 */

/*------------------------------------------------------------------------------------------------------------------*/
#define AT_Set_Ripping_Quality 		0x86
/*  	
acRParam[0]: 0: 128k bps		1:96k bps		2:48k bps	
*/
/*------------------------------------------------------------------------------------------------------------------*/
#define EV_USB_Ripping					0xc8
#define MODULE_RIPPING_DISC				0
#define MODULE_RIPPING_PROGRAM			1
#define MODULE_RIPPING_ALBUM			2
#define MODULE_RIPPING_TRACK			3
#define MODULE_RIPPING_PLAYLIST			4
#define MODULE_RIPPING_TO_END			5
#define MODULE_RIPPING_INVALID			6
#define MODULE_RIPPING_NO_DEVICE		7
#define MODULE_RIPPING_PROTECTED_CD		8
#define MODULE_RIPPING_WMA_DRM			9
#define MODULE_RIPPING_FILE_EXIST		10

#define MODULE_DEL_DISC					0x10
#define MODULE_DEL_PROGRAM				0x11
#define MODULE_DEL_ALBUM				0x12
#define MODULE_DEL_TRACK				0x13
#define MODULE_DEL_PLAYLIST				0x14
#define MODULE_DEL_TO_END				0x15
#define MODULE_DEL_INVALID				0x16
#define MODULE_DEL_ERROR				0x17

#define MODULE_RIPPING_USER_STOP			0x20
#define MODULE_RIPPING_END_STOP				0x21
#define MODULE_RIPPING_ERROR_STOP			0x22
#define MODULE_RIPPING_USB_FULL_STOP		0x23
#define MODULE_RIPPING_NUMBER_FULL_STOP		0x24
#define MODULE_RIPPING_NO_USB				0x25
#define MODULE_RIPPING_DEVICE_NOT_SUPPORT	0X26
#define MODULE_RIPPING_SYSTEM_NOT_SUPPORT	0X27

/*  	
  	acRParam[0]: 0: ripping disc		1:ripping program	2:ripping album	
  				3: ripping track	4:ripping playlist	5:ripping from current track to end	6:ripping invalid
  				
  				0x10:del disc		0x11:del program	0x12:del album	
  				0x13:del track		0x14:del playlist	0x15:del from current track to end	0x16:del invalid
  				
   				0x20:user stop	0x21:end stop		0x22:error stop	0x23:usb full stop	0x24:number full stop
   				
      acRParam[1]: track number high byte 
       acRParam[2]: track number low byte
       acRParam[3]: album number
*/
//#endif

#endif // _CD_CMD_H_
