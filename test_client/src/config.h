#ifndef TEST_CLIENT_CONFIG
#define TEST_CLIENT_CONFIG

/**
 * @brief get_filepathes. Dynamically allocates a buffer and reads
 * contents of a config file into that buffer.
 * @param filepathes: ptr to ptr to the buffer; responsibility of
 * freeing (&filepathes) lies on the function's caller.
 * @return 1 -- success, 0 -- failure (and files_string is returned NULL).
 */
int get_filepathes(char** files_string);

#endif /* TEST_CLIENT_CONFIG */
