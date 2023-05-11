#!/bin/sh
bt_hciattach="rtk_hciattach"

start_hci_attach()
{
	h=`ps | grep "$bt_hciattach" | grep -v grep`
	[ -n "$h" ] && {
		killall "$bt_hciattach"
		sleep 1
	}

	#8723ds h5 init
	echo 0 > /sys/class/rfkill/rfkill0/state;
	sleep 1
	echo 1 > /sys/class/rfkill/rfkill0/state;
	sleep 1

	"$bt_hciattach" -n -s 115200 /dev/ttyS1 rtk_h5 >/dev/null 2>&1 &
	sleep 1

	wait_hci0_count=0
	while true
	do
		[ -d /sys/class/bluetooth/hci0 ] && break
		sleep 1
		let wait_hci0_count++
		[ $wait_hci0_count -eq 8 ] && {
			echo "bring up hci0 failed"
			exit 1
		}
	done
}

start() {

	if [ -d "/sys/class/bluetooth/hci0" ];then
		echo "Bluetooth init has been completed!!"
	else
		start_hci_attach
	fi
#	hci0_up_str=`hciconfig -a | grep "UP RUNNING"`
#	if [ -n "$hci0_up_str" ];then
#		echo "hci0 already up."
#	else
#		hciconfig hci0 up
#		sleep 1
#	fi

    d=`ps | grep bluetoothd | grep -v grep`
#    if [ -n "$d" ];then
#		killall bluetoothd
#		sleep 1
#		bluetoothd -n &
#	else
#		bluetoothd -n &
#	fi
	[ -z "$d" ] && {
		bluetoothd -n  &
		sleep 1
	}
#   DEBUG
	hcidump -i hci0 -w /tmp/btsnoop.cfa &
#	hciconfig hci0 piscan

}

stop() {

#    d=`ps | grep bluetoothd | grep -v grep`
#	[ -n "$d" ] && killall bluetoothd

#  DEBUG
	killall hcidump

#	hciconfig hci0 down

#	h=`ps | grep "$bt_hciattach" | grep -v grep`
#	[ -n "$h" ] && {
#		killall "$bt_hciattach"
#	}
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
