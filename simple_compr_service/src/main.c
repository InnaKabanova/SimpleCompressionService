#include "requests_queue.h"
#include "responses_queue.h"
#include "receiver.h"
#include "processor.h"
#include "sender.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

//--------------------------------------------------------------------
// Service configuration:

#define SERVICE_NAME "simple_compr_service"

/**
 * @brief The lowest priority of a log message required for that message
 * to be submitted to Syslog.
 */
#define SCS_LOG_UPTO (LOG_DEBUG)

/**
 * @brief Max. number of pending incoming connections.
 */
#define SERVICE_BACKLOG 8

/**
 * @brief Number of worker threads that process incoming requests and
 * produce responses.
 */
#define PROCESSORS_NUM 2

/**
 * @brief Number of worker threads that send responses to clients.
 */
#define SENDERS_NUM 1

/**
 * @brief Percentage of successfully created PROCESSOR threads
 * (relative to PROCESSORS_NUM) required for the service to operate.
 * Value range: [0.0,1.0].
 */
#define MIN_PROCESSORS_PERCENTAGE 0.6

/**
 * @brief Percentage of successfully created SENDER threads
 * (relative to SENDERS_NUM) required for the service to operate.
 * Value range: [0.0,1.0].
 */
#define MIN_SENDERS_PERCENTAGE 1.0

#define REQUESTS_QUEUE_SIZE 50
#define RESPONSES_QUEUE_SIZE 50

//--------------------------------------------------------------------

pthread_attr_t attr; // holds common attributes of worker threads

//--------------------------------------------------------------------

void init_syslog(void);
int init_workers_attributes(pthread_attr_t* attr);
int init_daemon(void);
int init_networking(int* sock_descr, const char* port_num,
                    const int backlog);
int init_queues(void);
void accept_connections(const int sock_descr);
void cleanup(void);

//--------------------------------------------------------------------

int main(int argc, char* argv[])
{
    int sock_descr = -1;
    // Worker threads:
    pthread_t processors_pool[PROCESSORS_NUM];
    pthread_t senders_pool[SENDERS_NUM];

    if(argc > 2 || argc < 2 || 0 == atoi(argv[1]))
        exit(EXIT_FAILURE);
    if(0 == init_workers_attributes(&attr))
        exit(EXIT_FAILURE);

    atexit(cleanup);
    init_syslog();

    if(0 == init_daemon())
        exit(EXIT_FAILURE);

    if(0 == init_networking(&sock_descr, argv[1], SERVICE_BACKLOG))
        exit(EXIT_FAILURE);

    if(0 == init_queues())
        exit(EXIT_FAILURE);

    int err = 0;
    int processors_created = 0;
    int senders_created = 0;
    for(int i = 0; i < PROCESSORS_NUM; i++)
    {
        if(0 != (err = pthread_create(&processors_pool[i], &attr,
                                      process_requests, NULL)))
        {
            syslog(LOG_ERR, "Failed to create a processor thread #%d: "
                            "[%d]", i, err);
        }
        else
        {
            syslog(LOG_DEBUG, "Created a processor thread #%d with ID %lu",
                   i + 1, processors_pool[i]);
            processors_created++;
        }
    }
    for(int i = 0; i < SENDERS_NUM; i++)
    {
        if(0 != (err = pthread_create(&senders_pool[i], &attr,
                                      send_responses, NULL)))
        {
            syslog(LOG_ERR, "Failed to create a sender thread #%d: "
                            "[%d]", i, err);
        }
        else
        {
            syslog(LOG_DEBUG, "Created a sender thread #%d with ID %lu",
                   i + 1, senders_pool[i]);
            senders_created++;
        }
    }
    if(processors_created < PROCESSORS_NUM * MIN_PROCESSORS_PERCENTAGE ||
       senders_created < SENDERS_NUM * MIN_SENDERS_PERCENTAGE)
    {
        syslog(LOG_CRIT, "Not enough worker threads created for normal"
                         "operation. Terminating.");
        exit(EXIT_FAILURE);
    }

    accept_connections(sock_descr);
    pthread_exit(NULL);
}

void cleanup(void)
{
    pthread_attr_destroy(&attr);
    closelog();
}

int init_workers_attributes(pthread_attr_t* attr)
{
    if(0 != pthread_attr_init(attr))
        return 0;
    if(0 != pthread_attr_setdetachstate(attr, PTHREAD_CREATE_DETACHED))
        return 0;

    return 1;
}

