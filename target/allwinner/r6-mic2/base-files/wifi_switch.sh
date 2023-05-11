#!/bin/sh
if [ $1 == "up" ]
then
ifconfig wlan0 up > /dev/null && ifconfig wlan0 192.168.100.1 > /dev/null && route add default gw 192.168.100.1 wlan0 && hostapd /etc/hostapd.conf -B > /dev/null && udhcpd /etc/udhcpd.conf > /dev/null && stupid-ftpd -f /etc/ftpd.conf

elif [ $1 == "down" ]
then
kill `pidof stupid-ftpd`
kill `pidof udhcpd`
kill `pidof hostapd`
ifconfig wlan0 down

elif [ $1 == "get_status" ]
then
ifconfig -a | grep "mon.wlan0" > /dev/null
#echo "$?"
fi
