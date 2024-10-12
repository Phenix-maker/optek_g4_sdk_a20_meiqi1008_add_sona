#ifndef __APP_SDRAM_H__
#define	__APP_SDRAM_H__

#include "flash_boot.h"


//#define FS_TRACKS_LIMITED

#define FS_FILE_MAX       999
#define FS_FOLDER_MAX     999

//#define MAX_DISP_FILE_FOLDER_NAME    64
#define MAX_DISP_FILE_FOLDER_NAME    32

#define MAX_TITLE_LEN                32


//#pragma pack(1,1,0)

extern U8 BT_STREAM_START[];
extern U8 BT_STREAM_END[];

extern U8 BT_HFP_STREAM_START[];
extern U8 BT_HFP_STREAM_END[];

extern U8 MP3_STREAM_START[];
extern U8 MP3_STREAM_END[];

extern U8 SLAVE_DACOUT_START[];
extern U8 SLAVE_DACOUT_END[];

extern U8 SPDIF_STREAM_START[];
extern U8 SPDIF_STREAM_END[];

extern U8 SPDIF_DOLBY_AC3_STREAM_START[];
extern U8 SPDIF_DOLBY_AC3_STREAM_END[];

extern U8 SPDIF_DOLBY_AC3_STREAM_START[];
extern U8 SPDIF_DOLBY_AC3_STREAM_END[];
	
extern U8 SPDIF_DTS_DMP_STREAM_START[];
extern U8 SPDIF_DTS_DMP_STREAM_END[];

extern U8 SPDIF_DOLBY_EC3_STREAM_START[];
extern U8 SPDIF_DOLBY_EC3_STREAM_END[];

extern U8 WAV_STREAM_START[];
extern U8 WAV_STREAM_END[];;

extern U8 FLAC_STREAM_START[];
extern U8 FLAC_STREAM_END[];

extern U8 XA_MEMTYPE_COPY_START[];
	
extern U8 XA_MEMTYPE_PERSIST_START[];
extern U8 XA_MEMTYPE_PERSIST_END[];

extern U8 XA_MEMTYPE_SCRATCH_START[];
extern U8 XA_MEMTYPE_SCRATCH_END[];

extern U8 XA_MEMTYPE_INPUT_START[];
extern U8 XA_MEMTYPE_INPUT_END[];

extern U8 XA_MEMTYPE_OUTPUT_START[];
extern U8 XA_MEMTYPE_OUTPUT_END[];

extern U8 DDPLUS_PERSISTENT_START[];
extern U8 DDPLUS_SCRATCH_START[];
extern U8 DDPLUS_OUTPUT_START[];

extern U8 USB_SPEAKER_STREAM_START[];
extern U8 USB_SPEAKER_STREAM_END[];

extern U8 RECORD_DATA_BUF_START[];
extern U8 RECORD_DATA_BUF_END[];

extern U8 RECORD_FIFO_BUF_START[];
extern U8 RECORD_FIFO_BUF_END[];

extern U8 DELAY_MEMORY_START[];
extern U8 DELAY_MEMORY_END[];

extern U8 ROM_APP_RUN_FLAG_ADDR[];
extern U8 ROM_APP_START_ADDR[];

extern U8 ROM_APP_READ_START_ADDR[];
extern U8 ROM_APP_COPY_START_ADDR[];

extern U8 SFLASH_BOOT1_FLAG_ADDR[];

extern U8 SDRAM_START_ADDR[];
extern U8 SFLASH_START_ADDR[];

extern U8 __btdm_bsss_ram_start[];
extern U8 __btdm_bsss_ram_end[];

	
typedef enum
{
	SD_SEEK_CUR = 0,
	SD_SEEK_ORG,
	SD_SEEK_END
}	SD_SEEK_POS;

#define TOTAL_OF_STREAMS    4

#define OVERWRITE_ENABLE    1
#define OVERWRITE_DISABLE   0

#define STREAM_ID U16

