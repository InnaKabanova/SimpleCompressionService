#include "utilities.h"
#include "server.h"

#include <string.h>

#include <sys/types.h> // data types used in system calls
#include <sys/socket.h> // structs for sockets
#include <netinet/in.h> // consts & structs for Internet domain addresses
#include <arpa/inet.h>

#define VERBOSE_SERVER

void start_server(int port_num, int backlog)
{
    int socket_descr = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_descr < 0)
        exit_with_failure("could not get a socket");

    sockaddr_in server_addr;
    memset((void*)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET; /* code for the address family;
    Internet domain address is used */
    server_addr.sin_addr.s_addr = INADDR_ANY; // IP address of the server
    server_addr.sin_port = htons(port_num);

    int binding_status = bind(socket_descr, (sockaddr*)&server_addr,
                              sizeof(server_addr));
    if(binding_status < 0)
        exit_with_failure("could not bind the socket to the address");

    if(listen(socket_descr, backlog) < 0)
        exit_with_failure("could not start listening for client connections");

    sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int temp_socket_descr = accept(socket_descr, (sockaddr*)&client_addr,
                                   &client_addr_size);
    if(temp_socket_descr < 0)
        exit_with_failure("could not accept an incoming connection");



}

