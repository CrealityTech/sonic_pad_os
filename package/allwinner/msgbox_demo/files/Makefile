TARGET = msgbox_demo

SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))

override INCLUDES +=
override LDFLAGS +=
override LIBS +=


all: $(TARGET) install

$(TARGET): $(OBJS)
	$(CC) $^ $(CFLAGS) $(INCLUDES) $(LDFLAGS) $(LIBS) -o $@

install:
	mkdir -p $(INSTALL_PREFIX)/usr/bin
	cp $(TARGET) $(INSTALL_PREFIX)/usr/bin

clean:
	rm -rf $(TARGET)

.PHONY: all clean install
