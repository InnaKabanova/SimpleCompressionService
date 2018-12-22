#ifndef SCS_REQUEST_PROCESSOR
#define SCS_REQUEST_PROCESSOR

void* accept_requests(void* args);
void* process_requests(void* args);
void* send_responses(void* args);

#endif /* SCS_REQUEST_PROCESSOR */
