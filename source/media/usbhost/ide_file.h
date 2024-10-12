#ifndef _APP_FILE_H_
#define _APP_FILE_H_


#define NO_CD_APPLICATION

//#define FILE_SEARCH_TYPE_MOVED_OUTSIDE

//#define FS_TWO_SUPPORT

#define NEW_FS_LIB

#define USB_VOLUME_LABLE_LEN        11

/*
#ifdef _WIN32
#define Little_Endian
#else
#define Big_Endian
#endif
*/


//#define Big_Endian
//Here We suppose the data in buffer is Little Endian data stream ,
//So We should change to the Little Endian order in read sector func of app_flash.c
// if we use Big Endian Processor like Motorola.

//#define FAT32_TEST

#define CLOSED            0
#define OPENED	          1

#ifdef FS_TWO_SUPPORT
/*limitation*/
#define MAX_FILE_DEVICE    2 /*we only support 2 file devices*/
#else
#define MAX_FILE_DEVICE    1 /*we only support 1 file device*/
#endif

#define	MAX_DIRECTOY_DEEP_LENGTH    32

#define MAX_FILE_HANDLE             3

#define MAX_LFN                     255


/*we only support sector size 512 bytes*/
#define SECTOR_SIZE	        512
#define FAT12_MAX_CLUSTER   0xFF5
#define FAT16_MAX_CLUSTER   0xFFF5

/*FDBs in a sector*/
#define FDB_SIZE            32
//#define FDBs_Sector       SECTOR_SIZE / 32


#define MAX_PARTITION_EXTENDED	4


#define ACTICE		 1
#define INACTIVE	 0

#define READ_ONLY	 0
#define WRITE_READ	 1
#define ERASE_WRITE	 2


/*
*MAX_BYTES_PER_SECTOR SUPPORT
*#define MAX_BYTES_PER_SECTOR    4096
*#define MAX_BYTES_PER_SECTOR    2048
*#define MAX_BYTES_PER_SECTOR    1024
*#define MAX_BYTES_PER_SECTOR    512
*/
#define MAX_BYTES_PER_SECTOR    512

#define MIN_BYTES_PER_SECTOR    512

/*
*#define FOLDER_MAX_NUMBER          999
*#define SEARCH_FODLER_MAX_NUMBER   1200
*/
#define FOLDER_MAX_NUMBER          99
#define SEARCH_FODLER_MAX_NUMBER   200


enum {
	NO_ERR,
	DATA_ERR,
	FAT_ERR,
	FDB_ERR,
	MEDIA_ERR,
	MEDIA_FULL,
	FAT_NO_CLUSTER
};


enum {
	enMEDIA_CONTS_UNKNOWN,
	enMEDIA_CONTS_MP3,
	enMEDIA_CONTS_WMA,
	enMEDIA_CONTS_MP3_WMA_MIX,
};


#ifndef FILE_SEARCH_TYPE_MOVED_OUTSIDE
extern U32 usb_media_contents;
extern U32 usb_media_contents_mask;
#endif


typedef struct _DIRECTORY {
	DWORD start_cluster;            /* 0, Boot Root */
	DWORD cur_cluster;
	DWORD FDB_index;                /* index from start cluster */
	DWORD FDB_cur_index;            /* index in current cluster */
} DIRECTORY;



typedef struct {
	BYTE BootIndicator;
	BYTE StartingHead;
	WORD StartingCylinder_Sector;
	BYTE SystemID;
	BYTE EndingHead;
	WORD EndingCylinderSector;
	DWORD RelativeSector;
	DWORD TotalSectors;
} __attribute__ ((packed)) PARTITONTABLE;


struct __iobuf {
	BOOL        state;				/*if it is used*/		
	DIRECTORY   dir;
	BYTE        mode;				/*open mode*/
	DWORD       start_cluster;
	DWORD       lenth;
	DWORD       pos;
	DWORD       current_cluster;
	WORD        sector_index;
	BOOL        modified;
	WORD        fs_index;
	WORD        Error;
};

typedef struct __iobuf MYFILE;


typedef struct {
	DWORD	Sector;						//cash sector address
	WORD	Modified;					//if data in cash has been modified
} FCASH;								//file system cash


