#ifndef SIMPLE_COMPR_SERVICE_RECEIVER_LOGGING
#define SIMPLE_COMPR_SERVICE_RECEIVER_LOGGING

// TODO: rename
typedef enum receiver_event
{
    RECV_FAILED_EVENT,
    RECV_TIMEOUT_EVENT,
    RECV_DONE_EVENT,
} receiver_event_t;

/**
 * @brief
 * @param[in] args: .
 */
// SCS_TODO: variadic number of parameters?
void log_event(int connection_id, receiver_event_t event);

#endif /* SIMPLE_COMPR_SERVICE_RECEIVER_LOGGING */
