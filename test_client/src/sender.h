#ifndef TEST_CLIENT_SENDER
#define TEST_CLIENT_SENDER

typedef struct sender_args
{
    const char* filepath;
    const char* node;
    const char* port_num;
} sender_args_t;

void* send_requests(void* args);

#endif /* TEST_CLIENT_SENDER */
