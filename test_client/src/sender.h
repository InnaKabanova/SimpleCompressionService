#ifndef TEST_CLIENT_SENDER
#define TEST_CLIENT_SENDER

typedef enum sender_exit_status
{
    OK,
    REQUESTS_FILE_ERROR,
    CONNECTION_ERROR,
    OTHER_ERROR
} sender_exit_status_t;

typedef struct sender_args
{
    const char* filepath;
    const char* node;
    const char* port_num;
    sender_exit_status_t exit_status;
} sender_args_t;

void* send_requests(void* args);

#endif /* TEST_CLIENT_SENDER */
