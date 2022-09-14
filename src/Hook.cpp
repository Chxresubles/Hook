#include "mongoose.h"
#include "mp3_helper.h"
#include "utils.h"

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
	struct mg_http_serve_opts opts = {
		.root_dir = ".",			// Server local directory
		.ssi_pattern = ".shtml",	// SSI file name pattern
		.extra_headers = nullptr,	// No extra header
		.mime_types = nullptr,		// No extra MIME types
		.page404 = nullptr,			// Path to the 404 page
		.fs = nullptr				// Filesystem implementation (POSIX)
	};   
	if (ev == MG_EV_HTTP_MSG) {
		// Cast the event data to HTTP message
		struct mg_http_message* hm = (struct mg_http_message*)ev_data;

		// Get the artist and song name from the URI
		int splits;
		char** split_array = split(hm->uri.ptr, '/', splits);
		if (splits == 2) {
			char* artist = split_array[0];
			char* song_name = split_array[1];

			// Get song infos
			MG_INFO(("GET INFOS OF '%s' by '%s'...", song_name, artist));
			mp3_infos* infos = mp3_get_infos(artist, song_name);
			if (infos == nullptr) return;
			MG_INFO(("Title: %s", infos->title.ptr));
			MG_INFO(("Artist: %s", infos->artist.ptr));
			MG_INFO(("Duration: %u", infos->duration));
			MG_INFO(("Album: %s", infos->album.ptr));
			MG_INFO(("Date: %s", infos->date.ptr));
			MG_INFO(("File name: %s", infos->filename.ptr));
			MG_INFO(("READING MP3 FILE..."));
			mg_http_serve_file(c, hm, infos->filename.ptr, &opts);

			// Free all data
			MG_INFO(("CLEAN UP INFOS CHAR ARRAYS..."));
			MG_INFO(("CLEAN UP TITLE..."));
			free((char*)infos->title.ptr);
			MG_INFO(("CLEAN UP ARTIST..."));
			free((char*)infos->artist.ptr);
			MG_INFO(("CLEAN UP ALBUM..."));
			free((char*)infos->album.ptr);
			MG_INFO(("CLEAN UP DATE..."));
			free((char*)infos->date.ptr);
			MG_INFO(("CLEAN UP FILENAME..."));
			free((char*)infos->filename.ptr);
			MG_INFO(("CLEANED UP INFOS CHAR ARRAYS"));
			free(infos);
			MG_INFO(("CLEANED UP INFOS"));
		}
		for (int i = 0; i < splits; i++) {
			free(split_array[i]);
		}
		free(split_array);
	}
}

int main(int argc, char *argv[]) {
	struct mg_mgr mgr;
	mg_mgr_init(&mgr);                                            // Init manager
	mg_http_listen(&mgr, "http://localhost:80", fn, &mgr);        // Setup listener
	for (;;) mg_mgr_poll(&mgr, 1000);                             // Event loop
	mg_mgr_free(&mgr);                                            // Cleanup
	return 0;
}