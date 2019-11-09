#ifndef SIMPLE_COMPR_SERVICE_QUEUE
#define SIMPLE_COMPR_SERVICE_QUEUE

#include <pthread.h>

typedef struct scs_queue
{
    pthread_mutex_t mutex;
    pthread_cond_t non_empty;
    pthread_cond_t non_full;
    int size;
    void* circ_buffer;
    int next_in;
    int next_out;
} scs_queue_t;

// The following utility routines do not lock the mutex, and
// synchronization responsibility lies on the clients.
int queue_empty(const scs_queue_t* queue);
int queue_full(const scs_queue_t* queue);
int incr_index(int next_in, int size);

#endif /* SIMPLE_COMPR_SERVICE_QUEUE */
