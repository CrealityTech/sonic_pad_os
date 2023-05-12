TARGET = 4G-quectel-CM

SRCDIRS = $(shell find . -maxdepth 3 -type d)

INCLUDES += $(foreach dir,$(SRCDIRS),-I$(dir))  \
						-I$(STAGING_DIR)/usr/include/allwinner/include \
					  -I$(STAGING_DIR)/usr/include/allwinner
SRCS = $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c))
OBJS = $(SRCS:.c=.o)
LOCAL_LDFLAGS += -lpthread -ldl

%.o: %.c
	$(CC) $(INCLUDES) $(CFLAGS) $(LDFLAGS) $(LOCAL_CFLAGS) $(LOCAL_LDFLAGS) -c -o $@ $<

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(LDFLAGS) $(LOCAL_CFLAGS) $(LOCAL_LDFLAGS) -g

all:install
install:$(TARGET)

clean:
	-rm -rf $(OBJS) $(TARGET)
