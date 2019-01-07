#ifndef LIBCOMMON_REQUEST_FORMAT
#define LIBCOMMON_REQUEST_FORMAT

#include <stdint.h>

const int DEFAULT_MAGIC_VALUE = 0x53545259;

// All data fields must be in network byte order to be valid.
const struct request_header
{
    const uint32_t magic_value;
    uint16_t payload_length;
    uint16_t request_code;
} template_instance = {.magic_value = DEFAULT_MAGIC_VALUE};

enum REQUEST_CODE
{
    PING = 1, // checks if a service is operating normally
    GET_STATS = 2, // requests internal statistics of a service
    RESET_STATS = 3, // resets internal statistics of a service
    COMPRESS = 4 // requests some data to be compressed
};

#endif /* LIBCOMMON_REQUEST_FORMAT */
