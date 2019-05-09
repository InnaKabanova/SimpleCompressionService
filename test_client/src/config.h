#ifndef TEST_CLIENT_CONFIG
#define TEST_CLIENT_CONFIG

#include "internal_request.h"

// FILES_LIST is a config file containing a list of all files with
// requests. If a file is not written to FILES_LIST, it won't be
// processed.
#define FILES_LIST_PATH "../config/files_list"

/**
 * @brief get_filepathes. Dynamically allocates a buffer and reads
 * contents of FILES_LIST into that buffer.
 * @param[inout] filepathes: ptr to ptr to the buffer; responsibility of
 * freeing (&filepathes) lies on the function's caller.
 * @return 1 -- success, 0 -- failure.
 */
int get_filepathes(char** files_string);

/**
 * @brief get_requests.
 * @param[in] filepath
 * @param[in] request
 * @return
 */
int get_requests(const char* filepath, tc_internal_request_t** request);

#endif /* TEST_CLIENT_CONFIG */
