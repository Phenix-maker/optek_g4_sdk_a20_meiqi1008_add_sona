#include "c_def.h"  
#include "debug.h"
#include "oem.h"

#include "app_sdram.h"
#include "app_main.h"
#include "id3v2.h"
#include "string.h"
#include "wstring.h"
#include "ide_file.h"
#include "codec.h"
#include "tools.h"

#include "app_media_com.h"

#ifdef USB_HOST_ENABLE
//#define SLOW_MEMORY_USED


#if 1//def MP3CD

typedef struct{
	char Header[3];     /*±ØÐëÎª "ID3" ·ñÔòÈÏÎª±êÇ©²»´æÔÚ*/
	char Ver;           /*ID3V2°æ±¾ºÅ£¬ID3V2.3 ¾Í¼ÇÂ¼3*/
	char Revision;      /*¸±°æ±¾ºÅ£¬Ò»°ãÎª0*/
	char Flag;          /*´æ·Å±êÖ¾×Ö½Ú£¬Ò»°ãÈ«²¿Îª0*/
	char Size[4];       /*ËùÓÐ±êÇ©´óÐ¡£¬²»°üÀ¨±êÇ©Í·µÄ10¸ö×Ö½Ú*/
} ID3V2_TAG;

typedef struct{
	char FrameID[3];         /*ÓÃÈý¸ö×Ö·û±êÊ¾Ò»¸öÖ¡ID£¬±ÈÈç TT2(¸èÇúÃû)£¬TP1(ÒÕÊõ¼Ò)*/
	char Size[3];            /*Ö¡ÌåÄÚÈÝ´óÐ¡£¬²»°üº¬Ö¡ID£¬²»µ½Ð¡ÓÚ1*/
	//char FrameCont_encode; /*Ö¡Ìå×Ö·ûËùÓÃ±àÂë*/
} ID3V22_TAG;


/*
ID3V2.2 ³£ÓÃÖ¡±êÊ¾
	TT2:   ±êÌâ               	TP1:     ÒÕÊõ¼Ò
	TAL:   ×¨¼­Ãû            	TRK:     Òô¹ìºÅ/ºÏ¼ÆÒô¹ì
	TYE:   ·¢ÐÐÈÕÆÚ     	COM:     ×¢ÊÍ
	TEN:   ±àÂë·½Ê½          TCO:     Á÷ÅÉ


FrameCont_encode ÓÐ4¸öÖµ;
	0000 ´ú±íÖ¡ÌåÓÃ ISO-8859-1 ±àÂë·½Ê½´æ´¢£»
	0001 ´ú±íÖ¡ÌåÓÃ UTF-16LE   ±àÂë·½Ê½´æ´¢£»
	0002 ´ú±íÖ¡ÌåÓÃ UTF-16BE   ±àÂë·½Ê½´æ´¢£»
	0003 ´ú±íÖ¡ÌåÓÃ UTF-8      ±àÂë·½Ê½´æ´¢£»[Ö»ÓÃID3V2.4²ÅÖ§³ÖUTF-8±àÂë·½Ê½]
*/


typedef enum{
	ENCODE_ISO_8859,
	ENCODE_UTF_16LE,
	ENCODE_UTF_16BE,
	ENCODE_UTF_8
}ENCODE_TYPE;

#define MAX_FRAME_CONTENT    256 /*only 256 bytes be processed*/


typedef	struct _ID3v2_header
{
	U8	version;	
	U8	flag;
	long	size;
	long	exthead_size;
}	ID3v2_HEADER;

#define	FRAME_FORMAT_GROUPID	0x40
#define	FRAME_FORMAT_COMPRESS	0x08
#define	FRAME_FORMAT_ENCRYPT	0x04
#define	FRAME_FORMAT_UNSYNCH	0x02
#define	FRAME_FORMAT_DATALEN	0x01			

#define	ID3v2_FLAG_UNSYNCHRONIZATION	(1 << 7)
#define	ID3v2_FLAG_EXTHEADER			(1 << 6)
#define	ID3v2_FLAG_EXPERIMENTAL			(1 << 5)
#define	ID3v2_FLAG_FOOTER				(1 << 4)

#define ID3V2_HEADER_SIZE_UNKNOWN	0xFFFFFFFF

#define	MAXIM_FRAME_ID				7	

#ifndef NELEMS
#define NELEMS(arr) (sizeof(arr) / sizeof(arr[0]))
#endif

const U8 ID3v2_Frame_ID[MAXIM_FRAME_ID][5] =
{
	{"TIT2"},
	{"TPE1"},
	{"TALB"},
	{"SEEK"},
	{"TCON"},
	{"TYER"},
	{"TDRC"}
};

typedef	enum
{
	ID3_FRAME_TIT2		=	0,		// title
	ID3_FRAME_TPE1,					// artist
	ID3_FRAME_TALB,					// album
	ID3_FRAME_SEEK,

	ID3_FRAME_TCON,					// genre
	ID3_FRAME_TYER,					// year, in V2.3
	ID3_FRAME_TDRC,					// year in V2.4

	ID3_FRAME_PADDING	=	0xFE,
	ID3_FRAME_OTHER		=	0xFF
}	ID3_FRAME_RETURN;


ID3_FRAME_RETURN ID3_frameID_check(U8 * frame_buf )
{
	int i,j;
	U8 * ps;
	const U8* pt;
	
	if( frame_buf[0] == 0 ) {
		return ID3_FRAME_PADDING;
	}

	for ( i =0; i < MAXIM_FRAME_ID; i ++ ) {
		ps = frame_buf;
		pt = ID3v2_Frame_ID[i];

		for ( j =0; j < 4; j ++ ) {
			if( *ps ++ != *pt ++ ) {
				break;
			}
		}
		if ( j == 4 ) {
			return (ID3_FRAME_RETURN)i;
		}
	}

	return ID3_FRAME_OTHER;
}

long Synsafe_4byte( U8 *buf )
{
	return  ( ( ((long) buf[3]) )  + (((long) buf[2]) << 7 ) 
		+ ( ((long) buf[1]) << 14 ) 
		+ ( ((long) buf[0]) << 21 ) );	
}

void ID3v2_frame_unsynchronization(U8 *buf, long len)
{
	int i,k;
	
	i = 0;
	k = 0;

	while (i < len) {

		if ( (buf[i] == 0xFF) && (buf[i+1] == 0x0) ) {
			if ( (buf[i+2] & 0xE0) != 0 ) {
				buf[k++] = buf[i++];
				i++;						/*pass unsync byte*/
			}
			else if (buf [i+2] == 0 ) {
				buf[k++] = buf[i++];
				i++;						/*pass unsync byte*/
			}
			else {
				buf[k++] = buf[i++];			
			}
		}
		else {
			buf[k++] = buf[i++];			
		}
	}
}

