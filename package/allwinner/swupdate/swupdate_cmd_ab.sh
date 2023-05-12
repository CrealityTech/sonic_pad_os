#!/bin/sh

SWU_TOOL=/sbin/swupdate_cmd.sh

[ $# -ne 0 ] && {
        echo "swupdate start..."

        swu_input=$*
        echo "swu input : ##$swu_input##"

        swu_path=$1
        echo "swu path : ##$swu_path##"

        swu_current_system=$(fw_printenv -n boot_partition 2>/dev/null)

        if [ "x$swu_current_system" = "xbootA" ]; then
                $SWU_TOOL -i $swu_path -e stable,now_A_next_B
        elif [ "x$swu_current_system" = "xbootB" ]; then
                $SWU_TOOL -i $swu_path -e stable,now_B_next_A
        else
                echo "unknown current system!"
                exit 1
        fi
}
