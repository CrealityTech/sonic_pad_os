#! /bin/sh

PHOTO_NUM=5000000
COUNT=1000
CYCLE=20
ERROR=1

function cameratest()
{
        local count=0

        camerademo tinatest 0 /mnt/UDISK reopen &
	sleep 1
        camerademo tinatest 1 /mnt/UDISK reopen &

        local camerapid=`pidof camerademo`

        while [ -n "$camerapid" ]
        do
                let count+=1
                if [ $count -gt $CYCLE ] ; then
                        return 1
                fi
                echo " camerademo not stop, sleep 1S"
                sleep 1
                camerapid=`pidof camerademo`
        done

        return 0
}

i=0

while [ $i -ne $COUNT ]
do
        let i+=1
        echo " ************$i TEST CAMERA************"
        cameratest
        if [ $? -eq $ERROR ]; then
                echo " ************$i TEST ERROR************"
                exit 1
        fi
done

camerademo tinatest 0 /mnt/UDISK ${PHOTO_NUM} &
camerademo tinatest 1 /mnt/UDISK ${PHOTO_NUM} &

capturethread=`pidof camerademo`
while [ -n "$capturethread" ]
do
        sleep 1
        capturethread=`pidof camerademo`
done

