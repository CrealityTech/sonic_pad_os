#!/bin/bash

# 先进入 part0 文件夹，运行命令:
# ../../boot-play_sort_rename.sh
# 对所有jpg文件进行排序重命名
# part1文件夹单张jpg可手动修改

count=1

for pic in `ls -v *.jpg`
do
    #echo "[$count]: ${pic}"
    newname=`printf "screen%03d.jpg\n" ${count}`
    mv -v ${pic} ${newname}
    let count+=1
done