#define INVALID_STREAM_ID       0xFFFF
#define STREAM_UNDERFLOW        0xFFFFFFFF
#define STREAM_INVALID_ADDR     0xFFFFFFFF
#define STREAM_LENTH_UNKNOWN    0xFFFFFFFF
		
#define SDRAM_STREAM_WSTREAM_PROGRESS    1
#define SDRAM_STREAM_WSTREAM_END         0

#ifdef SDRAM64M
#define MAX_SDRAM_ADDR			0x800000
#define MAX_STREAM_BUF			0x700000
#define MAX_CDDA_STREAM_BUF		0x7F0000
#endif

#ifdef SDRAM16M
#ifdef MP3CD
#define MAX_SDRAM_ADDR			0x200000
#define MAX_STREAM_BUF			0x190000
#define MAX_CDDA_STREAM_BUF		0x1F0000
#else
#define MAX_SDRAM_ADDR			0x200000
#define MAX_STREAM_BUF			0x180000
#define MAX_CDDA_STREAM_BUF		0x1F0000
#endif
#endif

/*Please note that FS_STREAM_STRAT must be larger than sort_disc_info*/
/*
#define FS_STREAM_STRAT		 0x20015800			
#define FS_MAX_STRAM_BUF	(0x20020000 - FS_STREAM_STRAT)
#define FS_MAX_STREAM_SECTORS	(FS_MAX_STRAM_BUF/0x800)


#define MP3_STREAM_START		0x20012C00
#define MP3_STREAM_END			0x2001DC00

#define MP3_SRC_STREAM_START	0x20012C00
#define MP3_SRC_STREAM_END		0x2001DC00

#define WMA_STREAM_START		0x20017C00
#define WMA_STREAM_END			0x2001DC00

#define WMA_SRC_STREAM_START	0x20017C00
#define WMA_SRC_STREAM_END		0x2001DC00
*/

//#define FS_MAX_STRAM_BUF		0x80000
//#define FS_MAX_STRAM_BUF		0x64000

#if 0//ndef BUFFER_SIZE_SMALL
//for sdram
#define FS_MAX_STRAM_BUF        0x50000                 //please not ld stream buf size
#define FS_MAX_STREAM_SECTORS   (FS_MAX_STRAM_BUF/0x800)
#endif

#if 0
extern unsigned char SRAMbase[];
#define FS_STREAM_STRAT		((U32)&SRAMbase[0]+0x00015800)			
#define FS_MAX_STRAM_BUF	((U32)&SRAMbase[0]+0x00020000 - FS_STREAM_STRAT)
#define FS_MAX_STREAM_SECTORS	(FS_MAX_STRAM_BUF/0x800)


#define MP3_STREAM_START		((U32)&SRAMbase[0]+0x00012C00)
#define MP3_STREAM_END			((U32)&SRAMbase[0]+0x0001DC00)

#define MP3_SRC_STREAM_START	((U32)&SRAMbase[0]+0x00012C00)
#define MP3_SRC_STREAM_END		((U32)&SRAMbase[0]+0x0001DC00)

#define WMA_STREAM_START		((U32)&SRAMbase[0]+0x00017C00)
#define WMA_STREAM_END			((U32)&SRAMbase[0]+0x0001DC00)

#define WMA_SRC_STREAM_START	((U32)&SRAMbase[0]+0x00017C00)
#define WMA_SRC_STREAM_END		((U32)&SRAMbase[0]+0x0001DC00)
#endif


enum {
	INTERRUPT_CALL,
	NORMAL_CALL
};

enum {
	STREAM_DATA_CDDA,
	STREAM_DATA_CDROM
};


typedef struct {
	long start_addr;
	long end_addr;
	I16 overwrite;
	I16 windows;
} SDRAM_STREAM_CONFIG;   

#define TOTAL_SUBDIRS		99
#define TOTAL_ENTRIES		400
#define TOTAL_PLAYINGLIDT	400
#define BUFFER_CAPACITY		2304*2


enum {
	ID3_UNKNOWN,
	ID3_NO_TAG,
	ID3_TAG_PRESENT
};

