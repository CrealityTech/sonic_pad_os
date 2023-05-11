#!/bin/bash

pwd=$(dirname $0)

mkdir -vp $1/etc/cron.minutely
mkdir -vp $1/etc/cron.hourly
mkdir -vp $1/etc/cron.daily
mkdir -vp $1/etc/cron.weekly
mkdir -vp $1/etc/cron.monthly
mkdir -vp $1/etc/cron.d
#mkdir -vp $1/var/spool/cron/crontabs

install -m 777 $pwd/cron/run-cron $1/usr/bin
install -m 777 $pwd/cron/crontabs $1/etc/cron.d
install -m 777 $pwd/cron/ntpupdate $1/etc/cron.d
install -m 777 $pwd/cron/reboot $1/etc/cron.d

exit 0
