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
	return 0
}

hci_start()
{
	h=`ps | grep "$bt_hciattach" | grep -v grep`
	if [ -n "$h" ] ;then
		echo "Bluetooth init has been completed!!"
	else
		start_hci_attach
	fi

	hciconfig hci0 up

	if [ $? == 0 ] ; then
		return 0
	else
		return 1
	fi
}

hci_stop()
{

	h=`ps | grep "$bt_hciattach" | grep -v grep`
	if [ -n "$h" ] ;then
		hciconfig hci0 down
	fi
	return 0
}

start() {

	if [ -d "/sys/class/bluetooth/hci0" ];then
		echo "Bluetooth init has been completed!!"
	else
		start_hci_attach
	fi

    d=`ps | grep bluetoothd | grep -v grep`
	[ -z "$d" ] && {
#		bluetoothd -n  &
		/etc/bluetooth/bluetoothd start
		sleep 1
	}
}

ble_start() {
	if [ -d "/sys/class/bluetooth/hci0" ];then
		echo "Bluetooth init has been completed!!"
	else
		start_hci_attach
	fi

	hci_is_up=`hciconfig hci0 | grep UP RUNNING`
	[ -z "$hci_is_up" ] && {
		hciconfig hci0 up
	}

	MAC_STR=`hciconfig | grep "BD Address" | awk '{print $3}'`
	LE_MAC=${MAC_STR/2/C}
	OLD_LE_MAC_T=`cat /sys/kernel/debug/bluetooth/hci0/random_address`
	OLD_LE_MAC=$(echo $OLD_LE_MAC_T | tr [a-z] [A-Z])
	if [ -n "$LE_MAC" ];then
		if [ "$LE_MAC" != "$OLD_LE_MAC" ];then
			hciconfig hci0 lerandaddr $LE_MAC
		else
			echo "the ble random_address has been set."
		fi
	fi
}

stop() {
	echo "nothing to do."
}

case "$1" in
  start|"")
        start
        ;;
  stop)
        stop
        ;;
  ble_start)
	    ble_start
		;;
  hci_start)
		hci_start
		;;
  hci_stop)
		hci_stop
		;;
  *)
        echo "Usage: $0 {start|stop}"
        exit 1
esac
