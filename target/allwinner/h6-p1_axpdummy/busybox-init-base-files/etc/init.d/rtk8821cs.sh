#!/bin/sh

wifi_connect()
{
	#cp firmware
	echo "wifi rtk8821cs init start"
	#example for setting mac address,The actual production needs to be revised.
	insmod /lib/modules/4.9.118/8821cs.ko
	sleep 1
	echo "wifi rtk8821cs init end"
}

wifi_connect



