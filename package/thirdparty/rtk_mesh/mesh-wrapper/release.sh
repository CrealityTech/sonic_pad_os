#
# Copyright (C) 2019 Realtek Semiconductor Corporation.
# Author: Alex Lu
# mail: alex_lu@realsil.com.cn
#
#!/bin/bash

# app
# bluez
# gap
# hci_mgmt
# lib
# Makefile
# platform
# trace
CURRDIR=$PWD
CURR_DATEX=`date +%Y%m%d-%H%M%S`
DST=$1/linux-mesh-${CURR_DATEX}
TARDIR=`dirname $DST`
BASEN=`basename $DST`
if [ -e $DST ]; then
	rm -rf $DST
fi
mkdir $DST/
cp -f Makefile $DST/
cp -f makefile.defs $DST/

# Copy provisioner_app
mkdir -p $DST/provisioner_app/
cp -f provisioner_app/app.c $DST/provisioner_app/
cp -f provisioner_app/ftl.c $DST/provisioner_app/
cp -f provisioner_app/ftl.h $DST/provisioner_app/
cp -f provisioner_app/Makefile $DST/provisioner_app/
cp -f provisioner_app/mesh_provisioner.c $DST/provisioner_app/
cp -f provisioner_app/mesh_provisioner.h $DST/provisioner_app/
cp -f provisioner_app/provisioner_cmd.c $DST/provisioner_app/
cp -f provisioner_app/provisioner_cmd.h $DST/provisioner_app/

# Copy device_app
mkdir -p $DST/device_app/
cp -f device_app/app.c $DST/device_app/
cp -f device_app/device_cmd.c $DST/device_app/
cp -f device_app/device_cmd.h $DST/device_app/
cp -f device_app/ftl.c $DST/device_app/
cp -f device_app/ftl.h $DST/device_app/
cp -f device_app/Makefile $DST/device_app/
cp -f device_app/mesh_device.c $DST/device_app/
cp -f device_app/mesh_device.h $DST/device_app/

# Copy bluez
cp -rf bluez $DST/

# Copy gap
mkdir -p $DST/gap
mkdir -p $DST/gap/inc
mkdir -p $DST/gap/src
cp -f gap/inc/*.h $DST/gap/inc/
cp -f gap/src/*.c $DST/gap/src/
cp -f gap/src/Makefile $DST/gap/src/

# Copy hci_mgmt
mkdir -p $DST/hci_mgmt
cp -f hci_mgmt/*.c $DST/hci_mgmt/
cp -f hci_mgmt/*.h $DST/hci_mgmt/
cp -f hci_mgmt/Makefile $DST/hci_mgmt/

# Copy lib
# cmd/
# common/
# gap/
# inc/
# libmesh_device.a
# libmesh_provisioner.a
# model/
# model/realtek/
# platform/
# profile/
# utility/
lib_dirlist="
cmd
common
gap
inc
model
model/realtek
platform
profile
utility
"
mkdir -p $DST/lib/
for i in $lib_dirlist;do
	mkdir -p $DST/lib/$i
	cp lib/$i/*.h $DST/lib/$i/
done
cp -f lib/libmesh_device.a $DST/lib/
cp -f lib/libmesh_provisioner.a $DST/lib/

# Copy platform
mkdir -p $DST/platform
mkdir -p $DST/platform/inc
mkdir -p $DST/platform/src
cp -f platform/inc/*.h $DST/platform/inc/
cp -f platform/src/*.c $DST/platform/src/
cp -f platform/src/Makefile $DST/platform/src/

# Copy trace
mkdir -p $DST/trace/
cp -f trace/*.[ch] $DST/trace/
cp -f trace/Makefile $DST/trace/

cd $TARDIR
tar zcvf ${BASEN}.tgz ${BASEN}/
cd $CURRDIR
