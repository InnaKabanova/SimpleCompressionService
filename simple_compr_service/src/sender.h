#ifndef SIMPLE_COMPR_SERVICE_SENDER
#define SIMPLE_COMPR_SERVICE_SENDER

/**
 * @brief Routine for a worker thread which retrieves one response
 * at a time from the responses queue and sends it back to the corresponding
 * client via specified established connection.
 */
void* send_responses(void* args);

#endif /* SIMPLE_COMPR_SERVICE_SENDER */
