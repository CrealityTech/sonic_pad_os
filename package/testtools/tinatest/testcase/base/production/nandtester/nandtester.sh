#!/bin/sh

do_nftl_nand()
{
    test_dev="/dev/nanda"
    grep "gpt=1" /proc/cmdline &>/dev/null && test_dev="/dev/nand0p1"
    [ ! -b ${test_dev} ]  \
        && echo "not found ${test_dev}" \
        && exit 1

    echo "nand test ioctl start"
    nandrw "${test_dev}"
    if [ $? -ne 0 ];then
        echo "nand ioctl failed"
        exit 1
    else
        echo "nand ioctl test ok"
        exit 0
    fi
}

do_ubi_nand()
{
    if [ ! -e "/dev/ubi0" ]; then
        echo "not found /dev/ubi0"
        exit 1
    fi

    for part in $(ls /dev/ubi0_*)
    do
        if $(ubinfo ${part} | grep -q UDISK); then
            echo "ubinfo nand test ok"
            exit 0
        fi
    done
    echo "ubinfo nand test failed"
    exit 1
}

#platform info
boot_type=$(get_boot_media)

case "${boot_type}" in
    nand)
        echo "boot from ${boot_type}"
        echo "checking is no need"
        exit 0; #boot from nand, just exit 0
        ;;
    *)
        echo "boot from ${boot_type}";
        ;;
esac

[ -b "/dev/nanda" -o -b "/dev/nand0p1" ] && do_nftl_nand
[ -c "/dev/ubi0" ] && do_ubi_nand
echo "no any nand device"
exit 1