void ID3v2_init(ID3_TAG *id3_tag)
{
/*
	ID3v2_header.version		= 0;
	ID3v2_header.size			= ID3V2_HEADER_SIZE_UNKNOWN;
	ID3v2_header.flag			= 0;
	ID3v2_header.exthead_size	= 0;
*/

	id3_tag->artist[0] = 0x0;
	id3_tag->artist[1] = 0x0;
	id3_tag->title[0] = 0x0;
	id3_tag->title[1] = 0x0;

	id3_tag->album[0] = 0x0;
	id3_tag->album[1] = 0x0;				//unicode null
#ifdef ID3ALL
	id3_tag->genre[0] = 0x0;
	id3_tag->genre[1] = 0x0;
	id3_tag->year[0] = 0x0;
	id3_tag->year[1] = 0x0;
#endif
}
  
static void Id3v2_parse_content (U8 encoding, U8 *pFrame, U16 len, U16 * pBuf, U16 max_wlen)
{
	U16 tmp,i;
	U16 *pWord;

	if (encoding == ENCODE_UTF_16LE)
	{
		DBG_Printf ("16 LE Tag\n\r");

		tmp = *((U16 *) pFrame);
		pFrame += 2;
		pWord = (U16 *) pFrame;
		len -= 2;

		//len = wstrlen (pWord);

		//DBG_Printf ("content len %d\n\r", len);

		if (len > 2 * max_wlen)
			len = 2 * max_wlen;

		if (tmp == 0xFFFE) {
			//Little Endian

			for (i=0; i< len/2; i++) {
#ifdef LITTLE_ENDIAN
				pBuf[i] = pWord[i];
#else
				pBuf[i] = w_Little_Big_Endian(&pWord[i]);
#endif
			}
			

			pBuf[i] = 0x0;

			//DBG_Printf ("%s\n\r", pBuf);

		}
		else {
			//Big Endian
			for (i=0; i< len/2; i++) {
#ifdef LITTLE_ENDIAN
				pBuf[i] = w_Little_Big_Endian(&pWord[i]);
#else
				pBuf[i] = pWord[i];
#endif
			}

			pBuf[i] = 0x0;

		}

		wstrCutLeadingSpace (pBuf);
		wstrCutTrailingSpace (pBuf);

		//_unicode_to_oem (pBuf, pBuf);
	}
	else if (encoding == ENCODE_UTF_16BE) 
	{
		DBG_Printf ("16 BE Tag\n\r");

		pWord = (U16 *) pFrame;

		//len = wstrlen (pWord);
		if (len > 2 * max_wlen)
			len = 2 * max_wlen;
						
		for (i=0; i< len/2; i++) {
#ifdef LITTLE_ENDIAN
			pBuf[i] = w_Little_Big_Endian(&pWord[i]);
#else
			pBuf[i] = pWord[i];
#endif
		}

		pBuf[i] = 0x0;

		wstrCutLeadingSpace (pBuf);
		wstrCutTrailingSpace (pBuf);

		//_unicode_to_oem (pBuf, pBuf);

	}
	else //if( (encoding == ENCODE_ISO_8859) ||(encoding == ENCODE_UTF_8) )
	{
		//len = strlen ((const char *) pFrame);

		if (len > max_wlen) {
			len = max_wlen;
		}

		pFrame[len] = 0x0;

		strCutLeadingSpace (pFrame);
		strCutSpace (pFrame);
		oem_to_unicode (pFrame,pBuf);

	}
}

#define ID3V2_ARTIST_FOUND		0X01
#define ID3V2_TITLE_FOUND		0X02
#define ID3V2_ALBUM_FOUND		0X04

#ifdef ID3ALL
#define ID3V2_GENRE_FOUND		0X08
#define ID3V2_YEAR_FOUND		0X10
#else
#define ID3V2_GENRE_FOUND		0
#define ID3V2_YEAR_FOUND		0
#endif

#define ID3V2_ALL_FOUND	(ID3V2_ARTIST_FOUND|ID3V2_TITLE_FOUND|ID3V2_ALBUM_FOUND|ID3V2_GENRE_FOUND|ID3V2_YEAR_FOUND)

typedef struct{
	char id[3];
	ID3_FRAME_RETURN type;
}ID3V22_TAB;
static const ID3V22_TAB id3v22_id_tab[] =
{
	{'T','T','2',	ID3_FRAME_TIT2}, //title
	{'T','P','1',	ID3_FRAME_TPE1}, //artist
	{'T','A','L',	ID3_FRAME_TALB}, //album
	{'T','C','O',	ID3_FRAME_TCON}, //genre
	{'T','Y','E',	ID3_FRAME_TYER}, //year
};

static ID3_FRAME_RETURN ID3v22_frameID_check(U8 * frame_buf )
{
	U8 i;
	const ID3V22_TAB *id3v22 = id3v22_id_tab;
	
	if( frame_buf[0] == 0 ) {
		return ID3_FRAME_PADDING;
	}

	for ( i =0; i < NELEMS(id3v22_id_tab); i ++,id3v22++ )
	{
		if((frame_buf[0] == id3v22->id[0])&&
			(frame_buf[1] == id3v22->id[1])&&
			(frame_buf[2] == id3v22->id[2]))
		{
			return id3v22->type;
		}
	}

	return ID3_FRAME_OTHER;
}



static ID3_RETURN ID3v2_parse( ID3_TAG *id3_tag, MYFILE *pFile )
{
	STREAM_ID s;
	long len, rlen, size, frame_size;
	long frame_start_pos;
	U8 *pFrame;
	U16 frame_flag;
	ID3_FRAME_RETURN ftype;
	U8 read, find = 0;
	U8 encoding;
	
	//U32 mem_used = codec_get_mem_used();
	U8 *stream_buf;
	U8 *m_buf;

	codec_malloc_init(codec_fast_mem, CODEC_FAST_MEM_SIZE, codec_slow_mem, CODEC_SLOW_MEM_SIZE);

	#ifdef SLOW_MEMORY_USED
	stream_buf = (U8 *)codec_malloc(MAX_FRAME_CONTENT);
	#else
	stream_buf = (U8 *)codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, MAX_FRAME_CONTENT, CODEC_MEM_ALIGN_4);
	#endif
	
	#ifdef SLOW_MEMORY_USEDpc
	m_buf = (U8 *)codec_malloc(10);
	#else
	m_buf = (U8 *)codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, 10, CODEC_MEM_ALIGN_4);
	#endif
	
	#ifdef SLOW_MEMORY_USED
	ID3v2_HEADER *ID3v2_header = (ID3v2_HEADER *)codec_malloc(sizeof(ID3v2_HEADER));
	#else
    ID3v2_HEADER *ID3v2_header = (ID3v2_HEADER *)codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, sizeof(ID3v2_HEADER), CODEC_MEM_ALIGN_4);
	#endif

	len = xa_fread (m_buf, 1, 10, pFile);
	DBG_Assert (len == 10);

	if ( (m_buf[0] != 'I') || (m_buf[1] != 'D') || (m_buf[2] != '3') )
	{
		/*no id3 v2 tag present*/
		//codec_set_mem_used(mem_used);

		xa_fseek (pFile, 0, SEEK_SET);
		app_media_data.playing_stream_media_offset = 0;
		return NO_ID3_TAG;
	}

