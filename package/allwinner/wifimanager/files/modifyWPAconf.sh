#!/bin/ash

RET_OK=0
RET_ERR=1

WPA_SUPPLICANT_CONF=/etc/wifi/wpa_supplicant.conf
LOG_FILE=/var/log/modifyWPAconf.log
SSID=
PWD=
DEL_LINE_START=
DEL_LINE_END=
FIND_SSID_LINE=
FIND_PWD_LINE=

help()
{
    echo "####################################################"
    echo "modifyWPAconf.sh {del} {SSID} {Passwd}"
    echo "####################################################"
}

write_log()
{
    echo `date` ": " $1 >> $LOG_FILE
}

#如果参数不等于3，返回报错
if [ $# -ne 3 ]
then
    echo "Parameter error"
    help
    exit $RET_ERR
fi

if [ $1 == "del" ]
then
    write_log "del /etc/wifi/wpa_supplicant.conf $2/$3(ssid/pwd) ..."
    SSID=$2
    PWD=$3
    #匹配最后一个为$SSID的行号
    FIND_SSID_LINE=`sed -n "/$SSID/=" $WPA_SUPPLICANT_CONF | tail -n 1`

    while [ $FIND_SSID_LINE ]
    do 
        write_log "find $SSID in line $FIND_SSID_LINE"

        #20210413 DTS单号为bug#1054
        #因wpa_supplicant会保存wpa_supplicant.conf的缓存，所以单纯通过sed删除conf文件，并不能清理缓存，增加系统指令来清理缓存
        /bin/wifi_remove_network_test $SSID 2
        if [ $? -ne 0 ]
        then
            /bin/wifi_remove_network_test $SSID 3
            if [ $? -ne 0 ]
            then
                /bin/wifi_remove_network_test $SSID 0
                if [ $? -ne 0 ]
                then
                    write_log "remove $SSID failed."
                    break
                fi
            else
                write_log "remove $SSID 3 OK"
            fi
        else
            write_log "remove $SSID 2 OK"
        fi
        FIND_SSID_LINE=`sed -n "/$SSID/=" $WPA_SUPPLICANT_CONF | tail -n 1`
        sleep 1
    done
    write_log "del /etc/wifi/wpa_supplicant.conf $2/$3(ssid/pwd) success"
    sync
    exit $RET_OK
fi
