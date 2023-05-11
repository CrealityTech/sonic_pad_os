#!/bin/bash

#set -e

ARISC_CONFIG_FILE=${LICHEE_BOARD_CONFIG_DIR}/arisc.config

pmu_type_parse()
{
	PMU_TYPE=`awk '/x-powers,axp/{print $3;exit;}' ${LICHEE_BOARD_CONFIG_DIR}/board.dts`
	PMU_TYPE=${PMU_TYPE##*,}
	PMU_TYPE=${PMU_TYPE%%\"*}

	rm -f $ARISC_CONFIG_FILE
	touch $ARISC_CONFIG_FILE
	echo "export LICHEE_ARISC_DEFDIR=ar100s" >> $ARISC_CONFIG_FILE
	echo "export LICHEE_ARISC_DEFCONFIG=sun50iw10p1_${PMU_TYPE}_defconfig" >> $ARISC_CONFIG_FILE
}

pmu_type_parse

