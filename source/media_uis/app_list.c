#include "c_def.h"
#include "debug.h"
#include "oem.h"

#include "app_main.h"
#include "app_list.h"
#include "app_window.h"
#include "app_sdram.h"
#include "app_string_oem.h"
#include "app_media_com.h"

extern U16 dynamic_list_entry;
extern U16 dynamic_list_attribute;

void app_list_create_mp3_entry_list (void)
{
	dynamic_list_attribute = ITEM_ENTRY;

	dynamic_list_entry = app_media_data.total_tracks;
}

void app_list_create_mp3_folder_list (void)
{
	dynamic_list_attribute = ITEM_FOLDER;
	dynamic_list_entry = app_media_data.total_folders;
}

#ifdef MULTI_FOLDER_LEVEL

void app_list_create_mp3_node_list (U16 node)
{	
#if 0
	I16 i;
	I16 index = 0;
	I16 lists = 0;

	//app_sdram_udata_read_subdir (tree_node,subdir);

	if (app_sdram_get_folder_entries(node) > 0) {
		//get out tracks in this node
		for (i=pCSubdir[node].first_entry; 
			i<(pCSubdir[node].first_entry + app_sdram_get_folder_entries(node)); i++) {
			
			dynamic_mp3_list[lists].index = i;
			dynamic_mp3_list[lists].attribute = ITEM_ENTRY;
			dynamic_mp3_list[lists].status = ITEM_NOT_SELECTED;
			
			lists++;

		}
	}

#ifndef FOLDER_NUMBER

	for (i = 1; i<app_media_data.total_folders; i++) {
			

			if (disc_info.subdir[i].parent_no == node) {
				

				if ( (app_list_get_first_entry (i)) != NO_ENTRY_IN_FOLDER ) {
				
					
					dynamic_mp3_list[lists].index = i;
					dynamic_mp3_list[lists].attribute = ITEM_FOLDER;
					dynamic_mp3_list[lists].status = ITEM_NOT_SELECTED;
			
					lists++;
				
				}
			}
	}

#endif
	
	dynamic_list_entry = lists;
#endif
}

#endif


void app_list_create_mp3_nomral_playlist (void)
{
	playing_info.total_of_playlist = app_media_data.total_tracks;
}

#ifdef MP3_PROGRAM_ENABLE
void app_list_create_mp3_playlist_list (void)
{
	dynamic_list_attribute = ITEM_PLAYLIST_ENTRY;
	dynamic_list_entry = playing_info.total_of_program_list;
}
#endif

U16 app_list_get_list_content (U16 index)
{
	U16 val;

	if (dynamic_list_attribute == ITEM_ENTRY)
		val = index;
	else if (dynamic_list_attribute == ITEM_FOLDER)
		val = index;
#ifdef MP3_PROGRAM_ENABLE
	else if (dynamic_list_attribute == ITEM_PLAYLIST_ENTRY)
		val = playing_info.mp3_program_list[index].entry;
#endif
	else
		val = 0;

	return val;
}

void app_list_create_mp3_program_list (void)
{
#ifdef MP3_PROGRAM_ENABLE
	dynamic_list_attribute = ITEM_PLAYLIST_ENTRY;
	dynamic_list_entry = playing_info.total_of_program_list;
	playing_info.total_of_playlist = playing_info.total_of_program_list;
#endif
}

//total dirs in playlist
U16 app_list_playlist_total_dirs (void)
{
	U16 i, t, dir;
	U16 dirs = 0;

	DBG_Assert (playing_info.total_of_playlist > 0);

	t = app_list_get_list_content(0);
	{
#ifdef FILE_SYSTEM_ENABLE
		//for USB
		dir = app_file_get_folder_of_entry(t);

		dirs++;

		for (i=0; i<playing_info.total_of_playlist; i++) {

			t = app_list_get_list_content(i);

			if (dir != app_file_get_folder_of_entry(t)) {
				dirs++;
				dir = app_file_get_folder_of_entry(t);
			}
		}
#endif
	}

	return dirs;
}

