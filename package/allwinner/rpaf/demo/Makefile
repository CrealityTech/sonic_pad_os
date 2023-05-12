target	= awrpaf_demo

INCLUDES        += -I. -I./include -I$(CONFIG_PREFIX)/usr/include
LIBS            += -lawrpaf

SRCS =	main.c

LOCAL_CFLAGS = $(INCLUDES)

install: $(target)
	-@mkdir -p $(CONFIG_PREFIX)/usr/bin
	@cp $(target) $(CONFIG_PREFIX)/usr/bin

$(target): $(SRCS)
	$(CC) $(CFLAGS) $(LOCAL_CFLAGS) $(LDFLAGS) -L$(CONFIG_PREFIX)/usr/lib $^ -o $@ $(LIBS)

####################################################################
all: install $(target)
clean:
	rm -rf $(target)
