#include "uuid.h"

#include <pthread.h>

// TC_TODO: write UUID generation algorithm which uses /dev/urandom or
// use some utility/library.
static uint32_t dummy_uuid = 0;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void get_uuid(uint32_t* uuid)
{
    if(NULL == uuid) return;
    pthread_mutex_lock(&mtx);
    dummy_uuid++;
    *uuid = dummy_uuid;
    pthread_mutex_unlock(&mtx);
}