typedef struct {
	BOOL Valid;
	WORD  BytesPerSector;	
    DWORD Boot_Sector;
	DWORD Last_Sector;
	DWORD FAT_Sector;
	DWORD Root_Addr;
	DWORD Root_Cluster;
	DWORD Cluster_Sector;
	DWORD Total_Clusters;
    DWORD Total_Sectors;
	DWORD Free_Clusters;

	BYTE Sectors_per_Cluster;
    BYTE Numbers_of_FAT;
    WORD Reserved_Sectors;
    WORD Number_of_Root_Entries;
    DWORD Sectors_of_FAT;
	BYTE FAT_Type;
	BYTE Device_Interface;				/*physical interface*/
	BYTE Media_Type;
		
	DWORD SwapSector;					/*for swap*/
	BYTE DirIndex;

	DIRECTORY Dir[MAX_DIRECTOY_DEEP_LENGTH];

	BYTE FsCashBuf [2*MAX_BYTES_PER_SECTOR];

	FCASH FsCash[2*MAX_BYTES_PER_SECTOR/MIN_BYTES_PER_SECTOR];
	WORD FsCashLRU[2*MAX_BYTES_PER_SECTOR/MIN_BYTES_PER_SECTOR];

} FILE_DEVICE_INFO;


#define FDB_DIR_NAME_LENGTH     11

#define ATTR_READ_ONLY          0x01
#define ATTR_HIDDEN             0x02
#define ATTR_SYSTEM             0x04
#define ATTR_VOLUME_ID          0x08
#define ATTR_DIRECTORY          0x10
#define ATTR_ARCHIVE            0x20
#define ATTR_LONG_NAME          0x0F


typedef struct _FDB {
	BYTE	DIR_Name[FDB_DIR_NAME_LENGTH];
	BYTE	DIR_Attr;
	BYTE	DIR_NTRes;
	BYTE	DIR_CrtTimeTenth;
	WORD	DIR_CrtTime;
	WORD	DIR_CrtDate;
	WORD	DIR_LstAccDate;
	WORD	DIR_FstClusHi;
	WORD	DIR_WrtTime;
	WORD	DIR_WrtDate;
	WORD	DIR_FstClusLo;
	DWORD	DIR_FileSize;
//} FDB;
} __attribute__ ((packed)) FDB;

typedef struct {
	BYTE	dir_sigDB;
	BYTE	dir_lname1[10];
	BYTE	dir_attr;
	BYTE	dir_flags;
	BYTE	dir_chksum;
	BYTE	dir_lname2[12];
	WORD	dir_first;
	BYTE	dir_lname3[4];
} LNDIRENT;



/* Seek method constants */
#define SEEK_CUR    1
#define SEEK_END    2
#define SEEK_SET    0


/*FAT system type*/
#define	FAT12       0
#define FAT16       1
#define FAT32       2


//we suppose the length that caller wanna read is integer multiple of 512.
//we will improve this function to support random length later.
MYFILE *FS_open_fdb(FDB *pFDB, BYTE mode);
MYFILE *app_file_create_file(const U16 *);
MYFILE *app_file_create_voice_file(void);


DWORD FirstSectorofCluster (DWORD n);

extern FILE_DEVICE_INFO FileDevInfo[MAX_FILE_DEVICE];

extern U32 Cur_Parse_Folder_Index;
extern U16 gwEntryNum;
extern U16 gwFolderNum;
extern U16 gwArtistNum;
extern U16 gwAlbumNum;
extern U16 gwGenreNum;


/***********************************************************************************************************************/
enum {
	PARSE_PARTITION,
	PARSE_BOOT,
	PARSE_ROOT,
	PARSE_DIR,
	PARSE_FILE,
	PARSE_FILE_END
};


#define FS_CREATE_FILE_YEAR_OFFSET      1980
#define FS_CREATE_FILE_YEAR             2019
#define FS_CREATE_FILE_MONTH            1
#define FS_CREATE_FILE_DAY              1

#define FS_CREATE_FILE_HOUR             0
#define FS_CREATE_FILE_MINUTE           0
#define FS_CREATE_FILE_SECOND           0

#define LITTLE_DEFALT_CREATE_DATE       0x2134			/*Jan. 1, 2006*/			
#define BIG_DEFALT_CREATE_DATE          0x2134			/*Jan. 1, 2006*/			


#define MP3_EXT_SUPPORT_MASK      0x0001
#define WMA_EXT_SUPPORT_MASK      0x0002
#define AAC_EXT_SUPPORT_MASK      0x0004
#define WAV_EXT_SUPPORT_MASK      0x0008
#define FLAC_EXT_SUPPORT_MASK     0x0010
#define APE_EXT_SUPPORT_MASK      0x0020
#define SBC_EXT_SUPPORT_MASK      0x0040 /*bt*/
#define AC3_EXT_SUPPORT_MASK      0x0080
#define EC3_EXT_SUPPORT_MASK      0x0100
#define DTSHD_EXT_SUPPORT_MASK    0x0200



