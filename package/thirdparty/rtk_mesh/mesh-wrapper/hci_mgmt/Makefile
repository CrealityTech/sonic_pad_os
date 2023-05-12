CLEANEXTS = o a

OUTPUTFILE = libhci_mgmt.a
OUTPUT = output

CFLAGS += -Wno-pointer-to-int-cast
CFLAGS += -I../bluez/inc
CFLAGS += -fPIC

include ../makefile.defs

.PHONY: all clean

all: $(OUTPUTFILE)

OBJECTS = hci_mgmt.o

$(OUTPUTFILE): $(OBJECTS)
	$(CROSS_COMPILE)ar ru $@ $^
	$(CROSS_COMPILE)ranlib $@
	cp $^ ../$(OUTPUT)/

clean:
	for ext in $(CLEANEXTS); do rm -f *.$$ext; done

