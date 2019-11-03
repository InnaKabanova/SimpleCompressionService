#include "processor.h"

#include <pthread.h>
#include <sys/syslog.h>

void* process_requests(void* args)
{
    // syslog(LOG_DEBUG, "From %lu | ", pthread_self());
    pthread_exit(NULL);
}
