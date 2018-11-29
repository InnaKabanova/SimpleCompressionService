#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>

#include "utilities.h"

void exit_with_failure(const char* msg)
{
    printf("ERROR: %s. Exiting.\n", msg);
    exit(EXIT_FAILURE);
}

int print_addrinfo(struct addrinfo* info, const char* tag,
                   int print_error)
{
    if(info == NULL || info->ai_family == AF_UNSPEC)
    {
        if(print_error)
            printf("ERROR: print_addrinfo: invalid input data.\n");
        return 0;
    }

    int family = info->ai_family;
    char dest[family == AF_INET ? INET_ADDRSTRLEN : INET6_ADDRSTRLEN];
    const void* src;

    if(AF_INET == family)
        src = (const void*)
              (&(((struct sockaddr_in*)info->ai_addr)->sin_addr));
    else if(AF_INET6 == family)
        src = (const void*)
              (&(((struct sockaddr_in6*)info->ai_addr)->sin6_addr));

    const char* dest_ptr = inet_ntop(family, src, (const void*)&dest,
                                          sizeof(dest));
    if(dest_ptr == NULL)
    {
        if(print_error)
            printf("ERROR: print_addrinfo: could not convert the "
                   "provided address from binary to text.\n");
        return 0;
    }

    printf("%s: ai_family: %i | ai_socktype: %i | "
           "ai_protocol: %i | ai_addr: %s | ai_addrlen: %i\n",
           tag, info->ai_family, info->ai_socktype,
           info->ai_protocol, dest_ptr, info->ai_addrlen);
    return 1;
}
