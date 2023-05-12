TARGET			 =  smart-music-player
SRCDIRS   =  $(shell find . -maxdepth 3 -type d)

INCLUDES	:= $(INCLUDES)       \
						 $(foreach dir,$(SRCDIRS),-I$(dir))  \
						 -I$(STAGING_DIR)/usr/include/allwinner/include \
						 -I$(STAGING_DIR)/usr/include/allwinner

SRCS			= $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c))
OBJS			= $(SRCS:.c=.o)
LIBS			+= -lpthread -ldl -lminigui_ths -lpng  -ljpeg -lfreetype -lz -lm \
					-ltplayer -luapi -lrt -lc -lts -lasound -lmgplus -lstdc++ \

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS) $(LDFLAGS)

all:$(TARGET)

clean:
	rm -rf $(TARGET) $(OBJS) *.o *.a *~
