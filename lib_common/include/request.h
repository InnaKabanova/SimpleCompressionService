#ifndef LIBCOMMON_REQUEST_FORMAT
#define LIBCOMMON_REQUEST_FORMAT

#include <stdint.h>

#define DEFAULT_MAGIC_VALUE 0x53545259

/**
 * @brief request_header. All data fields must be in network byte order
 * to be valid.
 */
typedef struct request_header
{
    const uint32_t magic_value;
    uint16_t payload_length;
    uint16_t request_code;
} request_header_t;

static const request_header_t template_request_header =
{.magic_value = DEFAULT_MAGIC_VALUE};

enum REQUEST_CODE
{
    PING = 1, // checks if a service is operating normally
    GET_STATS = 2, // requests internal statistics of a service
    RESET_STATS = 3, // resets internal statistics of a service
    COMPRESS = 4 // requests some data to be compressed
};

#endif /* LIBCOMMON_REQUEST_FORMAT */
