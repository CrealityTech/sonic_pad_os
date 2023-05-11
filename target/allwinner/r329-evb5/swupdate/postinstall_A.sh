#!/bin/sh

[ -e /dev/by-name/bootA_fifo ] && dd if=/dev/zero of=/dev/by-name/bootA_fifo
[ -e /dev/by-name/rootfsA_fifo ] && dd if=/dev/zero of=/dev/by-name/rootfsA_fifo
[ -e /dev/by-name/dsp0A_fifo ] && dd if=/dev/zero of=/dev/by-name/dsp0A_fifo
[ -e /dev/by-name/dsp1A_fifo ] && dd if=/dev/zero of=/dev/by-name/dsp1A_fifo

sleep 1

[ -e /dev/by-name/bootA_fifo ] && return 1
[ -e /dev/by-name/rootfsA_fifo ] && return 1
[ -e /dev/by-name/dsp0A_fifo ] && return 1
[ -e /dev/by-name/dsp1A_fifo ] && return 1

return 0
