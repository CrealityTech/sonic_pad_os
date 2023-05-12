cmake_minimum_required(VERSION 3.4.1)
#set(CMAKE_VERBOSE_MAKEFILE on)

# build for target zhouyi-onnx-oruntime_test
project(oruntime_test)
include_directories(oruntime_test/include)
add_executable(
    zhouyi-onnx-${PROJECT_NAME}
    oruntime_test/main.cpp
)

target_link_libraries(zhouyi-onnx-${PROJECT_NAME}
    libaipu_oruntime.so
)

install(TARGETS zhouyi-onnx-${PROJECT_NAME} DESTINATION bin)

## build for target custom_op_cpu_demo
project(custom_op_cpu_demo)
add_executable(
   zhouyi-onnx-${PROJECT_NAME}
   custom_op_cpu_demo/custom_op_cpu_demo_main.cpp
)

target_link_libraries(
   zhouyi-onnx-${PROJECT_NAME}
   libaipu_oruntime.so
)

install(TARGETS zhouyi-onnx-${PROJECT_NAME} DESTINATION bin)

### build for target ctc_decoder
project(ctc_decoder)
include_directories(oruntime_test)
include_directories(ctc_decoder/include)
add_executable(
    zhouyi-onnx-${PROJECT_NAME}
    ctc_decoder/main.cpp
    ctc_decoder/src/utils.cpp
    ctc_decoder/src/sequence.cpp
    ctc_decoder/src/beam_decode.cpp
    ctc_decoder/src/run_model.cpp
)

target_link_libraries(
    zhouyi-onnx-${PROJECT_NAME}
    libaipu_oruntime.so
)

install(TARGETS zhouyi-onnx-${PROJECT_NAME} DESTINATION bin)