//dir index in playlist for the track, start for 0
U16 app_list_playlist_dir (U16 index)
{
	U16 i, t, dir;
	U16 dirs = 0;

	DBG_Assert (playing_info.total_of_playlist > 0);

	t = app_list_get_list_content(0);
	{
#ifdef FILE_SYSTEM_ENABLE
		//For USB
		dir = app_file_get_folder_of_entry(t);

		for (i=0; i<=index; i++) {

			t = app_list_get_list_content(i);

			if (dir != app_file_get_folder_of_entry(t)) {
				dirs++;
				dir = app_file_get_folder_of_entry(t);
			}
		}
#endif
	}

	return dirs;
}

//retrun start index in playlist for a dir index
U16 app_list_playlist_dir_start_index ( U16 dir_index)
{
	U16 i, t, dir;
	U16 dirs = 0;
	
	DBG_Assert (playing_info.total_of_playlist > 0);

	if (dir_index == 0) {
		return 0;
	}

	t = app_list_get_list_content(0);
	{
#ifdef FILE_SYSTEM_ENABLE
		//For USB
		dir = app_file_get_folder_of_entry(t);

		for (i=0; i<playing_info.total_of_playlist; i++) {

			t = app_list_get_list_content(i);

			if (dir != app_file_get_folder_of_entry(t) ) {
				dirs++;
				dir = app_file_get_folder_of_entry(t);
				
				if (dirs == dir_index) {
					break;
				}		
			}
		}
#endif
	}

	DBG_Assert ( i < playing_info.total_of_playlist);			//dir > total of dirs

	return i;
}

//return num of tracks for a dir index
U16 app_list_playlist_tracks_in_dir (U16 dir_index)
{
	U16 i, t, dir, tracks;
	U16 dirs = 0;
	
	DBG_Assert (playing_info.total_of_playlist > 0);
	{
#ifdef FILE_SYSTEM_ENABLE
		//For USB
		//find the start index for the dir
		if (dir_index == 0) {
			i = 0;
		}
		else {
			
			t = app_list_get_list_content(0);
			dir = app_file_get_folder_of_entry(t);

			for (i=0; i<playing_info.total_of_playlist; i++) {

				t = app_list_get_list_content(i);

				if (dir != app_file_get_folder_of_entry(t)) {
					dirs++;
					dir = app_file_get_folder_of_entry(t);
				
					if (dirs == dir_index) {
						break;
					}		
				}
			}	
		}

		DBG_Assert (i < playing_info.total_of_playlist);
		
		dir = app_file_get_folder_of_entry(i);
		tracks = 0;

		for (; i<playing_info.total_of_playlist; i++) {

			t = app_list_get_list_content(i);

			if (dir != app_file_get_folder_of_entry(t)) {
				break;
			}
			else {
				tracks++;
			}	
		}
#endif //FILE_SYSTEM_ENABLE
	}

	return tracks;
}

void app_list_copy_playlist (void)
{
#if 0
	int i;

	for (i=0; i <playing_info.total_of_playlist; i++) {
	
		dynamic_mp3_list[i].index = playing_info.playlist[i];
		dynamic_mp3_list[i].attribute = ITEM_ENTRY;
		dynamic_mp3_list[i].status = ITEM_NOT_SELECTED;
	}
#endif


	dynamic_list_attribute = ITEM_PLAYLIST_ENTRY;
	dynamic_list_entry = playing_info.total_of_playlist;
}

void app_list_create_nomral_playlist(void)
{
#if 0
	I16 i;

	for (i=0; i<app_media_data.total_tracks; i++) {
		playing_info.playlist[i] = i;
	}
#endif

	dynamic_list_attribute = ITEM_ENTRY;
	playing_info.total_of_playlist = app_media_data.total_tracks;
}
