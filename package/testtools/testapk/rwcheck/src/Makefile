CC      ?= gcc
CFLAGS  ?= -Wall -O
TARGET  := rwcheck
OBJS    := rwcheck.o crc16.o fops.o log.o size_unit.o sys_info.o
LDFLAGS += -lpthread -rdynamic

.PHONY: all
all : $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS)

%.d:%.c
	@set -e; rm -f $@; $(CC) -MM $< > .$@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < .$@.$$$$ > $@; \
	rm -f .$@.$$$$

-include $(OBJS:.o=.d)

.PHONY: clean
clean:
	rm -f $(TARGET) *.o *.d *.d.*
