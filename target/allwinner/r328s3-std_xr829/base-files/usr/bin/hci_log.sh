#!/bin/sh

FILE_NEW="/tmp/btsnoop_hci.cfa"
FILE_LAST="/tmp/btsnoop_hci.cfa.last"
FILE_LAST_LAST="/tmp/btsnoop_hci.cfa.last.last"
HCI_DEV=hci0
CHECK_INTERVAL=1m
MAX_FILE_SIZE=$((1024*1024*500))

clean() {
	pid=$(ps | grep hcidump | grep -v grep | awk '{print $1}')
        kill $pid
	exit 0
}

trap clean SIGTERM SIGINT

file_check() {
	if [ -f $FILE_LAST_LAST ]; then
		echo "remove $FILE_LAST_LAST"
		rm -f $FILE_LAST_LAST
	fi

	if [ -f $FILE_LAST ]; then
		echo "move $FILE_LAST to $FILE_LAST_LAST"
		mv $FILE_LAST $FILE_LAST_LAST
	fi

	if [ -f $FILE_NEW ]; then
		echo "move $FILE_NEW to $FILE_LAST"
		mv $FILE_NEW $FILE_LAST
	fi
}

start() {
	file_check

	hcidump -i $HCI_DEV -w $FILE_NEW &

	while [ 1 ]; do
		sleep $CHECK_INTERVAL
		log_size=$(ls -l $FILE_NEW | awk '{ print $5 }')
		if [ $log_size -gt $MAX_FILE_SIZE ]; then
			pid=$(ps | grep hcidump | grep -v grep | awk '{print $1}')
			kill $pid
			file_check
			hcidump -i $HCI_DEV -w $FILE_NEW &
		fi
	done
}

stop() {
        pid=$(ps | grep hcidump | grep -v grep | awk '{print $1}')
        kill $pid
}

case "$1" in
  start|"")
        start
        ;;
  stop)
        stop
        ;;
  *)
        echo "Usage: $0 {start|stop}"
        exit 1
esac

exit $?
