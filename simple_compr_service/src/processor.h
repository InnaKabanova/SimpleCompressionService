#ifndef SCS_REQUESTS_PROCESSOR
#define SCS_REQUESTS_PROCESSOR

typedef enum processor_exit_status
{
    PRC_OK,
    PRC_ANOTHER_ERROR
} processor_exit_status_t;

typedef struct processor_args
{
    processor_exit_status_t exit_status;
} processor_args_t;

void* process_requests(void* args);

#endif /* SCS_REQUESTS_PROCESSOR */
