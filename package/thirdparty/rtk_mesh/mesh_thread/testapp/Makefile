CLEANEXTS = o a

LIBAPP = libappinternal.so
ALIAPP = mesh_thread_provisioner

CFLAGS += -Wno-pointer-to-int-cast
#CFLAGS += -Wunused-parameter
CFLAGS += -Wunused-variable
CFLAGS += -g -O0
CFLAGS += -imacros platform_macros.h

CFLAGS += -I../../bluez/inc
CFLAGS += -I../../trace
CFLAGS += -I../../lib/inc
CFLAGS += -I../../platform/inc
CFLAGS += -I../../lib/utility
CFLAGS += -I..
CFLAGS += -I../../
CFLAGS += -I../../lib/cmd
CFLAGS += -I../../lib/common
CFLAGS += -I../../lib/gap
CFLAGS += -I../../lib/inc
CFLAGS += -I../../lib/platform
CFLAGS += -I../../lib/model
CFLAGS += -I../../lib/model/realtek
CFLAGS += -I../../mesh_thread/provisioner/
CFLAGS += -I../../mesh_thread/testapp/
CFLAGS += -I../../ftl
#CFLAGS += -fshort-enums
CFLAGS += -fPIC
#CFLAGS += $(shell pkg-config --cflags glib-2.0)

LDFLAGS += -L../../lib

LDFLAGS += -lappinternal
LDFLAGS += -lmesh_provisioner
LDFLAGS += -lmeshprov_ag
LDFLAGS += -lm
#LDFLAGS += $(shell pkg-config --libs glib-2.0)

SRC_ALIS = mesh_app.c app_cmd.c app_handler.c

SRC_LIB_APP += ../provisioner/mesh_lib_thread.c
SRC_LIB_APP += ../provisioner/mesh_cmd_handler.c
SRC_LIB_APP += ../provisioner/mesh_blue_cmd.c
SRC_LIB_APP += ../provisioner/mesh_provisioner.c

OBJS_ALI = $(foreach SRC,$(SRC_ALIS),$(SRC:.c=.o))
OBJS_LIB_APP = $(foreach SRC,$(SRC_LIB_APP),$(SRC:.c=.o))

.PHONY: all clean

all:$(LIBAPP) $(ALIAPP) 
#all:$(LIBAPP)

$(ALIAPP): $(OBJS_ALI)
	$(CROSS_COMPILE)gcc -g $^ $(CFLAGS) $(LDFLAGS) -lpthread -o $@

$(LIBAPP): $(OBJS_LIB_APP)
	$(CROSS_COMPILE)gcc -g -shared -o $@ $^ 
	mv $@ ../../lib

%.o: %.c
	$(CROSS_COMPILE)gcc -g $(CFLAGS) -c $^ -o $@

clean:
	for ext in $(CLEANEXTS); do rm -f ./*.$$ext; done
	rm $(ALIAPP)
	for ext in $(CLEANEXTS); do rm -f ../*.$$ext; done
