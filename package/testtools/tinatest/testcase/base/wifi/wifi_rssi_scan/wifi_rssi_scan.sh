#!/bin/sh

testcase_path="/base/wifi/wifi_rssi_scan"
SCAN_TIME=`mjson_fetch ${testcase_path}/scan_time`
echo "scan_time=${SCAN_TIME}"
BASIC_RSSI=`mjson_fetch ${testcase_path}/basic_rssi`
echo "basic_rssi=${BASIC_RSSI}"

num=0

wifi_scan()
{
    wlan=`ifconfig | grep wlan0`
    if [ -z "$wlan" ]; then
	    echo "please up wlan first!"
	    return 1
    fi
    wifi_scan_results_test > /tmp/wifi_scan.txt
    sed -i '1,5d' /tmp/wifi_scan.txt
    sed -i '$d' /tmp/wifi_scan.txt
    sed -i '$d' /tmp/wifi_scan.txt
    sed -i '$d' /tmp/wifi_scan.txt

    #====== Analysis wifi_scan_results ======
    scan_rssi=`cat /tmp/wifi_scan.txt | awk '{print $3}'`
    line=0
    for i in $scan_rssi
    do
	let line+=1
	if [ $i -ge $BASIC_RSSI ];then
		cat /tmp/wifi_scan.txt | awk NR==$line
		let num+=1
	fi
    done
}

#===============================Main Function=======================================

for j in $(seq 1 $SCAN_TIME)
do
	wifi_scan
	if [ $num -gt 0 ] ; then
		echo "=================in $j test, have find $num wifi that rssi > $BASIC_RSSI successful!================"
		return 0
	else
		echo "=================in $j test, can not find wifi that rssi > $BASIC_RSSI failed!================"
		if [ $j -ge $SCAN_TIME ]; then
			return 1
		fi
	fi
done

