#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h> // data types used in system calls
#include <sys/socket.h> // structs for sockets
#include <netinet/in.h> // consts & structs for Internet domain addresses
#include <netdb.h>

#include "utilities.h"

#define VERBOSE_TEST_CLIENT

void connect_to_service(int* socket_descr, const int* serv_port_num,
                        const struct hostent* const serv_addr_info);

int main(int argc, char* argv[])
{
    // The data that must be provided via input parameters:
    int serv_port_num;
    struct hostent* serv_addr_info; // TODO: who is responsible for freeing this?

    //----------------------------------------------------------------

    // Parse the command line arguments:

    // 1. If help was requested, give it away:
    if(0 == strcmp(argv[1], "-h") || 0 == strcmp(argv[1], "--help"))
    {
        printf("1. test_client [serv-ip-addr] [serv-port-num]\n");
        printf("[serv-ip-addr] -- IP address of the target compression service.\n");
        printf("[serv-port-num] -- port number of the target compression service.\n");
        printf("2. test_client --help (or -h)\n");
        exit(EXIT_SUCCESS);
    }
    // In all other cases, less than 3 args (incl. the name of the program)
    // are considered to be invalid program input:
    else if(argc < 3)
        exit_with_failure("not enough parameters. Hint: use --help");

    // 2. Test the input parameters:
    serv_port_num = atoi(argv[2]);
    if(0 == serv_port_num) // conversion failed
        exit_with_failure("invalid service port number provided");
    serv_addr_info = gethostbyname(argv[1]);
    if(NULL == serv_addr_info)
        exit_with_failure("could not find the target service's host");

    //----------------------------------------------------------------

    // Try to connect:
    int socket_descr;
    connect_to_service(&socket_descr, &serv_port_num, serv_addr_info);





    return EXIT_SUCCESS;
}

void connect_to_service(int* socket_descr, const int* serv_port_num,
                        const struct hostent* const serv_addr_info)
{
    *socket_descr = socket(AF_INET, SOCK_STREAM, 0);
    if(*socket_descr < 0)
        exit_with_failure("could not create a socket");

    struct sockaddr_in serv_addr;
    memset((void*)&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(*serv_port_num);

    void *memcpy(void *dest, const void *src, size_t n);

    memcpy((void*)&serv_addr.sin_addr.s_addr, (void*)(&serv_addr_info->h_addr_list[0]),
            serv_addr_info->h_length);

   if(connect(*socket_descr, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        exit_with_failure("could not connect to a specified server");
   else
        printf("SUCCESS");
}

