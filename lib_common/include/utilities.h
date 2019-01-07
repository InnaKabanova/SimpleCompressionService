#ifndef LIBCOMMON_UTILITIES
#define LIBCOMMON_UTILITIES

#include <netdb.h>

/**
 * @brief exit_with_failure. Prints the provided error message to
 * stdout and terminates the application with FAIL status.
 * @param msg: recommended format for consistent code style:
 * no spaces at the beginning/end of the string, starts with lowercase,
 * no '\n' at the end of msg.
 */
void exit_with_failure(const char* msg);

/**
 * @brief print_addrinfo. Prints parameters of the provided
 * addrinfo struct to stdout: ai_family, ai_socktype, ai_protocol,
 * ai_addr, ai_addrlen.
 * @param info: must have either AF_INET or AF_INET6 address domain.
 * @param tag: optional text to print before the parameters.
 * @param print_error: whether to print error message to stdout in case
 * the function's internal error occurs.
 * @return 1 -- success, 0 -- fail.
 */
int print_addrinfo(struct addrinfo* info, const char* tag,
                   int print_error);

/**
 * @brief send_all. Makes sure all the data is sent (because the 'send'
 * system call doesn't always send all the data at once).
 * @param sock_descr: socket to send data to.
 * @param buff: buffer containing the data to send.
 * @param buff_size: pointer to an int containing the buffer's size in
 * bytes.
 * @return 1 -- success, 0 --fail.
 */
int send_all(int sock_descr, char* buff, int* buff_size);

#endif /* LIBCOMMON_UTILITIES */
