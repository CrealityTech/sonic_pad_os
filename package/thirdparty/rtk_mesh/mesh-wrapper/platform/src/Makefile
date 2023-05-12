CLEANEXTS = o a

OUTPUTFILE = libplatform.a
OUTPUT = output

CFLAGS += -I../../trace
CFLAGS += -I../inc
CFLAGS += -I../../bluez/inc
CFLAGS += -I../../
CFLAGS += -I../../hci_mgmt
CFLAGS += -Wno-pointer-to-int-cast
#CFLAGS += -Wunused-parameter
CFLAGS += -Wunused-variable
CFLAGS += -fPIC

include ../../makefile.defs

.PHONY: all clean

all: $(OUTPUTFILE)

OBJECTS = hci_imp.o os_msg.o os_sched.o os_sync.o os_timer.o data_uart.o

$(OUTPUTFILE): $(OBJECTS)
	$(CROSS_COMPILE)ar ru $@ $^
	$(CROSS_COMPILE)ranlib $@
	cp $^ ../../$(OUTPUT)/

clean:
	for ext in $(CLEANEXTS); do rm -f *.$$ext; done
	rm -f libplatform.a
	rm -f tags
	rm -f cscope.*

