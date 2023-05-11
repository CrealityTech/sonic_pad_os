#!/bin/bash

pwd=$(dirname $0)

S00mpp="S00mpp_imx386"

install -m 777 $pwd/rcK $1/etc/init.d
install -m 777 $pwd/S00appdriver $1/etc/init.d
install -m 777 $pwd/${S00mpp} $1/etc/init.d/S00mpp
install -m 777 $pwd/S41netparam $1/etc/init.d
install -m 777 $pwd/S11dev $1/etc/init.d
install -m 777 $pwd/S00kfc $1/etc/init.d
install -m 777 $pwd/S50usb $1/etc/init.d
install -m 777 $pwd/S00part $1/etc/init.d

install -m 777 $pwd/S01logging $1/etc/init.d
install -m 777 $pwd/S10udev    $1/etc/init.d
install -m 777 $pwd/S20urandom $1/etc/init.d
install -m 777 $pwd/S40network $1/etc/init.d
install -m 777 $pwd/S50telnet  $1/etc/init.d

exit 0

