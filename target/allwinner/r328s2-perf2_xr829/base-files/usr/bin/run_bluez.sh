#!/bin/sh

check_modules_version() {
        KERNEL_VERSION=$(uname -r)
        CURRENT_MODULES_VERSION=$(ls /lib/modules)
        if [ ! -d /lib/modules/$KERNEL_VERSION ]; then
                mv /lib/modules/$CURRENT_MODULES_VERSION /lib/modules/$KERNEL_VERSION
                kmodloader
        fi
}

start() {
	check_modules_version

	bluetoothd -n -d &

	#xradio_lpm
#	insmod /lib/modules/4.9.56/xradio_btlpm.ko

	#hciattach
	hciattach -n ttyS1 xradio &

	sleep 6

	bt-a2dpsink-test $1
	#PulseAudio
#	pulseaudio -n --log-level=3 --dl-search-path=/usr/lib/pulse-9.0/modules/ --file=/etc/pulse/default.pa &

	#A64 p1 tinymix
#	tinymix 55 1 && tinymix 59 1 && tinymix 61 1 && tinymix 25 1

	#for compatibility, we configure '--localstatedir=/etc' for tina
	if [ -d "/etc/lib/bluetooth" ] && [ ! -d "/var/lib/bluetooth" ];then
		ln -s /etc/lib/bluetooth /var/lib/bluetooth
	fi
}

stop() {
	hciconfig hci0 down

#	pid=$(ps | grep pulseaudio | grep -v grep | awk '{print $1}')
#	kill $pid
	pid=$(ps | grep bluealsa-aplay | grep -v grep | awk '{print $1}')
	kill $pid
	pid=$(ps | grep arecord | grep -v grep | awk '{print $1}')
	kill $pid
	pid=$(ps | grep arecord | grep -v grep | awk '{print $1}')
	kill $pid
	#FIXME: bluealsa need to be killed twice, maybe thread stuck?
	pid=$(ps | grep bluealsa | grep -v grep | awk '{print $1}')
	kill $pid
	sleep 1
	kill $pid
	pid=$(ps | grep hcidump_xr | grep -v grep | awk '{print $1}')
	kill $pid
	pid=$(ps | grep hciattach | grep -v grep | awk '{print $1}')
	kill $pid
	pid=$(ps | grep bluetoothd | grep -v grep | awk '{print $1}')
	kill $pid
}

case "$1" in
  start|"")
        start $2
        ;;
  stop)
        stop
        ;;
  *)
        echo "Usage: $0 {start|stop}"
        exit 1
esac

exit $?