BOOL Fs_init (int media_index);
WORD Fs_open (int media_index);

void Fs_number_select (int media_index);

BOOL Fs_close (void);
BOOL Fs_fini (void);
long Fs_file_lenth(MYFILE *pFile);
long Fs_file_tell(MYFILE *pFile);
BOOL Fs_file_seek(MYFILE *pFile, DWORD pos);

U32 Fs_read_file(U8 *pBuf, U32 rlen, MYFILE *pFile);
U32 Fs_file_write (U8 *pBuf, U32 len, MYFILE * pFile);
BOOL Fs_change_dir (U16 *pName);
BOOL Fs_rename_file (U16 *sName, U16 *dName);
BOOL Fs_del_file (U16 *pName);
BOOL Fs_create_file(U16 *pName, U8 attr);
MYFILE * Fs_file_open(const U16 *fname, BYTE mode);
BOOL Fs_close_file(MYFILE *pFile);
U16 FS_system_file_init (void);
FDB * Fs_find_file (const U16 *pChar);
FDB * Fs_find_first_file (const U16 *pChar);
U32 Fs_get_last_cluster (void);
U32 Fs_find_max_rec_file (U8 number_start_pos);
U32 Fs_find_max_rec_folder (U8 number_start_pos);

BOOL Fs_set_file_eof (MYFILE * pFile, U32 pos);
BOOL Fs_expand_file (MYFILE * pFile, U32 size);
BOOL Fs_flush_file_buf (MYFILE *pFile);
BOOL Fs_close_drm_file (MYFILE *pFile);
WORD Fs_get_last_err (MYFILE *pFile);
BOOL Fs_del_close_file (MYFILE *pFile);


int Init_Fat32_Sys( void );
int search_one_dir( long index );
int parse_dir( void );
BOOL change_to_subdir( int dir_index );
MYFILE* app_file_open_entry( long file_entry, int mode);
MYFILE* app_file_open_entry_by_cluster( U32 cluster, U32 file_len, int mode);

void Get_FDB_Name ( const FDB * pFDB );
void app_file_search_type (U16 type);
void app_file_search_init (void);

U16 app_file_system_process (void);

void Fs_test (void);


//added for Nx cdda ripping and play music
MYFILE* app_file_open_cluster( long start_cluster, int mode);
//added for Nx cdda ripping and play music
long app_file_set_len ( MYFILE *pWFile, MYFILE *pRFile);
//added for Nx cdda ripping and play music
long app_file_get_start_cluster (MYFILE *pFile);

#ifdef NO_CD_APPLICATION

#define FOLDER_NOT_PARSED	0xFFFF

//usb only


typedef struct _FOLDER_NODE {
	WORD	start_index;					/* folder start index in  SEARCH_FOLDER_INFO*/
	WORD	num;							/* how many folders in the current folder node */
	WORD	index;							/* folder index which has beed parsed in this node */
} __FOLDER_NODE;


#define FOLDER_NODE_DEPTH	64

typedef struct _SEARCH_FOLDER_NODE {
	__FOLDER_NODE node[FOLDER_NODE_DEPTH];
	WORD index;									/* current parsing node */
} SEARCH_FOLDER_NODE;


typedef struct _SEARCH_FOLDER_INFO {

#ifndef ONLY_DIGITAL_DISPLAY	
	DWORD parent_cluster;            /* parent folder start cluster, 0, Boot Root */
#endif //ONLY_DIGITAL_DISPLAY
	
	DWORD cluster;					/* current folder start cluster, to speed up search */
	WORD index;						/* folder number in parent cluster */
	WORD num;						/* total number of music tracks in this folder */

	WORD search_sequence;			/*searching folder*/

#ifdef FOLDER_TRACK_SORTING
	WORD folder_name_pos;					/* folder name str pos in buf */ 
#endif

#ifdef FOLDER_VIEW
	WORD parent_folder_index		 /* index to parent folder */
#endif

} SEARCH_FOLDER_INFO;



typedef struct _FOLDER_INFO {

#ifdef ONLY_DIGITAL_DISPLAY	
	DWORD cluster;					/* folder start cluster, 0, Boot Root */
	WORD num;						 /* total number of music tracks in this folder */
#else
	//text display
	DWORD parent_cluster;            /* parent folder start cluster, 0, Boot Root */
	WORD index;						 /* folder number in parent cluster */
	WORD num;						 /* total number of music tracks inf this folder */

#ifdef FOLDER_VIEW
	WORD parent_folder_index		 /* index to parent folder */
#endif

#endif //ONLY_DIGITAL_DISPLAY
	

} FOLDER_INFO;


