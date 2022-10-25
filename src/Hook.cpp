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

		// Route to search musics, artists, playlists, ... from the query /search
		if (mg_http_match_uri(hm, "/search")) {
			mg_http_reply(c, 200, "", "{\"result\": \"search\"}\n");

		// Route to get the previous music in line /get/previous
		}
		else if (mg_http_match_uri(hm, "/get/previous")) {
			mg_http_reply(c, 200, "", "{\"result\": \"get previous\"}\n");

		// Route to get the current music in line /get/current
		}
		else if (mg_http_match_uri(hm, "/get/current")) {
			mg_http_reply(c, 200, "", "{\"result\": \"get current\"}\n");

		// Route to get the next music in line /get/next
		} else if (mg_http_match_uri(hm, "/get/next")) {
			mg_http_reply(c, 200, "", "{\"result\": \"get next\"}\n");

		// Route to get the musics that have been be played and will be played /get/line
		}
		else if (mg_http_match_uri(hm, "/get/line")) {
			mg_http_reply(c, 200, "", "{\"result\": \"get line\"}\n");

		// Route to get the musics from a given album /get/album
		}
		else if (mg_http_match_uri(hm, "/get/album")) {
			mg_http_reply(c, 200, "", "{\"result\": \"get album\"}\n");

		// Route to get the musics from a given artist /get/artist
		}
		else if (mg_http_match_uri(hm, "/get/artist")) {
			mg_http_reply(c, 200, "", "{\"result\": \"get artist\"}\n");

		// Route to get the musics from a given playlist /get/playlist
		}
		else if (mg_http_match_uri(hm, "/get/playlist")) {
			mg_http_reply(c, 200, "", "{\"result\": \"get playlist\"}\n");

		// Route to get all the musics in the server /get/all
		}
		else if (mg_http_match_uri(hm, "/get/all")) {
			mg_http_reply(c, 200, "", "{\"result\": \"get all\"}\n");

		// Route to get the musics from the given query /get
		}
		else if (mg_http_match_uri(hm, "/get")) {
			mg_http_reply(c, 200, "", "{\"result\": \"get\"}\n");

		// Route to list the playlists from the user /list/playlists
		}
		else if (mg_http_match_uri(hm, "/list/playlists")) {
			mg_http_reply(c, 200, "", "{\"result\": \"list playlists\"}\n");

		// Route to list the albums from the given artist /list/albums?artist=XXX
		}
		else if (mg_http_match_uri(hm, "/list/albums")) {
			mg_http_reply(c, 200, "", "{\"result\": \"list albums\"}\n");

		// Route to list the artists /list/artists
		}
		else if (mg_http_match_uri(hm, "/list/artists")) {
			mg_http_reply(c, 200, "", "{\"result\": \"list artists\"}\n");

		// Route to get the next music in line /play/next
		}
		else if (mg_http_match_uri(hm, "/play/next")) {
			mg_http_reply(c, 200, "", "{\"result\": \"play next\"}\n");

		// Route to get the previous music in line /play/previous
		}
		else if (mg_http_match_uri(hm, "/play/previous")) {
			mg_http_reply(c, 200, "", "{\"result\": \"play previous\"}\n");

		// Route to get a music from a given artist /play/artist
		}
		else if (mg_http_match_uri(hm, "/play/artist")) {
			mg_http_reply(c, 200, "", "{\"result\": \"play artist\"}\n");

		// Route to get a music from a given album /play/album
		}
		else if (mg_http_match_uri(hm, "/play/album")) {
			mg_http_reply(c, 200, "", "{\"result\": \"play album\"}\n");

		// Route to get a music from a given playlist /play/playlist
		}
		else if (mg_http_match_uri(hm, "/play/playlist")) {
			mg_http_reply(c, 200, "", "{\"result\": \"play playlist\"}\n");

		// Route to get the musics from the given query /play
		}
		else if (mg_http_match_uri(hm, "/play")) {
			mg_http_reply(c, 200, "", "{\"result\": \"play\"}\n");
		}
		else if (mg_http_match_uri(hm, "/random")) {
			mg_http_reply(c, 200, "", "{\"result\": \"random\"}\n");
		}
		else {
			// Get the artist and song name from the URI
			int splits;
			mg_str* split_array;
			if (hm->uri.len > 1) {
				MG_INFO(("SPLIT URI..."));
				split_array = split(hm->uri, '/', splits);
				MG_INFO(("DONE."));
				MG_INFO(("Found %d splits", splits));
				if (splits == 2) {
					char* artist = (char*)malloc(split_array[0].len * sizeof(char));
					char* song_name = (char*)malloc(split_array[1].len * sizeof(char));
					memcpy(artist, split_array[0].ptr, split_array[0].len);
					memcpy(song_name, split_array[1].ptr, split_array[1].len);

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
				MG_INFO(("CLEAN UP SPLIT ARRAYS..."));
				for (int i = 0; i < splits; i++) {
					free((char*)split_array[i].ptr);
				}
				MG_INFO(("CLEAN UP SPLIT ARRAY..."));
				free(split_array);
				MG_INFO(("DONE"));
			}
		}
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