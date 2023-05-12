target	= mad-capture

SRCS = main.c

INCLUDES =
LIBS		+= -lasound -lpthread -lm -ldl
LOCAL_CFLAGS = $(INCLUDES)

LOCAL_CFLAGS += -D_GNU_SOURCE

install:

$(target): $(SRCS)
	$(CC) $(CFLAGS) $(LOCAL_CFLAGS) $(LDFLAGS)  $^ -o $@ $(LIBS)

all: $(target) install
clean:
	rm -rf $(target)
