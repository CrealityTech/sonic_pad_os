
INCLUDES += -I$(STAGING_DIR)/usr/include/ -I$(STAGING_DIR)/usr/
LIBS +=	-L$(STAGING_DIR)/usr/lib/ -lpthread  -lm -lx264

TARGET = x264demo
C_SRCS = x264demo.c

OBJS = $(C_SRCS:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<
	echo "2"$(1) > 1.log


$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS) $(LDFLAGS)


all:$(TARGET)


clean:
	rm -rf $(TARGET) *.o *.a *~

