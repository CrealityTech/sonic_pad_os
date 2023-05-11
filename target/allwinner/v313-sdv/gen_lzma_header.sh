#!/bin/bash

add_lzma_header()
{
    lzma_file=$1
    original_file=$2
    file_size=$(printf "%.8x\n" `stat -c%s ${lzma_file}`)
    original_file_size=$(printf "%.8x\n" `stat -c%s ${original_file}`)

    bin_str=""

    file_size_len=${#file_size}

    #"LZMA"+size+origin_size
    bin_str="\x4c\x5a\x4d\x41\x${file_size:6:2}\x${file_size:4:2}\x${file_size:2:2}\x${file_size:0:2}"
    bin_str+="\x${original_file_size:6:2}\x${original_file_size:4:2}\x${original_file_size:2:2}\x${original_file_size:0:2}"


    printf "%b" ${bin_str} > tempbin

    cat ${lzma_file} >> tempbin

    mv tempbin "${lzma_file}.head"
}

lzma -k ./configs/bootlogo.bmp
add_lzma_header "./configs/bootlogo.bmp.lzma" "./configs/bootlogo.bmp"
cp ./configs/bootlogo.bmp.lzma.head ./configs/bootlogo.fex
rm ./configs/bootlogo.bmp.lzma.head
rm ./configs/bootlogo.bmp.lzma

