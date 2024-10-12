#ifndef _APP_LIST_H_
#define _APP_LIST_H_


typedef struct {
	I16 parent_node;
	U16 curr_node;
	U16 index_in_node;
} FOLDER_NODE;


void app_list_create_cdda_normal_playlist (void);
void app_list_create_cdda_tracks_list (void);
void app_list_create_cdda_playlist_list (void);
void app_list_create_cdda_playlist_menu (void);

U16 app_list_get_list_content (U16 index);

void app_list_create_mp3_node_list (U16 node);
void app_list_create_mp3_nomral_playlist (void);
void app_list_create_mp3_playlist_list (void);
void app_list_create_mp3_entry_list (void);
void app_list_create_mp3_folder_list (void);
void app_list_create_mp3_playlist_menu (void);

U16 app_list_create_mp3_program_playlist (U16 program_index);

U16 app_list_playlist_total_dirs (void);
U16 app_list_playlist_dir (U16 index);
U16 app_list_playlist_dir_start_index ( U16 dir_index);
U16 app_list_playlist_tracks_in_dir (U16 dir_index);
void app_list_copy_playlist (void);
void app_list_create_nomral_playlist(void);

#endif //_APP_LIST_H_