//---------------------------------------------------	/*id3 v2 tag present*/
	ID3v2_header->version	=	m_buf[3];
	ID3v2_header->flag		=	m_buf[5];
	ID3v2_header->size		=	Synsafe_4byte( &m_buf[6] );
	DBG_Printf ("ID3 TAG V2.%d\n\r", ID3v2_header->version);

	app_media_data.playing_stream_media_offset = (ID3v2_header->size + 10);


//---------------------------------------------------	/*id3 v2.2 present*/
	if( ID3v2_header->version < 2 || ID3v2_header->version > 4 )
	{
		//codec_set_mem_used(mem_used);
		xa_fseek (pFile, app_media_data.playing_stream_media_offset, SEEK_SET);
		return NO_ID3_TAG;
	}
//---------------------------------------------------	/*	jump pass the extended header */
	if ( ID3v2_header->flag & ID3v2_FLAG_FOOTER )
	{
		//write id3v2.4 footer here laser
	}

	if ( ID3v2_header->flag & ID3v2_FLAG_EXTHEADER )
	{
		len = xa_fread (m_buf, 1, 4, pFile);
		DBG_Assert (len == 4);

		ID3v2_header->exthead_size =  Synsafe_4byte(m_buf);
		frame_start_pos = 10 + ID3v2_header->exthead_size;

		xa_fseek(pFile, frame_start_pos, SEEK_SET);
	}
	else
	{
		frame_start_pos = 10;
		ID3v2_header->exthead_size =	0;
	}
//---------------------------------------------------
//	process frames , note all frames are restricted to <= 64 bytes
	size = ID3v2_header->size - ID3v2_header->exthead_size;
	read = (ID3v2_header->version==2) ? sizeof(ID3V22_TAG) : 10;

	while( (size > 0) && (find != ID3V2_ALL_FOUND))
	{
		len = xa_fread (m_buf, 1, read, pFile);
		DBG_Assert (len == read);

		if (ID3v2_header->version == 2)
			ftype = (U8) ID3v22_frameID_check(m_buf);
		else
			ftype = (U8) ID3_frameID_check(m_buf);

		if (ftype == ID3_FRAME_PADDING)
		{
			break;
		}
		else 
		{
			if (ID3v2_header->version == 2)
			{
				frame_size = m_buf[3]<<16 | m_buf[4]<<8|m_buf[5];
				frame_flag = 0;
			}
			else
			{
				// frame size is exclude header
				frame_size = m_buf[4]<<24 | m_buf[5]<<16 | m_buf[6]<<8 | m_buf[7];
				frame_flag = m_buf[8]<<8 | m_buf[9];
			}

			if ( frame_size > (size - read) ) {
				/*in error condition*/
				frame_size = size - read;
			}

			size -= (frame_size + read );			
			frame_start_pos += (frame_size + read );			/*next frame start pos*/
			
			if ( ftype == ID3_FRAME_OTHER )
			{
			}
			else if ( ftype == ID3_FRAME_SEEK )
			{
				//seek frames, later process
				break;
			}
			else if (frame_size > 0)
			{
				rlen = (frame_size > MAX_FRAME_CONTENT) ? MAX_FRAME_CONTENT: frame_size;	
				pFrame = (U8 *) stream_buf;

				xa_fread (&encoding, 1,1, pFile);
				rlen--;

				len = xa_fread (pFrame, 1,rlen, pFile);
				DBG_Assert (len == rlen);
				
				pFrame[len] = 0x0;		/*add null*/

				if( ( ID3v2_header->flag & ID3v2_FLAG_UNSYNCHRONIZATION )|| ( frame_flag & FRAME_FORMAT_UNSYNCH ))
				{
					ID3v2_frame_unsynchronization ((U8 *) pFrame, rlen);
				}

				//DBG_Printf ("encoding %d len %d\n\r", encoding, rlen);


				switch ( ftype )
				{
				case ID3_FRAME_TIT2:
					Id3v2_parse_content(encoding, pFrame,len, id3_tag->title, MAX_ID3_TEXT);
					//DBG_Printf ("T%s\n\r", id3_tag->title);

					find |= ID3V2_TITLE_FOUND;
					break;

				case ID3_FRAME_TPE1:
					Id3v2_parse_content(encoding, pFrame, len, id3_tag->artist, MAX_ID3_TEXT);
					//DBG_Printf ("A%s\n\r", id3_tag->artist);
					find |= ID3V2_ARTIST_FOUND;
					break;

				case ID3_FRAME_TALB:
//#ifdef ID3ALL
					Id3v2_parse_content(encoding, pFrame,len, id3_tag->album, MAX_ID3_TEXT);
					//DBG_Printf ("Album%s\n\r", id3_tag->album);
					find |= ID3V2_ALBUM_FOUND;
//#endif
					break;
				
				// genre
				// V2.3 is different from V2.4
				case ID3_FRAME_TCON:
#ifdef ID3ALL
					Id3v2_parse_content(encoding, pFrame, id3_tag->genre, MAX_ID3_GENRE_TEXT);
					find |= ID3V2_GENRE_FOUND;
//change (13) to Pop
					{
						int len = wstrlen(id3_tag->genre);

						if ( (id3_tag->genre[0] == '(') && (id3_tag->genre[len-1] == ')') )
						{
							int i,genre;
							U16 *p = id3_tag->genre +1;
							
							for(genre=0,i=0;i<len-2;i++,p++)
							{
								genre = genre*10 + (*p-'0');
							}

							if (genre >= KNOWN_MP3_GENRES) 
								id3_tag->genre[0] = 0x0;
							else 
								oem_to_unicode (mp3_genre[genre], id3_tag->genre);
						}
					}
#endif
					break;

				// year for V2.3
				case ID3_FRAME_TYER:
				// year for V2.4
				case ID3_FRAME_TDRC:
#ifdef ID3ALL
					Id3v2_parse_content(encoding, pFrame, id3_tag->year, MAX_ID3_YEAR);
					find |= ID3V2_YEAR_FOUND;
#endif
					break;

				default:
					DBG_Assert(0);
				}
			}
			
			xa_fseek (pFile, frame_start_pos, SEEK_SET);
			//app_main_data.playing_stream_media_offset = frame_start_pos;
		}
	}
	
	//codec_set_mem_used(mem_used);
	xa_fseek (pFile, app_media_data.playing_stream_media_offset, SEEK_SET);
	return ID3_TAG_OK;
}

