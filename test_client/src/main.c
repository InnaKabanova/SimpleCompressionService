#include "senders_manager.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const char* TC_HELP_STR =
        "1. test_client [serv-ip-addr] [serv-port-num]\n"
        "[serv-ip-addr] -- IP address of the target compression service.\n"
        "Valid values for connecting to a service running on the same "
        "host as the client: '127.0.0.1', '0.0.0.0', 'localhost'.\n"
        "[serv-port-num] -- port number of the target compression service.\n"
        "2. test_client --help (or -h)\n";

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

int main(int argc, char* argv[])
{
    if(argc < 2)
        exit_with_failure("not enough parameters. Hint: use --help");
    else if(argc > 3)
        exit_with_failure("too many parameters. Hint: use --help");

    int help_requested =
            0 == strcmp(argv[1], "-h") || 0 == strcmp(argv[1], "--help");
    if(2 == argc && !help_requested)
        exit_with_failure("not enough parameters. Hint: use --help");
    else if(help_requested)
    {
        printf("%s", TC_HELP_STR);
        exit(EXIT_SUCCESS);
    }

    // Evaluate [serv-ip-addr]:
    const char* node;
    if(0 == strcmp(argv[1], "localhost"))
        node = "0.0.0.0";
    else
        node = argv[1];

    // Evaluate [serv-port-num]:
    if(atoi(argv[2]) <= 0)
        exit_with_failure("invalid service port number provided");

    // Run requests-sending jobs:
    if(0 == run_senders(node, argv[2]))
        exit_with_failure("failure in sender threads' initialization");
}
