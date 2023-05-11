#!/bin/sh
echo "-- run dram_config --"
mode="video_mode"
if [ x"$mode" == x"video_mode" ]; then
	echo 7 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
	echo 7 2 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
	echo 7 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
	echo 7 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

	echo 8 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
	echo 8 1 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
	echo 8 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
	echo 8 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1
elif [ x"$mode" == x"music_mode" ]; then
	echo 7 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
	echo 7 2 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
	echo 7 1024 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
	echo 7 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

	echo 6 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
	echo 6 6 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
	echo 6 1024 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
	echo 6 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1
elif [ x"$mode" == x"picture_mode" ]; then
	echo 0 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
	echo 0 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
	echo 0 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
	echo 0 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

	echo 1 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
	echo 1 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
	echo 1 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
	echo 1 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

	echo 6 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
	echo 6 6 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
	echo 6 1024 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
	echo 6 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

	echo 7 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
	echo 7 2 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
	echo 7 1024 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
	echo 7 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

	echo 8 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
	echo 8 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
	echo 8 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
	echo 8 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1
elif [ x"$mode" == x"encoder_mode" ]; then
	echo 6 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
	echo 6 6 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
	echo 6 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
	echo 6 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

	echo 7 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
	echo 7 2 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
	echo 7 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
	echo 7 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

	echo 11 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
	echo 11 5 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
	echo 11 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
	echo 11 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

	echo 9 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
	echo 9 5 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
	echo 9 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
	echo 9 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

	echo 4 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
	echo 4 3 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
	echo 4 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
	echo 4 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

	echo 8 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
	echo 8 4 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
	echo 8 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
	echo 8 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

	echo 0 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
	echo 0 3 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
	echo 0 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
	echo 0 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

	echo 1 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
	echo 1 3 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
	echo 1 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
	echo 1 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1
else
	echo 6 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
	echo 6 6 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
	echo 6 1024 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
	echo 6 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

	echo 7 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
	echo 7 2 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
	echo 7 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
	echo 7 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

	echo 11 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
	echo 11 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
	echo 11 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
	echo 11 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

	echo 9 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
	echo 9 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
	echo 9 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
	echo 9 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

	echo 4 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
	echo 4 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
	echo 4 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
	echo 4 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

	echo 8 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
	echo 8 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
	echo 8 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
	echo 8 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

	echo 0 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
	echo 0 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
	echo 0 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
	echo 0 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

	echo 1 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
	echo 1 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
	echo 1 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
	echo 1 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1
fi
echo 9 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
echo 9 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
echo 9 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
echo 9 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

echo 11 2 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
echo 11 4 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
echo 11 2 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
echo 11 1 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

echo 6 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
echo 6 6 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
echo 6 1024 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
echo 6 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

echo 7 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
echo 7 4 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
echo 7 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
echo 7 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

echo 4 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
echo 4 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
echo 4 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
echo 4 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

echo 8 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
echo 8 6 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
echo 8 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
echo 8 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

echo 3 3 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
echo 3 4 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
echo 3 560 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
echo 3 480 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

echo 0 1 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
echo 0 4 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
echo 0 16 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
echo 0 8 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

echo 1 1 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
echo 1 4 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
echo 1 16 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
echo 1 8 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1

echo 10 0 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_qos
echo 10 4 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_acs
echo 10 4 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl0
echo 10 2 > /sys/class/hwmon/mbus_pmu/device/hwmon/mbus_pmu/device/port_bwl1
