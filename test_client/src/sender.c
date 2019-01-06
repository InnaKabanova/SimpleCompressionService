#include "sender.h"
#include "utilities.h"

#include <stdlib.h>

#include <stdio.h>
#include <pthread.h>

void parse_req_data(const char* filepath);

void* send_requests(void* args)
{
    struct sender_args* req_data = (struct sender_args*)args;
    printf("From %lu. File: '%s'.\n", pthread_self(), req_data->filepath);
}

void parse_req_data(const char* filepath)
{
    // TODO
}


//if(req_data->sock_descr <= 0 || req_data->filepath == NULL)
//    return NULL; // TODO: return with an error code

//    sock_descr = try_to_connect(node, argv[2]);
//    if(-1 == sock_descr)
//        exit_with_failure("could not connect to a specified service");

//    // TODO: create sender threads detached or joinable?
//    pthread_t test_sender;
//    if(0 != pthread_create(&test_sender, NULL, send_requests, NULL))
//        exit_with_failure("failed to create a sender thread");
//#ifdef DEBUGGING
//    else
//        printf("Created a sender thread with ID %lu.\n", test_sender);
//#endif

