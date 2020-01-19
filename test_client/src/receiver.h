#ifndef TEST_CLIENT_RECEIVER
#define TEST_CLIENT_RECEIVER

typedef enum receiver_exit_status
{
    RECV_SUCCESS
} receiver_exit_status_t;

const char* receiver_exit_status_2_str(receiver_exit_status_t status);

/**
 * @brief Worker thread which receives responses from the server.
 * @param[in] args: a socket descriptor for a current valid connection
 * with the server.
 */
void* receive_responses(void* args);

#endif /* TEST_CLIENT_RECEIVER */
