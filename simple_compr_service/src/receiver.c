#include "receiver.h"
#include "requests_queue.h"

#ifdef SCS_RECEIVER_DBG
#include "logging_utilities.h"
#endif

#include <stdlib.h>

#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <sys/syslog.h>

/**
 * @brief Max. amount of time to wait for new data on a socket before
 * considering the connection no longer opened/valid.
 */
#define MAX_DATA_WAITING_TIME_SEC 5

/**
 * @brief Max. amount of time to wait for a free slot in the request
 * queue in case it is full until to-be-pushed request is dropped.
 */
#define MAX_QUEUE_WAITING_TIME_SEC 2

typedef enum recv_status
{
    RECV_BAD_ARGS = -2,
    RECV_FAILED = -1,
    RECV_TIMEOUT = 0,
    RECV_SUCCESS = 1, // complete package is received
    RECV_DONE = 2, // client is done sending requests
} recv_status_t;

recv_status_t receive_raw_data(const int sock_descr, void* buff,
                               const size_t buff_size);
int deserialize_data(scs_internal_request_t* candidate);

void* receive_requests(void* args)
{
    if(!args) pthread_exit(NULL);
    receiver_args_t* thread_io = (receiver_args_t*)args;

    struct timeval timeout;
    timeout.tv_sec = MAX_DATA_WAITING_TIME_SEC;
    timeout.tv_usec = 0;
    setsockopt(thread_io->sock_descr, SOL_SOCKET,
               SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

    unsigned int requests_received = 0;
    recv_status_t status = RECV_SUCCESS;
    do
    {
        void* buff = malloc(sizeof(scs_internal_request_t));
        // Try to receive a request's header:
        status = receive_raw_data(thread_io->sock_descr, buff,
                                  sizeof(request_header_t));
        switch(status)
        {
            case RECV_TIMEOUT:
                free(buff);
#ifdef SCS_RECEIVER_DBG
                log_conn_result(thread_io->sock_descr, CC_TIMEOUT);
#endif
                break;
            case RECV_FAILED:
                free(buff);
#ifdef SCS_RECEIVER_DBG
                log_conn_result(thread_io->sock_descr, CC_FAILED, errno);
#endif
                break;
            case RECV_DONE:
                free(buff);
#ifdef SCS_RECEIVER_DBG
                log_conn_result(thread_io->sock_descr, CC_DONE);
#endif
                break;
            case RECV_SUCCESS:
            {
                // Try to interpret the received raw data as a request
                // header and receive payload if any:
                scs_internal_request_t* temp = (scs_internal_request_t*)buff;
                temp->sock_descr = thread_io->sock_descr;
                if(deserialize_data(temp))
                {
                    if(0 == push_request_blocking(temp,
                                                  MAX_QUEUE_WAITING_TIME_SEC))
                        free(temp);
                    else
                        requests_received++;
                }
                else
                {
                    free(temp);
                }
                break;
            }
        }
    } while(RECV_TIMEOUT != status && RECV_DONE != status &&
            RECV_FAILED != status && RECV_BAD_ARGS != status);

#ifdef SCS_RECEIVER_DBG
    log_requests_received(requests_received, thread_io->sock_descr);
#endif

    free(thread_io);
}

recv_status_t receive_raw_data(const int sock_descr, void* buff,
                               const size_t buff_size)
{
    if(NULL == buff || 0 == buff_size) return RECV_BAD_ARGS;

    ssize_t bytes_received = 0;
    while(bytes_received < buff_size)
    {
        ssize_t ret = recv(sock_descr, buff + bytes_received,
                           buff_size - bytes_received, 0);

        if(errno == EAGAIN || errno == EWOULDBLOCK) // timeout expired
        {
            if(ret + bytes_received == buff_size)
                return RECV_SUCCESS;
            else
                return RECV_TIMEOUT;
        }
        else if(-1 == ret) // error generated by the socket layer
        {
            return RECV_FAILED;
        }
        else if(0 == ret) // client disconnected
        {
            return RECV_DONE;
        }
        else if(ret > 0) // some bytes were successfully received
        {
            bytes_received += ret;
            if(bytes_received < buff_size) continue;
            else if(bytes_received == buff_size) return RECV_SUCCESS;
        }
    }
}

int deserialize_data(scs_internal_request_t* candidate)
{
    if(NULL == candidate) return 0;

    candidate->header.magic_value = ntohl(candidate->header.magic_value);
    if(REQUEST_MAGIC_VALUE != candidate->header.magic_value) return 0;

    candidate->header.code = ntohs(candidate->header.code);
    candidate->header.uuid = ntohl(candidate->header.uuid);
    candidate->header.payload_len = ntohs(candidate->header.payload_len);

    // In the current implementation, payload_len is the main indicator
    // that there is payload data following the request. If request code
    // doesn't correspond to payload's presence (i.e. != REQ_COMPRESS),
    // what is considered to be payload is disposed and the request
    // of such an invalid format will be handled later on during processing.

    if(candidate->header.payload_len > 0)
    {
        void* payload_buff = malloc(sizeof(char) * candidate->header.payload_len);
        recv_status_t status = receive_raw_data(candidate->sock_descr,
                                                payload_buff,
                                                candidate->header.payload_len);
        if(RECV_SUCCESS == status &&
           REQ_COMPRESS == candidate->header.code &&
           candidate->header.payload_len <= MAX_REQUEST_PAYLOAD_LEN)
        {
            candidate->payload = payload_buff;

            syslog(LOG_DEBUG, "PAYLOAD | %s", candidate->payload);
        }
        else
        {
            free(payload_buff);
            candidate->payload = NULL;
        }
    }
    return 1;
}

