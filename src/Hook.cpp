#include "mongoose.h"

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
		mg_http_serve_dir(c, (mg_http_message*)ev_data, &opts);
	}
}

int main(int argc, char *argv[]) {
	struct mg_mgr mgr;
	mg_mgr_init(&mgr);                                              // Init manager
	mg_http_listen(&mgr, "http://localhost:80", fn, &mgr);        // Setup listener
	for (;;) mg_mgr_poll(&mgr, 1000);                               // Event loop
	mg_mgr_free(&mgr);                                              // Cleanup
	return 0;
}