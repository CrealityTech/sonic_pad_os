TARGET := libCrLog.so
INCLUDES +=	-I$(STAGING_DIR)/usr/include/allwinner/include -I$(STAGING_DIR)/usr/include/allwinner

LIBS +=-lpthread

SRCS = CrLog.c CrMutex.c

OBJS = $(SRCS:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -g $(INCLUDES) -c -o $@ $<

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS) $(LDFLAGS) -fPIC -shared

all:$(TARGET) install


clean:
	rm -rf $(TARGET) *.o *.a *~
	cd common && rm -f *.o *.a *.bak *~ .depend

install:
	-mkdir -p $(CONFIG_PREFIX)/usr/lib
	-mkdir -p $(CONFIG_PREFIX)//usr/include/
	-cp $(TARGET) $(CONFIG_PREFIX)/usr/lib/
	-cp CrLog.h $(CONFIG_PREFIX)/usr/include/

.PHONY:all clean $(TARGET) install
