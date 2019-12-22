#ifndef TEST_CLIENT_SENDER
#define TEST_CLIENT_SENDER

#include "main_config.h"
#include "internal_request.h"

typedef enum sender_exit_status
{
    SND_BAD_ARGS,
    SND_REQUESTS_OBTAINING_ERROR,
    SND_CONNECTION_ERROR,
    SND_THREAD_ERROR,
    SND_SUCCESS
} sender_exit_status_t;

typedef struct sender_args
{
#ifdef REQUESTS_IMPORT_MODE
    const char* config_path;
#endif
    const char* node;
    const char* port_num;
    sender_exit_status_t exit_status;
} sender_args_t;

void* send_requests(void* args);

#endif /* TEST_CLIENT_SENDER */
