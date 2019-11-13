#include "requests_generation.h"
#include "uuid.h"
#include "test_data.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>

extern pthread_key_t SOCK_DESCR_KEY;

/**
 * Bit masks to determine which test actions are required for the
 * requested test scenario.
 */
typedef enum test_action_mask
{
    TAM_INITIALIZATION =                       0b0000000000,
    TAM_SEND_VALID_REQUESTS =                  0b0000000001,
    TAM_SEND_VALID_REQUESTS_WITH_PAYLOAD =     0b0000000010,
    TAM_DISCONNECT_NORMALLY =                  0b0000000100,
    TAM_GENERATE_INCOMPLETE_DATA =             0b0000001000,
    TAM_MAKE_RECV_TIMEOUT_EXPIRE =             0b0000010000,
    TAM_CAUSE_SOCKET_ERROR =                   0b0000100000,
    TAM_MAKE_INVALID_REQUEST =                 0b0001000000,
    TAM_MAKE_INVALID_MAGIC_NUMBER =            0b0010000000,
    TAM_MAKE_INVALID_REQUEST_CODE =            0b0100000000,
    TAM_MAKE_LONG_PAYLOAD =                    0b1000000000
} test_action_mask_t;

/**
 * Test actions' descriptions.
 */
static const char* TA_INITIALIZATION = "Initialization";
static const char* TA_SEND_VALID_REQUESTS =
        "Send valid requests without payload";
static const char* TA_SEND_VALID_REQUESTS_WITH_PAYLOAD =
        "Send valid requests with payload";
static const char* TA_DISCONNECT_NORMALLY = "Disconnect normally";

/**
 * Test actions.
 */
static void initialization(void);
static void send_valid_requests_without_payload(void);
static void send_valid_requests_with_payload(void);
static void disconnect_normally(void);

static test_action_t* create_test_action(test_action_mask_t mask)
{
    test_action_t* ta = (test_action_t*)malloc(sizeof(test_action_t));
    switch(mask)
    {
    case TAM_INITIALIZATION:
        ta->descr = TA_INITIALIZATION;
        ta->action = &initialization;
        break;
    case TAM_SEND_VALID_REQUESTS:
        ta->descr = TA_SEND_VALID_REQUESTS;
        ta->action = &send_valid_requests_without_payload;
        break;
    case TAM_SEND_VALID_REQUESTS_WITH_PAYLOAD:
        ta->descr = TA_SEND_VALID_REQUESTS_WITH_PAYLOAD;
        ta->action = &send_valid_requests_with_payload;
        break;
    case TAM_DISCONNECT_NORMALLY:
        ta->descr = TA_DISCONNECT_NORMALLY;
        ta->action = &disconnect_normally;
        break;
    }
    ta->next_test_action = NULL;
    return ta;
}

/**
 * Test actions compilation into a test case as per the requested
 * scenario.
 */
test_action_t* generate_requests(test_scenario_t scenario)
{
    test_action_t* first_ta = create_test_action(TAM_INITIALIZATION);
    test_action_t* curr_ta = first_ta;

    if(scenario & TAM_SEND_VALID_REQUESTS)
    {
        curr_ta->next_test_action =
                create_test_action(TAM_SEND_VALID_REQUESTS);
        curr_ta = curr_ta->next_test_action;
    }
    if(scenario & TAM_SEND_VALID_REQUESTS_WITH_PAYLOAD)
    {
        curr_ta->next_test_action =
                create_test_action(TAM_SEND_VALID_REQUESTS_WITH_PAYLOAD);
        curr_ta = curr_ta->next_test_action;
    }
    if(scenario & TAM_DISCONNECT_NORMALLY)
    {
        curr_ta->next_test_action =
                create_test_action(TAM_DISCONNECT_NORMALLY);
        curr_ta = curr_ta->next_test_action;
    }

    return first_ta;
}

static void initialization(void)
{
//    void* sock_descr = pthread_getspecific(SOCK_DESCR_KEY);
}

static void send_valid_requests_without_payload(void)
{
    void* sock_descr = pthread_getspecific(SOCK_DESCR_KEY);

    tc_internal_request_t r_1 = r_ping;
    tc_internal_request_t r_2 = r_get_stats;
    tc_internal_request_t r_3 = r_reset_stats;

    r_1.header.uuid = get_uuid();
    r_2.header.uuid = get_uuid();
    r_3.header.uuid = get_uuid();

    send_request(&r_1, *((int*)sock_descr));
    send_request(&r_2, *((int*)sock_descr));
    send_request(&r_3, *((int*)sock_descr));
}

static void send_valid_requests_with_payload(void)
{
    void* sock_descr = pthread_getspecific(SOCK_DESCR_KEY);

    tc_internal_request_t r_1 = r_compress;
    tc_internal_request_t r_2 = r_compress;
    tc_internal_request_t r_3 = r_compress;

    r_1.header.uuid = get_uuid();
    r_2.header.uuid = get_uuid();
    r_3.header.uuid = get_uuid();

    r_1.payload = test_str_1;
    r_2.payload = test_str_2;
    r_3.payload = test_str_3;

    r_1.header.payload_len = strlen(test_str_1);
    r_2.header.payload_len = strlen(test_str_2);
    r_3.header.payload_len = strlen(test_str_3);

    send_request(&r_1, *((int*)sock_descr));
    send_request(&r_2, *((int*)sock_descr));
    send_request(&r_3, *((int*)sock_descr));
}

static void disconnect_normally(void)
{
    void* sock_descr = pthread_getspecific(SOCK_DESCR_KEY);
    shutdown(*((int*)sock_descr), SHUT_RDWR);
}

