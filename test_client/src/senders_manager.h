#ifndef TEST_CLIENT_SENDERS_MANAGER
#define TEST_CLIENT_SENDERS_MANAGER

/**
 * @brief Creates & launches requests-sending threads. Blocks until all
 * the threads are done with their jobs.
 * @param[in] node: IP address of the target compression service.
 * @param[in] port number of the target compression service.
 */
int run_senders(const char* node, const char* port_num);

#endif /* TEST_CLIENT_SENDERS_MANAGER */
