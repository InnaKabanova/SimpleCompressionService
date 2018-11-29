#ifndef REQUEST_PROCESSOR
#define REQUEST_PROCESSOR

void* accept_requests(void* args);
void* process_requests(void* args);
void* send_responses(void* args);

#endif /* REQUEST_PROCESSOR */
