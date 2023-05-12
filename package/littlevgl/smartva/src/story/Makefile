TARGET = story_bin

SRCDIRS = $(shell find . -maxdepth 5 -type d)

INCLUDES += $(foreach dir,$(SRCDIRS),-I$(dir))  \
						-I$(STAGING_DIR)/usr/include/allwinner/include \
					  -I$(STAGING_DIR)/usr/include/allwinner
SRCS = $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c))
OBJS = $(SRCS:.c=.o)
LOCAL_LDFLAGS += -ltrecorder -lpthread -ldl\
						 -ltplayer -lxplayer -lvdecoder -ladecoder -lsubdecoder -lcdc_base -lVE -lMemAdapter \
             -lcdx_parser  -lcdx_playback -lcdx_stream -lcdx_base \
             -luapi -lrt -lc  -lasound \
             -lawrecorder -lvencoder -laencoder \
             -lcdx_muxer -ljpegdecode  -ltmetadataretriever -lcdx_common -lwifimg

%.o: %.c
	$(CC) $(INCLUDES) $(CFLAGS) $(LDFLAGS) $(LOCAL_CFLAGS) $(LOCAL_LDFLAGS) -c -o $@ $<

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(LDFLAGS) $(LOCAL_CFLAGS) $(LOCAL_LDFLAGS) -g

all:install
install:$(TARGET)

clean:
	-rm -rf $(OBJS) $(TARGET)