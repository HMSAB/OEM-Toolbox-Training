#!/bin/sh

log() {
  logger -t "${SCRIPT_HOME}" "${1}"
  echo "$1"
}

err() {
  msg="$(log "${1}")"
  >&2 echo "${msg}"
}

# start the sw_updater.  This process should only run once during startup and then exit

case "$1" in
start)
    echo -n "Starting $DESC: "
    
    /home/apps/<application name> 
    
    exit
    ;;
stop)
    echo -n "Stopping $DESC: "
    exit
    ;;
restart)
    echo -n "Restarting $DESC: "
    exit
    ;;
*)
    echo "Usage: $0 {start|stop|restart|try-restart|force-reload|status}" >&2
    exit 3
    ;;
esac
