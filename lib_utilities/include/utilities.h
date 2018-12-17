#ifndef UTILITIES
#define UTILITIES

#include <netdb.h>

/**
 * Prints the provided error message to stdout and terminates the
 * application with FAIL status.
 * @param msg: recommended format for consistent code style:
 * no spaces at the beginning/end of the string, starts with lowercase,
 * no '\n' at the end of the string.
 */
void exit_with_failure(const char* msg);

/**
 * Prints parameters of the provided addrinfo struct to stdout:
 * ai_family, ai_socktype, ai_protocol, ai_addr, ai_addrlen.
 * @param info: must have either AF_INET or AF_INET6 address domain.
 * @param tag: optional text to print before the parameters.
 * @param print_error: whether to print error message to stdout in case
 * an internal error occurs.
 * @return 1 -- success, 0 -- fail.
 */
int print_addrinfo(struct addrinfo* info, const char* tag,
                   int print_error);

#endif /* UTILITIES */
