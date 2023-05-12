CLEANEXTS = o a

OUTPUTFILE = libtrace.a
OUTPUT = output

CFLAGS += -I.
CFLAGS += -I../platform/inc/
CFLAGS += -I../bluez
CFLAGS += -I../bluez/inc
CFLAGS += -I../
CFLAGS += -Wno-pointer-to-int-cast
#CFLAGS += -Wunused-parameter
CFLAGS += -Wunused-variable
CFLAGS += -fPIC

OBJECTS = trace.o

include ../makefile.defs

.PHONY: all clean

all: $(OUTPUTFILE)
$(OUTPUTFILE): $(OBJECTS)
	$(CROSS_COMPILE)ar ru $@ $^
	$(CROSS_COMPILE)ranlib $@
	cp $^ ../$(OUTPUT)/

clean:
	for ext in $(CLEANEXTS); do rm -f *.$$ext; done
	rm -f tags
	rm -f cscope.*

