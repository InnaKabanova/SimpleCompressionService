#ifndef SCS_SERVER
#define SCS_SERVER

/**
 * @brief start_server. binds to 'port_num' and marks the obtained
 * socket as passive (intended for incoming connections).
 * @param sock_descr: where the received socket will be written to
 * in case of the function's success.
 * @param port_num.
 * @param backlog: max. num to which the queue of pending connections
 * for *sock_descr may grow.
 * @return 1 -- success, 0 -- failure.
 */
int start_server(int* sock_descr, const char* port_num, int backlog);

#endif /* SCS_SERVER */
