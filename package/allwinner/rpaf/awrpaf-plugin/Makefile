target	= libasound_module_pcm_awrpaf.so

INCLUDES        += -I. -I./include -I$(CONFIG_PREFIX)/usr/include

SRCS = awrpaf_plugin.c

OBJS = $(SRCS:.c=.o)

LOCAL_CFLAGS = $(INCLUDES) $(CFLAGS) -DPIC -fPIC

LIBS            += -lawrpaf -lasound -ldl -lm -lpthread

install: $(target)
	-@mkdir -p $(CONFIG_PREFIX)/usr/lib
	-@mkdir -p $(CONFIG_PREFIX)/usr/include
	@cp $(target) $(CONFIG_PREFIX)/usr/lib

%.o:%.c
	$(CC) $(CFLAGS) $(LOCAL_CFLAGS) -c -o $@ $<

$(target): $(OBJS)
	$(CC)  -fPIC -shared $(LOCAL_CFLAGS) $(LDFLAGS) $^ -o $@ -L$(CONFIG_PREFIX)/usr/lib $(LIBS)

####################################################################
all: install $(target)
clean:
	rm -rf $(target)
