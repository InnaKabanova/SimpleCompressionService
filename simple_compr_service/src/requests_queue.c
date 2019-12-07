#include "queue.h"
#include "requests_queue.h"

#include <stdlib.h>
#include <time.h>

static int requests_queue_initialized = 0;
static scs_internal_request_t** requests_buffer = NULL;
static scs_queue_t requests_queue;
static pthread_mutex_t initial_mutex = PTHREAD_MUTEX_INITIALIZER;

int init_requests_queue(int size)
{
    pthread_mutex_lock(&initial_mutex);
    if(requests_queue_initialized)
    {
        pthread_mutex_unlock(&requests_queue.mutex);
        return 0;
    }

    if(0 != pthread_mutex_init(&requests_queue.mutex, NULL)) return 0;
    if(0 != pthread_cond_init(&requests_queue.non_full, NULL)) return 0;
    if(0 != pthread_cond_init(&requests_queue.non_empty, NULL)) return 0;

    requests_queue.size = size;

    requests_queue.circ_buffer = calloc(size, sizeof(scs_internal_request_t*));
    requests_buffer = (scs_internal_request_t**)requests_queue.circ_buffer;

    requests_queue.next_in = 0;
    requests_queue.next_out = 0;

    requests_queue_initialized = 1;
    pthread_mutex_unlock(&initial_mutex);
    return 1;
}

static int push_request_internal(scs_internal_request_t* request)
{
    if(!requests_queue_initialized) return 0;

    requests_buffer[requests_queue.next_in] = request;
    requests_queue.next_in = incr_index(requests_queue.next_in,
                                        requests_queue.size);
    return 1;
}

int push_request_blocking(scs_internal_request_t* request,
                          int max_waiting_time_secs)
{
    if(NULL == request) return 0;

    int ret = 0;
    int pushed = 0;
    struct timespec tm = {.tv_sec = max_waiting_time_secs,
                          .tv_nsec = 0};

    pthread_mutex_lock(&requests_queue.mutex);
    while(queue_full(&requests_queue) && 0 == ret)
    {
        ret = pthread_cond_timedwait(&requests_queue.non_full,
                                     &requests_queue.mutex,
                                     &tm);
    }
    if(0 == ret) pushed = push_request_internal(request);
    pthread_cond_broadcast(&requests_queue.non_empty);
    pthread_mutex_unlock(&requests_queue.mutex);
    return pushed;
}

static scs_internal_request_t* pop_request_internal(void)
{
    if(!requests_queue_initialized) return NULL;

    scs_internal_request_t* request =
            requests_buffer[requests_queue.next_out];
    requests_queue.next_out = incr_index(requests_queue.next_out,
                                          requests_queue.size);
    return request;
}

scs_internal_request_t* pop_request_blocking(void)
{
    int ret = 0;
    scs_internal_request_t* request = NULL;

    pthread_mutex_lock(&requests_queue.mutex);
    while(queue_empty(&requests_queue) && 0 == ret)
    {
        ret = pthread_cond_wait(&requests_queue.non_empty,
                                &requests_queue.mutex);
    }
    if(0 == ret) request = pop_request_internal();
    pthread_cond_broadcast(&requests_queue.non_full);
    pthread_mutex_unlock(&requests_queue.mutex);
    return request;
}

// SCS_TODO: deinit_requests_queue is to be called on the daemon's
// stop/pause
static int deinit_requests_queue(void)
{
    pthread_mutex_lock(&requests_queue.mutex);
    if(!requests_queue_initialized)
    {
        pthread_mutex_unlock(&requests_queue.mutex);
        return 0;
    }

    pthread_mutex_destroy(&requests_queue.mutex);
    pthread_cond_destroy(&requests_queue.non_full);
    pthread_cond_destroy(&requests_queue.non_empty);

    requests_queue.size = 0;
    requests_buffer = NULL;
    free(requests_queue.circ_buffer);
    requests_queue.next_in = 0;
    requests_queue.next_out = 0;

    requests_queue_initialized = 0;
    pthread_mutex_unlock(&requests_queue.mutex);
    return 1;
}