ID3_RETURN ID3v2_process( ID3_TAG *id3_tag )
{
	return ID3v2_parse(id3_tag, pMediaRead);
}

/*********************************************************
	ID3 V1 Genre function
*********************************************************/

#define	KNOWN_MP3_GENRES	148

const I8 * const mp3_genre[KNOWN_MP3_GENRES] = {
	"Blues", "Classic Rock", "Country", "Dance", "Disco", "Funk",
	"Grunge", "Hip-Hop", "Jazz", "Metal", "New Age", "Oldies",
	"Other", "Pop", "R&B", "Rap", "Reggae", "Rock",
	"Techno", "Industrial", "Alternative", "Ska", "Death Metal", "Pranks",
	"Soundtrack", "Euro-Techno", "Ambient", "Trip-Hop", "Vocal", "Jazz+Funk",
	"Fusion", "Trance", "Classical", "Instrumental", "Acid", "House",
	"Game", "Sound Clip", "Gospel", "Noise", "AlternRock", "Bass",
	"Soul", "Punk", "Space", "Meditative", "Instrumental Pop", "Instrumental Rock",
	"Ethnic", "Gothic", "Darkwave", "Techno-Industrial", "Electronic", "Pop-Folk",
	"Eurodance", "Dream", "Southern Rock", "Comedy", "Cult", "Gangsta",
	"Top 40", "Christian Rap", "Pop/Funk", "Jungle", "Native American", "Cabaret",
	"New Wave", "Psychadelic", "Rave", "Showtunes", "Trailer", "Lo-Fi",
	"Tribal", "Acid Punk", "Acid Jazz", "Polka", "Retro", "Musical",
	"Rock & Roll", "Hard Rock", "Folk", "Folk/Rock", "National Folk", "Swing",
	"Fast-Fusion", "Bebob", "Latin", "Revival", "Celtic", "Bluegrass", "Avantgarde",
	"Gothic Rock", "Progressive Rock", "Psychedelic Rock", "Symphonic Rock", "Slow Rock", "Big Band",
	"Chorus", "Easy Listening", "Acoustic", "Humour", "Speech", "Chanson",
	"Opera", "Chamber Music", "Sonata", "Symphony", "Booty Bass", "Primus",
	"Porn Groove", "Satire", "Slow Jam", "Club", "Tango", "Samba",
	"Folklore", "Ballad", "Power Ballad", "Rhythmic Soul", "Freestyle", "Duet",
	"Punk Rock", "Drum Solo", "A capella", "Euro-House", "Dance Hall",
	"Goa", "Drum & Bass", "Club House", "Hardcore", "Terror",
	"Indie", "BritPop", "NegerPunk", "Polsk Punk", "Beat",
	"Christian Gangsta", "Heavy Metal", "Black Metal", "Crossover", "Contemporary C",
	"Christian Rock", "Merengue", "Salsa", "Thrash Metal", "Anime", "JPop",
	"SynthPop",
};

U8 * get_genre_info( int genre)
{
	if( genre < 0 || genre >= KNOWN_MP3_GENRES ) {
		return 0;
	}
	else {
		return (U8 *) mp3_genre[genre] ;
	}
}

typedef struct {
	BYTE	tag [3];
	BYTE	title [30];
	BYTE	artist [30];
	BYTE	album [30];
	BYTE	year[4];
	BYTE	comment[30];
	BYTE	genre;
} ID3V1_TAG;
#define ID3V1_READ_SIZE		128

static ID3_RETURN id3v1_parser (U8 *buf, ID3_TAG *id3_tag)
{
	ID3V1_TAG *pId3v1_tag;
	int genre;
	int i;

	id3_tag->artist[0] = 0x0;
	id3_tag->artist[1] = 0x0;
	id3_tag->title[0] = 0x0;
	id3_tag->title[1] = 0x0;

	id3_tag->album[0] = 0x0;
	id3_tag->album[1] = 0x0;

#ifdef ID3ALL
	id3_tag->genre[0] = 0x0;
	id3_tag->genre[1] = 0x0;
	id3_tag->year[0] = 0x0;
	id3_tag->year[1] = 0x0;
#endif

	for(i=0; i<= ID3V1_READ_SIZE-sizeof(ID3V1_TAG); i++)
	{
		if ( (buf[i] == 0x54) && (buf[i+1] == 0x41) && (buf[i+2] == 0x47) )
		{
			/* TAG */
			pId3v1_tag = (ID3V1_TAG *) &buf[i];
			strncpy ( (I8 *) id3_tag->title, (const I8 *) pId3v1_tag->title, 30);
			id3_tag->title [15] = 0x0;									//word
			strCutSpace ((U8 *) id3_tag->title);
			strAddUnicodeNull ((U8 *) id3_tag->title);
			
			strncpy ( (I8 *) id3_tag->artist, (const I8 *) pId3v1_tag->artist, 30);
			id3_tag->artist [15] = 0x0;									//word
			strCutSpace ((U8 *) id3_tag->artist);
			strAddUnicodeNull ((U8 *) id3_tag->artist);

			strncpy ((I8 *) id3_tag->album, (const I8 *)pId3v1_tag->album, 30);			
			id3_tag->album [15] = 0x0;									//word
			strCutSpace ((U8 *) id3_tag->album);
			strAddUnicodeNull ((U8 *) id3_tag->album);

#ifdef ID3ALL
			strncpy ((I8 *) id3_tag->year, (const I8 *)pId3v1_tag->year, 4);			
			id3_tag->year [2] = 0x0;										//word
			strCutSpace ((U8 *) id3_tag->year);
			strAddUnicodeNull ((U8 *) id3_tag->year);

			genre = pId3v1_tag->genre;

			if (genre >= KNOWN_MP3_GENRES) {
				id3_tag->genre[0] = 0x0;
				id3_tag->genre[1] = 0x0;
			}
			else {
				strncpy ((I8 *)id3_tag->genre, (const I8 *) mp3_genre[genre], 2*MAX_ID3_GENRE_TEXT);
				id3_tag->genre [MAX_ID3_GENRE_TEXT] = 0x0;				//word
			}

			strAddUnicodeNull ((U8 *) id3_tag->genre);
#endif

			return ID3_TAG_OK;
		}
	}

	return NO_ID3_TAG;
}


