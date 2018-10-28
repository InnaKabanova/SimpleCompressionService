#ifndef COMPRESSION_REQUEST
#define COMPRESSION_REQUEST

#include <stdint.h>

const int DEFAULT_MAGIC_VALUE = 0x53545259;

// All fields must be in network byte order to be valid.
struct request_header
{
    uint32_t magic_value; // should be equal to DEFAULT_MAGIC_VALUE to be valid
    uint16_t payload_length;
    uint16_t request_code;
} default_request = { .magic_value = DEFAULT_MAGIC_VALUE };

enum REQUEST_CODE
{
    PING = 1, // check if the service is operating normally
    GET_STATS = 2, // receive internal statistics of the service
    RESET_STATS = 3, // reset internal statistics of the service
    COMPRESS = 4 // request some data to be compressed
};

#endif /* COMPRESSION_REQUEST */
