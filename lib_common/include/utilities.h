#ifndef LIBCOMMON_UTILITIES
#define LIBCOMMON_UTILITIES

#include <netdb.h>

/**
 * @brief Prints the following fields of a provided addrinfo struct to
 * stdout: ai_family, ai_socktype, ai_protocol, ai_addr, ai_addrlen.
 * @param[in] info: must have either AF_INET or AF_INET6 address
 * domain.
 * @param[in] tag: optional text to print before the parameters.
 * @param[in] print_error: whether to print error message to stdout
 * in case this function's internal error occurs, i.e. the value is
 * evaluated in if-conditions.
 * @return 1 -- success, 0 -- failure.
 */
int print_addrinfo(const struct addrinfo* info, const char* tag,
                   const int print_error);

/**
 * @brief Makes sure all the data is sent through the specified socket
 * (as the 'send' system call doesn't always send all the data at once).
 * @param[in] sock_descr: socket to send data through.
 * @param[in] buff: buffer containing the data to send.
 * @param[inout] buff_size: when [in], a pointer to an int containing
 * the buff's size in bytes; when [out], set to num of bytes actually
 * sent (on success) or set to -1 if 'send' syscall returned with an
 * error.
 * @return 1 -- success, 0 -- failure.
 */
int send_all(const int sock_descr, const char* buff, int* buff_size);

typedef enum recv_status
{
    RECV_BAD_ARGS = -2,
    RECV_FAILED = -1,
    RECV_TIMEOUT = 0,
    RECV_SUCCESS = 1, // complete package is received
    RECV_DONE = 2, // client has closed the connection
} recv_status_t;

/**
 * @brief Receives 'buff_size' bytes of raw data on 'sock_descr' socket
 * and writes them to a buffer located by 'buff' address.
 */
recv_status_t receive_raw_data(const int sock_descr, void* buff,
                               const size_t buff_size);

#endif /* LIBCOMMON_UTILITIES */
