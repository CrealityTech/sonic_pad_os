TARGET = leakage_current

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SRCS))

override INCLUDES +=
override LIBS += -L./lib/$(CPU_VARIANT)/$(TOOLCHAIN_LIBC) -lleakage_current

all: $(TARGET) install

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $^ $(LDFLAGS) $(LIBS) -o $@

install:
	mkdir -p $(INSTALL_PREFIX)/usr/bin
	cp $(TARGET) $(INSTALL_PREFIX)/usr/bin

clean:
	rm -rf $(OBJS) $(TARGET)

.PHONY: all clean install
