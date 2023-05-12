#!/bin/sh /etc/rc.common
# Copyright (C) 2006-2020 OpenWrt.org

export QT_QPA_PLATFORM=eglfs
export QT_QPA_EGLFS_WIDTH=800
export QT_QPA_EGLFS_HEIGHT=800
export QT_QPA_EGLFS_PHYSICAL_WIDTH=0
export QT_QPA_EGLFS_PHYSICAL_HEIGHT=0
export QT_QPA_EGLFS_INTEGRATION=none
export XDG_RUNTIME_DIR=/dev/shm
export QT_QPA_GENERIC_PLUGINS=tslib
export TSLIB_TSDEVICE=/dev/input/event2
