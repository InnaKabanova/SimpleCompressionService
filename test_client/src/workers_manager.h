#ifndef TEST_CLIENT_WORKERS_MANAGER
#define TEST_CLIENT_WORKERS_MANAGER

typedef struct sender_result
{
    int sock_descr;
    struct sender_result* next;
} sender_result_t;

/**
 * @brief Creates & launches requests-sending threads. Blocks until all
 * the threads' execution is completed (either successfully or not).
 * @param[in] node: IP address of the target compression service.
 * @param[in] port number of the target compression service.
 * @return In case of success, this function returns a linked list of data
 * necessary for response-receiving threads execution. The returned list's
 * size corresponds to the number of requests-sending threads that were
 * successfully created and returned SND_SUCCESS on join. Disposal
 * of the linked list's items must be done by this function's caller.
 * In case of failure, this function returns NULL.
 */
sender_result_t* run_senders(const char* node, const char* port_num);

/**
 * @brief Creates & launches responses-receiving threads. Blocks until all
 * the threads' execution is completed (either successfully or not).
 * @param[in] results: linked list with the data necessary for
 * response-receiving threads execution obtained as a result of preceding
 * call to 'run_senders'.
 * @return Number of response-receiving threads successfully created &
 * run.
 */
int run_receivers(sender_result_t* results);

#endif /* TEST_CLIENT_WORKERS_MANAGER */
