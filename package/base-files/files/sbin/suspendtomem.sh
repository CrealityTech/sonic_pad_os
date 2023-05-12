#!/bin/ash

function suspend()
{
	while true; do
		if [ -f '/sys/power/wakeup_count' ] ; then
			cnt=$(cat /sys/power/wakeup_count)
			echo "Read wakeup_count: $cnt"
			echo $cnt > /sys/power/wakeup_count

			if [ $? -eq 0 ] ; then
				echo mem > /sys/power/state
				break
			else
				echo "Warn: write wakeup_count($cnt)"
				sleep 1
				continue
			fi
		else
			echo "Error: File wakeup_count not exist"
			break
		fi
	done
}

echo "try to mem..."
echo "LOG: if block here, please remove usb connect..."
suspend
