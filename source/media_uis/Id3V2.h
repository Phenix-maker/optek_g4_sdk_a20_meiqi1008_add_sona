#ifndef	_ID3V2_H_
#define	_ID3V2_H_

typedef struct {
	U8	Tag[3];
	U8	Title [30];
	U8	Artist [30];
	U8	Album [30];
	U32	Year;
	U8	Comment [30];
	U8	Genre;
} ID3_V1_1;



typedef	enum 
{
	ID3_TAG_OK	=	0,
	ID3_WAIT_DATA,
	NO_ID3_TAG
} ID3_RETURN;


//void ID3v2_init(ID3_TAG *id3_tag);
//ID3_RETURN ID3v2_process( ID3_TAG *id3_tag );
//ID3_RETURN id3v1_process (U8 *buf, ID3_TAG *id3_tag);


// this is for ID3v1 use
//U8 * get_genre_info( int genre);

//extern ID3_TAG id3_tag;

#endif //_ID3V2_H_
