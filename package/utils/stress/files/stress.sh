#!/bin/sh

start_stress()
{
    stress --cpu 1 --vm 1 --vm-bytes 512M >/dev/null &
}

stop_stress()
{
    pids=$(ps | grep -w stress | grep -v grep | awk  '{print $1}')
    [ -n "$pids" ] && {
        for pid in "$pids"
        do
            kill -9 $pid
        done
    }
}

echo "$pids"

case "$1" in
    start)
        start_stress
        ;;
    stop)
        stop_stress
        ;;
    *)
        echo "Usage:"
        echo "    $0 {start|stop}"
        exit 1
esac

exit $?
