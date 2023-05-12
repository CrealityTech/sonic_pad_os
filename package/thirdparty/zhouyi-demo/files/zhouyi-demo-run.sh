#!/bin/sh

CASES_DIR=/etc/zhouyi/cases
OUTPUT_DUMP_DIR=

help () {
    echo "=====================Zhouyi Demo Help============================"
    echo "Options:"
    echo "-h, --help                   help"
    echo "-C, --case <CASE_NAME>       test case to run"
    echo "-d, --dump-dir <DIR>         directory to dump output data"
    echo ""
    echo "Supported cases:"
    for CASE in $(ls $CASES_DIR); do
        echo "    $CASE"
    done
    echo "================================================================="
    exit 1
}

if [ $# -eq 0 ]; then
    help
fi

eval set -- "$@"

while [ -n "$1" ]
do
    case "$1" in
        -h|--help)
            help
            ;;
        -C|--case)
            CASE="$2"
            shift
            ;;
        -d|--dump-dir)
            OUTPUT_DUMP_DIR="$2"
            shift
            ;;
        *)
            break
    esac
    shift
done

if [ -n "$OUTPUT_DUMP_DIR" ]; then
    MKDIR_OUTPUT_DUMP_DIR="mkdir -p ${OUTPUT_DUMP_DIR}"
    OPT_OUTPUT_DUMP_DIR="-d ${OUTPUT_DUMP_DIR}"
fi

case "${CASE}" in
    resnet_50)
        ${MKDIR_OUTPUT_DUMP_DIR}
        zhouyi-demo -b ${CASES_DIR}/${CASE}/aipu_resnet_50_Z1_0701.bin \
            -i ${CASES_DIR}/${CASE}/input.bin \
            -c ${CASES_DIR}/${CASE}/output_ref.bin \
            ${OPT_OUTPUT_DUMP_DIR}
        ;;
    tflite_inception_v3)
        ${MKDIR_OUTPUT_DUMP_DIR}
        zhouyi-demo -b ${CASES_DIR}/${CASE}/aipu_tflite_inception_v3_Z1_0701.bin \
            -i ${CASES_DIR}/${CASE}/input.bin \
            -c ${CASES_DIR}/${CASE}/output_ref.bin \
            ${OPT_OUTPUT_DUMP_DIR}
        ;;
    *)
        echo "[ERROR] Unknown case"
        echo "Supported cases:"
        echo "$(ls $CASES_DIR)"
        ;;
esac
