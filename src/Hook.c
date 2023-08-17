#include "mongoose.h"
#include <string.h>
#include <stdlib.h>

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    struct mg_http_serve_opts opts = {
        .root_dir = ".",            // Server local directory
        .ssi_pattern = ".shtml",    // SSI file name pattern
        .extra_headers = NULL,      // No extra header
        .mime_types = NULL,         // No extra MIME types
        .page404 = NULL,            // Path to the 404 page
        .fs = NULL                  // Filesystem implementation (POSIX)
    };
    if (ev == MG_EV_HTTP_MSG) {
        // Cast the event data to HTTP message
        struct mg_http_message* hm = (struct mg_http_message*)ev_data;

        // Route to list the server musics, artists, playlists, ...
        if (mg_http_match_uri(hm, "/list")) {
            // Initialize variables
            size_t size = 0;
            int offset = 0;
            int length = 0;
            int n_objects = 0;
            int offset_sum = 0;

            // Read JSON file
            char* data = mg_file_read(&mg_fs_posix, "songs/songs.json", &size);
            char* modified_data = (char*) malloc(size * sizeof(char));
            memcpy(modified_data, data, size * sizeof(char));
            struct mg_str json_content = mg_str_n(data, size);

            // Go through all song object and remove the "filename" line
            while(offset >= 0) {
                // Find the filename line for the next song object
                char* path = mg_mprintf("$[%d].filename", n_objects);
                offset = mg_json_get(json_content, path, &length);

                // If it was found
                if (offset >= 0) {
                    // Copy the rest of the json data over this line
                    n_objects++;
                    memcpy(modified_data + offset - offset_sum - 16, json_content.ptr + offset + length + 3, (json_content.len - offset - (length + 3)) * sizeof(char));
                    offset_sum += (16 + length + 3);
                }

                // Free path memory
                free(path);
            }
            // Add NULL at the end of the string
            modified_data[size - offset_sum] = '\0';

            // Send the songs description JSON without the "filename" lines
            mg_http_reply(c, 200, "", modified_data);

            // Free memory
            free(data);
            free(modified_data);
        }
        // Route to get the musics from the given query /play?id=XXX
        else if (mg_http_match_uri(hm, "/play")) {
            // Retrieve ID from query
            struct mg_str query_id = mg_http_var(hm->query, mg_str("id"));

            // If there is no query, return error
            if(query_id.ptr == NULL) {
                mg_http_reply(c, 400, "", "Missing ID of song to play in query\n");
                return;
            }

            // Store ID in a valid C-string
            char* id_str = (char*) malloc((query_id.len + 1) * sizeof(char));
            memcpy(id_str, query_id.ptr, query_id.len * sizeof(char));
            id_str[query_id.len] = '\0';

            // Retrieve ID as an int
            int id = atoi(id_str);
            free(id_str);

            // Initialize variables
            size_t size = 0;
            int offset = 0;
            int length = 0;

            // Read JSON file
            char* data = mg_file_read(&mg_fs_posix, "songs/songs.json", &size);
            struct mg_str json_content = mg_str_n(data, size);

            // Find the filename of the wanted song
            char* path = mg_mprintf("$[%d].filename", id);
            offset = mg_json_get(json_content, path, &length);
            free(path);

            // If it was not found
            if (offset < 0) {
                mg_http_reply(c, 404, "", "The requested song ID was not found\n");
                free(data);
                return;
            }

            // Retrieve filepath of the requested song
            char* filepath = (char*) malloc((length + 6 + 1) * sizeof(char));
            memcpy(filepath, "songs/", 6 * sizeof(char));
            memcpy(filepath + 6, json_content.ptr + offset + 1, (length - 2) * sizeof(char));
            filepath[length + 4] = '\0';
            free(data);

            // Send file back
            mg_http_serve_file(c, hm, filepath, &opts);

            // Free memory
            free(filepath);
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