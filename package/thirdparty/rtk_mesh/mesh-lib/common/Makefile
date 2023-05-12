SRCS = generic_client_app.c light_client_app.c ping_app.c

include ../makefile.defs

CFLAGS += -I../model/realtek
CFLAGS += -I../common

.PHONY: all clean
all: $(OBJS)

clean:
	$(RM) -f $(OBJS)

