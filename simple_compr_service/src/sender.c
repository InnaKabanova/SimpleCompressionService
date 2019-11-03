#include "sender.h"

#include <pthread.h>
#include <sys/syslog.h>

#define SCS_SENDER_DEBUGGING

void* send_responses(void* args)
{
    // syslog(LOG_DEBUG, "From %lu | ", pthread_self());
    pthread_exit(NULL);
}
