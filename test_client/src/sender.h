#ifndef TEST_CLIENT_SENDER
#define TEST_CLIENT_SENDER

#include "main_config.h"
#include "internal_request.h"

typedef enum sender_exit_status
{
#ifdef REQUESTS_IMPORT_MODE
    SND_REQUESTS_IMPORT_ERROR,
#else
    SND_REQUESTS_GENERATION_ERROR,
#endif
    SND_BAD_ARGS,
    SND_CONNECTION_ERROR,
    SND_THREAD_ERROR,
    SND_SUCCESS
} sender_exit_status_t;

const char* sender_exit_status_2_str(sender_exit_status_t status);

typedef struct sender_args
{
#ifdef REQUESTS_IMPORT_MODE
    const char* config_path;
#endif
    const char* node;
    const char* port_num;
    int sock_descr;
    sender_exit_status_t exit_status;
} sender_args_t;

void* send_requests(void* args);

#endif /* TEST_CLIENT_SENDER */
