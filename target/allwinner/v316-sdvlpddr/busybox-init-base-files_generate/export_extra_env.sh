#!/bin/bash

pwd=$(dirname $0)

if ! grep -q "export TERM" $1/etc/profile; then
    echo -e "\nexport TERM=\"xterm\"" >> $1/etc/profile
fi

if ! grep -q "export LD_HWCAP_MASK" $1/etc/profile; then
    echo -e "\nexport LD_HWCAP_MASK=\"0x00000000\"" >> $1/etc/profile
fi
