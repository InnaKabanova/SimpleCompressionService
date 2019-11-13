#ifndef SIMPLE_COMPR_SERVICE_LOGGING_UTILITIES
#define SIMPLE_COMPR_SERVICE_LOGGING_UTILITIES

#include "internal_request.h"

typedef enum client_connection_result
{
    CC_FAILED,
    CC_TIMEOUT,
    CC_DONE,
} client_connection_result_t;

/**
 * @brief Log result of connection with a client.
 * @param[in] conn_id: connection ID
 * @param[in] result: result of the connection with the client
 * @param[in] errno: mandatory if result is CC_FAILED
 */
void log_conn_result(int conn_id, client_connection_result_t result, ...);

void log_requests_received(unsigned int num, int conn_id);

void log_request(scs_internal_request_t* request);

#endif /* SIMPLE_COMPR_SERVICE_LOGGING_UTILITIES */
