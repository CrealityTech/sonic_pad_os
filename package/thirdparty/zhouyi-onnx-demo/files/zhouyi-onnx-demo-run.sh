#!/bin/sh

CASES_DIR=/etc/zhouyi-onnx/cases
OUTPUT_DUMP_DIR=/tmp

help () {
    echo "====================Zhouyi ONNX Demo Help===================="
    echo "Options:"
    echo "-h, --help                   help"
    echo "-C, --case <CASE_NAME>       test case to run"
    echo "-d, --dump-dir <DIR>         directory to dump output data"
    echo ""
    echo "Supported cases:"
    for CASE in $(ls $CASES_DIR); do
        echo "    $CASE"
    done
    echo "=============================================================="
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

case "${CASE}" in
    custom_op_aipu)
        mkdir -p ${OUTPUT_DUMP_DIR}
        zhouyi-onnx-oruntime_test --bin=${CASES_DIR}/${CASE}/aipu.bin \
            --input=${CASES_DIR}/${CASE}/input0.bin,${CASES_DIR}/${CASE}/input1.bin \
            --outdir=${OUTPUT_DUMP_DIR}
        ;;
    custom_op_cpu)
        mkdir -p ${OUTPUT_DUMP_DIR}
        zhouyi-onnx-custom_op_cpu_demo --bin=${CASES_DIR}/${CASE}/aipu.bin \
            --input=${CASES_DIR}/${CASE}/input0.bin,${CASES_DIR}/${CASE}/input1.bin \
            --outdir=${OUTPUT_DUMP_DIR}
        ;;
    WaveNet)
        mkdir -p ${OUTPUT_DUMP_DIR}
        zhouyi-onnx-oruntime_test --bin=${CASES_DIR}/${CASE}/aipu.bin \
            --input=${CASES_DIR}/${CASE}/input0.bin \
            --outdir=${OUTPUT_DUMP_DIR}
        ;;
    RNN)
        mkdir -p ${OUTPUT_DUMP_DIR}
        zhouyi-onnx-ctc_decoder \
            --bin="${CASES_DIR}/${CASE}/encoder/aipu_encoder.bin,${CASES_DIR}/${CASE}/decoder/aipu_decoder.bin,${CASES_DIR}/${CASE}/Joint/aipu_joint.bin" \
            --input=${CASES_DIR}/${CASE}/input0.bin \
            --phone_map=${CASES_DIR}/${CASE}/phones.txt \
            --beam_width=10 \
            --seq_len=10 \
            --outdir=${OUTPUT_DUMP_DIR}
        ;;
    DeepVoice)
        mkdir -p ${OUTPUT_DUMP_DIR}
        zhouyi-onnx-oruntime_test \
            --bin=${CASES_DIR}/${CASE}/aipu.bin \
            --input=${CASES_DIR}/${CASE}/input0.bin,${CASES_DIR}/${CASE}/input1.bin \
            --outdir=${OUTPUT_DUMP_DIR}
        ;;
    *)
        echo "[ERROR] Unknown case"
        echo "Supported cases:"
        echo "$(ls $CASES_DIR)"
        ;;
esac
