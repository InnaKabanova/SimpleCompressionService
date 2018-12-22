#ifndef TEST_CLIENT_SENDER
#define TEST_CLIENT_SENDER

struct sender_args
{
    int sock_descr;
    const char* filepath;
};

void* send_requests(void* args);

#endif /* TEST_CLIENT_SENDER */
