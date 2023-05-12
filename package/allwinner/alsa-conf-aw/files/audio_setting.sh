#!/bin/sh

#set -x

ASOUND_STATE=/etc/asound.state

volume_array0="0,0"
volume_array1="170,170"
volume_array2="180,180"
volume_array3="190,190"
volume_array4="200,200"
volume_array5="210,210"
volume_array6="220,220"
volume_array7="230,230"
volume_array8="240,240"
volume_array9="250,250"
volume_array10="255,255"

get_volume()
{
    values=$(amixer -D hw:audiocodec cget name='Soft Volume Master' | grep ": values=" | cut -d '=' -f 2)
    echo $values
}

set_volume()
{
#    echo "set_volume $1"
    amixer -D hw:audiocodec cset name='Soft Volume Master' $1 >/dev/null 2>&1
    [ ! -f $ASOUND_STATE ] && touch $ASOUND_STATE
    alsactl store -f $ASOUND_STATE && sync
}


if [ $# -eq 1 ]; then

    param=$1
#    echo "set volume $param"
    [ $param -gt 10 ] && param=10
    [ $param -lt 0 ] && param=0

    for i in $(seq 0 10)
    do
        [ $i -eq $param ] && {
            eval vol=$"volume_array$i"
            set_volume $vol
            break
        }
    done

    exit 0

elif [ $# -eq 0 ]; then

    cur_vol=$(get_volume)
#    echo "current volume $cur_vol"

    for i in $(seq 0 10)
    do
        eval volume=$"volume_array$i"
        [ "x$cur_vol" = "x$volume" ] && break
    done

    echo "level: $i"

    exit 0

else

    echo "unsupport operation!"

    exit 1

fi
