CLEANEXTS = o a

OUTPUTFILE = libftl.a
OUTPUT = output

CFLAGS += -I.
CFLAGS += -I../trace
CFLAGS += -Wno-pointer-to-int-cast
#CFLAGS += -Wunused-parameter
CFLAGS += -Wunused-variable
CFLAGS += -fPIC
OBJECTS = ftl.o

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

