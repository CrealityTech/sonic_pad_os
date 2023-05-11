#!/bin/bash

pwd=$(dirname $0)

cp -vrf $pwd/TZ $1/etc

if [ ! -d "$1/etc/sysconfig" ];then
    mkdir -p $1/etc/sysconfig
fi

cp -vrf $pwd/clock $1/etc/sysconfig/clock

if [ ! -d "$1/usr/share" ];then
    mkdir -p $1/usr/share
fi

cp -vrf $pwd/zoneinfo $1/usr/share/

if ! grep -q "export TZ" $1/etc/profile; then
    echo -e "\nexport TZ=\"Asia/Shanghai\"" >> $1/etc/profile
fi

exit 0
