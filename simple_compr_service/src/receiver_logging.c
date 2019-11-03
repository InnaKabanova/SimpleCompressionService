#include "receiver_logging.h"

#include <syslog.h>

/*
SCS_TODO:

- one log file for all receiver threads; logs only errors or special
events; requests logging is for the debug purposes only (conditional
compilation)

- allows to stop all the logging on daemon's start (parameter) or
in runtime (signal)

- sets special log files' permissions

- creates a new log file on its start

- manages previous log files (configurable)
*/


void log_event(int connection_id, receiver_event_t event)
{
    if(event == RECV_DONE_EVENT)
    {
        syslog(LOG_ERR, "Connection '%d' is now closed", connection_id);
    }
}