#define SEARCH_MP3_MASK         0x0001
#define SEARCH_WMA_MASK         0x0002
#define SEARCH_WAV_MASK         0x0004

#define SEARCH_AAC_M4A_MASK     0x0008
#define SEARCH_AAC_ADTS_MASK    0x0010
#define SEARCH_AAC_ADIF_MASK    0x0020
#define SEARCH_AAC_LOAS_MASK    0x0040

#define SEARCH_FLAC_MASK        0x0080
#define SEARCH_APE_MASK         0x0100

#define SEARCH_AAC_RAW_MASK     0x0200
#define SEARCH_SBC_MASK         0x0400

#define SEARCH_AC3_MASK         0x0800

#define SEARCH_EC3_MASK         0x1000
#define SEARCH_DTSHD_MASK       0x2000


#define SEARCH_ALL_FOLDER           1
#define SEARCH_EXCEPT_REC_FOLDER    2
#define SEARCH_REC_FOLDER           3


extern FOLDER_INFO folder_info [FOLDER_MAX_NUMBER];

extern SEARCH_FOLDER_INFO *gpSearchFielInfo;
extern SEARCH_FOLDER_NODE *gpSearchFolderNode;

#ifdef FOLDER_TRACK_SORTING
#define MAX_TRACK_NUM                  9999
#define MAX_FLODER_TRACK_NAME_LEN      0x1000 //0x4000 //32k byte
extern U16 *gpFolderTrackName;         //[MAX_FLODER_TRACK_NAME_LEN];
extern U16 track_info[MAX_TRACK_NUM];  //index in one folder, start from 1
extern U16 *track_name_pos;            //[MAX_TRACK_NUM]; //for temp sorting in one folder
#endif



int app_file_search_folder (U32 cluster, U32 index, U16 *folder_name, U16 folder_name_len, U32 *folder_cluster);

//add for PANASONIC
int app_file_search_folder_time (U32 cluster, U32 index, U16 *folder_name, U16 folder_name_len, U32 *folder_cluster, 
								 u16 *cdate, u16 *ctime);

#ifndef NEW_FS_LIB
int app_file_search_file (U32 cluster, U32 index, U16 *file_name, U16 file_name_len, U16 *file_type, U32 *file_cluster, U32 *file_len);
#else
int app_file_search_file (U32 cluster, U32 index, U16 *file_name, U16 file_name_len, U16 *file_type, 
                                U32 *file_cluster, U32 *file_len, U16 *cdate, U16 *ctime);
#endif

int app_file_delete_file(U32 cluster, U32 index);

//add for PANASONIC
int app_file_search_folder_time (U32 cluster, U32 index, U16 *folder_name, U16 folder_name_len, U32 *folder_cluster, 
								 U16 *cdate, U16 *ctime);
//add for PANASONIC
U32 Fs_search_dir_all_files(void);
//add for PANASONIC
U32 Fs_search_dir_all_folders(void);
//add for PANASONIC
U16 Fs_search_record_folder_all_files(void);
//add for PANASONIC
void Fs_delete_subdir_all_files(U32 subdir_cluster);
//add for PANASONIC
int app_delete_RECORD_folder(void);



//get the track number in a folder.
U16 app_file_get_entry_in_folder (U16 track_index);

//get the folder number of the track number.
U16 app_file_get_folder_of_entry (U16 track_index);

//get the total track number in a folder.
U16 app_file_get_first_entrie_of_folder (U16 folder_index);

U16 app_file_get_folder_entries (U16 folder_index);

void app_file_serach_set (U16 search_mask);
void app_file_search_floder_set(unsigned char seek_flag);
void app_file_record_floder_as_root_set(unsigned char con);


BOOL change_to_subdir_by_addr ( U32 addr );

void app_usb_vlome_lable_name_init(void);
void *app_usb_get_vlome_lable_name(void);


int Fs_rename_file_by_index(U32 cluster, U32 index,U16 *dName);
int Fs_rename_folder_by_index(U32 cluster, U32 index,U16 *dName);

int app_file_del_file_from_folder_update (u16 fold);
void app_usb_vlome_lable_name_init(void);

U32 app_file_get_continous_free_space (U32 size);
void app_file_search_free_space_set(U8 con);

#endif

#endif //_APP_FILE_H_