enum {
	LIST_TRACK,
	LIST_FOLDER
};

typedef struct {
	U16 entry;
	U8 attrib;
} LIST;

enum PROG_ATTRIBUTE {
	PROG_ITEM_DIR,
	PROG_ITEM_FILE
};

typedef struct {
	/*track no or track no which has the property of the following attribute*/
	U16 entry;
#if 0
	U8 attribute;
#endif
} MP3_PROGRAM_ITEM;

#define PLAYLIST long

#define MAX_FILE_NAME	48			/*132*/

#define USER_DATA_SECTORS	2l

typedef struct {
#if 0
	U16 name[MAX_FILE_NAME+1];

//	U8 ID3_flag;
	I16 song_folds;
	I16 fold_entries;
	U8 stream_type;
	
	long start_addr;							//if fragment is true, it is file udf entry addr
	long len;

	U8 index;									//index in total tracks; for cdda tno no.
	U8 fragment;								//flag for fragment file

	U16	fdb_index;								//for delete
#else
	long start_addr;							//if fragment is true, it is file udf entry addr
	long len;

	I16 song_folds;
	I16 fold_entries;

	U16 name[MAX_FILE_NAME+1];

	U8 stream_type;

	U8 index;									//index in total tracks; for cdda tno no.
	U8 fragment;								//flag for fragment file

//	U16	fdb_index;								//for delete
#endif
} ENTRY;


typedef struct {
	U16 name[MAX_FILE_NAME+1];
	long location;
	long len;
	I16 parent_no;
	I16 first_entry;
	I16 entries;
} SUBDIR;

#define C_MAX_FILE_NAME 12

#define MAX_C_DISC_INFO_SPACE	0x2380			/* Please not the src buf is used for this space*/

typedef struct {

	//U16 name[C_MAX_FILE_NAME+1];
	
	int start_addr		:	24;							//if fragment is true, it is file udf entry addr
	int index			:	7;									//index in total tracks; for cdda tno no.
	int	fragment		:	1;									//flag for fragment file
	int len				:	28;
	int stream_type		:	4;

	//I16 song_folds;
	//I16 fold_entries;
	U16	fdb_index;										//for delete
} C_ENTRY;


typedef struct {
	//U16 name[C_MAX_FILE_NAME+1];
	I16 first_entry;
	//I16 entries;
	U32	location;										//for delte func
} C_SUBDIR;

//extern ENTRY *pCEntry;
//extern U8 *pCInfoStart;
//extern U16 wCInfoLen;

typedef struct {
	I16 subdirs;
	I16 entries;								/*number of digital autio entries*/
	I16 playlists;

	long entry_addr;
	long playlist_addr;
	long subdir_addr;
	long volume_addr;
	long tag_addr;
	long prg_addr;

	I16 open;

	long start_addr;							/*must be in block addr*/
	long end_addr;								/*must be in block addr*/
} SDRAM_UDATA;


#define	CD_ROM_DATA_SIZE 1170
 
typedef struct {
	U16 buf[CD_ROM_DATA_SIZE];
	U16 *ptr;
	long	addr;
	I16		size;
} SEC_BUF;


#ifdef SDRAM64M

#define MAX_MP3_PLAYLIST_ENTRY	50
#define MAX_ENTRY				999
#define MAX_FOLDER				999
#define MAX_PLAYLIST_ITEM		999
#define MAX_BROWSE_LIST_ITEM	999
#define MAX_TAG_LENGTH			128

#define MAX_ID3_TEXT			64
#define MAX_ID3_GENRE_TEXT		16
#define MAX_ID3_YEAR			5

#endif


#if 1//def SDRAM16M

#ifdef BOOT_FROM_FLASH
//#define MAX_MP3_PLAYLIST_ENTRY      99
#define MAX_MP3_PLAYLIST_ENTRY      32
//#define MAX_MP3_PLAYLIST_ENTRY      20
#else
#define MAX_MP3_PLAYLIST_ENTRY      3
#endif

