#ifndef SCSCOMMON_RESPONSE_FORMAT
#define SCSCOMMON_RESPONSE_FORMAT

#include <stdint.h>

#define RESPONSE_MAGIC_VALUE 0x53545265

typedef enum response_status
{
    RESP_OK = 0,
    RESP_UNKNOWN_ERROR = 1,
    RESP_UNKNOWN_REQUEST_CODE = 2,
    RESP_STR_TOO_LARGE = 3 // payload (data for compression) is too large
} response_status_t;

typedef struct response_header
{
    uint32_t magic_value;
    uint32_t uuid;
    uint16_t status;
    uint16_t payload_len;
} response_header_t;

#endif /* SCSCOMMON_RESPONSE_FORMAT */
