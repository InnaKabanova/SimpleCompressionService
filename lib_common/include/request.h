#ifndef SCSCOMMON_REQUEST_FORMAT
#define SCSCOMMON_REQUEST_FORMAT

#include <stdint.h>

#define REQUEST_MAGIC_VALUE 0x53545259
#define MAX_REQUEST_PAYLOAD_LEN UINT8_MAX

typedef enum request_code
{
    REQ_PING = 1, // checks if the service is operating normally
    REQ_GET_STATS = 2, // requests internal statistics of the service
    REQ_RESET_STATS = 3, // resets internal statistics of the service
    REQ_COMPRESS = 4 // requests data to be compressed
} request_code_t;

/**
 * @brief All data fields must be in network byte order to be valid.
 */
typedef struct request_header
{
    uint32_t magic_value;
    uint32_t uuid;
    uint16_t code;
    uint8_t payload_len;
} request_header_t;

#endif /* SCSCOMMON_REQUEST_FORMAT */
