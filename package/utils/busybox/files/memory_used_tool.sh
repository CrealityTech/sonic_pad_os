#!/bin/sh

#set -x

# Requirement:
# 1. kernel must enable /proc page monitoring
# 2. sort must support -k option
# 3. get cmd from ps, need to confirm cmd info at which column
#  in TinaLinux, default is 5.

LIST_CNT=10
CMD_IDX=5

if [ $# -gt 0 ]; then
    if [ $1 -gt 0 ]; then
        #echo "Is valid num"
        LIST_CNT=$1
    else
        #echo "Not num"
        exit 1
    fi
fi

TMP_FILE=`mktemp /tmp/mem.XXXXXX`

find /proc/ -maxdepth 1 -name "[1-9]*" -exec ls {}/smaps_rollup \; 2>/dev/null | xargs grep -w Rss 2>/dev/null | sort -n -k2 -r | head -n ${LIST_CNT} > ${TMP_FILE}

sed -i -e 's/\/proc\///' ${TMP_FILE}
sed -i -e 's/\/smaps_rollup:Rss://' ${TMP_FILE}

#cat ${TMP_FILE}

echo -e "PID\tRSS\tCMD"

while read line
do
    pid=`echo $line | awk '{print $1}'`

    # filter pid = 1, cause 'ps -w | grep -w 1' will get kernel thread info
    if [ "x$pid" = "x1" ]; then
        continue
    fi

    mem=`echo $line | awk '{$1=""; print $0}'`
    cmd=`ps -w | grep -w $pid | grep -v grep  | awk '{for(i=1;i<"'$CMD_IDX'";i++) $i=""; print $0}'`
    echo -e "$pid\t$mem\t$cmd"
done < ${TMP_FILE}

rm ${TMP_FILE}
