#include <stdio.h>
#include <stdlib.h>

#include "utilities.h"

void exit_with_failure(const char* msg)
{
    printf("ERROR: %s. Exiting.\n", msg);
    exit(EXIT_FAILURE);
}
