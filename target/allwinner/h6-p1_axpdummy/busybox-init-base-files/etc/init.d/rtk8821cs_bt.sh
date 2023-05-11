#!/bin/sh

bt_setup()
{
	echo "bt rtk8821cs init start"
	#example for setting mac address,The actual production needs to be revised.
	#power on
	echo 0 > /sys/class/rfkill/rfkill0/state
	echo 1 > /sys/class/rfkill/rfkill0/state

	echo "step1: bt init"
	rtk_hciattach -n -s 115200 ttyS1 rtk_h5 &

	sleep 3

	echo "step2: hci enable"
	rfkill unblock all
	hciconfig hci0 up
	hciconfig hci0 reset

	echo "step3: bluez init"
	/etc/init.d/dbus start
	bluetoothd -n -d &

	#echo "step4: bt scan"
	#hcitool scan
	#hcitool lescan

	#echo "step5:bt pair"
	## 运行 bluetoothctl, 进入内部comment prompt
	## [bluetooth]# show //查看是否power:yes,如果为no,则运行:power on
	## [bluetooth]# agent NoInputNoOutput //可设置其他io caps
	## [bluetooth]# default-agent
	## [bluetooth]# scan on //扫描到应用的设备,可以使用scan off关闭scan
	## [bluetooth]# pair 00:22:48:DC:89:0F //配对远端设备
	## [bluetooth]# conect 00:22:48:DC:89:0f //连接
}

bt_setup