ID3_RETURN ID3v1_process (ID3_TAG *id3_tag)
{
#if 1
	U8 *buf;
	int ret, i, j;
	MYFILE *pFile = pMediaRead;

	codec_malloc_init(codec_fast_mem, CODEC_FAST_MEM_SIZE, codec_slow_mem, CODEC_SLOW_MEM_SIZE);

#if 1
	
	#ifdef SLOW_MEMORY_USEDpc
	buf = (U8 *)codec_malloc(0x4096);
	#else
	buf = (U8 *)codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, 0x4096, CODEC_MEM_ALIGN_4);
	#endif

	DBG_Assert (buf != NULL);

	if (buf == NULL)
	{
		DBG_Puts("malloc err\n\r");
	}

	ret = xa_fseek (pFile, 4096, 2); //2 SEEK_END
	if (ret == -1)
	{
		return NO_ID3_TAG;
	}
	
	ret = xa_fread (buf, 1, 4096, pFile);
	if(ret == STREAM_UNDERFLOW)
	{
		DBG_Puts("xa_fread err\n\r");
	}

	ret = NO_ID3_TAG;

	for (i=0; i <(4096 - 3); i++)
	{
		//seek the "TAG" sring
		if ( (buf[i] == 0x54) && (buf[i+1] == 0x41) && (buf[i+2] == 0x47) )
		{
			ret = id3v1_parser (&buf[i], id3_tag);	

			if (ret == ID3_TAG_OK)
			{
				break;
			}
		}
	}

#else

	#ifdef SLOW_MEMORY_USEDpc
	buf = (U8 *)codec_malloc(2048+8);
	#else
	buf = (U8 *)codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, 2048+8, CODEC_MEM_ALIGN_4);
	#endif

	DBG_Assert (buf != NULL);

	if (buf == NULL)
	{
		DBG_Puts("malloc err\n\r");
	}

	ret = xa_fseek (pFile, 4096, 2); //2 SEEK_END
	if (ret == -1)
	{
		return NO_ID3_TAG;
	}
	
	ret = NO_ID3_TAG;

	xa_fread (buf, 1, 2048+1, pFile);
	for (i=0; i <(2048+1-2); i++)
	{
		if ( (buf[i] == 0x54) && (buf[i+1] == 0x41) && (buf[i+2] == 0x47) )
		{
			ret = id3v1_parser (&buf[i], id3_tag);	
		}
	}
	
	xa_fread (buf+2, 1, 2048-1, pFile);
	for (i=0; i <(2048-1-2); i++)
	{
		if ( (buf[i] == 0x54) && (buf[i+1] == 0x41) && (buf[i+2] == 0x47) )
		{
			ret = id3v1_parser (&buf[i], id3_tag);	
		}
	}
#endif

	xa_fseek (pFile, 0, 0);				//jump to the file start pos

	return ret;

#else

	U8 *buf;
	ID3_RETURN r;
	U32 len;

	codec_malloc_init ();
	
	#ifdef SLOW_MEMORY_USEDpc
	buf = (U8 *)codec_malloc(ID3V1_READ_SIZE);
	#else
	buf = (U8 *)codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, ID3V1_READ_SIZE, CODEC_MEM_ALIGN_4);
	#endif

	DBG_Assert (buf != NULL);

	if (buf == NULL)
	{
		DBG_Puts("malloc err\n\r");
	}

	app_nav_lock_stream_mutex ();//force the hdd to idle state
				
	Fs_file_seek(pMediaRead , pMediaRead->lenth - ID3V1_READ_SIZE);
	len = Fs_read_file(buf, ID3V1_READ_SIZE, pMediaRead);

	app_nav_unlock_stream_mutex ();

	r = id3v1_process1 (buf, id3_tag);

//back to the stream start pos
#ifdef FOR_ESD_PROTECT
	if (resume_trk_info.need_resume_play)
	{
		app_cmd_jmp(resume_trk_info.resume_pos);
	}
	else
	{
#endif
		app_cmd_jmp(0);
	}

	return r;
#endif
}



#if 0

typedef struct {
	U64	ID[2];
	U64	Size;
} sObjGUID;

/*
typedef struct {
	sObjGUID GUID;
	U32  NumberofHeaderObjects;
	U16   Reserved;
	
} __attribute__((packed)) sHeaderObject;
*/

typedef struct {
	U8	ObjectID[16];
	U32	ObjectSize[2];
	U32  NumHeadObjects;
	U8   Rev[2];
} __attribute__((packed)) sHeaderObject;


typedef struct {
	U8	ObjectID[16];
	U32	ObjectSize[2];
} __attribute__((packed)) sObject;

typedef struct {
	U8	ObjectID[16];
	U32	ObjectSize[2];
	U16	Title_Len;
	U16 Author_Len;
	U16 Copyright_Len;
	U16 Discription_Len;
	U16 Rating_Len;
} __attribute__((packed)) sContentObject;

typedef struct {
	U8	ObjectID[16];
	U32	ObjectSize[2];
	U16	ContentCount;
} __attribute__((packed)) sExtendContentObject;


#define WM_GENRE_LEN	18
#define WM_ALBUM_LEN	28
	
//static const U16 WM_Genre[WM_GENRE_LEN] = {'W'<<8,'M'<<8,0x2F<<8,'G'<<8,'e'<<8,'n'<<8,'r'<<8,'e'<<8,0x0};
//static const U16 WM_Album[WM_ALBUM_LEN] = {'W'<<8,'M'<<8,0x2F<<8,'A'<<8,'l'<<8,'b'<<8,'u'<<8,'m'<<8,'T'<<8,'i'<<8,'t'<<8,'l'<<8,'e'<<8,0x0};

static const U8 ASF_Header_Object[]={0x30,0x26,0xB2,0x75,0x8E,0x66,0xCF,0x11,0xA6,0xD9,0x00,0xAA,0x00,0x62,0xCE,0x6C};
static const U8 ASF_Content_Description_Object[]={0x33,0x26,0xB2,0x75,0x8E,0x66,0xCF,0x11,0xA6,0xD9,0x00,0xAA,0x00,0x62,0xCE,0x6C};
static const U8 ASF_Extended_Content_Description_Object[]={0x40,0xA4,0xD0,0xD2,0x07,0xE3,0xD2,0x11,0x97,0xF0,0x00,0xA0,0xC9,0x5E,0xA8,0x50};
static const U8 ASF_File_Properties_Object[]={0xa1,0xdc,0xab,0x8c,0x47,0xa9,0xCF,0x11,0x8e,0xe4,0x00,0xC0,0x0C,0x20,0x53,0x65};


static U16 GUID_compare (const U8 *pGuid, U8 *pBuf)
{
	U16 i,ret = TRUE;

	for (i=0; i<16; i++) {
		if (pGuid[i] != pBuf[i]) {
			ret = FALSE;
			break;
		}	
	}

	return ret;
}