//#define MAX_ENTRY             999
//#define MAX_FOLDER            500


#define MAX_ENTRY				59		//99
#define MAX_FOLDER				9

#define MAX_PLAYLIST_ITEM		MAX_ENTRY
#define MAX_BROWSE_LIST_ITEM	MAX_ENTRY
#define MAX_TAG_LENGTH			128

//#define MAX_ID3_TEXT			64
//#define MAX_ID3_TEXT			48
#define MAX_ID3_TEXT			32

#define MAX_TITLE_LEN           MAX_ID3_TEXT

#define MAX_ID3_GENRE_TEXT		16
#define MAX_ID3_YEAR			5

#endif
	
typedef struct {
	U16 title [MAX_ID3_TEXT + 1];
	U16 artist [MAX_ID3_TEXT + 1];

	U16 album [MAX_ID3_TEXT + 1];

#ifdef ID3ALL
	U16 genre [MAX_ID3_GENRE_TEXT +1];
	U16 year[MAX_ID3_YEAR + 1];	
#endif

} ID3_TAG;


//extern ID3_TAG entry_id3_tag;
extern ID3_TAG id3_tag;
#define entry_id3_tag id3_tag

extern U8 entry_id3_flag;

extern U16 file_name[MAX_DISP_FILE_FOLDER_NAME+1];
extern U16 folder_name[MAX_DISP_FILE_FOLDER_NAME+1];

//for converting unicode to oem
extern U16 unicode_name_to_oem[MAX_DISP_FILE_FOLDER_NAME+1];

extern U8 MP3_C_DISC_INFO[];
extern U8 WMA_C_DISC_INFO[];
extern U8 CDDA_C_DISC_INFO[];
extern U8 SRC_START[];
extern U8 BUF_END[];
extern U8 USB_BUF_END[];
extern U8 _CDDA_STREAM_START[];

extern U8 SDRAM_CDROM_STREAM_START[];
extern U8 SDRAM_CDROM_STREAM_END[];
extern U8 SDRAM_CDDA_STREAM_START[];
extern U8 SDRAM_CDDA_STREAM_END[];
extern U8 SDRAM_MP3_STREAM_START[];
extern U8 SDRAM_MP3_STREAM_END[];

extern U8 WAV_STREAM_START[];
extern U8 WAV_STREAM_END[];

extern U8 SDRAM_FLAC_STREAM_START[];
extern U8 SDRAM_FLAC_STREAM_END[];

extern U8 FLAC_INPUT_BUF_START[];
extern U8 FLAC_INPUT_BUF_END[];

extern U8 FLAC_OUTPUT_BUF_START[];
extern U8 FLAC_OUTPUT_BUF_END[];

extern U8 FLAC_SCRATCH_BUF_START[];
extern U8 FLAC_SCRATCH_BUF_END[];

extern U8 SDRAM_CDDA_ENC_STREAM_START[];
extern U8 SDRAM_CDDA_ENC_STREAM_END[];


extern U8 SDRAM_CDROM_MP3_STREAM_START[];
extern U8 SDRAM_CDROM_MP3_STREAM_END[];

extern U8 SDRAM_CDROM_SEARCH_DATA_START[];
extern U8 SDRAM_CDROM_SEARCH_DATA_END[];
extern U8 PCD_CDROM_STREAM_START[];
extern U8 PCD_CDROM_WMA_STREAM_START[];
	
extern U8 SDRAM_CDROM_SEARCH_FOLDER_TRACK_NAME_START[];
extern U8 SDRAM_CDROM_SEARCH_FOLDER_TRACK_NAME_END[];

extern U8 SDRAM_IPOD_STREAM_END[];
extern U8 SDRAM_IPOD_STREAM_START[];

extern U8 SPDIF_STREAM_END[];
extern U8 SPDIF_STREAM_START[];
extern U8 SPDIF_READ_BUF_START[];

extern U8 IRAM_BSS_START[];
extern U8 IRAM_BSS_END[];

