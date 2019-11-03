#ifndef SCSCOMMON_RESPONSE_FORMAT
#define SCSCOMMON_RESPONSE_FORMAT

#include <stdint.h>

enum response_code_t
{
    SUCCESS = 0,
    UNKNOWN_ERROR = 1,
    UNKNOWN_REQUEST_CODE = 2,
    STR_TOO_LARGE = 3 // payload (data for compression) is too large
};

typedef struct response_header
{
    uint32_t magic_value;
    uint32_t uuid;
    uint16_t code;
    uint16_t payload_len;
} response_header_t;

typedef struct response
{
    response_header_t header;
    const char* payload;
} response_t;

#endif /* SCSCOMMON_RESPONSE_FORMAT */
