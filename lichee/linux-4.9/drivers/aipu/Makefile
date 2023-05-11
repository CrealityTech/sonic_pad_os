
SRC_DIR  := aipu_src

EXTRA_CFLAGS +=-I$(src)/$(SRC_DIR) -I$(src)/$(SRC_DIR)/aipu -I$(src)/$(SRC_DIR)/aipu/zhouyi/z1 \
			 -I$(src)/$(SRC_DIR)/aipu/zhouyi -I$(src)/$(SRC_DIR)/aipu/soc -I$(src)/$(SRC_DIR)/config \
			 -I$(src)/$(SRC_DIR)/uk_interface

FOPS_OBJ  := $(SRC_DIR)/aipu_fops.o \
            $(SRC_DIR)/aipu_session.o
MEM_OBJ  := $(SRC_DIR)/aipu_mm.o
HW_OBJ   := $(SRC_DIR)/aipu/aipu.o \
            $(SRC_DIR)/aipu/aipu_core.o \
            $(SRC_DIR)/aipu/zhouyi/zhouyi.o \
            $(SRC_DIR)/aipu/soc/aipu_soc.o \
            $(SRC_DIR)/aipu_io.o \
            $(SRC_DIR)/aipu_irq.o
JOB_OBJ  := $(SRC_DIR)/aipu_job_manager.o \
            $(SRC_DIR)/aipu_thread_waitqueue.o
MISC_OBJ := $(SRC_DIR)/aipu_errcode_map.o \
            $(SRC_DIR)/aipu_sysfs.o

AIPU_OBJ := $(SRC_DIR)/aipu/zhouyi/z1/z1.o
SOC_OBJ  := $(SRC_DIR)/aipu/soc/default/default.o

INIT_OBJ := $(SRC_DIR)/aipu_platform_driver.o
OBJS     := $(FOPS_OBJ) \
            $(MEM_OBJ) \
            $(HW_OBJ) \
            $(JOB_OBJ) \
            $(MISC_OBJ) \
            $(AIPU_OBJ) \
            $(SOC_OBJ)


obj-$(CONFIG_AIPU) += aipu.o
aipu-objs += $(INIT_OBJ)
aipu-objs += $(OBJS)
