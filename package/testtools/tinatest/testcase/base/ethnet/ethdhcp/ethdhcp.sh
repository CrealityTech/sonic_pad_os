#!/bin/sh

ethdhcp()
{
    #judge eth0 up or down
    echo "====== start eth net dhcp_test ======"
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
    #udhcpc get ip
    udhcpc -i eth0
    if [ $? -eq 0 ];then
	ip=ifconfig eth0 | grep inet | awk '{print $2}' | awk -F ':' '{print $2}'
	if [ ! -n "$ip" ];then
		echo "====== eth0 get ip failed!!! ======"
		exit 1
	else
		echo "====== eth0 get ip $ip ======"
		echo "====== eth0 get ip successed!!! ======"
		exit 0
	fi
    fi
}

ethdhcp
