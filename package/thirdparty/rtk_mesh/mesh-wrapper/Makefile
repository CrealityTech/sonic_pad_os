OUTPUT = ./output
RTL_CFLAGS = -I./trace
RTL_CFLAGS += -I./platform/inc
RTL_CFLAGS += -I./ftl
RTL_CFLAGS += -I./hci_mgmt
RTL_CFLAGS += -I./bluez/inc

RTL_CFLAGS += -fPIC

C_SRC = $(wildcard ./trace/*.c)
C_SRC += $(wildcard ./platform/src/*.c)
C_SRC += $(wildcard ./ftl/*.c)
C_SRC += $(wildcard ./hci_mgmt/*.c)
C_SRC += $(wildcard ./bluez/src/*.c)

C_OBJ = $(patsubst %c, %o, $(C_SRC))

.PHONY: all clean
all:libmesh-wrapper.so

libmesh-wrapper.so:$(C_OBJ)
	$(CC) -g -shared -o $@ $^
	mv $@ ../

%.o:%.c
	$(CC) -g -c $(RTL_CFLAGS) $^ -o $@

clean:
	-rm $(C_OBJ)
	-rm ../libmesh-wrapper.so
