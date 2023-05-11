#!/bin/bash -
#===============================================================================
#
#          FILE: create-new-pro.sh
#
#         USAGE: ./create-new-pro.sh copy_pro new_pro
#
#   DESCRIPTION: create new project for tina
#
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: oujiayu@allwinnertech.com
#  ORGANIZATION: PDC-PSW
#       CREATED: 2020年10月09日 10时44分39秒
#      REVISION: V1.0
#===============================================================================

echo_help()
{
    echo -e "Usage:"
    echo -e "./create-new-pro.sh copied_target new_target"
    echo -e "eg:"
    echo -e "./create-new-pro.sh r329-evb5_v1 r329-evb6\n"
    echo -e "Running this script success will create a new target"
}

judge_args()
{
    if [ ! -n "$1" ]; then
        echo -e "\033[31m !!!Please enter the original programme which you want to copy!!!\033[0m"
        echo
        echo_help
        exit 1
    fi
    if [ ! -n "$2" ]; then
        echo -e "\033[31m !!!Please enter the new programme!!!\033[0m"
        echo
        echo_help
        exit 1
    fi
}

copy_target_dir()
{
    croot
    cd target/allwinner
    if [ ! -d $OLD_TARGET ]; then
        echo -e "\033[031mPlease input the correct copied target name\033[0m"
        echo_help
        exit 1
    fi
    mkdir $NEW_TARGET
    cp -fr $OLD_TARGET/* $NEW_TARGET/
    cd $NEW_TARGET
    sed -i "s/$OLD_TARGET/$NEW_TARGET/g" `find -type f -name "*"`
    sed -i "s/${OLD_TARGET/-/_}/${NEW_TARGET/-/_}/g" `find -type f -name "*"`
    sed -i "s/$OLD_PROG/$NEW_PROG/g" `find -type f -name "*"`
    mv "${OLD_TARGET/-/_}.mk" "${NEW_TARGET/-/_}.mk"
    echo "create dir-target/ file"
}

copy_device_dir()
{
    croot
    cd device/config/chips/
    mkdir -p $NEW_PLATFORM/configs/$NEW_PROG
    cp -fr $OLD_PLATFORM/configs/$OLD_PROG/* $NEW_PLATFORM/configs/$NEW_PROG
    cd $NEW_PLATFORM/configs/$NEW_PROG
    sed -i "s/$OLD_PROG/$NEW_PROG/g" `find -type f -name "*"`
    echo "create dir-device/ file"
}

main()
{
    #获取当前路径，若当前是scripts则跳回上一层路径去source tina环境变量
    fdir=$(awk -F/ '{print $NF}' <<< $PWD)
    sdir=$(awk -F/ '{print $(NF-1)}' <<< $PWD)
    if [ "$fdir" == "scripts" ] && [ "$sdir" != "scripts" ]; then
        cd ..
    fi
    source build/envsetup.sh >/dev/null
    if [ $? != 0 ]; then
        echo -e "\033[031mPlease run in tina root dir\033[0m"
        echo ""
        exit 1
    fi
    #获取新旧方案的芯片型号和方案
    OLD_TARGET=$1
    NEW_TARGET=$2
    OLD_PLATFORM="$(awk -F- '{print $1}' <<< ${OLD_TARGET})"
    OLD_PROG="$(awk -F- '{print $2}' <<< ${OLD_TARGET})"
    NEW_PLATFORM="$(awk -F- '{print $1}' <<< ${NEW_TARGET})"
    NEW_PROG="$(awk -F- '{print $2}' <<< ${NEW_TARGET})"
    echo "Begin to create $NEW_TARGET programme"
    #judge whether input the target name
    judge_args $OLD_TARGET $NEW_TARGET
    copy_target_dir
    copy_device_dir
    echo -e "\033[32mIt's better to run the command 'make menuconfig' after running the script.\033[0m"
}

main $1 $2
