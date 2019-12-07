#include "queue.h"
#include "responses_queue.h"

#include <stdlib.h>
#include <time.h>

static int responses_queue_initialized = 0;
static scs_internal_response_t** responses_buffer = NULL;
static scs_queue_t responses_queue;
static pthread_mutex_t initial_mutex = PTHREAD_MUTEX_INITIALIZER;

int init_responses_queue(int size)
{
    pthread_mutex_lock(&initial_mutex);
    if(responses_queue_initialized)
    {
        pthread_mutex_unlock(&responses_queue.mutex);
        return 0;
    }

    if(0 != pthread_mutex_init(&responses_queue.mutex, NULL)) return 0;
    if(0 != pthread_cond_init(&responses_queue.non_full, NULL)) return 0;
    if(0 != pthread_cond_init(&responses_queue.non_empty, NULL)) return 0;

    responses_queue.size = size;

    responses_queue.circ_buffer = calloc(size, sizeof(scs_internal_response_t*));
    responses_buffer = (scs_internal_response_t**)responses_queue.circ_buffer;

    responses_queue.next_in = 0;
    responses_queue.next_out = 0;

    responses_queue_initialized = 1;
    pthread_mutex_unlock(&initial_mutex);
    return 1;
}

static int push_response_internal(scs_internal_response_t* response)
{
    if(!responses_queue_initialized) return 0;

    responses_buffer[responses_queue.next_in] = response;
    responses_queue.next_in = incr_index(responses_queue.next_in,
                                         responses_queue.size);
    return 1;
}

int push_response_blocking(scs_internal_response_t* response,
                           int max_waiting_time_secs)
{
    if(NULL == response) return 0;

    int ret = 0;
    int pushed = 0;
    struct timespec tm = {.tv_sec = max_waiting_time_secs,
                          .tv_nsec = 0};

    pthread_mutex_lock(&responses_queue.mutex);
    while(queue_full(&responses_queue) && 0 == ret)
    {
        ret = pthread_cond_timedwait(&responses_queue.non_full,
                                     &responses_queue.mutex,
                                     &tm);
    }
    if(0 == ret) pushed = push_response_internal(response);
    pthread_cond_broadcast(&responses_queue.non_empty);
    pthread_mutex_unlock(&responses_queue.mutex);
    return pushed;
}

static scs_internal_response_t* pop_response_internal(void)
{
    if(!responses_queue_initialized) return NULL;

    scs_internal_response_t* response =
            responses_buffer[responses_queue.next_out];
    responses_queue.next_out = incr_index(responses_queue.next_out,
                                          responses_queue.size);
    return response;
}

scs_internal_response_t* pop_response_blocking()
{
    int ret = 0;
    scs_internal_response_t* response = NULL;

    pthread_mutex_lock(&responses_queue.mutex);
    while(queue_empty(&responses_queue) && 0 == ret)
    {
        ret = pthread_cond_wait(&responses_queue.non_empty,
                                &responses_queue.mutex);
    }
    if(0 == ret) response = pop_response_internal();
    pthread_cond_broadcast(&responses_queue.non_full);
    pthread_mutex_unlock(&responses_queue.mutex);
    return response;
}

// SCS_TODO: deinit_responses_queue is to be called on the daemon's
// stop/pause
static int deinit_responses_queue(void)
{
    pthread_mutex_lock(&responses_queue.mutex);
    if(!responses_queue_initialized)
    {
        pthread_mutex_unlock(&responses_queue.mutex);
        return 0;
    }

    pthread_mutex_destroy(&responses_queue.mutex);
    pthread_cond_destroy(&responses_queue.non_full);
    pthread_cond_destroy(&responses_queue.non_empty);

    responses_queue.size = 0;
    responses_buffer = NULL;
    free(responses_queue.circ_buffer);
    responses_queue.next_in = 0;
    responses_queue.next_out = 0;

    responses_queue_initialized = 0;
    pthread_mutex_unlock(&responses_queue.mutex);
    return 1;
}
