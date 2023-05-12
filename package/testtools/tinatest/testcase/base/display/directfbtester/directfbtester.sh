#!/bin/ash

df_andi &

sleep 8

killall -q -KILL df_andi

ttrue "Is the penguin on the screen moving normally?"
if [ $? = 0 ]
then
echo "GOOD, lcd test success."
return 0
else
echo "ERROR, lcd test failed."
return 1
fi
