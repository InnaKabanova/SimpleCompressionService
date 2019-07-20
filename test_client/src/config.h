#ifndef TEST_CLIENT_CONFIG
#define TEST_CLIENT_CONFIG

#include "internal_request.h"

/**
 * @brief FILES_LIST. A config file containing a list of all files with
 * textually serialized requests.
 */
#define FILES_LIST "../config/files_list"

/**
 * @brief get_filepathes. Provides a dynamically allocated buffer with
 * a copy of FILES_LIST contents.
 * @param[inout] filepathes: ptr to ptr to the allocated buffer;
 * responsibility of freeing (&files_string) lies on the function's caller.
 * @return 1 -- success, 0 -- failure.
 */
int get_filepathes(char** files_string);

/**
 * @brief get_requests. Provides requests deserialized from a specified
 * config file.
 * @param[in] filepath: a config file with serialized requests;
 * @param[inout] request: ptr to ptr to a linked list of requests which
 * is returned in case of the function's success; responsibility of
 * freeing each of the requests in the chain is on the function's caller;
 * (*request) must be NULL when passed to the function.
 * @return 1 -- success, 0 -- failure.
 */
int get_requests(const char* filepath, tc_internal_request_t** request);

#endif /* TEST_CLIENT_CONFIG */
