#ifndef TEST_CLIENT_RECEIVER
#define TEST_CLIENT_RECEIVER

typedef enum receiver_exit_status
{
    RECV_SUCCESS
} receiver_exit_status_t;

/**
 * @brief Worker thread which receives responses from the server.
 * @param[in] args: a socket descriptor for a current valid connection
 * with the server.
 */
<<<<<<< HEAD
void* receive_requests(void* args);
=======
void* receive_responses(void* args);
>>>>>>> ccd6553... TC: add response-receiving threads

#endif /* TEST_CLIENT_RECEIVER */
