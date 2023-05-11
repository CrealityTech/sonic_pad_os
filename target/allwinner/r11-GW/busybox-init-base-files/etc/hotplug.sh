#!/bin/sh

case $MDEV in
    sda[0-9])
        MOUNTPOINT=/mnt
        ;;
    mmcblk[0-9]p[0-9])
        MOUNTPOINT=/mnt/SDCARD
        ;;
    mmcblk[0-9])
        MOUNTPOINT=/mnt/SDCARD
        ;;
    *)
        exit 0
        ;;
esac

case $ACTION in
remove)
    /bin/umount $MOUNTPOINT || true
    ;;
add)
    /bin/mount -o rw,noatime,nodiratime,nosuid,nodev -t vfat /dev/$MDEV $MOUNTPOINT > /dev/null 2>&1 || true
    ;;
esac

exit 0
