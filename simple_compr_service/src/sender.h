#ifndef SCS_RESPONSES_SENDER
#define SCS_RESPONSES_SENDER

typedef enum sender_exit_status
{
    SND_OK,
    SND_ANOTHER_ERROR
} sender_exit_status_t;

typedef struct sender_args
{
    sender_exit_status_t exit_status;
} sender_args_t;

void* send_responses(void* args);

#endif /* SCS_RESPONSES_SENDER */
