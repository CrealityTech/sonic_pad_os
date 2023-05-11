#!/bin/sh
bt_hciattach="rtk_hciattach"

start() {

    d=`ps | grep bluetoothd | grep -v grep`
    [ -z "$d" ] && {
		bluetoothd -n &
#        /etc/init.d/bluetoothd start
		sleep 1
    }

	h=`ps | grep "$bt_hciattach" | grep -v grep`
	[ -n "$h" ] && {
		killall "$bt_hciattach"
		sleep 1
	}

	#8723ds h5 init
	echo 0 > /sys/class/rfkill/rfkill0/state;
	echo 1 > /sys/class/rfkill/rfkill0/state;
	"$bt_hciattach" -n -s 115200 ttyS1 rtk_h4 >/dev/null 2>&1 &


	# xr829 hciattach init
#	"$bt_hciattach" -n ttyS1 xradio >/dev/null 2>&1 &

	#for compatibility, we configure '--localstatedir=/etc' for tina
	if [ -d "/etc/lib/bluetooth" ] && [ ! -d "/var/lib/bluetooth" ];then
		ln -s /etc/lib/bluetooth /var/lib/bluetooth
	fi

}

stop() {
	hciconfig hci0 down
	pid=$(ps | grep "$bt_hciattach" | grep -v grep | awk '{print $1}')
	kill $pid
	pid=$(ps | grep bluetoothd | grep -v grep | awk '{print $1}')
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
