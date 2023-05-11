#!/bin/sh

insmod /lib/modules/3.10.65/xradio_core.ko
#sleep 1
insmod /lib/modules/3.10.65/xradio_wlan.ko
#sleep 1

ifconfig wlan0 up
ifconfig wlan0 192.168.100.1
route add default gw 192.168.100.1 wlan0

#/etc/sd-spi-sw/sdspiinit.sh

hostapd -B /etc/hostapd.conf

udhcpd /etc/udhcpd.conf
sleep 1

stupid-ftpd -f /etc/ftpd.conf
