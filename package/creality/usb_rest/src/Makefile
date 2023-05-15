TARGET := usb_rest

MAIN_SRC := usb_rest.c

local_CFLAGS := $(CFLAGS) -D_GNU_SOURCE -Werror

SRC		:= $(strip $(SRC))
MAIN_SRC	:= $(strip $(MAIN_SRC))
OBJS		:= $(addsuffix .o, $(basename $(SRC)))
MAIN_OBJS	:= $(addsuffix .o, $(basename $(MAIN_SRC)))

$(TARGET): %: %.o $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(local_CFLAGS) $^ -o $@

all: $(TARGET)

clean:
	-rm $(MAIN_OBJS) $(OBJS) $(TARGET)
