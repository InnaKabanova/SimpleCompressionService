#ifndef TEST_CLIENT_REQUESTS_GENERATION
#define TEST_CLIENT_REQUESTS_GENERATION

#include "internal_request.h"

/**
 * Test scenario defines a specific test case, either positive or
 * negative, which ensures that compression service under test processes
 * incoming data properly.
 */
typedef enum test_scenario
{
    /**
     * Positive test: send several valid requests without payload and
     * disconnect normally.
     */
    TS_VALID_REQUESTS_WITHOUT_PAYLOAD = 0b101,

    /**
     * Positive test: send several valid requests with payload and
     * disconnect normally.
     */
    TS_VALID_REQUESTS_WITH_PAYLOAD = 0b110,

    /**
     * Negative test: send several valid requests with payload and
     * do not disconnect normally, but wait for recv timeout on
     * the server's side.
     */
    TS_VALID_REQUESTS_TIMEOUT_EXPIRED = 0b10011,

    /**
     * Negative test: send incomplete data and do not disconnect
     * normally, but wait for recv timeout on the server's side.
     */
    TS_INCOMPLETE_DATA_TIMEOUT_EXPIRED = 0b11000,

    /**
     * Negative test: send several valid requests without payload and
     * do not disconnect normally, but cause a socket layer error.
     */
    TS_VALID_REQUESTS_SOCKET_LAYER_ERROR = 0b100001,

    /**
     * Negative test: send incomplete data and do not disconnect
     * normally, but cause a socket layer error.
     */
    TS_INCOMPLETE_DATA_SOCKET_LAYER_ERROR = 0b101000,

    /**
     * Negative test: send requests with invalid magic number in the
     * headers and disconnect normally.
     */
    TS_INVALID_REQUEST_MAGIC_NUMBER = 0b11000000,

    /**
     * Negative test: send requests with invalid request codes in the
     * headers and disconnect normally.
     */
    TS_INVALID_REQUEST_CODE = 0b101000000,

    /**
     * Negative test: send requests with payload of size that exceeds
     * the limit and disconnect normally.
     */
    TS_INVALID_REQUEST_PAYLOAD_TOO_LONG = 0b1001000000,
} test_scenario_t;

/**
 * Test actions comprise a test case.
 */
typedef struct test_action
{
    const char* descr;
    void (*action)(void);
    struct test_action* next_test_action;
} test_action_t;

test_action_t* generate_requests(test_scenario_t scenario);

#endif /* TEST_CLIENT_REQUESTS_GENERATION */
