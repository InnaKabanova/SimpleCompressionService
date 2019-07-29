#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "utilities.h"

void exit_with_failure(const char* msg)
{
    const char* postfix = "";
    const char* output = "";
    if(NULL != msg && 0 != strcmp(msg, output))
    {
        postfix = ": ";
        output = msg;
    }

    printf("ERROR%s%s. Exiting.\n", postfix, output);
    exit(EXIT_FAILURE);
}

int print_addrinfo(const struct addrinfo* info, const char* tag,
                   const int print_error)
{
    int family;
    if(NULL == info || AF_UNSPEC == (family = info->ai_family))
    {
        if(print_error)
            printf("ERROR: print_addrinfo: invalid arguments.\n");
        return 0;
    }

    // Obtain the to-be-converted address (in binary network format):
    const void* src;
    if(AF_INET == family)
    {
        src = (const void*)
                (&(((struct sockaddr_in*)info->ai_addr)->sin_addr));
    }
    else if(AF_INET6 == family)
    {
        src = (const void*)
                (&(((struct sockaddr_in6*)info->ai_addr)->sin6_addr));
    }

    // Buffer for presentation form of (&src):
    char buff[family == AF_INET ? INET_ADDRSTRLEN : INET6_ADDRSTRLEN];

    const char* dest = inet_ntop(family, src, (void*)&buff, sizeof(buff));
    if(NULL == dest)
    {
        if(print_error)
            printf("ERROR: print_addrinfo: could not convert the "
                   "address provided from binary to text.\n");
        return 0;
    }
    else
    {
        const char* prefix = "";
        if(NULL != tag)
            prefix = ": ";

        printf("%s%s"
               "ai_family: %i | ai_socktype: %i | "
               "ai_protocol: %i | ai_addr: %s | ai_addrlen: %i\n",
               (NULL == tag ? "" : tag), prefix,
               info->ai_family, info->ai_socktype,
               info->ai_protocol, dest, info->ai_addrlen);
        return 1;
    }
}

int send_all(const int sock_descr, const char* buff, int* buff_size)
{
    if(NULL == buff || NULL == buff_size)
        return 0;

    int bytes_sent = 0;
    int bytes_left = *buff_size;
    int n;

    while(bytes_sent < *buff_size)
    {
        n = send(sock_descr, buff + bytes_sent, bytes_left, 0);
        if(-1 == n) break;

        bytes_sent += n;
        bytes_left -= n;
    }

    *buff_size = bytes_sent;
    return -1 != n;
}