ID3_RETURN WMA_ID3_parse(U16 *total_time, ID3_TAG *id3_tag, MYFILE *pFile )
{
	U32	i,k,len;
	U8 *m_buf;
	U16 m_buf16[64]; /* 64 bytes */
	int ret,j;
	U16	wHeaderObjectHits;
	U16 Hits;


	U32 dwHeaderLen,dwHeaderObjectPos,dwHeaderObjectLen,dwNumberofHeaderObjects;

	DBG_Assert (pFile!= NULL);

	memset (id3_tag,0,sizeof (ID3_TAG));

	m_buf = (U8 *)m_buf16;
	DBG_Assert(((U32)m_buf & 1) == 0);

	
	/*init id3 tag*/
	id3_tag->title[0] = 0x0;
	id3_tag->artist[0] = 0x0;
	id3_tag->album[0] = 0x0;

#ifdef ID3ALL
	
	id3_tag->genre[0] = 0x0;
	id3_tag->year[0] = 0x0;

#endif	

	*total_time = 0;

	
	/*parse header object*/
	//len = Fs_read_file (m_buf, sizeof(sHeaderObject), pFile);	
	len = xa_fread (m_buf, 1, sizeof(sHeaderObject), pFile);

	if (len != sizeof (sHeaderObject) ) {
		return NO_ID3_TAG;
	}

	if ( (GUID_compare (ASF_Header_Object, m_buf)) == FALSE) {
		/*the first object is not header object*/
		return NO_ID3_TAG;		
	}

	if ( ((sHeaderObject *) m_buf)->ObjectSize[1] != 0 ) {
		/*Header object is too bigger*/
		return NO_ID3_TAG;			
	}

#ifdef LITTLE_ENDIAN

	dwHeaderLen = ((sHeaderObject *) m_buf)->ObjectSize[0];
	dwNumberofHeaderObjects = ((sHeaderObject *) m_buf)->NumHeadObjects;

#else
	
	dwHeaderLen = w_Little_Big_Endian(&(((sHeaderObject *) m_buf)->ObjectSize[0]));
	dwNumberofHeaderObjects = w_Little_Big_Endian(&(((sHeaderObject *) m_buf)->NumHeadObjects));

#endif

	wHeaderObjectHits = 0;

	for (i=0; i<dwNumberofHeaderObjects; i++) {
	
		//len = Fs_read_file (m_buf, sizeof (sObject), pFile);
		len = xa_fread (m_buf, 1, sizeof (sObject), pFile);
		if (len != sizeof (sObject) ) {
			return NO_ID3_TAG;
		}

		if ( ((sHeaderObject *) m_buf)->ObjectSize[1] != 0 ) {
			/*Header object is too bigger*/
			return NO_ID3_TAG;			
		}


#ifdef LITTLE_ENDIAN

		dwHeaderObjectLen = ((sHeaderObject *) m_buf)->ObjectSize[0];

#else
		
		dwHeaderObjectLen = w_Little_Big_Endian(&(((sHeaderObject *) m_buf)->ObjectSize[0]));

#endif

 
		//dwHeaderObjectPos = Fs_file_tell (pFile);
		dwHeaderObjectPos = xa_ftell (pFile);

		if (dwHeaderObjectLen < 24) {
			return NO_ID3_TAG;			
		}

		//if ( (pFile->pos + (dwHeaderObjectLen-24)) > dwHeaderLen ) {
		if ( (dwHeaderObjectPos + (dwHeaderObjectLen-24)) > dwHeaderLen ) {
			break;			
		}

//--------------------------------------------------------------------------------------
		if ( (GUID_compare (ASF_Content_Description_Object, m_buf)) == TRUE) {

			U32	wTitleLen,wAthorLen,wCopyrightLength,wDescriptionLength,wRatingLength;
			U32 dwContentDescriptionPos;

			/*the first object is not header object*/
			wHeaderObjectHits++;
			//len = Fs_read_file (&m_buf[24], (sizeof (sContentObject)-24), pFile);
			len = xa_fread (&m_buf[24], 1, (sizeof (sContentObject)-24), pFile);
			if (len != (sizeof (sContentObject)-24) ) {
				return NO_ID3_TAG;
			}

			//dwContentDescriptionPos = Fs_file_tell (pFile);
			dwContentDescriptionPos = xa_ftell (pFile);


#ifdef LITTLE_ENDIAN
			wTitleLen = ((sContentObject *) m_buf)->Title_Len;
			wAthorLen = ((sContentObject *) m_buf)->Author_Len;
			wCopyrightLength = ((sContentObject *) m_buf)->Copyright_Len;
			wDescriptionLength = ((sContentObject *) m_buf)->Discription_Len;
			wRatingLength = ((sContentObject *) m_buf)->Rating_Len;
#else
			wTitleLen = w_Little_Big_Endian(&(((sContentObject *) m_buf)->Title_Len));
			wAthorLen = w_Little_Big_Endian(&(((sContentObject *) m_buf)->Author_Len));
			wCopyrightLength = w_Little_Big_Endian(&(((sContentObject *) m_buf)->Copyright_Len));
			wDescriptionLength = w_Little_Big_Endian(&(((sContentObject *) m_buf)->Discription_Len));
			wRatingLength = w_Little_Big_Endian(&(((sContentObject *) m_buf)->Rating_Len));
#endif
			
			len = wTitleLen;
			
			if (len > 2*MAX_ID3_TEXT)
					len = 2*MAX_ID3_TEXT;

			//Fs_read_file ( (BYTE *) id3_tag->title, len, pFile);
			len = xa_fread ((BYTE *) id3_tag->title, 1, len, pFile);
			if (len == 0) {
				return NO_ID3_TAG;
			}

			/*Unicode*/
			len = (wTitleLen+1)/2;
			if (len > MAX_ID3_TEXT)
					len = MAX_ID3_TEXT;

			id3_tag->title[len] = 0x0;								/*Null*/

#ifndef LITTLE_ENDIAN
			for (j=0; j<len;j++) {
				id3_tag->title[j] = w_Little_Big_Endian(&(id3_tag->title[j]));
			}
#endif
		
			wstrCutLeadingSpace ( id3_tag->title);
			wstrCutTrailingSpace ( id3_tag->title);

			if ( (dwContentDescriptionPos + wTitleLen) > dwHeaderLen ) {
				break;			
			}

			//Fs_file_seek (pFile, (dwContentDescriptionPos + wTitleLen) );
			ret = xa_fseek(pFile, (dwContentDescriptionPos + wTitleLen),SEEK_SET);
			if (ret == -1) {
				return NO_ID3_TAG;
			}

			len = wAthorLen;
			
			if (len > 2*MAX_ID3_TEXT)
					len = 2*MAX_ID3_TEXT;

			//Fs_read_file ( (BYTE *) id3_tag->artist, len, pFile);
			len = xa_fread ((BYTE *) id3_tag->artist, 1, len, pFile);
			if (len == 0) {
				return NO_ID3_TAG;
			}


			/*Unicode*/
			len = (wAthorLen+1)/2;
			if (len > MAX_ID3_TEXT)
					len = MAX_ID3_TEXT;

			id3_tag->artist[len] = 0x0;								/*Null*/

#ifndef LITTLE_ENDIAN
			for (j=0; j<len;j++) {
				id3_tag->artist[j] = w_Little_Big_Endian(&(id3_tag->artist[j]));
			}
#endif
		
			wstrCutLeadingSpace ( id3_tag->artist);
			wstrCutTrailingSpace ( id3_tag->artist);
			
		}
		else if ( (GUID_compare (ASF_Extended_Content_Description_Object, m_buf)) == TRUE) {
			
			U16 wContentDescriptorsCount,wDescriptorNameLength,wDescriptorValueDataType,wDescriptorValueLength;
			U16	wContentDescriptorsHits;
			U32 dwContentDescriptorsPos;
			U16 wContentType;
			
			/*the first object is not header object*/
			wHeaderObjectHits++;


			wContentDescriptorsHits = 0;

			//len = Fs_read_file (m_buf, 2, pFile);
			len = xa_fread (m_buf, 1, 2, pFile);
			if ( len != 2 ) {
				return NO_ID3_TAG;
			}

#ifdef LITTLE_ENDIAN
			wContentDescriptorsCount = *((U16 *) m_buf);
#else			
			wContentDescriptorsCount = w_Little_Big_Endian(m_buf);
#endif


			Hits = 0;

			for (k=0; k<wContentDescriptorsCount; k++) {

				//len = Fs_read_file (m_buf, 2, pFile);
				len = xa_fread (m_buf, 1, 2, pFile);
				if ( len != 2 ) {
					return NO_ID3_TAG;
				}

#ifdef LITTLE_ENDIAN
				wDescriptorNameLength = *((U16 *) m_buf);
#else				
				wDescriptorNameLength = w_Little_Big_Endian(m_buf);
#endif

				memset (m_buf,0,sizeof(m_buf));
				len = xa_fread (m_buf, 1, wDescriptorNameLength, pFile);
				if ( len != wDescriptorNameLength ) {
					return NO_ID3_TAG;
				}

				wContentType = Other_Content;

				if((Hits & 0x01)==0){
					if(memcmp(m_buf,WM_AlbumTitle,wDescriptorNameLength)==0){
						wContentType = Album_Content;
						Hits |= 0x01;
					}
				}

				if((Hits & 0x02)==0){
					if(memcmp(m_buf,WM_Genre,wDescriptorNameLength)==0){
						wContentType = Genre_Content;
						Hits |= 0x02;
					}
				}
		
				if((Hits & 0x04)==0){
					if(memcmp(m_buf,WM_Year,wDescriptorNameLength)==0){
						wContentType = Year_Content;
						Hits |= 0x04;
					}
				}

#if 0

				//dwContentDescriptorsPos = Fs_file_tell (pFile);
				dwContentDescriptorsPos = xa_ftell (pFile);

				wContentType = Other_Content;

				if ( (wDescriptorNameLength == WM_GENRE_LEN) || 
					(wDescriptorNameLength == WM_ALBUM_LEN) )
				{
				
					if (wDescriptorNameLength < (sizeof(m_buf16) - 2) )
					{
						//len = Fs_read_file ( m_buf, wDescriptorNameLength, pFile);			
						len = xa_fread (m_buf, 1, wDescriptorNameLength, pFile);
						if (len == 0) {
							return NO_ID3_TAG;
						}
				
					}

					/*unicoe null*/
					m_buf [sizeof(m_buf16) - 2] = 0x0;
					m_buf [sizeof(m_buf16) - 1] = 0x0;
					

					if ( (wstrcmp ((U16 *)m_buf, (U16 *)WM_Album)) == 0 ) {
						/*album content*/
						wContentType = Album_Content;
					}
					else if ( (wstrcmp ((U16 *)m_buf, (U16 *)WM_Genre)) == 0 ) {
						/*Genre content*/
						wContentType = Genre_Content;
					}

				}

				
				if ( (dwContentDescriptorsPos+wDescriptorNameLength) > dwHeaderLen ) {
					break;			
				}

				//Fs_file_seek (pFile,(dwContentDescriptorsPos+wDescriptorNameLength));
				ret = xa_fseek(pFile,(dwContentDescriptorsPos+wDescriptorNameLength),SEEK_SET);
				if (ret == -1) {
					return NO_ID3_TAG;
				}

#endif

				//len = Fs_read_file ( m_buf, 2, pFile);
				len = xa_fread (m_buf, 1, 2, pFile);
				if ( len != 2 ) {
					return NO_ID3_TAG;
				}

#ifdef LITTLE_ENDIAN
				wDescriptorValueDataType = *((U16 *) m_buf);
#else				
				wDescriptorValueDataType = w_Little_Big_Endian(m_buf);
#endif


				//len = Fs_read_file (m_buf, 2, pFile);
				len = xa_fread (m_buf, 1, 2, pFile);
				if ( len != 2 ) {
					return NO_ID3_TAG;
				}

				DBG_Assert(((U32)m_buf & 1) == 0);
#ifdef LITTLE_ENDIAN	
				wDescriptorValueLength = *((U16 *) m_buf);
#else
				wDescriptorValueLength = w_Little_Big_Endian(m_buf);
#endif
				
				//dwContentDescriptorsPos = Fs_file_tell (pFile);
				dwContentDescriptorsPos = xa_ftell (pFile);
				

				if (wContentType == Album_Content) {

					wContentDescriptorsHits++;

					if (wDescriptorValueDataType == 0) {
						/*unicode string*/

						len = wDescriptorValueLength;
						if (len > 2*MAX_ID3_TEXT)
							len = 2*MAX_ID3_TEXT;

						//Fs_read_file ( (BYTE *) id3_tag->album, len, pFile);
						len = xa_fread ((BYTE *) id3_tag->album, 1, len, pFile);
						if (len == 0) {
							return NO_ID3_TAG;
						}
			
						/*Unicode*/
						len = (wDescriptorValueLength+1)/2;
						if (len > MAX_ID3_TEXT)
							len = MAX_ID3_TEXT;
						
						id3_tag->album[len] = 0x0;								/*Null*/

#ifndef LITTLE_ENDIAN
						for (j=0; j<len;j++) {
							id3_tag->album[j] = w_Little_Big_Endian(&(id3_tag->album[j]));
						}
#endif
		
						wstrCutLeadingSpace ( id3_tag->album);
						wstrCutTrailingSpace ( id3_tag->album);
					}
					else if (wDescriptorValueDataType == 1) {

						/*Byte string*/
						len = wDescriptorValueLength;
						if (len > sizeof (m_buf16) )
							len = sizeof (m_buf16);

						//Fs_read_file (m_buf, len, pFile);
						len = xa_fread (m_buf, 1, len, pFile);
						if (len == 0) {
							return NO_ID3_TAG;
						}

						if (len > MAX_ID3_TEXT)
							len = MAX_ID3_TEXT;

						for (j=0; j<len;j++) {
							id3_tag->album[j] = m_buf[j];
						}

						id3_tag->album[j] = 0x0;
		
						wstrCutLeadingSpace ( id3_tag->album);
						wstrCutTrailingSpace ( id3_tag->album);					
					}
				
				}
				else if (wContentType == Genre_Content) {

#ifdef ID3ALL

					wContentDescriptorsHits++;


					if (wDescriptorValueDataType == 0) {
						/*unicode string*/
						len = wDescriptorValueLength;
						if (len > 2*MAX_ID3_GENRE_TEXT)
							len = 2*MAX_ID3_GENRE_TEXT;

						//Fs_read_file ( (BYTE *) id3_tag->genre, len, pFile);
						len = xa_fread ((BYTE *) id3_tag->genre, 1, len, pFile);
						if (len == 0) {
							return NO_ID3_TAG;
						}

						/*Unicode*/
						len = (wDescriptorValueLength+1)/2;
						if (len > MAX_ID3_GENRE_TEXT)
							len = MAX_ID3_GENRE_TEXT;


						id3_tag->genre[len] = 0x0;								/*Null*/

#ifndef LITTLE_ENDIAN
						for (j=0; j<len;j++) {
							id3_tag->genre[j] = w_Little_Big_Endian(&(id3_tag->genre[j]));
						}
#endif
		
						wstrCutLeadingSpace ( id3_tag->genre);
						wstrCutTrailingSpace ( id3_tag->genre);


					}
					else if (wDescriptorValueDataType == 1) {

						/*Byte string*/
						len = wDescriptorValueLength;
						if (len > sizeof (m_buf16) )
							len = sizeof (m_buf16);

						//Fs_read_file (m_buf, len, pFile);
						len = xa_fread (m_buf, 1, len, pFile);
						if (len == 0) {
							return NO_ID3_TAG;
						}
       
						if (len > MAX_ID3_GENRE_TEXT)
							len = MAX_ID3_GENRE_TEXT;

						for (j=0; j<len;j++) {
							id3_tag->genre[j] = m_buf[j];
						}

						id3_tag->genre[j] = 0x0;
		
						wstrCutLeadingSpace ( id3_tag->genre);
						wstrCutTrailingSpace ( id3_tag->genre);					
					}
#endif


				}
				else if (wContentType == Year_Content) {

#ifdef ID3ALL

					wContentDescriptorsHits++;


					if (wDescriptorValueDataType == 0) {
						/*unicode string*/
						len = wDescriptorValueLength;
						if (len > 2*MAX_ID3_GENRE_TEXT)
							len = 2*MAX_ID3_GENRE_TEXT;

						//Fs_read_file ( (BYTE *) id3_tag->genre, len, pFile);
						len = xa_fread ((BYTE *) id3_tag->year, 1, len, pFile);
						if (len == 0) {
							return NO_ID3_TAG;
						}

						/*Unicode*/
						len = (wDescriptorValueLength+1)/2;
						if (len > MAX_ID3_YEAR_TEXT)
							len = MAX_ID3_YEAR_TEXT;


						id3_tag->year[len] = 0x0;								/*Null*/

#ifndef LITTLE_ENDIAN
						for (j=0; j<len;j++) {
							id3_tag->year[j] = w_Little_Big_Endian(&(id3_tag->year[j]));
						}
#endif
		
						wstrCutLeadingSpace ( id3_tag->year);
						wstrCutTrailingSpace ( id3_tag->year);


					}
					else if (wDescriptorValueDataType == 1) {

						/*Byte string*/
						len = wDescriptorValueLength;
						if (len > sizeof (m_buf16) )
							len = sizeof (m_buf16);

						//Fs_read_file (m_buf, len, pFile);
						len = xa_fread (m_buf, 1, len, pFile);
						if (len == 0) {
							return NO_ID3_TAG;
						}

						if (len > MAX_ID3_YEAR_TEXT)
							len = MAX_ID3_YEAR_TEXT;

						for (j=0; j<len;j++) {
							id3_tag->year[j] = m_buf[j];
						}

						id3_tag->year[j] = 0x0;
		
						wstrCutLeadingSpace ( id3_tag->year);
						wstrCutTrailingSpace ( id3_tag->year);					
					}
#endif
				}

				
				if (wContentDescriptorsHits >= 2)
					break;

				if ((dwContentDescriptorsPos+wDescriptorNameLength) > dwHeaderLen)
					break;

				//Fs_file_seek (pFile,(dwContentDescriptorsPos+wDescriptorValueLength));
				ret = xa_fseek(pFile,(dwContentDescriptorsPos+wDescriptorValueLength),SEEK_SET);
				if (ret == -1) {
					return NO_ID3_TAG;
				}

			}

		}
//--------------------------------------------------------------------------------------
		else if ( (GUID_compare (ASF_File_Properties_Object, m_buf)) == TRUE) {
			U64 PlayDuration,Preroll;
			U32 total_playtime_ms;
#if 0			
			sFilePropertiesObject *pfile;
#endif			
			wHeaderObjectHits++;

			//Fs_file_seek (pFile, Fs_file_tell (pFile)+4*sizeof(U32)+3*sizeof(U64));//skip no use infomation
			ret = xa_fseek(pFile,(xa_ftell(pFile)+4*sizeof(U32)+3*sizeof(U64)),SEEK_SET);
			if (ret == -1) {
				return NO_ID3_TAG;			
			}

			//Fs_read_file ((BYTE *)&PlayDuration, sizeof(U64), pFile);
			len = xa_fread ((BYTE *)&PlayDuration, 1, sizeof(U64), pFile);
			if (len == 0) {
				return NO_ID3_TAG;
			}
			//Fs_file_seek (pFile, Fs_file_tell (pFile)+sizeof(U64));//skip no use infomation
			ret = xa_fseek(pFile,(xa_ftell(pFile)+sizeof(U64)),SEEK_SET);
			if (ret == -1) {
				return NO_ID3_TAG;			
			}

			//Fs_read_file ((BYTE *)&Preroll, sizeof(U64), pFile);
			len = xa_fread ((BYTE *)&Preroll, 1, sizeof(U64), pFile);
			if (len == 0) {
				return NO_ID3_TAG;
			}

			total_playtime_ms = PlayDuration / 10000 - Preroll;//ms
			*total_time = total_playtime_ms/1000;
		}		

//--------------------------------------------------------------------------------------
		if (wHeaderObjectHits >= 3) {
			break;
		}
		
		//Fs_file_seek (pFile, (dwHeaderObjectPos+(dwHeaderObjectLen-24)));	
		ret = xa_fseek(pFile,(dwHeaderObjectPos+(dwHeaderObjectLen-24)),SEEK_SET);
		if (ret == -1) {
				return NO_ID3_TAG;		
		}

	}	
	
	return ID3_TAG_OK;	
}

#endif //0

#endif
#endif