extern U8 MIC_ECHO_STREAM_START[];
extern U8 MIC_ECHO_STREAM_END[];

extern U8 BT_HP_SBC_ENC_FIFO_START[];
extern U8 BT_HP_SBC_ENC_FIFO_END[];


#if 0//def BUFFER_SIZE_SMALL
//CD file systm:for no sdram
#define FS_MAX_STRAM_BUF         (SDRAM_CDROM_STREAM_END-SDRAM_CDROM_STREAM_START) //please not ld stream buf size
#define FS_MAX_STREAM_SECTORS    (FS_MAX_STRAM_BUF/0x800)

//increasing memory sizes,it isn't ok
//#define FS_MAX_STRAM_BUF          (SDRAM_MP3_STREAM_END-SDRAM_MP3_STREAM_START)
//#define FS_MAX_STREAM_SECTORS     (FS_MAX_STRAM_BUF/0x800)
#endif

extern U8 BT_STREAM_START[];
extern U8 BT_STREAM_END[];

extern U8 BT_DATA_1[];
extern U8 BT_DATA_2[];


typedef struct {
	ENTRY entry [MAX_ENTRY];
	SUBDIR subdir[MAX_FOLDER];
} DISC_INFO;

typedef struct {

#ifdef MP3_PROGRAM_ENABLE
	MP3_PROGRAM_ITEM mp3_program_list [MAX_MP3_PLAYLIST_ENTRY];

	U16	total_of_program_list;
#endif

#if (defined MP3_DECODE_ENABLE || defined WMA_DECODE_ENABLE)
	//U16 playlist [MAX_PLAYLIST_ITEM]; //playlist or browse list
#endif

	U16	total_of_playlist;
//	U8	mp3_vbr_toc[100];

} PLAYING_INFO;


extern PLAYING_INFO playing_info;


//#define START_DAC_STREAM_ADDR		0x3FA000l
//#define END_DAC_STREAM_ADDR			0x400000l
//#define DAC_STREAM_CAPACITY			(END_DAC_STREAM_ADDR - START_DAC_STREAM_ADDR)
			
//typedef struct {
//	long pRead;
//	long pWrite;
//	I16 size;
//} DAC_STREAM;

//extern DAC_STREAM dac_stream;

#define CDDA_MINI_STREAM_SIZE			0x80000l		/*s secs*/
#define MP3_MINI_STREAM_SIZE			0x1000		/*s secs*/

#if 1
//#define WRITE_STREAM_MARGIN			0x1000			/*there must be some margin for wma seek and mp3*/
#define WRITE_STREAM_MARGIN			0x80			/*there must be some margin for wma seek and mp3*/
#else
//if sdram present, it should be larger for cd/mtp mp3 find header
#define WRITE_STREAM_MARGIN			0x4000			/*there must be some margin for wma seek and mp3*/
#endif



U8 *app_sdram_get_volume_lable_name(void);
U8 *app_sdram_get_file_name (U16 index);
U8 *app_sdram_get_folder_name (U16 folder_index);
U16 app_sdram_get_folder_of_entry (U16 index);
U16 app_sdram_get_entry_in_folder (U16 index);
U16 app_sdram_get_folder_entries (U16 index);
void app_sdram_shift_c_disc_info (U16 decode_mode);
U16 app_sdram_buf_flush (U16 decode_mode);
U32 app_sdram_get_stream_buf_len (void);
U8 app_sdram_stream_type_get(U16 entry_index);

BOOL app_sdram_delete_entry (U16 index);
U16 *app_sdram_get_cpy_file_name (U16 index);



enum {
	FILE_AUDIO,
	FILE_PHOTO,
	FILE_RECORD
};

/*size 64 words*/
typedef struct {
	U32 location;
	U32	name_pos;
	U16	index;						/*reindex to NODE for node name sort*/
	U16 parent_no;
	U16 first_entry;
	U16 entries;
} NODE;


//#pragma pack(0)

#endif //__APP_SDRAM_H__
