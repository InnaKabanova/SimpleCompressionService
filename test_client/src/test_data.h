#ifndef TEST_CLIENT_TEST_DATA
#define TEST_CLIENT_TEST_DATA

#include "internal_request.h"

#include <stdio.h>

static const tc_internal_request_t r_ping = { .header.magic_value = REQUEST_MAGIC_VALUE,
                                              .header.uuid = 0,
                                              .header.code = REQ_PING,
                                              .header.payload_len = 0,
                                              .payload = NULL,
                                              .next_request = NULL};

static const tc_internal_request_t r_get_stats = { .header.magic_value = REQUEST_MAGIC_VALUE,
                                                   .header.uuid = 0,
                                                   .header.code = REQ_GET_STATS,
                                                   .header.payload_len = 0,
                                                   .payload = NULL,
                                                   .next_request = NULL};

static const tc_internal_request_t r_reset_stats = { .header.magic_value = REQUEST_MAGIC_VALUE,
                                                     .header.uuid = 0,
                                                     .header.code = REQ_RESET_STATS,
                                                     .header.payload_len = 0,
                                                     .payload = NULL,
                                                     .next_request = NULL};

static const tc_internal_request_t r_compress = { .header.magic_value = REQUEST_MAGIC_VALUE,
                                                  .header.uuid = 0,
                                                  .header.code = REQ_COMPRESS,
                                                  .header.payload_len = 0,
                                                  .payload = NULL,
                                                  .next_request = NULL};

static const char* test_str_1 = "aaabbbggaaaaagbnhr";
static const char* test_str_2 = "aaabbb";
static const char* test_str_3 = "aaabbbccc";

#endif /* TEST_CLIENT_TEST_DATA */
