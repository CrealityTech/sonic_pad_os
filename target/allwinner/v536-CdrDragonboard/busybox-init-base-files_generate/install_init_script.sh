#!/bin/bash

pwd=$(dirname $0)

S00mpp="S00mpp_imx317"

install -m 777 $pwd/rcK $1/etc/init.d
install -m 777 $pwd/${S00mpp} $1/etc/init.d/S00mpp
install -m 777 $pwd/S11dev $1/etc/init.d
install -m 777 $pwd/S50usb $1/etc/init.d

install -m 777 $pwd/S10udev    $1/etc/init.d

exit 0

