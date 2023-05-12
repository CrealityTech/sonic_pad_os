#!/bin/sh

eth_ping()
{
    #judge eth0 up or down
    echo "====== start ethping test ======"
    lan=`ifconfig | grep eth0 | awk '{print $1}'`
    if [ x"eth0" != x"$lan" ];then
	echo "eth0 not up,try up"
	ifconfig eth0 up
	if [ $? -eq 0 ];then
		echo "eth0 up successed!!!"
	else
		echo "eth0 up failed!!!"
		exit 1
	fi
    fi

    state=`cat /sys/class/net/eth0/brport/state`
    echo "=====eth0 state = $state====="
    if [ $state == 0 ];then
	    exit 1
    else
	    echo "=====eth0 up state=$state====="
	    ifconfig eth0 192.168.1.200 netmask 255.255.255.0
	    ping -w 5 192.168.1.200
            if [ $? -eq 0 ];then
		ifconfig eth0 down
		echo "====== eth ping_test successed!!! ======"
		exit 0
	    else
		echo "====== eth ping_test failed!!! ======"
		exit 1
	    fi
    fi
}

eth_ping
