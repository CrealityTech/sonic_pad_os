#!/bin/ash

modify_rotate(){
	`uci set /etc/config/browser.core.egl_rotate=$1`
	`uci set /etc/config/browser.core.tp_rotate=$2`
}

if [ $# = 0 ]
then
	echo "Usage: rotate.sh [0/90/180/270]"
else
	case $1 in
		90)
		modify_rotate 90 90
		;;
		180)
		modify_rotate 180 180
		;;
		270)
		modify_rotate -90 270
		;;
		*)
		modify_rotate 0 0
		;;
	esac
fi
