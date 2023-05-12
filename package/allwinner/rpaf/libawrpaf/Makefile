target	= libawrpaf.so

INCLUDES        += -I. -I./include

SRCS = awrpaf_component.c

LOCAL_CFLAGS = $(INCLUDES) $(CFLAGS)

install: $(target)
	-@mkdir -p $(CONFIG_PREFIX)/usr/lib
	-@mkdir -p $(CONFIG_PREFIX)/usr/include
	@cp $(target) $(CONFIG_PREFIX)/usr/lib
	@cp ./include/awrpaf_component.h $(CONFIG_PREFIX)/usr/include

$(target): $(SRCS)
	$(CC) -fPIC -shared $(LOCAL_CFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

####################################################################
all: install $(target)
clean:
	rm -rf $(target)
