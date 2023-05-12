#!/bin/ash

RET_OK=0
RET_ERR=1

IP_LOG_FILE=/var/log/setIP.log
#用于存放用户配置的静态IP值，确保重启后能继续按照用户之前的配置来设置IP
STATIC_IP_FILE=/etc/wifi/staticIP.conf
#保存最后一次连接的wifi热点名称
SAVE_LAST_WIFI_SSID_FILE=/etc/wifi/saveLastWifiSSID.conf

write_log()
{
    echo `date` ": " $1 >> $IP_LOG_FILE
}

write_staticIP_to_file()
{
    echo "IP=$1" > $STATIC_IP_FILE
    echo "NETMASK=$2" >> $STATIC_IP_FILE
    echo "GATEWAY=$3" >> $STATIC_IP_FILE
    sync
}

if [ $# = 0 ]
then
    echo "################################################"
    echo "USAGE: $0 {static/dhcp/status} {ip} {netmask} {gateway}"
    echo "################################################"
    exit $RET_ERR
fi
if [ $1 = "dhcp" ]
then
    #如果是动态参数，则配置动态IP，并把静态的配置文件删除
    /etc/wifi/udhcpc_wlan0 restart
    write_log "/etc/wifi/udhcpc_wlan0 restart"
    if [ -e $STATIC_IP_FILE ]
    then
        rm -rf $STATIC_IP_FILE
        write_log "rm -rf $STATIC_IP_FILE"
    fi
    if [ -e $SAVE_LAST_WIFI_SSID_FILE ]
    then
        rm -rf $SAVE_LAST_WIFI_SSID_FILE
        write_log "rm -rf $SAVE_LAST_WIFI_SSID_FILE"
    fi
    write_log "set the WIFI IP DHCP success"
    exit $RET_OK
fi

if [ $1 = "status" ]
then
    if [ -e $STATIC_IP_FILE ] && [ -e $SAVE_LAST_WIFI_SSID_FILE ]
    then
        echo "static"
        write_log "check the IP status: static"
    else
        echo "dhcp"
        write_log "check the IP status: dhcp"
    fi
    exit $RET_OK
fi

if [ $1 = "static" ]
then
    if [ $# -le 2 ]
    then
        echo "################################################"
        echo "USAGE: $0 {static/dhcp/status} {ip} {netmask} {gateway}"
        echo "################################################"
        exit $RET_ERR
    fi
    IP=$2
    NETMASK=$3
    GATEWAY=$4
    /sbin/ifconfig wlan0 $2 netmask $3
    if [ $? -ne 0 ]
    then
        write_log "ifconfig wlan0 $IP netmask $NETMASK error"
        exit RET_ERR
    fi
    write_log "IP=$IP netmask=$NETMASK gateway $GATEWAY"
    write_log "ifconfig wlan0 $IP netmask $NETMASK"
    #add by cx2668,20210126
    if [ $# -eq 4 ]
    then
        GATEWAY=$4
        write_log "ip route add default via $GATEWAY dev wlan0"
        ip route add default via $GATEWAY dev wlan0
    fi
    write_staticIP_to_file $IP $NETMASK $GATEWAY
    write_log "save Static IP to file $STATIC_IP_FILE"
    write_log "set the WIFI IP Static success"
    #把当前的SSID保存起来
    CONNECT_AP_SSID=`wifid -t | grep ssid`
    SSID=${CONNECT_AP_SSID#*:}
    echo $SSID > $SAVE_LAST_WIFI_SSID_FILE
    sync

    exit $RET_OK
else
    echo "################################################"
    echo "USAGE: $0 {static/dhcp/status} {ip} {netmask} {gateway}"
    echo "################################################"
    exit $RET_ERR
fi
