#include "processor.h"

#include <unistd.h>
#include <stdio.h>

void* process_requests(void* args)
{
    // Dummy work:
    FILE* fp = (FILE*)args;
    while(1)
    {
        sleep(1);
        fputs("PROCESS ", fp);
    }
}
