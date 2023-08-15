#include "mp3_helper.h"

// This function gets the infos from the given song using the music.json file.
mp3_infos* mp3_get_infos(char* artist, char* song_name) {
	// Read JSON file
	size_t size = 0;
	char* data = mg_file_read(&mg_fs_posix, "songs/songs.json", &size);
	mg_str json_content = mg_str_n(data, size);

	// Check if the song exists
	char* path = mg_mprintf("$.%s.%s", artist, song_name);
	int offset = mg_json_get(json_content, path, 0);
	free(path); // Don't forget to free the path
	if (offset < 0) return nullptr;

	// Create return struct
	mp3_infos* infos = new mp3_infos;

	// Init variables
	double d;

	// Fill struct infos
	infos->title = mg_str(song_name);
	infos->artist = mg_str(artist);

	path = mg_mprintf("$.%s.%s.duration", artist, song_name);
	mg_json_get_num(json_content, path, &d);
	infos->duration = (int)d;
	free(path); // Don't forget to free the path

	path = mg_mprintf("$.%s.%s.album", artist, song_name);
	infos->album = mg_str(mg_json_get_str(json_content, path));
	free(path); // Don't forget to free the path

	path = mg_mprintf("$.%s.%s.date", artist, song_name);
	infos->date = mg_str(mg_json_get_str(json_content, path));
	free(path); // Don't forget to free the path

	path = mg_mprintf("$.%s.%s.filename", artist, song_name);
	char* filename = mg_json_get_str(json_content, path);
	infos->filename = mg_str(mg_mprintf("songs/%s", filename));
	free(path); // Don't forget to free the path
	free(filename); // Don't forget to free the filename

	// Free the json content
	free(data);

	return infos;
}
