# Quick & dirty script that launches 'simple_compression_service' 
# daemon and greps its syslog. Doesn't cover the case when there 
# already exists another 'simple_compression_service' instance  
# running and listening on [target_port_number].  

if [ "$#" -ne 1 ]; then
    echo "'$0 [target_port_number]'"
    exit
fi

PORT_NUMBER=$1
SCS_DIR="simple_compr_service/build/"
SCS_EXEC_NAME="simple_compr_service"
START_COMMAND="./$SCS_EXEC_NAME $PORT_NUMBER"
SYSLOG_PATH="/var/log/syslog"

if [ -f "$SCS_DIR$SCS_EXEC_NAME" ]; then
    echo "$SCS_DIR$SCS_EXEC_NAME exists."
    echo "Trying to launch the service listening on port $1."
else 
    echo "Binary $SCS_DIR$SCS_EXEC_NAME does not exist."
    exit
fi

cd $SCS_DIR
$START_COMMAND
#$./$SCS_EXEC_NAME $PORT_NUMBER
PID=`/bin/ps -eo pid,cmd | grep "$START_COMMAND" | grep -v "grep" | awk '{print $1}'`
echo "Service's PID is $PID"
echo "______________________________________________________________________________"

cat $SYSLOG_PATH | grep "$PID"







