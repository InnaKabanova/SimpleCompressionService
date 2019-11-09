#include "logging_utilities.h"

#include <string.h>
#include <stdarg.h>
#include <syslog.h>

static const char* SCS_DBG_TAG = "SCS_DBG";

static const char* conn_result_2_str(const client_connection_result_t result)
{
    switch (result)
    {
        case CC_DONE: return "DONE";
        case CC_TIMEOUT: return "TIMEOUT";
        case CC_FAILED: return "FAILED";
    }
}

void log_conn_result(int conn_id, client_connection_result_t result, ...)
{
    va_list args;
    va_start(args, result);
    syslog(LOG_DEBUG, "%s | CONN %d: result=%s %s",
           SCS_DBG_TAG, conn_id, conn_result_2_str(result),
           CC_FAILED == result? strerror(va_arg(args,int)) : "");
    va_end(args);
}

void log_request(scs_internal_request_t* request)
{
    if(NULL == request) return;
    syslog(LOG_DEBUG, "%s: [mval: %d, uuid: %d, code: %d, plen: %d], PAYLOAD: %s, CONN: %d",
           SCS_DBG_TAG,
           request->header.magic_value, request->header.uuid,
           request->header.code, request->header.payload_len,
           request->header.payload_len > 0 ? request->payload : "none",
           request->sock_descr);
}