void init_syslog(void)
{
    setlogmask(LOG_UPTO (SCS_LOG_UPTO));
    openlog(SERVICE_NAME, LOG_CONS | LOG_NDELAY | LOG_PID, LOG_DAEMON);
    syslog(LOG_NOTICE, "Getting to work.");
}

int init_daemon(void)
{
    pid_t pid, sid;

    pid = fork();
    if(-1 == pid)
    {
        syslog(LOG_CRIT, "Failed to fork off: [%d] %m", errno);
        return 0;
    }
    else if(pid > 0)
        exit(EXIT_SUCCESS);

    syslog(LOG_INFO, "Parent ID: %d", getppid());
    sid = setsid();
    if(-1 == sid)
    {
        syslog(LOG_CRIT, "Failed to get a session ID: [%d] %m", errno);
        return 0;
    }

    umask(0);

    if(-1 == chdir("/"))
    {
        syslog(LOG_CRIT, "Failed to change working directory: [%d] %m",
               errno);
        return 0;
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    return 1;
}

int init_networking(int* sock_descr, const char* port_num,
                    const int backlog)
{
    struct addrinfo hints;
    struct addrinfo* serv_info;

    int status;
    struct addrinfo* i;

    memset((void*)&hints, 0, sizeof(hints));

    // This service will listen on the current host's IP address:[portnum]:
    hints.ai_family = AF_UNSPEC; // both IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE; // use the IP address of the local host

    if(0 != (status = getaddrinfo(NULL, port_num, &hints, &serv_info)))
    {
        syslog(LOG_CRIT, "Networking error: %s: [%d] %m",
               gai_strerror(status), errno);
        return 0;
    }

    // Loop through address lookup results and bind to the first we can:
    for(i = serv_info; i != NULL; i = i->ai_next)
    {
        if(-1 == (*sock_descr = socket(i->ai_family, i->ai_socktype,
                                       i->ai_protocol)))
        {
            continue;
        }

        status = setsockopt(*sock_descr, SOL_SOCKET, SO_REUSEADDR,
                            &(int){1}, sizeof(int));
        if(status < 0)
        {
            syslog(LOG_ERR, "setsockopt(SO_REUSEADDR) failed");
        }

        if(-1 == bind(*sock_descr, i->ai_addr, i->ai_addrlen))
        {
            close(*sock_descr);
            continue;
        }
        break;
    }

    freeaddrinfo(serv_info);

    if(NULL == i)
    {
        syslog(LOG_CRIT, "Failed to bind to the local host's IP "
                         "address");
        return 0;
    }
    if(-1 == listen(*sock_descr, backlog))
    {
        syslog(LOG_CRIT, "Failed to mark the obtained socket for "
                         "listening: [%d] %m", errno);
        return 0;
    }

    return 1;
}

int init_queues(void)
{
    return (init_requests_queue(REQUESTS_QUEUE_SIZE) &&
            init_responses_queue(RESPONSES_QUEUE_SIZE));
}

void accept_connections(const int sock_descr)
{
    int new_descr, err;
    struct sockaddr_storage client_addr;
    socklen_t client_addr_size = sizeof(client_addr);

    while(1)
    {
        new_descr = accept(sock_descr, (struct sockaddr*)&client_addr,
                           &client_addr_size);
        if(-1 == new_descr)
        {
            syslog(LOG_ERR, "Error accepting connection on the socket "
                            "%d: [%d] %m", sock_descr, errno);
            continue;
        }

        syslog(LOG_DEBUG, "Accepted connection on the socket %d. "
                         "New socket: %d", sock_descr, new_descr);
        receiver_args_t* args = (receiver_args_t*)malloc
                (sizeof(receiver_args_t));
        args->sock_descr = new_descr;
        err = pthread_create(&args->thread_id, &attr, receive_requests,
                             (void*)args);

#ifdef SCS_RECEIVER_DBG
        if(0 != err)
        {
            syslog(LOG_ERR, "Failed to create a receiver thread for "
                            "connection %d: [%d]", new_descr, err);
            close(new_descr);
            free(args);
            continue;
        }
        syslog(LOG_INFO, "Created a receiver thread for connection %d",
               new_descr);
#endif
    }
}

