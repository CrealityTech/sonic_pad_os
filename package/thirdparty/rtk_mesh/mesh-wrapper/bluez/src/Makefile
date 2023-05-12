CLEANEXTS = o a

OUTPUTFILE = libshared-mainloop.a
OUTPUT = output

CFLAGS += -I../inc
CFLAGS += -Wno-pointer-to-int-cast
#CFLAGS += -Wunused-parameter
CFLAGS += -Wunused-variable
CFLAGS += -fPIC
OBJECTS =  hci.o io-mainloop.o mainloop.o queue.o util.o

include ../../makefile.defs
.PHONY: all clean

all: $(OUTPUTFILE)
$(OUTPUTFILE): $(OBJECTS)
	$(CROSS_COMPILE)ar ru $@ $^
	$(CROSS_COMPILE)ranlib $@
	cp $@ ../../$(OUTPUT)/
	cp $@ ../lib/
clean:
	for ext in $(CLEANEXTS); do rm -f *.$$ext; done
	rm -f libshared-mainloop.a