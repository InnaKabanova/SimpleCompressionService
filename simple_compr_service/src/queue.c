#include "queue.h"

int queue_empty(const scs_queue_t* queue)
{
    if(NULL == queue) return 0;
    return queue->next_in == queue->next_out;
}

int queue_full(const scs_queue_t* queue)
{
    if(NULL == queue) return 0;
    return incr_index(queue->next_in, queue->size) == queue->next_out;
}

int incr_index(int index, int size)
{
    return (index + 1) % size;
}
