#ifndef TEST_CLIENT_CONFIG
#define TEST_CLIENT_CONFIG

// Global configuration of the test client:

/**
 * @brief The test_client's operation mode depends on the source of
 * requests which are sent to a compression service. A source can be
 * either:
 * - a file, from which requests are imported/deserialized
 * (REQUESTS_IMPORT_MODE define);
 * - programming code with ready requests of various format (both
 * valid & invalid) for debugging purposes (REQUESTS_GENERATION_MODE
 * define).
 *
 * If REQUESTS_IMPORT_MODE is enabled, 1 file with requests is processed
 * by 1 sender thread. Max. number of sender threads working at a time
 * is limited by MAX_SENDERS_NUM, although number of files with requests
 * is not limited by the implementation.
 */

//#define REQUESTS_IMPORT_MODE
#ifndef REQUESTS_IMPORT_MODE
#define REQUESTS_GENERATION_MODE
#endif

/**
 * @brief Max. number of worker threads that _simultaneously_ send
 * requests to a compression service.
 */
#define MAX_SENDERS_NUM 3

/**
 * @brief Max. number of worker threads that _simultaneously_ receive
 * responses from a compression service.
 */
#define MAX_RECEIVERS_NUM 3

#endif /* TEST_CLIENT_CONFIG */
