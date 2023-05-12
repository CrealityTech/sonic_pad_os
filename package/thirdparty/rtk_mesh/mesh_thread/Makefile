CFLAGS_WRAPPER = -I../mesh-wrapper/trace
CFLAGS_WRAPPER += -I../mesh-wrapper/platform/inc
CFLAGS_WRAPPER += -I../mesh-wrapper/ftl
CFLAGS_WRAPPER += -I../mesh-wrapper/hci_mgmt
CFLAGS_WRAPPER += -I../mesh-wrapper/bluez/inc

CFLAGS_LIB = -I../mesh-lib/cmd
CFLAGS_LIB += -I../mesh-lib/common
CFLAGS_LIB += -I../mesh-lib/gap
CFLAGS_LIB += -I../mesh-lib/inc
CFLAGS_LIB += -I../mesh-lib/model
CFLAGS_LIB += -I../mesh-lib/model/realtek
CFLAGS_LIB += -I../mesh-lib/platform
CFLAGS_LIB += -I../mesh-lib/profile
CFLAGS_LIB += -I../mesh-lib/utility

CFLAGS_APP += -I./testapp
CFLAGS_API += -I./provisioner

CFLAGS_LIB += -fPIC

RTL_LDFLAGS += -L./mesh-lib
RTL_LDFLAGS += -L./mesh-wrapper
RTL_LDFLAGS += -L../
RTL_LDFLAGS += -lmesh-api
RTL_LDFLAGS += -lmesh-lib
RTL_LDFLAGS += -lmesh-wrapper
RTL_LDFLAGS += -lpthread
RTL_LDFLAGS += -lm

SRC_API = $(wildcard ./provisioner/*.c)
OBJ_API = $(patsubst %c, %o, $(SRC_API))

SRC_APP = $(wildcard ./testapp/*.c)

.PHONY: all clean
all:libmesh-api.so mesh_thread_provisioner

mesh_thread_provisioner:$(SRC_APP)
	$(CC) -g $^ $(CFLAGS) $(CFLAGS_WRAPPER) $(CFLAGS_LIB) $(CFLAGS_API) $(CFLAGS_APP) $(RTL_LDFLAGS) -o $@
	mv $@ ../

libmesh-api.so:$(OBJ_API)
	$(CC) -g -shared -o $@ $^
	mv $@ ../

%.o:%.c
	$(CC) -g -c $(CFLAGS) $(CFLAGS_WRAPPER) $(CFLAGS_LIB) $(CFLAGS_API) $(CFLAGS_APP) $^ -o $@

clean:
	-rm $(OBJ_API)
	-rm ../libmesh-api.so
	-rm ../mesh_thread_provisioner
