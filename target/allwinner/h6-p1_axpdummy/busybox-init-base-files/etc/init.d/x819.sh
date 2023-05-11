#!/bin/sh

wifi_connect()
{
	#cp firmware
	echo "wifi x819 init start"
	mkdir -p /vendor/etc/firmware
	cp /lib/firmware/* /vendor/etc/firmware/
	#example for setting mac address,The actual production needs to be revised.
	mkdir -p /data/
	cp /etc/xr_wifi.conf /data/
	echo "insmod wifi kernel module"
	insmod /lib/modules/4.9.118/xradio_mac.ko
	insmod /lib/modules/4.9.118/xradio_core.ko
	insmod /lib/modules/4.9.118/xradio_wlan.ko
	sleep 1
	echo "wifi x819 init end"

}

wifi_connect



