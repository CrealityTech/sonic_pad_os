#!/bin/sh

wifi_change_mac()
{
	conf=`find -name xr_wifi.conf`
	if [ -z "$conf" ];then
		echo "this test case only for xradio"
		exit 1
	fi
	#make sure wlan0 is up
	wlan0=`ifconfig | grep -rn wlan0`
	if [ -z "$wlan0" ];then
		ifconfig wlan0 up
	fi

	mac_source=`ifconfig | grep -rn HWaddr | awk -F ' ' '{printf $NF}'`
	echo "====== start change mac addr_test ======"
	echo "mac_source: $mac_source"
	vir1=`echo ${mac_source:0:2}`
	vir2=`echo ${mac_source:3:2}`
	vir3=`echo ${mac_source:6:2}`
	vir4=`echo ${mac_source:9:2}`
	vir5=`echo ${mac_source:12:2}`
	vir6=`echo ${mac_source:15:2}`

	echo $vir6:$vir5:$vir4:$vir3:$vir2:$vir1 > $conf
	cat $conf
    	sync
    	reboot
	sleep 3
	wlan0=`ifconfig | grep -rn wlan0`
	if [ -z "$wlan0" ];then
		ifconfig wlan0 up
	fi

	mac_change=`ifconfig | grep -rn HWaddr | awk -F ' ' '{printf $NF}'`
	echo "mac_change: $mac_change"
	if [ -z "$mac_change" ];then
		echo "some error:no wlan0"
		exit 1
	fi
    	if [ "x$mac_source" == "x$mac_change" ];then
    	    echo "====== change mac addr failed!!! ======"
    	    exit 1
    	else
    	    echo "====== change mac addr successed!!!======"
    	    exit 0
	fi
}

wifi_change_mac
