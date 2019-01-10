#ifndef TEST_CLIENT_CONFIG
#define TEST_CLIENT_CONFIG

/**
 * @brief extract_config. Dynamically allocates a buffer and reads
 * contents of a config file into that buffer.
 * @param files_string: ptr to ptr to the buffer; responsibility of
 * freeing (&files_string) is on the function's caller.
 * @return 1 -- success, 0 -- failure (and files_string is returned NULL).
 */
int extract_config(char** files_string);

#endif /* TEST_CLIENT_CONFIG */
