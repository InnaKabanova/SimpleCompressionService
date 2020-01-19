#ifndef SIMPLE_COMPR_SERVICE_PROCESSOR
#define SIMPLE_COMPR_SERVICE_PROCESSOR

/**
 * @brief Routine for a worker thread which retrieves one request
 * at a time from the requests queue and processes it in order to
 * create a response. The response is then put into the response queue
 * for further sending.
 */
void* process_requests(void* args);

#endif /* SIMPLE_COMPR_SERVICE_PROCESSOR */
